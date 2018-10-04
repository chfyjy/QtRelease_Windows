#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>
#include <QDir>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    initControls();
}

Widget::~Widget()
{
    delete ui;
}

QString getQtVersion(const QString& qtcreatorpath)
{
    QString tmp = qtcreatorpath.split("/Tools").at(0);
    tmp = tmp.right(5);
    return tmp;
}

void Widget::initControls()
{
    QLabel *libpathl = new QLabel(this);
    libpathl->setText("库文件源目录");
    libsourcedir = new QLineEdit(this);
    QString tmp = getQtCreatorPath("qtcreator.exe");
    getQtVersion(tmp);

    //D:/Qt/Qt5.9.5/5.9.5/mingw53_32/bin
    //d:/qt/qt5.9.5/tools/qtcreator/bin/qtcreator.exe
    QHBoxLayout *libpathlayout = new QHBoxLayout();
    libpathlayout->addWidget(libpathl);
    libpathlayout->addWidget(libsourcedir);

    openbtn = new QPushButton(this);
    connect(openbtn, SIGNAL(clicked()), this, SLOT(openbtnClicked()));
    //openbtn->setWhatsThis(QString("选择pro文件"));
    QIcon openbtnicon = QIcon(":/icon/open.png");
    openbtn->setIcon(openbtnicon);
    openbtn->setAutoFillBackground(true);

    propath = new QLineEdit(this);

    QHBoxLayout *prolayout = new QHBoxLayout();
    prolayout->addWidget(openbtn);
    prolayout->addWidget(propath);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addLayout(libpathlayout);
    layout->addLayout(prolayout);
    layout->setStretchFactor(libpathlayout, 1);
    layout->setStretchFactor(prolayout, 1);
    setLayout(layout);
    setGeometry(100,100,400, 300);
}

void Widget::openbtnClicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"选择pro文件",".","*.pro");
    propath->setText(filename);
}
