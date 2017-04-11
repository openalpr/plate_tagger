#include <QtGui>
#include <QtCore>
#include <QtMath>

#include <algorithm>

#include "imageviewerplateselector.h"
#include "plateselector.h"
#include "utils.h"
#include "dot.h"

ImageViewerPlateSelector::ImageViewerPlateSelector(QWidget *parent)
    : ImageViewerBase(parent)
{}

bool ImageViewerPlateSelector::acceptableDotPosition(const QRect &rect) const
{
    return rect.intersected(pixmapBoundingRect()) == rect;
}

bool ImageViewerPlateSelector::addDotInViewCoordinates(const QPoint &point)
{
    // only 4 dots are allowed
    if(m_dots.size() == 4)
        return false;

    // add new selection dot
    Dot *dot = new Dot(this);

    connect(dot, SIGNAL(moving()), this, SLOT(slotDotMoving()));
    connect(dot, SIGNAL(moved()), this, SLOT(slotDotMoved()));

    dot->setCenterPoint(point);
    updatePointInImage(dot);

    m_dots.append(dot);

    if(!reorderDots())
        update();

    emitDotsChanged();

    return true;
}

bool ImageViewerPlateSelector::addDotInImageCoordinates(const QPoint &point)
{
    qDebug() << "Add dot" << point;
    return addDotInViewCoordinates(pixmapBoundingRect().topLeft() + point * zoom());
}

void ImageViewerPlateSelector::clearDots()
{
    if(m_dots.isEmpty())
        return;

    qDeleteAll(m_dots);
    m_dots.clear();
    update();

    emitDotsChanged();
}

QPolygon ImageViewerPlateSelector::selectedPolygon() const
{
    QPolygon polygon;

    if(m_dots.size() != 4)
        return polygon;

    polygon.reserve(m_dots.size());

    foreach(Dot *dot, m_dots)
    {
        polygon.append(dot->pointInImage());
    }

    return polygon;
}

void ImageViewerPlateSelector::mousePressEvent(QMouseEvent *e)
{
    e->accept();
}

void ImageViewerPlateSelector::mouseMoveEvent(QMouseEvent *e)
{
    e->accept();
}

void ImageViewerPlateSelector::mouseReleaseEvent(QMouseEvent *e)
{
    e->accept();

    // not interesting click
    if(!pixmapBoundingRect().contains(e->pos()))
        return;

    addDotInViewCoordinates(e->pos());
}

void ImageViewerPlateSelector::pixmapAboutToBeChanged()
{}

void ImageViewerPlateSelector::pixmapChanged()
{
    foreach(Dot *dot, m_dots)
    {
        dot->setCenterPoint(pixmapBoundingRect().topLeft() + dot->pointInImage() * zoom());
    }
}

void ImageViewerPlateSelector::draw(QPainter *painter)
{
    if(!painter)
        return;

    // draw only a full polygon
    if(m_dots.size() == 4)
    {
        painter->setPen(Utils::polygonPaintingPen());

        QPolygon polygon;
        polygon.reserve(m_dots.size());

        foreach(Dot *dot, m_dots)
        {
            polygon.append(dot->centerPoint());
        }

        painter->drawPolygon(polygon);
    }
}

void ImageViewerPlateSelector::updatePointInImage(Dot *dot) const
{
    if(!dot)
        return;

    dot->setPointInImage((dot->centerPoint() - pixmapBoundingRect().topLeft()) / zoom());
}

void ImageViewerPlateSelector::emitDotsChanged()
{
    emit dotsChanged(m_dots.size());
}

bool ImageViewerPlateSelector::reorderDots()
{
    // sort dots to make a valid region
    if(m_dots.size() != 4)
        return false;

    QList<Dot *> newDots;
    newDots.reserve(m_dots.size());

    struct Point
    {
        int index;
        QPointF point;
    };

    const QPoint &centroid = Utils::centroid(selectedPolygon());
    const QPolygon &selectedPolygon = this->selectedPolygon();

    QVector<Point> polarPolygon;
    polarPolygon.reserve(selectedPolygon.size());

    for(int i = 0;i < selectedPolygon.size();i++)
    {
        const QPoint &relativeToCentroid = selectedPolygon.at(i) - centroid;

        // convert to polar:
        //     x == theta (angle)
        //     y == r (length)
        polarPolygon.append( { i, { qAtan2(relativeToCentroid.y(), relativeToCentroid.x()),
                                    qSqrt(qPow(relativeToCentroid.x(),2) + qPow(relativeToCentroid.y(),2)) } } );
    }

    std::sort(polarPolygon.begin(),
              polarPolygon.end(),
              [](const Point &a, const Point &b) {
        return (a.point.x() == b.point.x()) ? a.point.y() < b.point.y() : a.point.x() < b.point.x();
    });

    foreach(const Point &point, polarPolygon)
    {
        newDots.append(m_dots.at(point.index));
    }

    m_dots = newDots;

    update();

    return true;
}

void ImageViewerPlateSelector::slotDotMoving()
{
    Dot *dot = qobject_cast<Dot *>(sender());

    if(!dot)
        return;

    updatePointInImage(dot);
    update();
}

void ImageViewerPlateSelector::slotDotMoved()
{
    reorderDots();
}
