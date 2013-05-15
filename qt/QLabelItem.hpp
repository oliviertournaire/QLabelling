#ifndef QLABELITEM_HPP
#define QLABELITEM_HPP

#include <QWidget>

class QRadioButton;
class QToolButton;

/*! \brief Brief Simple container for a generic label.
 *
 *  This class is a simple container for a generic label item. It stores various
 *  information (name, color ...) and QWidgets to enable querying and GUI creation.
 *  It also handles GUI events.
 *
 *  \author Olivier Tournaire
 */
class QLabelItem : public QWidget
{
    Q_OBJECT
public:
    explicit QLabelItem(QString labelName, QColor labelColor = Qt::gray, QWidget *parent = 0);
    ~QLabelItem();
    
    QString labelName() const;
    void setLabelName(const QString &labelName);

    QRadioButton *radioButtonlabel() const;
    void setRadioButtonlabel(QRadioButton *radioButtonlabel);

    QToolButton *toolButtonLabelColor() const;
    void setToolButtonLabelColor(QToolButton *toolButtonLabelColor);

    QColor labelColor() const;
    void setLabelColor(const QColor &labelColor, const int alpha = 127);

signals:
    
public slots:
    void toolButtonLabelColor_clicked(bool checked = false);

protected:
    void connectAll();
    void disconnectAll();

    QString _labelName;
    QRadioButton *_radioButtonlabel;
    QToolButton *_toolButtonLabelColor;
    QColor _labelColor;
};

#endif // QLABELITEM_HPP
