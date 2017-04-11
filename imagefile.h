#ifndef IMAGEFILE_H
#define IMAGEFILE_H

#include <QFileInfo>
#include <QList>

#include "platefile.h"

/*
 *  Represents a single image with associated YAML files
 *
 *  If an image file is "car.jpg", associated YAML files will be named
 *
 *  car-0.yaml
 *  car-1.yaml
 *  etc.
 */
class ImageFile
{
public:
    ImageFile();
    ImageFile(const QFileInfo &fileInfo);

    /*
     *  Image file info
     */
    const QFileInfo &fileInfo() const;

    /*
     *  Plate name template
     */

    QString plateNameTemplate() const;

    /*
     *  Associated plates
     */
    PlateFileList plates() const;

    /*
     *  Update plates
     */
    void setPlates(const PlateFileList &plateFileList);

    /*
     *  Image has been moved and its methods and properties
     *  should not be used anymore
     */
    bool moved() const;
    void setMoved(bool moved);

private:
    QFileInfo m_fileInfo;
    QString m_plateNameTemplate;
    PlateFileList m_plates;
    bool m_moved;
};

inline
const QFileInfo &ImageFile::fileInfo() const
{
    return m_fileInfo;
}

inline
QString ImageFile::plateNameTemplate() const
{
    return m_plateNameTemplate;
}

inline
PlateFileList ImageFile::plates() const
{
    return m_plates;
}

inline
bool ImageFile::moved() const
{
    return m_moved;
}

inline
void ImageFile::setMoved(bool moved)
{
    m_moved = moved;
}

using ImageFileList = QList<ImageFile>;

#endif // IMAGEFILE_H
