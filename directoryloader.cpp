#include <QtWidgets>
#include <QtCore>

#include "directoryloader.h"
#include "utils.h"
#include "ui_directoryloader.h"

DirectoryLoader::DirectoryLoader(const QString &dirPath, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DirectoryLoader)
    , m_step(Step::Images)
    , m_heapPlates(0)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    // images iterator
    m_iterator = new QDirIterator(dirPath,
                                  Utils::imageMatchingWildcard(),
                                  QDir::Files | QDir::Readable);

    ui->progress->startAnimation();

    // start iterating immediately
    QTimer::singleShot(0, this, SLOT(slotLoadNext()));
}

DirectoryLoader::~DirectoryLoader()
{
    delete m_iterator;
    delete ui;
}

void DirectoryLoader::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape)
    {
        e->accept();
        return;
    }

    QDialog::keyPressEvent(e);
}

void DirectoryLoader::slotLoadNext()
{
    if(m_step == Step::Images)
    {
        if(m_iterator->hasNext())
        {
            m_iterator->next();

            // append file & update status label
            m_imageFiles.append(m_iterator->fileInfo());
            ui->labelFound->setNum(m_imageFiles.size());

            // next iteration
            QTimer::singleShot(0, this, SLOT(slotLoadNext()));
        }
        else
        {
            if(m_imageFiles.isEmpty())
                reject();
            else
            {
                m_step = Step::Plates;

                // YAML iterator
                const QString &dirPath = m_iterator->path();
                ui->labelFoundText->setText(tr("Found plates:"));

                delete m_iterator;
                m_iterator = new QDirIterator(dirPath,
                                              QStringList() << "*.yaml" << "*.YAML",
                                              QDir::Files | QDir::Readable);

                // next iteration
                QTimer::singleShot(0, this, SLOT(slotLoadNext()));
            }
        }
    }
    else
    {
        if(m_iterator->hasNext())
        {
            m_iterator->next();

            PlateFile plateFileInHeap(m_iterator->fileInfo().absoluteFilePath());

            if(!plateFileInHeap.isValid())
            {
                // next iteration
                QTimer::singleShot(0, this, SLOT(slotLoadNext()));
                return;
            }

            bool handled = false;
            ImageFile *correspondingImageFile = nullptr;

            for(int i = 0;i < m_imageFiles.size();i++)
            {
                ImageFile &imageFile = m_imageFiles[i];

                // found the corresponding image in cache, e.g.
                // YAML::image_file == image file in cache
                if(plateFileInHeap.imageFile() == imageFile.fileInfo().fileName())
                {
                    correspondingImageFile = &imageFile;

                    foreach(const PlateFile &plateFile, imageFile.plates())
                    {
                        // already handled (autonumbered like car-nnn.yaml)
                        if(m_iterator->fileInfo() == QFileInfo(plateFile.plateFile()))
                        {
                            qDebug("Plate '%s' is already handled, skipping", qPrintable(m_iterator->fileInfo().fileName()));
                            handled = true;
                            break;
                        }
                    }

                    break;
                }
            }

            // append plate & update status label
            if(!handled)
            {
                if(correspondingImageFile)
                {
                    qDebug("New plate in heap '%s' -> '%s'",
                           qPrintable(m_iterator->fileInfo().fileName()),
                           qPrintable(correspondingImageFile->fileInfo().fileName()));

                    correspondingImageFile->addPlate(m_iterator->fileInfo().absoluteFilePath());
                    ui->labelFound->setNum(++m_heapPlates);
                }
                else
                {
                    qWarning("Lost plate with no corresponding image '%s'", qPrintable(m_iterator->fileInfo().absoluteFilePath()));
                    m_lostPlates.append(m_iterator->fileInfo().fileName());
                }
            }

            // next iteration
            QTimer::singleShot(0, this, SLOT(slotLoadNext()));
        }
        else
        {
            if(!m_lostPlates.isEmpty())
                Utils::warning(tr("YAML files without corresponding images:\n\n%1").arg(m_lostPlates.join("\n")), this);

            accept();
        }
    }
}
