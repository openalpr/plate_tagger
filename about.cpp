#include <QtGui>

#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::About)
{
    ui->setupUi(this);

    ui->labelProduct->setText(TARGET_HUMAN_STRING);
    ui->labelVersion->setText(tr("Version %1").arg(VERSION_STRING));
    ui->labelIcon->setPixmap(QIcon(":/images/" TARGET_STRING ".ico").pixmap(48, 48));

    // %1 will be replaced with a product name
    ui->labelWarranty->setText(tr("%1 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.")
                               .arg(TARGET_HUMAN_STRING));

#ifdef Q_OS_MAC
    ui->pushOK->hide();
#else
    setWindowTitle(tr("About"));
#endif

    adjustSize();
}

About::~About()
{
    delete ui;
}
