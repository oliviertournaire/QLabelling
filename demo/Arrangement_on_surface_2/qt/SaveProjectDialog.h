#ifndef SAVEPROJECTDIALOG_HPP
#define SAVEPROJECTDIALOG_HPP

#include <QDialog>

namespace Ui {
class SaveProjectDialog;
}

class SaveProjectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SaveProjectDialog(const QString& inputImagePath, QWidget *parent = 0);
    ~SaveProjectDialog();

protected:
    QString _inputImagePath;
    
private:
    Ui::SaveProjectDialog *ui;
};

#endif // SAVEPROJECTDIALOG_HPP
