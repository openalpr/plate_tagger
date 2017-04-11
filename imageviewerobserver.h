#ifndef IMAGEVIEWEROBSERVER_H
#define IMAGEVIEWEROBSERVER_H

#include "imageviewerbase.h"
#include "selection.h"
#include "polygons.h"

class ImageFile;

/*
 *  Image viewer to display a car with static license plates
 */
class ImageViewerObserver : public ImageViewerBase
{
    Q_OBJECT

public:
    explicit ImageViewerObserver(QWidget *parent = nullptr);

    /*
     *  Add information with static license plates
     */
    void setImageFile(ImageFile *imageFile);

    /*
     *  Reset override
     */
    virtual void reset() override;

protected:
    /*
     *  For handling selection
     */
    virtual void mousePressEvent(QMouseEvent *e) override;
    virtual void mouseMoveEvent(QMouseEvent *e) override;
    virtual void mouseReleaseEvent(QMouseEvent *e) override;

    /*
     *  Custom tooltips
     */
    virtual bool event(QEvent *event) override;

    virtual void pixmapAboutToBeChanged() override;
    virtual void pixmapChanged() override;

    virtual void draw(QPainter *painter) override;

signals:
    void platesChanged();

private slots:
    void slotShowPlateSelector();

private:
    ImageFile *m_imageFile;
    Polygons m_polygons;
    Selection m_selection;
    QRect m_fixedSelection;
    int m_plateFileIndexForEditing;
};

#endif // IMAGEVIEWEROBSERVER_H
