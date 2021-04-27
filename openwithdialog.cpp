#include "openwithdialog.h"
#include "ui_openwithdialog.h"

OpenWithDialog::OpenWithDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OpenWithDialog)
{
    ui->setupUi(this);
}

OpenWithDialog::~OpenWithDialog()
{
    delete ui;
}

