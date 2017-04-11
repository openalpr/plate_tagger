#include <QtGui>

#include "polygons.h"
#include "utils.h"

Polygons::Polygons()
{}

void Polygons::setZoom(double zoom)
{
    m_scaledPolygons.clear();
    m_scaledPolygons.reserve(m_polygons.size());

    foreach(QPolygon polygon, m_polygons)
    {
        for(int i = 0;i < polygon.size();i++)
        {
            polygon.setPoint(i, polygon.point(i) * zoom);
        }

        m_scaledPolygons.append(polygon);
    }
}

void Polygons::draw(QPainter *painter)
{
    if(!painter)
        return;

    painter->setPen(Utils::polygonPaintingPen());

    foreach(const QPolygon &polygon, m_scaledPolygons)
    {
        painter->drawPolygon(polygon);
    }
}

Polygons Polygons::fromPlates(const PlateFileList &plateFileList)
{
    Polygons polygons;

    foreach(const PlateFile &plateFile, plateFileList)
    {
        polygons.append(plateFile.plateCorners());
    }

    return polygons;
}
