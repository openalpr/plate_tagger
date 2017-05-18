#ifndef PLATEFILE_H
#define PLATEFILE_H

#include <QPolygon>
#include <QString>
#include <QList>

#include "plates-yaml.h"

class QTextStream;
class QFileInfo;

class PlateFile;
using PlateFileList = QList<PlateFile>;

class PlateFile
{
public:
    PlateFile(const QString &platePath);
    PlateFile(const QString &platePathTemplate, int index);

    /*
     *  Overwrite a plate file with index 'index'
     */
    bool writeToNumberedFile(int index, QString *error = nullptr);

    /*
     *  Overwrite a standalone plate file
     */
    bool writeToStandaloneFile(QString *error = nullptr);

    /*
     *  Full path to the plate file
     */
    QString plateFile() const;

    /*
     *  Plate file template like "/data/car" from "/data/car.jpg". Empty
     *  if the plate is loaded from the non-related YAML file (car.yaml for truck.jpg)
     */
    QString plateFileTemplate() const;

    /*
     *  Check is the corresponding plate file exists
     */
    bool exists();

    /*
     *  PlateFile is valid when it has
     *  - an image path set
     *  - valid image dimensions
     *  - valid plate corners
     */
    bool isValid() const;

    /*
     *  Some getters/setters
     */
    QString imageFile() const;
    void setImageFile(const QString &imageFile);

    void setImageWidth(int imageWidth);
    void setImageHeight(int imageHeight);

    QString regionCode() const;
    void setRegionCode(const QString &regionCode);

    QString plateNumber() const;
    void setPlateNumber(const QString &plateNumber);

    QPolygon plateCorners() const;
    void setPlateCorners(const QPolygon &plateCorners);

    bool plateInverted() const;
    void setPlateInverted(bool plateInverted);

    /*
     *  Get a list of associated plates for the specified image file
     *
     *  If an image file is "car.jpg", it will look for
     *
     *  car-0.yaml
     *  car-1.yaml
     *  etc.
     */
    static PlateFileList fromImageFile(const QString &fileTemplate);

private:
    bool writeToFile(const QString &yamlPath, QString *error);

private:
    QString m_plateFileTemplate;

    // YAML properies. We store polygon separately as it needs parsing
    YAML::Node m_yaml;
    QPolygon m_plateCorners;
    QString m_plateFile;
    bool m_parsed;
};

inline
QString PlateFile::plateFile() const
{
    return m_plateFile;
}

inline
QString PlateFile::plateFileTemplate() const
{
    return m_plateFileTemplate;
}

inline
bool PlateFile::isValid() const
{
    return m_parsed
            && m_yaml["image_width"]
            && m_yaml["image_width"].as<int>() > 0
            && m_yaml["image_height"]
            && m_yaml["image_height"].as<int>() > 0;
}

inline
QString PlateFile::imageFile() const
{
    return m_yaml["image_file"]
            ? QString::fromUtf8(m_yaml["image_file"].as<std::string>().c_str())
            : QString();
}

inline
void PlateFile::setImageFile(const QString &imageFile)
{
    m_yaml["image_file"] = imageFile.toStdString();
}

inline
void PlateFile::setImageWidth(int imageWidth)
{
    m_yaml["image_width"] = imageWidth;
}

inline
void PlateFile::setImageHeight(int imageHeight)
{
    m_yaml["image_height"] = imageHeight;
}

inline
QString PlateFile::regionCode() const
{
    return m_yaml["region_code_gt"]
            ? QString::fromUtf8(m_yaml["region_code_gt"].as<std::string>().c_str())
            : QString();
}

inline
void PlateFile::setRegionCode(const QString &regionCode)
{
    if(regionCode.isEmpty())
    {
        if(m_yaml["region_code_gt"])
            m_yaml.remove("region_code_gt");
    }
    else
        m_yaml["region_code_gt"] = regionCode.toStdString();
}

inline
QString PlateFile::plateNumber() const
{
    return m_yaml["plate_number_gt"]
            ? QString::fromUtf8(m_yaml["plate_number_gt"].as<std::string>().c_str())
            : QString();
}

inline
void PlateFile::setPlateNumber(const QString &plateNumber)
{
    m_yaml["plate_number_gt"] = plateNumber.toStdString();
}

inline
QPolygon PlateFile::plateCorners() const
{
    return m_plateCorners;
}

inline
bool PlateFile::plateInverted() const
{
    return m_yaml["plate_inverted_gt"]
            ? m_yaml["plate_inverted_gt"].as<bool>()
            : false;
}

inline
void PlateFile::setPlateInverted(bool plateInverted)
{
    m_yaml["plate_inverted_gt"] = plateInverted;
}

#endif // PLATEFILE_H
