#include "QLabellingAbout.hpp"
#include "ui_QLabellingAbout.h"
#include "config.hpp"

QLabellingAbout::QLabellingAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QLabellingAbout)
{
    ui->setupUi(this);

    ui->_labelVersion->setText( ui->_labelVersion->text() + QLABELLING_VERSION_STRING );
    ui->_labelDomain->setText( QString("<a href=\"http://") + QLABELLING_DOMAIN_STRING + QString("\">") + QLABELLING_DOMAIN_STRING + QString("</a>") );
}

QLabellingAbout::~QLabellingAbout()
{
    delete ui;
}
