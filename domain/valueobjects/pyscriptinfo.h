#ifndef PYSCRIPTINFO_H
#define PYSCRIPTINFO_H

#include <QtCore/qcontainerfwd.h>
#include <QtCore/qstring.h>

using namespace std;

struct PyScriptInfo {
    QString jsonFilePath;
    QString pythonFilePath;
};


#endif // PYSCRIPTINFO_H
