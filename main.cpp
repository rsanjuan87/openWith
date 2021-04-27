#include "openwithdialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OpenWithDialog w;
    w.show();
    return a.exec();
}
