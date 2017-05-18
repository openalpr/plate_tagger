#include <QtWidgets>
#include <QtGui>
#include <QtCore>

#include <cstdlib>

#include "platefile.h"
#include "utils.h"

constexpr double BOUNDING_ENLARGE_WIDTH_FACTOR = 0.15;
constexpr double BOUNDING_ENLARGE_HEIGHT_FACTOR = 0.25;

QString Utils::helpString()
{
    return QObject::tr("Help");
}

QString Utils::informationString()
{
    return QObject::tr("Information");
}

QString Utils::warningString()
{
    return QObject::tr("Warning");
}

QString Utils::errorString()
{
    return QObject::tr("Error");
}

QString Utils::fatalErrorString()
{
    return QObject::tr("Fatal error");
}

QPoint Utils::centroid(const QPolygon &polygon)
{
    QPoint centroid{0,0};

    foreach(const auto &point, polygon)
    {
        centroid.rx() += point.x();
        centroid.ry() += point.y();
    }

    return centroid / polygon.size();
}

void Utils::setShortcutTooltips(const QList<QPushButton *> &buttons)
{
    // set tooltips
    foreach(auto *button, buttons)
    {
        button->setToolTip(QObject::tr("Shortcut: %1").arg(button->shortcut().toString()));
    }
}

QStringList Utils::imageMatchingWildcard()
{
    return QStringList()
            << "*.jpg"
            << "*.JPG"
            << "*.jpeg"
            << "*.JPEG"
            << "*.png"
            << "*.PNG"
               ;
}

QPoint Utils::fitPoint(const QRect &m_referenceRect, const QPoint &_pos)
{
    QPoint pos = _pos;

    if(pos.x() > m_referenceRect.topRight().x())
        pos.setX(m_referenceRect.topRight().x());
    else if(pos.x() < m_referenceRect.topLeft().x())
        pos.setX(m_referenceRect.topLeft().x());

    if(pos.y() > m_referenceRect.bottomLeft().y())
        pos.setY(m_referenceRect.bottomLeft().y());
    else if(pos.y() < m_referenceRect.topLeft().y())
        pos.setY(m_referenceRect.topLeft().y());

    return pos;
}

QRect Utils::selectionForPlate(const PlateFile &plateFile, const QImage &inImage)
{
    const QRect &boundingRect = plateFile.plateCorners().boundingRect();

    // enlarge
    QRect selection = boundingRect.adjusted(-BOUNDING_ENLARGE_WIDTH_FACTOR * boundingRect.width(),
                                            -BOUNDING_ENLARGE_HEIGHT_FACTOR * boundingRect.height(),
                                            BOUNDING_ENLARGE_WIDTH_FACTOR * boundingRect.width(),
                                            BOUNDING_ENLARGE_HEIGHT_FACTOR * boundingRect.height());

    selection &= inImage.rect();

    return selection;
}

void Utils::help(const QString &message, QWidget *parent)
{
    QMessageBox::information(parent, Utils::helpString(), message);
}

void Utils::message(const QString &message, QWidget *parent)
{
    QMessageBox::information(parent, Utils::informationString(), message);
}

void Utils::warning(const QString &message, QWidget *parent)
{
    QMessageBox::warning(parent, Utils::warningString(), message);
}

void Utils::error(const QString &message, QWidget *parent)
{
    QMessageBox::critical(parent, Utils::errorString(), message);
}

void Utils::fatalError(const QString &message, QWidget *parent)
{
    QMessageBox::critical(parent, Utils::fatalErrorString(), message);
    ::exit(1);
}
