#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QStackedWidget>
#include <QString>

class QImage;

class ImageFile;

namespace Ui
{
    class ImageView;
}

class ThreadedImageLoader;

/*
 *  Image viewer preserving aspect ratio and supporting scaling.
 *
 *  Displays a spinning progress when loading a file. Utilizes ImageViewer class
 *  to display an actual image
 */
class ImageView : public QStackedWidget
{
    Q_OBJECT

public:
    explicit ImageView(QWidget *parent = 0);
    ~ImageView();

    /*
     *  Load the specified image
     */
    void setImageFile(ImageFile *imageFile);

    /*
     *  Is loading the specified image
     */
    bool busy() const;

    void reset();

private:
    void setImage(const QImage &image);

signals:
    void zoomChanged(double);
    void platesChanged();

private slots:
    void slotImageFileLoaded(const QImage &image);

private:
    Ui::ImageView *ui;
    ImageFile *m_imageFile;
    ThreadedImageLoader *m_threadedImageLoader;
};

#endif // IMAGEVIEW_H
