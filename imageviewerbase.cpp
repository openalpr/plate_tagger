#include <QtWidgets>
#include <QtGui>
#include <QtCore>

#include "imageviewerbase.h"

ImageViewerBase::ImageViewerBase(QWidget *parent)
    : QWidget(parent)
    , m_zoom(1)
{
    m_timerDelayedUpdate = new QTimer(this);
    m_timerDelayedUpdate->setSingleShot(true);
    m_timerDelayedUpdate->setInterval(10);
    connect(m_timerDelayedUpdate, SIGNAL(timeout()), this, SLOT(updatePixmap()));

    setMinimumSize(200, 120);
}

void ImageViewerBase::setImage(const QImage &image)
{
    m_timerDelayedUpdate->stop();
    m_image = image;

    updatePixmap();
}

void ImageViewerBase::reset()
{
    m_image = QImage();
    m_pixmap = QPixmap();
    m_pixmapBoundingRect = QRect();
    m_zoom = 1;

    update();
}

void ImageViewerBase::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)

    if(!m_timerDelayedUpdate->isActive())
        m_timerDelayedUpdate->start();
}

void ImageViewerBase::paintEvent(QPaintEvent *event)
{
    if(m_pixmap.isNull())
        return;

    QPainter painter(this);

    painter.setClipRegion(event->region());
    painter.setRenderHint(QPainter::Antialiasing);

    // car image
    painter.drawPixmap(m_pixmapBoundingRect.x(), m_pixmapBoundingRect.y(), m_pixmap);

    // custom paint in subclasses
    draw(&painter);
}

void ImageViewerBase::updatePixmap()
{
    pixmapAboutToBeChanged();

    m_pixmap = QPixmap();

    if(m_image.isNull())
        return;

    // scale with a higher quality
    m_pixmap = QPixmap::fromImage(m_image.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // calculate scaling
    m_zoom = static_cast<double>(m_pixmap.width()) / m_image.width();

    // pixmap bounding rectangle
    m_pixmapBoundingRect = QRect((width() - m_pixmap.width()) / 2,
                                 (height() - m_pixmap.height()) / 2,
                                 m_pixmap.width(),
                                 m_pixmap.height());

    pixmapChanged();

    emit zoomChanged(m_zoom);

    update();
}
