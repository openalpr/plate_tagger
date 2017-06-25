#include <QtWidgets>
#include <QtGui>

#include "imageviewerobserver.h"
#include "plateselector.h"
#include "imagefile.h"
#include "settings.h"
#include "utils.h"

ImageViewerObserver::ImageViewerObserver(QWidget *parent)
    : ImageViewerBase(parent)
    , m_imageFile(nullptr)
    , m_plateFileIndexForEditing(-1)
{}

void ImageViewerObserver::setImageFile(ImageFile *imageFile)
{
    // save image info
    m_imageFile = imageFile;

    // saved license plates
    m_polygons = Polygons::fromPlates(m_imageFile->plates());
}

void ImageViewerObserver::reset()
{
    m_imageFile = nullptr;
    m_polygons.clear();

    ImageViewerBase::reset();
}

void ImageViewerObserver::mousePressEvent(QMouseEvent *e)
{
    e->accept();

    // start to paint a selection rectangle
    m_selection.reset();

    // the point is outside the image or nothing to select
    if(pixmap().isNull())
        return;

    m_selection.start(e->pos());
    update();
}

void ImageViewerObserver::mouseMoveEvent(QMouseEvent *e)
{
    e->accept();

    // no selection or the point is outside the image
    if(!m_selection.hasSelection())
        return;

    m_selection.move(e->pos());
    update();
}

void ImageViewerObserver::mouseReleaseEvent(QMouseEvent *e)
{
    e->accept();

    // no selection/selection is too small, or the point is outside the image
    if(!m_selection.selectionIsFine())
    {
        QPoint pos = (e->pos() - pixmapBoundingRect().topLeft()) / zoom();

        // handle click
        for(int i = 0;i < m_imageFile->plates().size();i++)
        {
            const PlateFile &plateFile = m_imageFile->plates().at(i);

            if(plateFile.plateCorners().containsPoint(pos, Qt::OddEvenFill))
            {
                m_fixedSelection = Utils::selectionForPlate(plateFile, image());

                m_plateFileIndexForEditing = i;

                QTimer::singleShot(0, this, SLOT(slotShowPlateSelector()));
                break;
            }
        }

        return;
    }

    m_selection.stop();

    m_fixedSelection = m_selection.selection();

    // relative to the image itself
    m_fixedSelection.moveTopLeft(m_fixedSelection.topLeft() - pixmapBoundingRect().topLeft());

    // take zoom into account
    m_fixedSelection.moveTopLeft(m_fixedSelection.topLeft() * (1 / zoom()));
    m_fixedSelection.setSize(m_fixedSelection.size() * (1 / zoom()));

    // fix possible division errors:
    //  - negative coordinates
    //  - too large size
    m_fixedSelection &= image().rect();

    if(!m_fixedSelection.isValid())
    {
        qWarning() << "Selection rectangle" << m_fixedSelection << "is invalid for image size" << image().size();
        return;
    }

    qDebug() << "Actual selection: " << m_fixedSelection;

    m_plateFileIndexForEditing = -1;

    QTimer::singleShot(0, this, SLOT(slotShowPlateSelector()));

    update();
}

bool ImageViewerObserver::event(QEvent *event)
{
    if(event->type() == QEvent::ToolTip && SETTINGS_GET_BOOL(SETTING_TOOLTIPS))
    {
        const QPoint &pos = (mapFromGlobal(QCursor::pos()) - pixmapBoundingRect().topLeft()) / zoom();

        // handle click
        foreach(const PlateFile &plateFile, m_imageFile->plates())
        {
            if(plateFile.plateCorners().containsPoint(pos, Qt::OddEvenFill))
            {
                QToolTip::showText(QCursor::pos(),
                                   QString(
                                       // "<table cellspacing=5>"
                                       //"<tr><td><b>%1</b></td><td>%2</td></tr>"
                                       //"<tr><td><b>%3</b></td><td>%4</td></tr>"
                                       //"<tr><td><b>%5</b></td><td>%6</td></tr>"
                                       // "</table>"
                                       "<b>%1</b>&nbsp;&nbsp;%2<br>"
                                       "<b>%3</b>&nbsp;&nbsp;%4<br>"
                                       "<b>%5</b>&nbsp;&nbsp;%6"
                                       )
                                   .arg(tr("Plate number:"))
                                   .arg(plateFile.plateNumber())
                                   .arg(tr("Plate region:"))
                                   .arg(plateFile.regionCode())
                                   .arg(tr("Light-on-Dark:"))
                                   .arg(plateFile.plateInverted() ? tr("yes") : tr("no")),
                                   nullptr,
                                   QRect());
                break;
            }
        }

        event->accept();
        return true;
    }

    return QWidget::event(event);
}

void ImageViewerObserver::pixmapAboutToBeChanged()
{
    m_selection.reset();
}

void ImageViewerObserver::pixmapChanged()
{
    m_selection.setReferenceRect(pixmapBoundingRect());
    m_polygons.setZoom(zoom());
}

void ImageViewerObserver::draw(QPainter *painter)
{
    if(!painter)
        return;

    painter->translate(pixmapBoundingRect().topLeft());
    m_polygons.draw(painter);

    painter->resetTransform();
    m_selection.draw(painter);
}

void ImageViewerObserver::slotShowPlateSelector()
{
    qDebug() << "Image selection:" << m_fixedSelection;

    const QImage &frame = image().copy(m_fixedSelection);

    if(frame.isNull())
    {
        qWarning() << "Cannot edit a null image from region" << m_fixedSelection;
        return;
    }

    PlateSelector selector(frame, PlateSelector::WithDeleteButton, this);

    selector.setSelectionRect(m_fixedSelection);

    if(m_plateFileIndexForEditing >= 0)
        selector.setPlateFile(m_imageFile->plates().at(m_plateFileIndexForEditing));

    if(selector.exec() != PlateSelector::Accepted)
        return;

    PlateFileList plateFileList = m_imageFile->plates();
    PlateFile *plateFile = nullptr;

    // existing plate
    if(m_plateFileIndexForEditing >= 0)
    {
        qDebug("Editing existing plate index %d", m_plateFileIndexForEditing);

        // empty polygon -> delete it
        if(selector.selectedPolygon().isEmpty())
            plateFileList.removeAt(m_plateFileIndexForEditing);
        else
            plateFile = &plateFileList[m_plateFileIndexForEditing];
    }
    // new non-empty plate
    else if(!selector.selectedPolygon().isEmpty())
    {
        qDebug("Adding new plate with index %d", plateFileList.size());

        PlateFile newPlateFile(m_imageFile->plateNameTemplate(), plateFileList.size());

        newPlateFile.setImageFile(m_imageFile->fileInfo().fileName());
        newPlateFile.setImageWidth(image().width());
        newPlateFile.setImageHeight(image().height());

        plateFileList.append(newPlateFile);
        plateFile = &plateFileList.last();
    }
    // nothing has happend, just return
    else
        return;

    // update with entered data
    if(plateFile)
    {
        plateFile->setPlateCorners(selector.selectedPolygon());
        plateFile->setRegionCode(selector.plateRegion());
        plateFile->setPlateNumber(selector.plateNumber());
        plateFile->setPlateInverted(selector.lightOnDark());
    }

    m_imageFile->setPlates(plateFileList);

    emit platesChanged();

    m_polygons = Polygons::fromPlates(m_imageFile->plates());
    m_polygons.setZoom(zoom());

    update();
}
