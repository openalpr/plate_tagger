#ifndef POLYGONS_H
#define POLYGONS_H

#include <QPolygon>
#include <QList>

#include "platefile.h"

class QPainter;

/*
 *  Class to hold static license plates polygons and draw them
 */
class Polygons
{
public:
    Polygons();

    /*
     *  QList proxy methods
     */
    void append(const QPolygon &polygon);
    void clear();

    /*
     *  Scale the currently saved polygons
     */
    void setZoom(double zoom);

    /*
     *  Draw currently saved polygons with the given painter
     */
    void draw(QPainter *painter);

    /*
     *  Constructs a 'Polygons' object from saved license plates
     */
    static Polygons fromPlates(const PlateFileList &plateFileList);

private:
    QList<QPolygon> m_polygons;
    QList<QPolygon> m_scaledPolygons;
};

inline
void Polygons::append(const QPolygon &polygon)
{
    m_polygons.append(polygon);
}

inline
void Polygons::clear()
{
    m_polygons.clear();
}

#endif // POLYGONS_H
