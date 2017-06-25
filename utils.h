#ifndef UTILS_H
#define UTILS_H

#include <QStringList>
#include <QString>
#include <QPoint>
#include <QList>
#include <QPen>

class QPushButton;
class QPolygon;
class QWidget;
class QImage;
class QRect;

class PlateFile;

class Utils
{
public:
    static QString helpString();
    static QString informationString();
    static QString warningString();
    static QString errorString();
    static QString fatalErrorString();

    /*
     *  Pen to pain a polygon
     */
    static QPen polygonPaintingPen();

    /*
     *  Calculate a polygon centroid
     */
    static QPoint centroid(const QPolygon &polygon);

    /*
     *  Set tooltips for buttons with their shortcuts
     */
    static void setShortcutTooltips(const QList<QPushButton *> &buttons);

    /*
     *  Supported image formats as a UNIX wildcard
     */
    static QStringList imageMatchingWildcard();

    static QPoint fitPoint(const QRect &m_referenceRect, const QPoint &_pos);

    /*
     *  Return a selection rectangle to edit the specified plate
     */
    static QRect selectionForPlate(const PlateFile &plateFile, const QImage &inImage);

    static void help(const QString &message, QWidget *parent = nullptr);
    static void message(const QString &message, QWidget *parent = nullptr);
    static void warning(const QString &message, QWidget *parent = nullptr);
    static void error(const QString &message, QWidget *parent = nullptr);
    static void fatalError(const QString &message, QWidget *parent = nullptr);
};

inline
QPen Utils::polygonPaintingPen()
{
    return QPen(Qt::magenta, 2);
}

#endif // UTILS_H
