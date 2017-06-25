#include <QtCore>

#include <fstream>

#include "platefile.h"

PlateFile::PlateFile(const QString &platePath)
    : m_plateFile(platePath)
    , m_parsed(false)
{
    if(!exists())
        return;

    try
    {
        m_yaml = YAML::LoadFile(QDir::toNativeSeparators(m_plateFile).toStdString());
    }
    catch(...)
    {
        qWarning("Error parsing file '%s'", qPrintable(m_plateFile));
        return;
    }

    if(!m_yaml["plate_corners_gt"])
        return;

    // cache polygon (8 x,y numbers)
    int x, y;
    QPolygon polygon;

    QString corners = QString::fromStdString(m_yaml["plate_corners_gt"].as<std::string>());
    QTextStream ts(&corners, QIODevice::ReadOnly);

    while(true)
    {
        ts.skipWhiteSpace();

        if(ts.atEnd())
            break;

        ts >> x >> y;
        polygon.append(QPoint(x, y));
    }

    if(polygon.size() == 4)
    {
        m_plateCorners = polygon;
        m_parsed = true;
    }
    else
        qWarning("Polygon in file '%s' has only %d point(s) thus it's invalid", qPrintable(m_plateFile), polygon.size());
}

PlateFile::PlateFile(const QString &platePathTemplate, int index)
    : PlateFile(platePathTemplate + QString("-%1.yaml").arg(index))
{
    m_plateFileTemplate = platePathTemplate;
}

bool PlateFile::writeToNumberedFile(int index, QString *error)
{
    const QString &yamlPath = m_plateFileTemplate + QString("-%1.yaml").arg(index);
    qDebug("Writing INDEXED YAML %d '%s'", index, qPrintable(yamlPath));
    return writeToFile(yamlPath, error);
}

bool PlateFile::writeToStandaloneFile(QString *error)
{
    qDebug("Writing HEAP YAML '%s'", qPrintable(plateFile()));
    return writeToFile(m_plateFile, error);
}

bool PlateFile::exists()
{
    return QFileInfo(m_plateFile).exists();
}

void PlateFile::setPlateCorners(const QPolygon &plateCorners)
{
    m_plateCorners = plateCorners;

    QStringList cornersStringList;
    cornersStringList.reserve(m_plateCorners.size() * 2);

    foreach(const QPoint &point, m_plateCorners)
    {
        cornersStringList.append(QString::number(point.x()));
        cornersStringList.append(QString::number(point.y()));
    }

    m_yaml["plate_corners_gt"] = cornersStringList.join(" ").toStdString();
}

// static
PlateFileList PlateFile::fromImageFile(const QString &fileTemplate)
{
    PlateFileList plateList;

    int index = 0;

    // find associated car-nnn.yaml files
    while(true)
    {
        PlateFile plateFile(fileTemplate, index++);

        if(!plateFile.exists())
            break;

        if(plateFile.isValid())
            plateList.append(plateFile);
    }

    return plateList;
}

bool PlateFile::writeToFile(const QString &yamlPath, QString *error)
{
    const std::string &yamlFilePath = yamlPath.toStdString();

    try
    {
        std::ofstream fout(yamlFilePath);
        fout << m_yaml;
    }
    catch(...)
    {
        if(error)
            *error = QObject::tr("Cannot write into a file");

        return false;
    }

    return true;
}
