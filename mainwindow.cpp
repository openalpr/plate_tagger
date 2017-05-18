#include <QtWidgets>
#include <QtGui>
#include <QtCore>

#include "directoryloader.h"
#include "mainwindow.h"
#include "settings.h"
#include "options.h"
#include "about.h"
#include "utils.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_firstRun(true)
{
    ui->setupUi(this);

    setWindowTitle(TARGET_HUMAN_STRING);

    // shortcuts
    ui->actionOpen_directory->setShortcut(QKeySequence::Open);
    ui->actionQuit->setShortcut(QKeySequence::Quit);
    ui->actionOptions->setShortcut(QKeySequence::Preferences);
    ui->actionAbout->setShortcut(QKeySequence::HelpContents);

    // set tooltips
    Utils::setShortcutTooltips(QList<QPushButton *>()
                               << ui->pushPrevious
                               << ui->pushJumpTo
                               << ui->pushNext);

    // zoom
    connect(ui->imageView, SIGNAL(zoomChanged(double)), this, SLOT(slotZoomChanged(double)));
    connect(ui->imageView, SIGNAL(platesChanged()),     this, SLOT(slotPlatesChanged()));

    // open a directory at startup
    QTimer::singleShot(0, this, SLOT(slotOpenDirectory()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reset()
{
    m_imageFiles.clear();
    m_currentIndex = -1;

    ui->imageView->reset();
    ui->labelImageNumber->clear();
    ui->labelImageFile->clear();
    ui->labelCurrentImageInfo->clear();
    ui->labelZoom->clear();

    ui->labelImagesTagged->clear();
    ui->labelTotalPlates->clear();
}

void MainWindow::updateImageInformationFromIndex()
{
    if(m_currentIndex >= 0 && m_currentIndex < m_imageFiles.size())
    {
        ImageFile &imageFile = m_imageFiles[m_currentIndex];

        ui->imageView->setImageFile(&imageFile);
        ui->labelImageFile->setText(imageFile.fileInfo().fileName());
        ui->labelImageNumber->setText(QString("%1 / %2").arg(m_currentIndex+1).arg(m_imageFiles.size()));

        updateImagePlatesInformationFromIndex();
    }
    else
    {
        QStringList message = QStringList() << tr("No suitable images available");

        if(SETTINGS_GET_BOOL(SETTING_SKIP_TAGGED))
            message.append(tr("Tagged images have been skipped"));

        if(SETTINGS_GET_BOOL(SETTING_MOVE_UNTAGGED))
            message.append(tr("Untagged images have been moved into \"negatives\" subdirectory"));

        Utils::message(message.join(". "), this);

        ui->imageView->reset();
        ui->labelImageNumber->setText(QString("- / %1").arg(m_imageFiles.size()));
        ui->labelImageFile->clear();
        ui->labelCurrentImageInfo->clear();
        ui->labelZoom->clear();
    }
}

void MainWindow::updateImagePlatesInformationFromIndex()
{
    if(m_currentIndex < 0 || m_currentIndex >= m_imageFiles.size())
    {
        ui->labelCurrentImageInfo->clear();
        return;
    }

    ui->labelCurrentImageInfo->setText(m_imageFiles[m_currentIndex].plates().isEmpty()
                                       ? tr("Untagged: Drag a box around a license plate to tag it")
                                       : tr("Number of tagged license plates: %1")
                                         .arg(m_imageFiles[m_currentIndex].plates().size()));
}

void MainWindow::updateTaggedAndPlatesInformation()
{
    int tagged = 0;
    int plates = 0;

    foreach(const ImageFile &imageFile, m_imageFiles)
    {
        tagged += !imageFile.plates().isEmpty();
        plates += imageFile.plates().size();
    }

    ui->labelImagesTagged->setNum(tagged);
    ui->labelTotalPlates->setNum(plates);
}

void MainWindow::moveFileFromCurrentIndex()
{
    // return if:
    //     - moving disabled
    //     - invalid index
    //     - currently selected image has plates
    if(!SETTINGS_GET_BOOL(SETTING_MOVE_UNTAGGED)
            || m_currentIndex < 0
            || m_currentIndex >= m_imageFiles.size()
            || !m_imageFiles.at(m_currentIndex).plates().isEmpty())
        return;

    const QFileInfo &imageFileInfo = m_imageFiles.at(m_currentIndex).fileInfo();

    qDebug("Move '%s'", qPrintable(imageFileInfo.fileName()));

    QDir negatives(imageFileInfo.canonicalPath());

    if(!negatives.mkpath("negatives"))
    {
        Utils::error(tr("Cannot create \"negatives\" subdirectory"), this);
        return;
    }

    negatives.cd("negatives");

    const QString &target = negatives.absoluteFilePath(imageFileInfo.fileName());

    if(QFile::exists(target) && !QFile::remove(target))
    {
        Utils::error(tr("Cannot remove the file \"%1\" in \"negatives\" subdirectory").arg(imageFileInfo.fileName()), this);
        return;
    }

    if(!QFile::rename(imageFileInfo.canonicalFilePath(), target))
    {
        Utils::error(tr("Cannot move the file \"%1\" into \"negatives\" subdirectory").arg(imageFileInfo.fileName()), this);
        return;
    }

    m_imageFiles[m_currentIndex].setMoved(true);
}

void MainWindow::slotOpenDirectory()
{
    qDebug("Open directory");

    ImageFileList list;

    while(true)
    {
        const QString &dir = QFileDialog::getExistingDirectory(this,
                                                               tr("Select a directory"),
                                                               SETTINGS_GET_STRING(SETTING_LAST_DIRECTORY));

        if(dir.isEmpty())
        {
            if(QMessageBox::question(this,
                                     QString(),
                                     tr("No directory has been selected. Whould you like to try again?")) == QMessageBox::Yes)
                continue;
            else
            {
                // in auto-open mode quit immediately
                if(m_firstRun)
                    close();

                return;
            }
        }

        QFileInfo dirInfo(dir);

        SETTINGS_SET_STRING(SETTING_LAST_DIRECTORY, dir);

        if(!dirInfo.isWritable())
        {
            Utils::warning(tr("The selected directory is not writable. Please select another directory"));
            continue;
        }

        qDebug("Loading directory '%s'", qPrintable(dir));

        DirectoryLoader loader(dir, this);

        if(loader.exec() != DirectoryLoader::Accepted)
        {
            Utils::warning(tr("The selected directory doesn't contain JPEG or PNG images. Please select another directory"));
            continue;
        }

        list = loader.imageFiles();
        m_firstRun = false;

        break;
    }

    reset();

    m_imageFiles = list;

    qDebug("Loaded files: %d", m_imageFiles.size());

    // enable some buttons when more than 1 image is loaded
    QList<QPushButton *> buttons = QList<QPushButton *>()
                                   << ui->pushPrevious
                                   << ui->pushJumpTo;

    foreach(QPushButton *button, buttons)
        button->setEnabled(m_imageFiles.size() > 1);

    updateTaggedAndPlatesInformation();

    // load the next available image
    slotNext();
}

void MainWindow::slotQuit()
{
    close();
}

void MainWindow::slotOptions()
{
    qDebug("Options");

    Options options(this);
    options.exec();
}

void MainWindow::slotUsage()
{
    qDebug("Usage");

    Utils::help(tr("Drag a box around each plate in the image to tag it. "
                   "If the image has no plates, click Next to skip it. "
                   "If the option \"Move images without plates\" is selected, the image will be moved into \"negatives\" subdirectory.<br><br>"
                   "Hotkeys:"
                   "<ul>"
                   "<li>%1 - Previous"
                   "<li>%2 - Next"
                   "<li>%3 - Jump To..."
                   "</ul>")
                .arg(ui->pushPrevious->shortcut().toString())
                .arg(ui->pushNext->shortcut().toString())
                .arg(ui->pushJumpTo->shortcut().toString())
                , this);
}

void MainWindow::slotAbout()
{
    About about(this);
    about.exec();
}

void MainWindow::slotAboutQt()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::slotPrevious()
{
    qDebug("Previous");

    // nothing to load
    if(m_currentIndex <= 0 || ui->imageView->busy())
        return;

    do {
        m_currentIndex--;
    }while(m_currentIndex >= 0 && m_imageFiles.at(m_currentIndex).moved());

    updateImageInformationFromIndex();
}

void MainWindow::slotJumpTo()
{
    qDebug("Jump to");

    if(ui->imageView->busy())
        return;

    bool ok;
    int index = QInputDialog::getInt(this,
                                     tr("Jump To"),
                                     tr("Image number (from %1 to %2):").arg(1).arg(m_imageFiles.size()),
                                     m_currentIndex+1,    // current value
                                     1,                   // min
                                     m_imageFiles.size(), // max
                                     1,
                                     &ok);

    if(ok)
    {
        int newIndex = index-1;

        if(m_imageFiles.at(newIndex).moved())
        {
            Utils::message(tr("The specified image has been moved into \"negatives\" subdirectory and no longer available"), this);
            return;
        }

        m_currentIndex = newIndex;
        updateImageInformationFromIndex();
    }
}

void MainWindow::slotNext()
{
    qDebug("Next");

    // nothing to load
    if(m_currentIndex >= m_imageFiles.size() || ui->imageView->busy())
    {
        qDebug("Nothing to load");
        return;
    }

    // move if necessary
    moveFileFromCurrentIndex();

    // last image and it has not been moved
    if(m_currentIndex == m_imageFiles.size()-1 && !m_imageFiles.at(m_currentIndex).moved())
        return;

    // not last or moved
    do {
        m_currentIndex++;
    }while(m_currentIndex < m_imageFiles.size()
           && (m_imageFiles.at(m_currentIndex).moved()
               || (SETTINGS_GET_BOOL(SETTING_SKIP_TAGGED) && !m_imageFiles.at(m_currentIndex).plates().isEmpty()))
           );

    updateImageInformationFromIndex();
}

void MainWindow::slotZoomChanged(double zoom)
{
    ui->labelZoom->setText(QString("%1x (%2%)")
                           .arg(zoom, 0, 'f', 1)
                           .arg(zoom * 100, 0, 'f', 1));
}

void MainWindow::slotPlatesChanged()
{
    updateImagePlatesInformationFromIndex();
    updateTaggedAndPlatesInformation();
}
