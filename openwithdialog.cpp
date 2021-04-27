#include "openwithdialog.h"
#include "ui_openwithdialog.h"
#include <QDebug>
#include <QFileDialog>
#include <QGraphicsBlurEffect>
#include <QKeyEvent>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>

OpenWithDialog::OpenWithDialog(QPixmap pix, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OpenWithDialog)
{
    ui->setupUi(this);
    this->pix = pix;
    setWindowFlags(
                   Qt::NoDropShadowWindowHint |
                   Qt::FramelessWindowHint);
    setAutoFillBackground(true);
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet(" border-radius: 13px; "
                            "background-color: rgba(255, 255, 255, 0);");
    QGraphicsBlurEffect* p_blur = new QGraphicsBlurEffect;
        p_blur->setBlurRadius(30);
        p_blur->setBlurHints(QGraphicsBlurEffect::QualityHint);
        ui->back->setGraphicsEffect(p_blur);
        setFocus();

        connect(&signaler, SIGNAL(mouseButtonEvent(QWidget *, QMouseEvent *)),
                      this, SLOT(onGeneralClick(QWidget *, QMouseEvent *)));
        signaler.installOn(ui->btn1);
        signaler.installOn(ui->btn2);
        signaler.installOn(ui->btn3);
        signaler.installOn(ui->btn4);
        signaler.installOn(ui->btn5);
        signaler.installOn(ui->btn6);
        signaler.installOn(ui->btn7);
        signaler.installOn(ui->btn8);
        signaler.installOn(ui->btn9);
        if(qApp->arguments().size() >1 ){
            this->fileName = qApp->arguments().at(1);
            loadApps();
        }else {
            timer.singleShot(1000, this, SLOT(onTimeClose()));
        }
}


void OpenWithDialog::loadApps(){
    apps.clear();
    clearButtons();

    QSettings settings(qAppName());
    settings.beginGroup(getExt());
    for (int i = 0; i < 9; i++) {
     settings.beginGroup(QString::number(i));
         App app;
         app.name = settings.value("name").toString();
         if(app.name.isEmpty()){
             buttons().at(i)->setToolTip("");
             buttons().at(i)->setText("("+QString::number(i+1)+") Adicionar");
             buttons().at(i)->setIcon(QIcon(":/add.png"));
         }else{
             app.icon = settings.value("icon").toString();
             app.path = settings.value("path").toString();
             apps.insert(i, app);
             buttons().at(i)->setText("("+QString::number(i+1)+") "+app.name);
             buttons().at(i)->setToolTip(app.path);
             buttons().at(i)->setIcon(QIcon(app.icon));
         }
     settings.endGroup();
    }

    settings.endGroup();
}

QList<QToolButton*> OpenWithDialog::buttons(){
    QList<QToolButton*> list;
    list.append(ui->btn1);
    list.append(ui->btn2);
    list.append(ui->btn3);
    list.append(ui->btn4);
    list.append(ui->btn5);
    list.append(ui->btn6);
    list.append(ui->btn7);
    list.append(ui->btn8);
    list.append(ui->btn9);
    return list;
}


OpenWithDialog::~OpenWithDialog()
{
    delete ui;
}

void OpenWithDialog::show(){
    QDialog::show();
    QRect r = geometry();
    r.setX(mapToGlobal(ui->back->pos()).x());
    r.setY(mapToGlobal(ui->back->pos()).y());
    //if (pix.isNull())
        ui->back->setPixmap(pix.copy(r.x()*scale, r.y()*scale, r.width()*scale, r.height()*scale));
}

void OpenWithDialog::hideEvent(QHideEvent *)
{
    close();
}

void OpenWithDialog::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape){
        exit(0);
    }else{
        int i = e->text().toInt();
        if(i>0) runWith(i);
    }
}

void OpenWithDialog::onGeneralClick(QWidget *widget, QMouseEvent *e){
    if(e->type() == QEvent::MouseButtonPress) {
        if(e->buttons() == Qt::RightButton) {
            stay = true;
            showPopup(widget->objectName().remove("btn").toInt());
            stay = false;
        }else
            runWith(widget->objectName().remove("btn").toInt());
    }
}

void OpenWithDialog::showPopup(int pos){
    currentIndex = pos-1;
    QMenu menu;
    menu.addAction("Editar", this, SLOT(editApp()));
    menu.addAction("Quitar", this, SLOT(removeApp()));
    menu.exec(QCursor::pos());
}

void OpenWithDialog::editApp(){
    NewAppDialog dlg(apps.value(currentIndex));
    if(dlg.exec()){
        apps.insert(currentIndex, dlg.getApp());
        saveApps();
        loadApps();
    }
}

void OpenWithDialog::removeApp(){
    apps.remove(currentIndex);
    saveApps();
    loadApps();
}

void OpenWithDialog::focusOutEvent(QFocusEvent *)
{
    if(!stay)
        close();
}

void OpenWithDialog::runWith(int pos){
    int index = pos-1;
    bool stay1= false;
    if (!fileName.isEmpty() ){
        if(buttons().at(index)->toolTip().isEmpty()){
            stay = true;
            stay1 = true;
            App app = NewAppDialog::locateApp();
            if(!app.path.isEmpty()){
                buttons().at(index)->setIcon(QIcon(app.icon));
                buttons().at(index)->setText("("+QString::number(index+1)+") "+app.name);
                buttons().at(index)->setToolTip(app.path);
                apps.insert(index, app);
                saveApps();
            }
            stay = false;
        }else{
           QProcess::startDetached(buttons().at(index)->toolTip(), QStringList() << fileName);
        }
    }
    if(!stay1)
        close();
}

void OpenWithDialog::clearButtons(){
    for (int i = 0; i < 9 ; i++) {
        buttons().at(i)->setToolTip("");
        buttons().at(i)->setText("("+QString::number(i+1)+") Adicionar");
        buttons().at(i)->setIcon(QPixmap(":/imgs/add.png"));
    }
}

void OpenWithDialog::saveApps(){

    QSettings settings(qAppName());
    settings.beginGroup(getExt());
     for (int i = 0; i < 9; i++) {
         settings.beginGroup(QString::number(i));
         settings.setValue("name", apps.value(i).name);
         settings.setValue("icon", apps.value(i).icon);
         settings.setValue("path", apps.value(i).path);
//         qDebug() << "name"<< apps.value(i).name;
//         qDebug() << "icon"<< apps.value(i).icon;
//         qDebug() << "path"<< apps.value(i).path;
         settings.endGroup();
     }
     settings.endGroup();

}

void OpenWithDialog::connectOpenWithApp(OpenWithApplication *app) {
    connect(app, SIGNAL(fileReady(QString)), this, SLOT(setFileName(QString)));
}

void OpenWithDialog::setFileName(QString fileName){
    this->fileName = fileName;
    loadApps();
}

void OpenWithDialog::onTimeClose(){
    if (fileName.isEmpty()) {
        QMessageBox msgBox;
         msgBox.setText("Esta aplicacion es para el uso con parametros");
         msgBox.setIcon(QMessageBox::Warning);
         msgBox.exec();
        close();
    }
}


QString OpenWithDialog::getExt(){
    QString ext ;
    if(fileName.contains('.'))
        ext = fileName.split('.').last();
    return ext;
}





