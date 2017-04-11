#include <QtWidgets>
#include <QtCore>

#include "directoryloader.h"
#include "utils.h"
#include "ui_directoryloader.h"

DirectoryLoader::DirectoryLoader(const QString &dirPath, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DirectoryLoader)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    // directory iterator
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
            accept();
    }
}
