#ifndef IMAGEVIEWERPLATESELECTOR_H
#define IMAGEVIEWERPLATESELECTOR_H

#include <QPolygon>
#include <QList>

#include "imageviewerbase.h"

class Dot;

/*
 *  Image viewer to display a car with static license plates
 */
class ImageViewerPlateSelector : public ImageViewerBase
{
    Q_OBJECT

public:
    explicit ImageViewerPlateSelector(QWidget *parent = nullptr);

    /*
     *  Check if the given dot geometry fits into the pixmap
     */
    bool acceptableDotPosition(const QRect &rect) const;

    /*
     *  Add a new dot at the given point
     */
    bool addDotInViewCoordinates(const QPoint &point);
    bool addDotInImageCoordinates(const QPoint &point);

    /*
     *  Remove all dots
     */
    void clearDots();

    /*
     *  Selection result as a polygon with 4 points. Each point is
     *  in image coordinates
     */
    QPolygon selectedPolygon() const;

protected:
    /*
     *  For handling selection
     */
    virtual void mousePressEvent(QMouseEvent *e) override;
    virtual void mouseMoveEvent(QMouseEvent *e) override;
    virtual void mouseReleaseEvent(QMouseEvent *e) override;

    virtual void pixmapAboutToBeChanged() override;
    virtual void pixmapChanged() override;

    virtual void draw(QPainter *painter) override;

private:
    void updatePointInImage(Dot *dot) const;
    void emitDotsChanged();
    bool reorderDots();

signals:
    void dotsChanged(int);

private slots:
    void slotDotMoving();
    void slotDotMoved();

private:
    // selection dots
    QList<Dot *> m_dots;
};

#endif // IMAGEVIEWERPLATESELECTOR_H
