#include <QtWidgets>
#include <QtGui>
#include <QtCore>

#include "imageviewerplateselector.h"
#include "utils.h"
#include "dot.h"

constexpr int DOT_WIDTH = 14;
constexpr int DOT_HEIGHT = 14;

Dot::Dot(QWidget *parent)
    : QWidget(parent)
    , inMove(false)
{
    setMouseTracking(true);
    setFixedSize(DOT_WIDTH, DOT_HEIGHT);
    show();
}

void Dot::mousePressEvent(QMouseEvent *e)
{
    e->accept();

    m_lastMousePoint = e->pos();
    inMove = true;
}

void Dot::mouseMoveEvent(QMouseEvent *e)
{
    e->accept();

    if(!inMove)
        return;

    dragTo(e->pos());
}

void Dot::mouseReleaseEvent(QMouseEvent *e)
{
    e->accept();

    dragTo(e->pos());

    emit moved();

    inMove = false;
}

void Dot::dragTo(const QPoint &pos)
{
    ImageViewerPlateSelector *plateSelectorParent = qobject_cast<ImageViewerPlateSelector *>(parent());

    if(!plateSelectorParent)
    {
        qWarning("Parent must be of class ImageViewerPlateSelector");
        return;
    }

    const QPoint &targetPos = this->pos() + (pos - m_lastMousePoint);

    if(plateSelectorParent->acceptableDotPosition(QRect(targetPos, size())))
    {
        move(targetPos);
        emit moving();
    }
}

void Dot::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    painter.setClipRegion(e->region());
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Utils::polygonPaintingPen());
    painter.setBrush(painter.pen().brush());

    // squeeze the ellipse by the pen width
    painter.drawEllipse(painter.pen().width(),
                        painter.pen().width(),
                        width() - painter.pen().width()*2,
                        height() - painter.pen().width()*2);
}
