#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QDebug>
#include <QMap>

#include <Windows.h>
#include <tlhelp32.h>// for CreateToolhelp32Snapshot
#include <Psapi.h>   // for GetModuleFileNameEx
#define FORMAT_PATH(path) path.replace('\\','/')
#pragma  comment(lib, "psapi.lib")


QString GetPathByProcessID(DWORD pid);
QString getQtCreatorPath(const QString& aimexeName);

#endif // COMMON_H
