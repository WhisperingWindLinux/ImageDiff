#ifndef RECENTFILESPRESENTER_H
#define RECENTFILESPRESENTER_H

#include <QString>
#include <QPair>
#include <optional>

class RecentFilesPresenter {
public:
    // Converts a QPair<QString, QString> to a formatted QString
    static QString pairToString(const QPair<QString, QString>& pair);

    // Converts a formatted QString back to a QPair<QString, QString>
    static std::optional<QPair<QString, QString>> stringToPair(const QString& str);

private:
    // Helper method to split a string with a specific separator
    static std::optional<QPair<QString, QString>> splitString(const QString& str, const QString& separator);
};


#endif // RECENTFILESPRESENTER_H
