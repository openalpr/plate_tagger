#include <QtWidgets>
#include <QtCore>

#include "plateselector.h"
#include "platefile.h"
#include "utils.h"
#include "ui_plateselector.h"

PlateSelector::PlateSelector(const QImage &image, DeleteButtonExistense deleteButtonExistense, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PlateSelector)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint);

    ui->imageViewerPlateSelector->setImage(image);

    ui->plainTextPlateNumber->ensurePolished();
    ui->plainTextPlateNumber->setFixedHeight(
                (ui->plainTextPlateNumber->document()->documentMargin()
                 + QFontMetrics(ui->plainTextPlateNumber->font()).lineSpacing()) * 2);

    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
    ui->buttonBox->button(QDialogButtonBox::Reset)->setText(tr("Reset corners"));
    ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)->setText(tr("Delete && Save"));

    // shortcuts
    ui->buttonBox->button(QDialogButtonBox::Reset)->setShortcut(Qt::CTRL+Qt::Key_Q);
    ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)->setShortcut(Qt::CTRL+Qt::Key_D);
    ui->buttonBox->button(QDialogButtonBox::Save)->setShortcut(Qt::CTRL+Qt::Key_S);
    ui->buttonBox->button(QDialogButtonBox::Help)->setShortcut(QKeySequence::HelpContents);

    m_shortCutPlateNumber = new QShortcut(Qt::CTRL+Qt::Key_N, ui->plainTextPlateNumber, SLOT(setFocus()));
    m_shortCutPlateRegion = new QShortcut(Qt::CTRL+Qt::Key_R, ui->linePlateRegion,      SLOT(setFocus()));
    m_shortCutLightOnDark = new QShortcut(Qt::CTRL+Qt::Key_L, ui->checkLightOnDark,     SLOT(toggle()));

    // set tooltips
    Utils::setShortcutTooltips(QList<QPushButton *>()
                               << ui->buttonBox->button(QDialogButtonBox::Reset)
                               << ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)
                               << ui->buttonBox->button(QDialogButtonBox::Save)
                               << ui->buttonBox->button(QDialogButtonBox::Help));

    connect(ui->imageViewerPlateSelector, SIGNAL(dotsChanged(int)), this, SLOT(slotCheckData()));

    if(deleteButtonExistense == WithoutDeleteButton)
    {
        ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)->setEnabled(false);
        ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)->hide();
    }
}

PlateSelector::~PlateSelector()
{
    delete ui;
}

void PlateSelector::setPlateFile(const PlateFile &plateFile)
{
    qDebug() << "Existing polygon:" << plateFile.plateCorners();

    foreach(const QPoint &point, plateFile.plateCorners())
    {
        ui->imageViewerPlateSelector->addDotInImageCoordinates(point - m_selection.topLeft());
    }

    ui->plainTextPlateNumber->setPlainText(plateFile.plateNumber());
    ui->linePlateRegion->setText(plateFile.regionCode());
    ui->checkLightOnDark->setChecked(plateFile.plateInverted());
}

QString PlateSelector::plateNumber() const
{
    QString result;
    const QStringList plainLines = ui->plainTextPlateNumber->toPlainText().trimmed().split('\n', QString::SkipEmptyParts);

    foreach(QString plainLine, plainLines)
    {
        plainLine = plainLine.trimmed();

        if(!plainLine.isEmpty())
            result += plainLine + '\n';
    }

    // cut the last '\n'
    result.chop(1);

    return result;
}

QString PlateSelector::plateRegion() const
{
    return ui->linePlateRegion->text().trimmed();
}

bool PlateSelector::lightOnDark() const
{
    return ui->checkLightOnDark->isChecked();
}

void PlateSelector::slotCheckData()
{
    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(
                !plateNumber().isEmpty()
                && !ui->imageViewerPlateSelector->selectedPolygon().isEmpty()
                );
}

void PlateSelector::slotSave()
{
    m_selectedPolygon = ui->imageViewerPlateSelector->selectedPolygon();

    for(int i = 0;i < m_selectedPolygon.size();i++)
    {
        m_selectedPolygon[i] += m_selection.topLeft();
    }

    qDebug() << "Selected polygon:" << m_selectedPolygon;

    accept();
}

void PlateSelector::slotHelp()
{
    qDebug("Help");

    Utils::help(tr("Click the four corners of the license plate. "
                   "The lines should align with the edge of the license plate.<br><br>"
                   "Required fields:"
                   "<ul>"
                   "<li>Plate corners - the four corners of the license plate in the image"
                   "<li>Plate number - the plate number of the license plate"
                   "</ul>"
                   "Optional:"
                   "<ul>"
                   "<li>Plate region - the country/state of issuance (e.g. California)"
                   "<li>Light-on-Dark letters - the plate background is darker than the letters (e.g. white characters on a dark blue background)"
                   "</ul>"
                   "Hotkeys:"
                   "<ul>"
                   "<li>Tab - Move to the next field"
                   "<li>%1 - Jump to the plate number field"
                   "<li>%2 - Jump to the plate region field"
                   "<li>%3 - Toggle Light-on-Dark letters"
                   "<li>%4 - Reset corners"
                   "<li>%5 - Delete the corners and save"
                   "<li>%6 - Save"
                   "<li>Escape - Cancel"
                   "<li>%7 - Help"
                   "</ul>")
                .arg(m_shortCutPlateNumber->key().toString())
                .arg(m_shortCutPlateRegion->key().toString())
                .arg(m_shortCutLightOnDark->key().toString())
                .arg(ui->buttonBox->button(QDialogButtonBox::Reset)->shortcut().toString())
                .arg(ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)->shortcut().toString())
                .arg(ui->buttonBox->button(QDialogButtonBox::Save)->shortcut().toString())
                .arg(ui->buttonBox->button(QDialogButtonBox::Help)->shortcut().toString())
                , this);
}

void PlateSelector::slotClicked(QAbstractButton *button)
{
    // reset
    if(ui->buttonBox->button(QDialogButtonBox::Reset) == button)
    {
        qDebug("Reset corners");
        ui->imageViewerPlateSelector->clearDots();
    }
    // delete
    else if(ui->buttonBox->button(QDialogButtonBox::RestoreDefaults) == button)
    {
        qDebug("Delete plate");
        ui->imageViewerPlateSelector->clearDots();
        accept();
    }
}
