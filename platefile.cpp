#include <QtCore>

#include "platefile.h"

PlateFile::PlateFile(const QString &platePathTemplate, int index)
    : m_plateFileTemplate(platePathTemplate)
    , m_plateFile(platePathTemplate + QString("-%1.yaml").arg(index))
    , m_imageWidth(0)
    , m_imageHeight(0)
    , m_plateInverted(false)
{
    // very simple YAML parser
    QFile file(m_plateFile);

    if(!file.open(QFile::ReadOnly))
        return;

    QByteArray line;

    while(!file.atEnd())
    {
        line = file.readLine();

        // nothing to parse
        if(line.isEmpty())
            continue;

        QTextStream ts(&line, QIODevice::ReadOnly);
        ts.setCodec("UTF-8");

        QByteArray key;

        ts >> key;

        if(key == "image_file:")
        {} // nothing, we set image_file explicitely when saving
        else if(key == "image_width:")
            ts >> m_imageWidth;
        else if(key == "image_height:")
            ts >> m_imageHeight;
        else if(key == "region_code_gt:")
            ts >> m_regionCode;
        else if(key == "plate_number_gt:")
        {
            QString text;
            ts >> text;

            qDebug() << text;

            // literal multiline
            if(text == "|")
            {
                while(!file.atEnd())
                {
                    line = file.readLine().trimmed();

                    if(line.isEmpty())
                        break;

                    m_plateNumber.append(QString::fromUtf8(line) + '\n');
                }

                // cut the last \n
                m_plateNumber.chop(1);
            }
            else
                m_plateNumber = text;
        }
        else if(key == "plate_corners_gt:")
        {
            int x, y;
            QPolygon polygon;

            while(true)
            {
                ts.skipWhiteSpace();

                if(ts.atEnd())
                    break;

                ts >> x >> y;
                polygon.append(QPoint(x, y));
            }

            if(polygon.size() == 4)
                m_plateCorners = polygon;
            else
                qWarning("Polygon in file '%s' has only %d point(s)", qPrintable(m_plateFile), polygon.size());
        }
        else if(key == "plate_inverted_gt:")
        {
            QString boolString;
            ts >> boolString;
            m_plateInverted = (boolString == "true");
        }
    }
}

bool PlateFile::writeToFile(int index, QString *error) const
{
    // very simple YAML writer
    QFile file(m_plateFileTemplate + QString("-%1.yaml").arg(index));

    if(!file.open(QFile::WriteOnly | QFile::Truncate))
    {
        if(error)
            *error = file.errorString();

        return false;
    }

    QTextStream ts(&file);

    ts.setCodec("UTF-8");

    QStringList cornersStringList;

    cornersStringList.reserve(m_plateCorners.size() * 2);

    foreach(const QPoint &point, m_plateCorners)
    {
        cornersStringList.append(QString::number(point.x()));
        cornersStringList.append(QString::number(point.y()));
    }

    ts
        << "image_file: " << m_imageFile << endl
        << "image_width: " << m_imageWidth << endl
        << "image_height: "  << m_imageHeight << endl;

    // don't save an empty region code
    if(!m_regionCode.isEmpty())
        ts << "region_code_gt: " << m_regionCode << endl;

    // write with new lines or not?
    if(m_plateNumber.contains('\n'))
    {
        /*
         * literal style:
         *
         * plate_number_gt: |
         *   plate number
         *   with
         *   new lines
         * <empty line>
         */
        const QStringList &lines = m_plateNumber.split('\n');

        ts << "plate_number_gt: " << "|" << endl;

        // write line-by-line and ignore empty lines
        foreach(const QString &line, lines)
        {
            ts << ' ' << line << endl;
        }

        ts << endl;
    }
    else
        ts << "plate_number_gt: " << m_plateNumber << endl;

    ts
        << "plate_corners_gt: " << cornersStringList.join(" ") << endl
        << "plate_inverted_gt: " << (m_plateInverted ? "true" : "false") << endl
               ;

    ts.flush();

    if(ts.status() != QTextStream::Ok)
    {
        if(error)
            *error = QObject::tr("Cannot write into a file");

        return false;
    }

    return true;
}

bool PlateFile::exists()
{
    return QFileInfo(m_plateFile).exists();
}

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
