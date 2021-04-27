#ifndef OPENWITHAPPLICATION_H
#define OPENWITHAPPLICATION_H

#include <QApplication>
#include <QFileOpenEvent>
#include <QMessageBox>


class OpenWithApplication : public QApplication
{
    Q_OBJECT

public:
    QString fileName;

    OpenWithApplication(int &argc, char **argv)
        : QApplication(argc, argv)
    {
    }
signals:

    void fileReady(QString fn);

protected:

    bool event(QEvent *event)
    {
        if (event->type() == QEvent::FileOpen) {
            QFileOpenEvent *openEvent = static_cast<QFileOpenEvent *>(event);
            fileName = openEvent->file();

            emit fileReady(fileName); //  the file is ready
        }

        return QApplication::event(event);
    }
};

#endif // OPENWITHAPPLICATION_H

