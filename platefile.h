#ifndef PLATEFILE_H
#define PLATEFILE_H

#include <QPolygon>
#include <QString>
#include <QList>

class QTextStream;
class QFileInfo;

class PlateFile;
using PlateFileList = QList<PlateFile>;

class PlateFile
{
public:
    PlateFile(const QString &platePathTemplate, int index);

    /*
     *  Overwrite a plate file with index 'index'
     */
    bool writeToFile(int index, QString *error = nullptr) const;

    /*
     *  Plate file template like "/data/car" from "/data/car.jpg"
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
    QString m_plateFileTemplate;
    QString m_plateFile;
    QString m_imageFile;
    int m_imageWidth;
    int m_imageHeight;
    QString m_regionCode;
    QString m_plateNumber;
    QPolygon m_plateCorners;
    bool m_plateInverted;
};

inline
QString PlateFile::plateFileTemplate() const
{
    return m_plateFileTemplate;
}

inline
bool PlateFile::isValid() const
{
    return m_imageWidth > 0
            && m_imageHeight > 0
            && !m_plateCorners.isEmpty();
}

inline
void PlateFile::setImageFile(const QString &imageFile)
{
    m_imageFile = imageFile;
}

inline
void PlateFile::setImageWidth(int imageWidth)
{
    m_imageWidth = imageWidth;
}

inline
void PlateFile::setImageHeight(int imageHeight)
{
    m_imageHeight = imageHeight;
}

inline
QString PlateFile::regionCode() const
{
    return m_regionCode;
}

inline
void PlateFile::setRegionCode(const QString &regionCode)
{
    m_regionCode = regionCode;
}

inline
QString PlateFile::plateNumber() const
{
    return m_plateNumber;
}

inline
void PlateFile::setPlateNumber(const QString &plateNumber)
{
    m_plateNumber = plateNumber;
}

inline
QPolygon PlateFile::plateCorners() const
{
    return m_plateCorners;
}

inline
void PlateFile::setPlateCorners(const QPolygon &plateCorners)
{
    m_plateCorners = plateCorners;
}

inline
bool PlateFile::plateInverted() const
{
    return m_plateInverted;
}

inline
void PlateFile::setPlateInverted(bool plateInverted)
{
    m_plateInverted = plateInverted;
}

#endif // PLATEFILE_H
