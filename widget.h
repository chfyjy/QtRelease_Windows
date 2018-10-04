#ifndef WIDGET_H
#define WIDGET_H

#include "common.h"


#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QLayout>
#include <QIcon>
#include <QPixmap>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private slots:
    void openbtnClicked();

private:
    void initControls();


    Ui::Widget *ui;
    QPushButton *openbtn;
    QLineEdit *propath, *libsourcedir;
};

#endif // WIDGET_H
