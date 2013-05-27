#include <QApplication>
#include <QtGui>
#include <QMenuBar>
#include <QObject>

#include "ui_QCGALArrangementMainWindow.h"
#include "QCGALArrangementMainWindow.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    /*
    app.setApplicationName(QLABELLING_NAME_STRING);
    app.setOrganizationName(QLABELLING_ORGANIZATION_STRING);
    app.setOrganizationDomain(QLABELLING_DOMAIN_STRING);
    */

    QCGALArrangementMainWindow mainWindow;

    mainWindow.show();

    return app.exec();
}
