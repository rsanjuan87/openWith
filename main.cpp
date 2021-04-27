#include "openwithdialog.h"

#include <QApplication>
#include <QLabel>
#include <QScreen>
#include <QWindow>
#include <QMessageBox>
#include <QDebug>

int main(int argc, char *argv[])
{

    OpenWithApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);
    QScreen *screen = QGuiApplication::screenAt(QWidget().mapToGlobal(QCursor::pos()));
    if (const QWindow *window = QLabel().windowHandle())
        screen = window->screen();
        QPixmap pix = screen->grabWindow(WId());

        OpenWithDialog w(pix);
        w.connectOpenWithApp(&a);
        w.show();

    return a.exec();
}



