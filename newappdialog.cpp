#include "newappdialog.h"
#include "ui_newappdialog.h"

NewAppDialog::NewAppDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewAppDialog)
{
    ui->setupUi(this);
}

NewAppDialog::~NewAppDialog()
{
    delete ui;
}
