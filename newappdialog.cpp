#include "newappdialog.h"
#include "ui_newappdialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>

#include <QDebug>

NewAppDialog::NewAppDialog(App app, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewAppDialog)
{
    ui->setupUi(this);
    this->app = app;
    init();
}

void NewAppDialog::init(){
    ui->lineEdit->setText(app.name);
    ui->lineEdit_2->setText(app.path);
    ui->lineEdit_3->setText(app.icon);
    ui->btn1->setText("(+) "+app.name);
    ui->btn1->setIcon(QIcon(app.icon));
}

NewAppDialog::~NewAppDialog()
{
    delete ui;
}

App NewAppDialog::locateApp()
{
    App app;
    app.path = QFileDialog::getOpenFileName(0, tr("Open File"),
                                 "/Applications",
                                 tr("App (*.app)"));
    if(app.path.isEmpty()){
        return App();
    }
    app.name = QFileInfo(app.path).fileName();
    QProcess reader;
    reader.setReadChannelMode(QProcess::MergedChannels);

    ///name CFBundleName
    reader.start("defaults", QStringList()<<"read"<<app.path+"/Contents/Info.plist"<<"CFBundleName");
    reader.waitForFinished();
    app.name = reader.readAll().trimmed();
    //qDebug()<<"name" << app.name;

    ///icon CFBundleIconFile
    reader.start("defaults", QStringList()<<"read"<<app.path+"/Contents/Info.plist"<<"CFBundleIconFile");
    reader.waitForFinished();
    app.icon = app.path+"/Contents/Resources/"+reader.readAll().trimmed();
    //qDebug()<<"icon" << app.icon;

    ///exec CFBundleExecutable
    reader.start("defaults", QStringList()<<"read"<<app.path+"/Contents/Info.plist"<<"CFBundleExecutable");
    reader.waitForFinished();
    app.path = app.path+"/Contents/MacOS/"+reader.readAll().trimmed();
    //qDebug()<<"path" << app.path;
    return app;
}

void NewAppDialog::on_btn1_clicked()
{
    this->app = locateApp();
    init();
}

void NewAppDialog::on_toolButton_clicked()
{
    QString exec = QFileDialog::getOpenFileName(0, tr("Open File"), app.path);

}

void NewAppDialog::setName(QString name){
    this->app.name = name;
    init();
}

void NewAppDialog::setExec(QString exec){
    if(!exec.isEmpty()){
        if(!QFileInfo(exec).isExecutable()){
            QMessageBox msgBox;
             msgBox.setText("Este archivo no parece ser ejecutable");
             msgBox.setInformativeText("Recomendamos elegir uno que lo sea o revisar el archivo antes de continuar");
             msgBox.setStandardButtons(QMessageBox::Ok);
             msgBox.setDefaultButton(QMessageBox::Ok);
             msgBox.setIcon(QMessageBox::Warning);
             int ret = msgBox.exec();
        }
        this->app.path = exec;
        init();
    }
}


void NewAppDialog::on_toolButton_2_clicked()
{
    QString ico = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     app.icon,
                       tr("Icons(*icns *.ico *.cur *.png *.xpm *.jpg *.bmp)"));
   setIco(ico);
}

void NewAppDialog::setIco(QString ico){
    if(!ico.isEmpty()){
        this->app.icon = ico;
        init();
    }
}








