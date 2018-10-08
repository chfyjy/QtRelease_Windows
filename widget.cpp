#include "widget.h"
#include "ui_widget.h"

//QtRelease.exe
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("QtRelease");
    initControls();
}

Widget::~Widget()
{
    delete ui;
}


void Widget::qtcboxChecked(bool checked)
{
    for(int i = 0; i < qtlibviewmodel->rowCount(); i++)
    {
        if(checked)
            qtlibviewmodel->item(i)->setCheckState(Qt::Checked);
        else
            qtlibviewmodel->item(i)->setCheckState(Qt::Unchecked);

    }
}
void Widget::windowscboxChecked(bool checked)
{
    for(int i = 0; i < winlibviewmodel->rowCount(); i++)
    {
        if(checked)
            winlibviewmodel->item(i)->setCheckState(Qt::Checked);
        else
            winlibviewmodel->item(i)->setCheckState(Qt::Unchecked);
    }
}
void Widget::thirdcboxChecked(bool checked)
{
    for(int i = 0; i < thirdlibviewmodel->rowCount(); i++)
    {
        if(checked)
            thirdlibviewmodel->item(i)->setCheckState(Qt::Checked);
        else
            thirdlibviewmodel->item(i)->setCheckState(Qt::Unchecked);
    }
}
void Widget::analyzeClicked()
{
    exenameEdit->setDisabled(true);
    if(exenameEdit->text() == "")
    {
        QMessageBox::information(this, "Tip", "请输入exe名字，并确保运行");
        return;
    }
    QtLibraryMap.clear();
    WinLibraryMap.clear();
    thirdLibraryMap.clear();
    getPeDependDllInfo(exenameEdit->text());
    if(QtLibraryMap.isEmpty() || WinLibraryMap.isEmpty() || thirdLibraryMap.isEmpty())
    {
        QMessageBox::information(this, "Tip", exenameEdit->text() + "是否已经运行");
        exenameEdit->setEnabled(true);
        return;
    }
    initLibView();
}

void Widget::startCopyClicked()
{
    analyze->setDisabled(true);
    startCopy->setDisabled(true);
    exenameEdit->setDisabled(true);

    for(int i =0; i < libneeddir.count(); i++)
    {
        QDir dir(libneeddir.at(i));
        if(dir.exists())
          continue;
        else
           dir.mkdir(libneeddir.at(i));
    }

    QMap<QString, QString>::iterator it;
    if(qtcbox->isChecked())
    {
        for(it = QtLibraryMap.begin(); it != QtLibraryMap.end(); it++)
        {
            qDebug() << QFile::copy(it.key(), it.value()) << it.value();
            //qDebug() << it.value();
        }
    }
    if(windowscbox->isChecked())
    {
        for(it = WinLibraryMap.begin(); it != WinLibraryMap.end(); it++)
           QFile::copy(it.key(), it.value());
    }
    if(thirdcbox->isChecked())
    {
        for(it = thirdLibraryMap.begin(); it != thirdLibraryMap.end(); it++)
           QFile::copy(it.key(), it.value());
    }

    analyze->setDisabled(false);
    startCopy->setDisabled(false);
    exenameEdit->setDisabled(false);
}
void Widget::initLibView(void)
{
    QMap<QString, QString>::iterator it;
    qtlibviewmodel->clear();
    for(it = QtLibraryMap.begin(); it != QtLibraryMap.end(); it++)
    {
        QStandardItem* item = new QStandardItem(it.key());
        item->setCheckable( true );
        item->setCheckState(Qt::Checked);
        qtlibviewmodel->appendRow(item);
    }
    qtcbox->setCheckState(Qt::Checked);
    winlibviewmodel->clear();
    for(it = WinLibraryMap.begin(); it != WinLibraryMap.end(); it++)
    {
        QStandardItem* item = new QStandardItem(it.key());
        winlibviewmodel->appendRow(item);
    }
    thirdlibviewmodel->clear();
    for(it = thirdLibraryMap.begin(); it != thirdLibraryMap.end(); it++)
    {
        QStandardItem* item = new QStandardItem(it.key());
        thirdlibviewmodel->appendRow(item);
    }
}
void Widget::initControls()
{
    QLabel *exenamel = new QLabel(this);
    exenamel->setText("EXE Name");
    exenameEdit = new QLineEdit(this);
    analyze = new QPushButton(this);
    analyze->setText("获取dll信息");
    connect(analyze, SIGNAL(clicked()), this, SLOT(analyzeClicked()));
    startCopy = new QPushButton(this);
    startCopy->setText("拷贝");
    connect(startCopy, SIGNAL(clicked()), this, SLOT(startCopyClicked()));
    QHBoxLayout *exeNamelayout = new QHBoxLayout();
    exeNamelayout->addWidget(exenamel);
    exeNamelayout->addWidget(exenameEdit);
    exeNamelayout->addWidget(analyze);
    exeNamelayout->addWidget(startCopy);

    qtcbox = new QCheckBox(this);
    qtcbox->setText("Qt库");
    connect(qtcbox, SIGNAL(toggled(bool)),this, SLOT(qtcboxChecked(bool)));
    windowscbox = new QCheckBox(this);
    windowscbox->setText("Windows库");
    connect(windowscbox, SIGNAL(toggled(bool)), this, SLOT(windowscboxChecked(bool)));
    thirdcbox = new QCheckBox(this);
    thirdcbox->setText("未知来源库");
    connect(thirdcbox, SIGNAL(toggled(bool)),this, SLOT(thirdcboxChecked(bool)));
    QHBoxLayout *cboxLayout = new QHBoxLayout();
    cboxLayout->addWidget(windowscbox);
    cboxLayout->addWidget(qtcbox);
    cboxLayout->addWidget(thirdcbox);

    qtlibview = new QListView(this);
    qtlibviewmodel = new QStandardItemModel(this);
    qtlibview->setModel(qtlibviewmodel);
    qtlibview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    qtlibview->setDisabled(true);


    winlibview = new QListView(this);
    winlibviewmodel = new QStandardItemModel(this);
    winlibview->setModel(winlibviewmodel);
    winlibview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    winlibview->setDisabled(true);

    thirdlibview = new QListView(this);
    thirdlibviewmodel = new QStandardItemModel(this);
    thirdlibview->setModel(thirdlibviewmodel);
    thirdlibview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    thirdlibview->setDisabled(true);

    QVBoxLayout *viewLayout = new QVBoxLayout();
    viewLayout->addWidget(qtlibview);
    viewLayout->addWidget(winlibview);
    viewLayout->addWidget(thirdlibview);
    viewLayout->setStretchFactor(qtlibview, 7);
    viewLayout->setStretchFactor(winlibview, 2);
    viewLayout->setStretchFactor(thirdlibview, 1);


    QVBoxLayout *layout = new QVBoxLayout();
    layout->addLayout(exeNamelayout);
    layout->addLayout(cboxLayout);
    layout->addLayout(viewLayout);
    //layout->setStretchFactor(libpathlayout, 1);
    setLayout(layout);
    setGeometry(100,100,400, 300);
}
inline QString GetEXEDir(QString EXEAbsolatePath, const QString& EXEName)
{
    QString ret = EXEAbsolatePath.remove(EXEName);
    return  ret;
}
QString GetDLLName(const QString& DLLAbsolatePath, const QString& re)
{
    QStringList tmp = DLLAbsolatePath.split(re);
    return  tmp.at(tmp.count()-1);
}
QString GetPluginsDirName(const QString& DLLAbsolatePath)
{
    QStringList tmp = DLLAbsolatePath.split("/plugins/");
    QString tmpstr = tmp.at(tmp.count()-1);
    tmp = tmpstr.split("/");
    tmpstr = tmp.at(0);
    return  tmpstr;
}
void Widget::getPeDependDllInfo(const QString& aimexeName)
{
    PROCESSENTRY32 pe32;//进程结构
        pe32.dwSize = sizeof(pe32); //在使用这个结构前,先设置它的大小
        //给系统内所有的进程拍个快照
        HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        //某个进程所有的DLL快照句柄
        HANDLE hModuleSnap = NULL;
        if(hProcessSnap == INVALID_HANDLE_VALUE)
        {
            printf("CreateTollHelp32Snapshot Error!!\n");
            return ;
        }
        BOOL bMore = ::Process32First(hProcessSnap, &pe32);
        HANDLE hProcess = 0;
        WCHAR procPath[_MAX_PATH]={0};
        MODULEENTRY32 lpme;  //DLL结构
        lpme.dwSize = sizeof(MODULEENTRY32);//在使用这个结构前,先设置它的大小
        BOOL bRet = FALSE;
        QString exeName, exePath, dllPath,tmpdllPath, exeDir;
        //遍历进程快照,显示每个进程的信息
        while(bMore)
        {
            //打开一个已存在的进程对象,并返回进程的句柄
            hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pe32.th32ProcessID);
            //得到该进程的全路径
            GetModuleFileNameEx(hProcess,NULL,procPath, _MAX_PATH);
            if(QString::fromStdWString(  pe32.szExeFile) == aimexeName)
            {
                exeName = QString::fromStdWString(pe32.szExeFile);
                exeName.replace('\\','/');
                exePath = QString::fromStdWString(procPath).replace('\\','/');
                exePath.replace('\\','/');
                exeDir = GetEXEDir(exePath, exeName);
                //qDebug() << exeDir;
                //给一个已存在的进程内所有的DLL拍个快照
                hModuleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pe32.th32ProcessID);
                bRet = ::Module32First(hModuleSnap, &lpme);
                if(bRet)
                    bRet = ::Module32Next(hModuleSnap, &lpme);
                else
                    return;
                //遍历DLL快照,显示该进程所加在的DLL信息
                while(bRet)
                {
                    dllPath = QString::fromStdWString( lpme.szExePath);

                    dllPath.replace('\\','/');
                    //qDebug() << dllPath;
                    tmpdllPath = dllPath.toLower();
                    if(tmpdllPath.contains("qt"))
                    {
                        if(tmpdllPath.contains("plugins"))
                        {
                            QtLibraryMap.insert(dllPath, exeDir + GetDLLName(dllPath, "/plugins/"));
                            libneeddir.append(exeDir +GetPluginsDirName(dllPath));
                        }
                        else
                            QtLibraryMap.insert(dllPath, exeDir + GetDLLName(dllPath, "/"));
                    }

                    else if(tmpdllPath.contains("windows"))
                        WinLibraryMap.insert(dllPath, exeDir + GetDLLName(dllPath, "/"));
                    else
                        thirdLibraryMap.insert(dllPath, exeDir + GetDLLName(dllPath, "/"));

                    bRet = ::Module32Next(hModuleSnap, &lpme);
                }
                //关闭snapshot对象
                ::CloseHandle(hModuleSnap);
            }
            bMore = ::Process32Next(hProcessSnap, &pe32);
        }
        //关闭snapshot对象
        ::CloseHandle(hProcessSnap);
}
