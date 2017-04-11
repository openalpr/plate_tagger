#include <QtCore>
#include <QtGui>

#include "selection.h"
#include "utils.h"

constexpr int SELECTION_MIN_WIDTH = 6;
constexpr int SELECTION_MIN_HEIGHT = 6;

Selection::Selection()
    : m_hasSelection(false)
{}

bool Selection::selectionIsFine() const
{
    const QRect &selection = m_selection.normalized();
    return m_hasSelection && selection.width() > SELECTION_MIN_WIDTH && selection.height() > SELECTION_MIN_HEIGHT;
}

void Selection::reset()
{
    m_hasSelection = false;
    m_selection = QRect();
}

void Selection::start(const QPoint &pos)
{
    if(!m_referenceRect.contains(pos))
        return;

    m_hasSelection = true;
    m_selection = QRect(pos, pos);

    qDebug() << "Starting selection" << m_selection;
}

void Selection::move(const QPoint &pos)
{
    m_selection.setBottomRight(Utils::fitPoint(m_referenceRect, pos));
}

void Selection::draw(QPainter *painter)
{
    if(!painter)
        return;

    // selection must be normalized and checked against a minimal size
    const QRect &selection = m_selection.normalized();

    if(selectionIsFine())
    {
        painter->setCompositionMode(QPainter::RasterOp_SourceXorDestination);
        painter->setPen(Qt::white);
        painter->drawRect(selection);
    }
}
