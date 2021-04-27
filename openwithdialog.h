#ifndef OPENWITHDIALOG_H
#define OPENWITHDIALOG_H

#include "OpenWithApp.h"

#include <QDialog>
#include <QKeyEvent>
#include <QSettings>
#include <QTimer>
#include <QToolButton>
#include "newappdialog.h"


class MouseButtonSignaler : public QObject {
  Q_OBJECT
  bool eventFilter(QObject * obj, QEvent * ev) Q_DECL_OVERRIDE {
    if (obj->isWidgetType())
      emit mouseButtonEvent(static_cast<QWidget*>(obj),
                            static_cast<QMouseEvent*>(ev));
    return false;
  }
public:
  Q_SIGNAL void mouseButtonEvent(QWidget *, QMouseEvent *);
  MouseButtonSignaler(QObject * parent = 0) : QObject(parent) {}
  void installOn(QWidget * widget) {
    widget->installEventFilter(this);
  }
};

QT_BEGIN_NAMESPACE
namespace Ui { class OpenWithDialog; }
QT_END_NAMESPACE

class OpenWithDialog : public QDialog
{
    Q_OBJECT
public:
    OpenWithDialog(QPixmap pix, QWidget *parent = nullptr);
    ~OpenWithDialog();

    void connectOpenWithApp(OpenWithApplication *app);
    QString getExt();
public slots:
    void show();
    void setFileName(QString fileName);
protected:
    Ui::OpenWithDialog *ui;
    void hideEvent(QHideEvent *) override;
    void keyPressEvent(QKeyEvent *) override;
    void focusOutEvent ( QFocusEvent *) override;
    QPixmap pix;
    MouseButtonSignaler signaler;
    int scale = 2;
    QString fileName;
    QTimer timer;
    bool stay = false;
    QMap<int, App> apps;
    int currentIndex;
protected slots:
    void onGeneralClick(QWidget *, QMouseEvent *);
    void clearButtons();
    QList<QToolButton *> buttons();
    void saveApps();
    void loadApps();
    void runWith(int pos);
    void onTimeClose();
    void showPopup(int pos);
    void removeApp();
    void editApp();
};
#endif // OPENWITHDIALOG_H
