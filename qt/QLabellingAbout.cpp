#include "QLabellingAbout.hpp"
#include "ui_QLabellingAbout.h"

QLabellingAbout::QLabellingAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QLabellingAbout)
{
    ui->setupUi(this);
}

QLabellingAbout::~QLabellingAbout()
{
    delete ui;
}
