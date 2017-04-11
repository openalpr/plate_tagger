#ifndef SELECTION_H
#define SELECTION_H

#include <QRect>

class QPainter;

/*
 *  Rectangular selection painter
 */
class Selection
{
public:
    Selection();

    /*
     *  Actual selection rectangle
     */
    QRect selection() const;

    /*
     *  In selection mode
     */
    bool hasSelection() const;

    /*
     *  Has selection and it's enought large
     */
    bool selectionIsFine() const;

    /*
     *  Reset to inital state
     */
    void reset();

    /*
     *  Relative rectangle
     */
    void setReferenceRect(const QRect &rect);

    /*
     *  Start painting a selection rectangle from the given position
     */
    void start(const QPoint &pos);

    /*
     *  Move the selection rectangle
     */
    void move(const QPoint &pos);

    /*
     *  Stop painting selection
     */
    void stop();

    /*
     *  Paint selection using the specified painter
     */
    void draw(QPainter *painter);

private:
    QRect m_referenceRect;
    QRect m_selection;
    bool m_hasSelection;
};

inline
QRect Selection::selection() const
{
    return m_selection;
}

inline
bool Selection::hasSelection() const
{
    return m_hasSelection;
}

inline
void Selection::setReferenceRect(const QRect &rect)
{
    m_referenceRect = rect;
}

inline
void Selection::stop()
{
    m_hasSelection = false;
    m_selection = m_selection.normalized();
}

#endif // SELECTION_H
