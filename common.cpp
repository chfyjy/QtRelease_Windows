#include "common.h"

QString GetPathByProcessID(DWORD pid)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess)
    {
        return "";
    }
    WCHAR filePath[MAX_PATH];
    DWORD ret= GetModuleFileNameEx(hProcess, NULL, filePath, MAX_PATH) ;
    QString file = QString::fromStdWString( filePath );
    CloseHandle(hProcess);
    return ret==0?"":file;
}

QString getQtCreatorPath(const QString& aimexeName)
{
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    if(hProcessSnap == INVALID_HANDLE_VALUE)
    {
        return "";
    }
    QString exeName;
    QString exePath;
    BOOL bMore = Process32First(hProcessSnap,&pe32);
    while(bMore)
    {
        exeName = (QString::fromUtf16(reinterpret_cast<const unsigned short *>(pe32.szExeFile)));
        exePath = GetPathByProcessID( pe32.th32ProcessID );
        exePath = FORMAT_PATH( exePath );
        if(!exePath.isEmpty() )
        {
            if(exeName == aimexeName)
                break;
        }
        bMore = Process32Next(hProcessSnap,&pe32);
    }
    CloseHandle(hProcessSnap);
    return exePath;
}
