#include <QApplication>
#include <QtGui>
#include <QMenuBar>

#include "QLabellingMainWindow.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/QSnapEdit/QSnapEditIcon.png"));

    app.setApplicationName("QSnapEdit");
    app.setOrganizationName("IMAGINE / ENPC");
    app.setOrganizationDomain("www.imagine.enpc.fr");

    QLabellingMainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
