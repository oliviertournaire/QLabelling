#include <QApplication>
#include <QtGui>
#include <QMenuBar>
#include <QObject>

#include "ui_QLabellingMainWindow.h"
#include "QLabellingMainWindow.hpp"
#include "config.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/QLabelling/QLabellingIcon.png"));

    app.setApplicationName(QLABELLING_NAME_STRING);
    app.setOrganizationName(QLABELLING_ORGANIZATION_STRING);
    app.setOrganizationDomain(QLABELLING_DOMAIN_STRING);

    QLabellingMainWindow mainWindow;

    mainWindow.show();

    return app.exec();
}
