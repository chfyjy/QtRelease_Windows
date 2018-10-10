#ifndef WIDGET_H
#define WIDGET_H



#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QLayout>
#include <QIcon>
#include <QPixmap>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QString>
#include <QDebug>
#include <QMap>
#include <QCheckBox>
#include <QStandardItemModel>
#include <QListView>
#include <QThreadPool>
#include <QThread>
#include <QRunnable>


#include <Windows.h>
#include <tlhelp32.h>// for CreateToolhelp32Snapshot
#include <Psapi.h>   // for GetModuleFileNameEx


namespace Ui {
class Widget;
}
class CopyTask : public QObject, public QRunnable
{
    Q_OBJECT

public:
    CopyTask(const QString& a, const QString& b)
    {
        oldfilename = a;
        newfilename = b;
    }
    ~CopyTask()
    {}
protected:
    void run()
    {
        QFile::copy(oldfilename,newfilename);
    }

signals:
private:
    QString oldfilename,newfilename;
};



class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private slots:
    void analyzeClicked();
    void startCopyClicked();
    void qtcboxToggled(bool toggled);

private:
    void initControls(void);
    void initLibView(void);
    void getPeDependDllInfo(const QString& aimexeName);
    QString getCopyName(const QString& sourcename);
    void getNeedLibList(void);
    void createNeededDir(void);


    Ui::Widget *ui;
    QThreadPool *copytPool;
    CopyTask *runcopy;
    QLineEdit *exenameEdit, *viewEdit;
    QPushButton *analyze, *startCopy;
    QCheckBox *qtcbox;
    QListView *qtlibview;
    QStandardItemModel *qtlibviewmodel;
    QMap<QString,QString> qtlibrarymap, winlibrarymap, thirdlibrarymap;
    QStringList libneed, libneeddir;
    QLabel *infol;
};

#endif // WIDGET_H
