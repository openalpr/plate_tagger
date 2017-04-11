#ifndef IMAGEVIEWERBASE_H
#define IMAGEVIEWERBASE_H

#include <QPixmap>
#include <QLabel>
#include <QImage>
#include <QRect>

class QPainter;
class QTimer;

/*
 *  Base image viewer widget with auto scaling
 */
class ImageViewerBase : public QWidget
{
    Q_OBJECT

public:
    explicit ImageViewerBase(QWidget *parent = 0);
    virtual ~ImageViewerBase() = default;

    /*
     *  Display an image in the view
     */
    void setImage(const QImage &image);

    /*
     *  Current zoom
     */
    double zoom() const;

    /*
     *  Clear and reset view to initial state with no actual image set
     */
    virtual void reset();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

    /*
     *  Getters for derived classes
     */
    QPixmap pixmap() const;
    QImage image() const;
    QRect pixmapBoundingRect() const;

    /*
     *  Pixmap will be scaled right now!
     */
    virtual void pixmapAboutToBeChanged() = 0;

    /*
     *  Pixmap has been scaled and all the internal data has been updated
     */
    virtual void pixmapChanged() = 0;

    /*
     *  Draw viewer-specific stuff
     */
    virtual void draw(QPainter *painter) = 0;

signals:
    void zoomChanged(double);

private slots:
    void updatePixmap();

private:
    QTimer *m_timerDelayedUpdate;
    QImage m_image;
    QPixmap m_pixmap;
    double m_zoom;
    QRect m_pixmapBoundingRect;
};

inline
double ImageViewerBase::zoom() const
{
    return m_zoom;
}

inline
QPixmap ImageViewerBase::pixmap() const
{
    return m_pixmap;
}

inline
QImage ImageViewerBase::image() const
{
    return m_image;
}

inline
QRect ImageViewerBase::pixmapBoundingRect() const
{
    return m_pixmapBoundingRect;
}

#endif // IMAGEVIEWERBASE_H
