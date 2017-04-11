#include <QtCore>

#include "imagefile.h"
#include "utils.h"

ImageFile::ImageFile()
{}

ImageFile::ImageFile(const QFileInfo &fileInfo)
    : m_fileInfo(fileInfo)
    , m_moved(false)
{
    // extract "/data/car" from "/data/car.jpg"
    m_plateNameTemplate = m_fileInfo.canonicalPath() + QDir::separator() + m_fileInfo.completeBaseName();
    m_plates = PlateFile::fromImageFile(m_plateNameTemplate);
}

void ImageFile::setPlates(const PlateFileList &plateFileList)
{
    m_plates = plateFileList;

    // sync to disk
    for(int i = 0;i < m_plates.size();i++)
    {
        QString error;
        PlateFile plateFile = m_plates.at(i);

        plateFile.setImageFile(m_fileInfo.fileName());

        if(!plateFile.writeToFile(i, &error))
        {
            Utils::error(QObject::tr("Cannot save YAML file with index %1. %2")
                                    .arg(i)
                                    .arg(error.isEmpty()
                                        ? QObject::tr("Unknown error")
                                        : QObject::tr("Error: %1").arg(error)));
        }
    }

    int index = m_plates.size();

    while(true)
    {
        const QString &file = m_plateNameTemplate + QString("-%1.yaml").arg(index);

        if(!QFile::exists(file))
            break;

        QFile::remove(file);
    }
}
