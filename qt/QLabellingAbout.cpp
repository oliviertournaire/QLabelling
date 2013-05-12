#include "QLabellingAbout.hpp"
#include "ui_QLabellingAbout.h"
#include "config.hpp"

QLabellingAbout::QLabellingAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QLabellingAbout)
{
    ui->setupUi(this);

    ui->_labelVersion->setText( ui->_labelVersion->text() + QLABELLING_VERSION_STRING );
}

QLabellingAbout::~QLabellingAbout()
{
    delete ui;
}
