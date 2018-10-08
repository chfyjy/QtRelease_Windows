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


#include <Windows.h>
#include <tlhelp32.h>// for CreateToolhelp32Snapshot
#include <Psapi.h>   // for GetModuleFileNameEx


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
    void analyzeClicked();
    void startCopyClicked();
    void qtcboxChecked(bool checked);
    void windowscboxChecked(bool checked);
    void thirdcboxChecked(bool checked);

private:
    void initControls(void);
    void initLibView(void);
    void getPeDependDllInfo(const QString& aimexeName);


    Ui::Widget *ui;
    QLineEdit *exenameEdit, *viewEdit;
    QPushButton *analyze, *startCopy;
    QCheckBox *qtcbox, *windowscbox, *thirdcbox;
    QListView *qtlibview,*winlibview,*thirdlibview;
    QStandardItemModel *qtlibviewmodel,*winlibviewmodel,*thirdlibviewmodel;
    QMap<QString,QString> QtLibraryMap, WinLibraryMap, thirdLibraryMap;
    QStringList libneed, libneeddir;
};

#endif // WIDGET_H
