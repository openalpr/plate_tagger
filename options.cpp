#include "settings.h"
#include "options.h"
#include "ui_options.h"

Options::Options(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Options)
{
    ui->setupUi(this);

    // load settings
    ui->checkSkipTagged->setChecked(SETTINGS_GET_BOOL(SETTING_SKIP_TAGGED));
    ui->checkMove->setChecked(SETTINGS_GET_BOOL(SETTING_MOVE_UNTAGGED));
    ui->checkTooltips->setChecked(SETTINGS_GET_BOOL(SETTING_TOOLTIPS));
}

Options::~Options()
{
    delete ui;
}

void Options::slotAccept()
{
    SETTINGS_SET_BOOL(SETTING_SKIP_TAGGED, ui->checkSkipTagged->isChecked(), Settings::NoSync);
    SETTINGS_SET_BOOL(SETTING_MOVE_UNTAGGED, ui->checkMove->isChecked(), Settings::NoSync);
    SETTINGS_SET_BOOL(SETTING_TOOLTIPS, ui->checkTooltips->isChecked());

    accept();
}
