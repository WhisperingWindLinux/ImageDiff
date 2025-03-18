#ifndef PYSCRIPTINFO_H
#define PYSCRIPTINFO_H

#include <QtCore/qcontainerfwd.h>
#include <QtCore/qstring.h>

using namespace std;

struct PyScriptInfo {

    PyScriptInfo(shared_ptr<QByteArray> jsonData, QString pythonScriptPath)
        : jsonData(std::move(jsonData)),
          pythonScriptPath(std::move(pythonScriptPath)) {}

    PyScriptInfo(const PyScriptInfo& src)
        : jsonData(src.jsonData),
        pythonScriptPath(src.pythonScriptPath) {}

    PyScriptInfo(PyScriptInfo&& src) noexcept
        : jsonData(std::move(src.jsonData)),
          pythonScriptPath(std::move(src.pythonScriptPath)) {}

    virtual ~PyScriptInfo() = default;

    PyScriptInfo& operator=(const PyScriptInfo&) = delete;

    PyScriptInfo& operator=(PyScriptInfo&& src) noexcept {
        if (this != &src) {
            const_cast<shared_ptr<QByteArray>&>(jsonData) = std::move(src.jsonData);
            const_cast<QString&>(pythonScriptPath) = std::move(src.pythonScriptPath);
        }
        return *this;
    }

    const shared_ptr<QByteArray> jsonData;
    const QString pythonScriptPath;
};


#endif // PYSCRIPTINFO_H
