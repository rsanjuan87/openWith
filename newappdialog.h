#ifndef NEWAPPDIALOG_H
#define NEWAPPDIALOG_H

#include <QDialog>
struct App {
public:
    QString name;
    QString icon;
    QString path;
};

namespace Ui {
class NewAppDialog;
}

class NewAppDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewAppDialog(App app, QWidget *parent = nullptr);
    ~NewAppDialog();
    inline App getApp(){return app;}
#ifdef Q_OS_MAC
    static App locateApp();
#endif

protected slots:
    void setIco(QString ico);
    void setExec(QString exec);
    void setName(QString name);
private slots:

    void on_btn1_clicked();

    void init();
    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

private:
    Ui::NewAppDialog *ui;
    App app;
};

#endif // NEWAPPDIALOG_H
