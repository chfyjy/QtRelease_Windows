#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("QtRelease");
    setMinimumWidth(800);
    setMinimumHeight(600);
    initControls();
    copytPool = new QThreadPool(this);
    copytPool->setMaxThreadCount(5);
}

Widget::~Widget()
{
    delete ui;
}
void Widget::keywordCboxToggled(bool toggled)
{
    if(toggled && keywordEdit->text().isEmpty())
    {
        QMessageBox::information(this, "Tip", "Enter 'keyword(like 'Qt')' to lineedit");
        exenameEdit->setEnabled(true);
        keywordCbox->setCheckState(Qt::Unchecked);
        return;
    }
    initLibView();
}

void Widget::keywordEditingFinished()
{
    initLibView();
}
void Widget::allcboxToggled(bool toggled)
{
    if(toggled)
    {
        for(int i = 0; i < qtlibviewmodel->rowCount(); i++)
            qtlibviewmodel->item(i)->setCheckState(Qt::Checked);

    }
    else
    {
        for(int i = 0; i < qtlibviewmodel->rowCount(); i++)
            qtlibviewmodel->item(i)->setCheckState(Qt::Unchecked);
    }
}
void Widget::analyzeClicked()
{
    infol->setText("");
    exenameEdit->setEnabled(false);
    if(exenameEdit->text() == "")
    {
        QMessageBox::information(this, "Tip", "Enter \"xxx.exe\" to lineedit, and run it(no full path)");
        exenameEdit->setEnabled(true);
        return;
    }
    librarymap.clear();
    getPeDependDllInfo(exenameEdit->text());
    if(librarymap.isEmpty())
    {
        QMessageBox::information(this, "Tip", "Enter \"xxx.exe\" to lineedit, and run it(no full path)");
        exenameEdit->setEnabled(true);
        return;
    }
    initLibView();
    infol->setText("Geted");
    exenameEdit->setDisabled(false);
}
void Widget::getNeedLibList(void)
{
    libneed.clear();
    QString tmp;
    for(int i = 0; i < qtlibviewmodel->rowCount(); i++)
    {
        if(qtlibviewmodel->item(i, 0)->checkState() == Qt::Checked)
            libneed.append(qtlibviewmodel->index(i,0).data().toString());

    }
}
void  Widget::createNeededDir(void)
{
    infol->setText("start create some dir for need");
    for(int i =0; i < libneeddir.count(); i++)
    {
        QDir dir(libneeddir.at(i));
        if(dir.exists())
          continue;
        else
           dir.mkdir(libneeddir.at(i));
    }
    infol->setText("created dir");
}
QString Widget::getCopyName(const QString& sourcename)
{
    return librarymap[sourcename];
}
void Widget::startCopyClicked()
{
    if(librarymap.isEmpty())
    {
        QMessageBox::information(this, "Tip", "Enter \"xxx.exe\" to lineedit, and run it(no full path)");
        exenameEdit->setEnabled(true);
        return;
    }
    createNeededDir();
    getNeedLibList();
    infol->setText("start copy");
    QString tmp;
    for(int i = 0; i < libneed.count(); i++)
    {
        tmp = libneed.at(i);
        copytPool->start(new CopyTask(tmp, getCopyName(tmp)));
    }
    //copytPool->start(new CopyTask(exePath, getCopyName(releasedir+exeName)));
    QFile exeFile(exePath);
    exeFile.copy(releasedir+exeName);
    copytPool->waitForDone();
    infol->setText("copy success");
    exenameEdit->setText("Give me a star at https://github.com/chfyjy/QtRelease_Windows");
}
QStringList Widget::keywordFirst()
{
    QStringList ret;
    QString tmpup;
    QString keywordup = keywordEdit->text().toUpper();
    QMap<QString, QString>::iterator it;
    for(it = librarymap.begin(); it != librarymap.end(); it++)
    {
        tmpup = it.key().toUpper();
        if(tmpup.contains(keywordup))
        {
            ret << it.key();
        }

    }
    for(it = librarymap.begin(); it != librarymap.end(); it++)
    {
        if(!ret.contains(it.key()))
            ret << it.key();

    }
    return  ret;
}
void Widget::initLibView(void)
{
    QString tmpup;
    QString keywordup = keywordEdit->text().toUpper();
    QStringList firstlist = keywordFirst();
    qtlibviewmodel->clear();
    foreach (QString tmp, firstlist)
    {
        QStandardItem* item = new QStandardItem(tmp);
        item->setCheckable( true );
        tmpup = tmp.toUpper();
        if(keywordCbox->isChecked())
        {
            if(tmpup.contains(keywordup))
                item->setCheckState(Qt::Checked);
            else
                item->setCheckState(Qt::Unchecked);
        }
        qtlibviewmodel->appendRow(item);
    }
}
void Widget::initControls()
{
    QLabel *exenamel = new QLabel(this);
    exenamel->setText("exe name");
    exenameEdit = new QLineEdit(this);
    analyze = new QPushButton(this);
    analyze->setText("get dll info");
    connect(analyze, SIGNAL(clicked()), this, SLOT(analyzeClicked()));
    startCopy = new QPushButton(this);
    startCopy->setMaximumWidth(40);
    startCopy->setText("copy");
    connect(startCopy, SIGNAL(clicked()), this, SLOT(startCopyClicked()));
    QHBoxLayout *exeNamelayout = new QHBoxLayout();
    exeNamelayout->addWidget(exenamel);
    exeNamelayout->addWidget(exenameEdit);
    exeNamelayout->addWidget(analyze);
    exeNamelayout->addWidget(startCopy);

    keywordCbox = new QCheckBox(this);
    keywordCbox->setText("keyword first");
    keywordCbox->setFixedWidth(150);
    keywordCbox->setCheckState(Qt::Checked);
    connect(keywordCbox, SIGNAL(toggled(bool)), this, SLOT(keywordCboxToggled(bool)));
    keywordEdit = new QLineEdit(this);
    keywordEdit->setText("qt");
    keywordEdit->setFixedWidth(150);
    connect(keywordEdit, SIGNAL(editingFinished()), this, SLOT(keywordEditingFinished()));
    allcbox = new QCheckBox(this);
    allcbox->setText("select all");
    allcbox->setFixedWidth(150);
    connect(allcbox, SIGNAL(toggled(bool)), this, SLOT(allcboxToggled(bool)));
    QHBoxLayout *cboxLayout = new QHBoxLayout();
    infol = new QLabel(this);
    infol->setText("");
    cboxLayout->addWidget(keywordCbox);
    cboxLayout->addWidget(keywordEdit);
    cboxLayout->addWidget(allcbox);
    cboxLayout->addWidget(infol);

    qtlibview = new QListView(this);
    qtlibviewmodel = new QStandardItemModel(this);
    qtlibview->setModel(qtlibviewmodel);
    qtlibview->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QVBoxLayout *viewLayout = new QVBoxLayout();
    viewLayout->addWidget(qtlibview);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addLayout(exeNamelayout);
    layout->addLayout(cboxLayout);
    layout->addLayout(viewLayout);
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
    QString  dllPath,tmpdllPath, exeDir;
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
            releasedir = QString("%1%2/").arg(exeDir).arg("QtRelease");
            if(!libneeddir.contains(releasedir))
                libneeddir.append(releasedir);
            //qDebug() << releasedir;
            //给一个已存在的进程内所有的DLL拍个快照
            hModuleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pe32.th32ProcessID);
            bRet = ::Module32First(hModuleSnap, &lpme);
            if(bRet)
                bRet = ::Module32Next(hModuleSnap, &lpme);
            else
                return;
            //遍历DLL快照,显示该进程所加在的DLL信息
            QString tmpdir;
            while(bRet)
            {
                dllPath = QString::fromStdWString( lpme.szExePath);

                dllPath.replace('\\','/');
                //qDebug() << dllPath;
                tmpdllPath = dllPath.toLower();
                if(tmpdllPath.contains("qt") && tmpdllPath.contains("plugins"))
                {
                    tmpdir = releasedir +GetPluginsDirName(dllPath);
                    if(!libneeddir.contains(tmpdir))
                    {
                        //qDebug() << tmpdir;
                        libneeddir.append(tmpdir);
                    }
                    librarymap.insert(dllPath, releasedir + GetDLLName(dllPath, "/plugins/"));
                    //qDebug() << dllPath << releasedir + GetDLLName(dllPath, "/plugins/");
                }
                else
                {
                    librarymap.insert(dllPath, releasedir + GetDLLName(dllPath, "/"));
                    //qDebug() << dllPath << releasedir + GetDLLName(dllPath, "/");
                }

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
