#include <QtCore/QCoreApplication>
#include "controller.h"
#include <QtDebug>
#include <QFile>
#include <QTextStream>

void customMessageHandler(QtMsgType type, const char *msg)
{
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        QFile outFile("/var/log/proximuslog.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&outFile);
        ts << txt << endl;
        outFile.close();
        abort();
    }

    QFile outFile("/var/log/proximuslog.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
    outFile.close();
}


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

//    qInstallMsgHandler(customMessageHandler);
    Controller controller;
//    MainWindow mainWindow;
//    mainWindow.setOrientation(MainWindow::ScreenOrientationAuto);
//    mainWindow.showExpanded();
//    app.setGlobalStrut(QSize(45,45));//increase size of scrollbars by setting global minimum size of all widgets
    return app.exec();
}

