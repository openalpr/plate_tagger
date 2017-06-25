#ifndef PLATESELECTOR_H
#define PLATESELECTOR_H

#include <QPolygon>
#include <QDialog>

class QAbstractButton;
class QShortcut;

class PlateFile;

namespace Ui
{
    class PlateSelector;
}

/*
 *  Dialog to select or edit a license plate with four points
 */
class PlateSelector : public QDialog
{
    Q_OBJECT

public:
    enum DeleteButtonExistense { WithDeleteButton, WithoutDeleteButton };

    explicit PlateSelector(const QImage &image, DeleteButtonExistense deleteButtonExistense, QWidget *parent = 0);
    ~PlateSelector();

    /*
     *  Set an existing plate to edit
     */
    void setPlateFile(const PlateFile &plateFile);

    /*
     *  Selection rectangle in the original (full) image. This rectangle is used to re-calculate
     *  correct relative dot coordinates and pass them into addDotInImageCoordinates() method
     */
    void setSelectionRect(const QRect &rect);

    /*
     *  Getters
     */
    QString plateNumber() const;
    QString plateRegion() const;
    bool lightOnDark() const;

    QPolygon selectedPolygon() const;

private slots:
    void slotCheckData();
    void slotSave();
    void slotHelp();
    void slotClicked(QAbstractButton *button);

private:
    Ui::PlateSelector *ui;
    QRect m_selection;
    QPolygon m_selectedPolygon;
    QShortcut *m_shortCutPlateNumber;
    QShortcut *m_shortCutPlateRegion;
    QShortcut *m_shortCutLightOnDark;
};

inline
void PlateSelector::setSelectionRect(const QRect &rect)
{
    m_selection = rect;
}

inline
QPolygon PlateSelector::selectedPolygon() const
{
    return m_selectedPolygon;
}

#endif // PLATESELECTOR_H
