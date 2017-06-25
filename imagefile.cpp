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

void ImageFile::addPlate(const PlateFile &plateFile)
{
    m_plates.append(plateFile);
}

void ImageFile::setPlates(const PlateFileList &newPlateFileList)
{
    // determine plates to be removed and added
    PlateFileList platesToRemove = m_plates;

    foreach(const PlateFile &newPlateFile, newPlateFileList)
    {
        QMutableListIterator<PlateFile> itOldPlates(platesToRemove);

        while(itOldPlates.hasNext())
        {
            const PlateFile &oldPlateFile = itOldPlates.next();

            if(oldPlateFile.plateFile() == newPlateFile.plateFile())
                itOldPlates.remove();
        }
    }

    foreach(const PlateFile &plateFile, platesToRemove)
    {
        // standalone YAML
        if(plateFile.plateFileTemplate().isEmpty())
        {
            qDebug() << "Removing HEAP YAML" << plateFile.plateFile();

            if(QFile::exists(plateFile.plateFile()) && !QFile::remove(plateFile.plateFile()))
                Utils::error(QObject::tr("Cannot remove YAML file %1").arg(plateFile.plateFile()));
        }
    }

    m_plates = newPlateFileList;

    int numberedIndex = 0;

    // sync to disk
    for(int i = 0;i < m_plates.size();i++)
    {
        QString error;
        PlateFile plateFile = m_plates.at(i);

        // new plates don't have an image file set
        if(plateFile.imageFile().isEmpty())
            plateFile.setImageFile(m_fileInfo.fileName());

        bool written = plateFile.plateFileTemplate().isEmpty()
                       ? plateFile.writeToStandaloneFile(&error)
                       : plateFile.writeToNumberedFile(numberedIndex++, &error);

        if(!written)
        {
            Utils::error(QObject::tr("Cannot save YAML file. %1")
                         .arg(error.isEmpty()
                              ? QObject::tr("Unknown error")
                              : QObject::tr("Error: %1").arg(error)));
        }
    }

    int index = numberedIndex;

    while(true)
    {
        const QString &file = m_plateNameTemplate + QString("-%1.yaml").arg(index);

        if(!QFile::exists(file))
            break;

        qDebug() << "Removing INDEXED YAML" << file;

        QFile::remove(file);
    }
}
