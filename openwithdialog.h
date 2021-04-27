#ifndef OPENWITHDIALOG_H
#define OPENWITHDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class OpenWithDialog; }
QT_END_NAMESPACE

class OpenWithDialog : public QDialog
{
    Q_OBJECT

public:
    OpenWithDialog(QWidget *parent = nullptr);
    ~OpenWithDialog();

private:
    Ui::OpenWithDialog *ui;
};
#endif // OPENWITHDIALOG_H
