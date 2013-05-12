#ifndef QLABELLINGABOUT_HPP
#define QLABELLINGABOUT_HPP

#include <QDialog>

namespace Ui {
class QLabellingAbout;
}

class QLabellingAbout : public QDialog
{
    Q_OBJECT
    
public:
    explicit QLabellingAbout(QWidget *parent = 0);
    ~QLabellingAbout();
    
private:
    Ui::QLabellingAbout *ui;
};

#endif // QLABELLINGABOUT_HPP
