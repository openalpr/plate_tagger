#ifndef DOT_H
#define DOT_H

#include <QWidget>
#include <QPoint>

/*
 *  Represents a single polygon dot in a plate selector
 */
class Dot : public QWidget
{
    Q_OBJECT

public:
    Dot(QWidget *parent = nullptr);

    /*
     *  Center point of the dot in parent's coordinates
     */
    void setCenterPoint(const QPoint &center);

    /*
     *  Center point
     */
    QPoint centerPoint() const;

    /*
     *  Appropriate not scaled point in the original image in pixels
     */
    void setPointInImage(const QPoint &point);
    QPoint pointInImage() const;

protected:
    virtual void mousePressEvent(QMouseEvent *e) override;
    virtual void mouseMoveEvent(QMouseEvent *e) override;
    virtual void mouseReleaseEvent(QMouseEvent *e) override;
    virtual void paintEvent(QPaintEvent *e) override;

private:
    QPoint relativeCenter() const;
    void dragTo(const QPoint &pos);

signals:
    void moving();
    void moved();

private:
    QPoint m_lastMousePoint;
    QPoint m_pointInImage;
    bool inMove;
};

inline
void Dot::setCenterPoint(const QPoint &center)
{
    move(center - relativeCenter());
}

inline
QPoint Dot::centerPoint() const
{
    return pos() + relativeCenter();
}

inline
void Dot::setPointInImage(const QPoint &point)
{
    m_pointInImage = point;
}

inline
QPoint Dot::pointInImage() const
{
    return m_pointInImage;
}

inline
QPoint Dot::relativeCenter() const
{
    return QPoint(width()/2, height()/2);
}

#endif // DOT_H
