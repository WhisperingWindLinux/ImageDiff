#include "recentfilespathformater.h"

// Converts a QPair<QString, QString> to a formatted QString
QString RecentFilesPathFormater::pairToString(const QPair<QString, QString>& pair) {
    // Check if either string is empty
    if (pair.first.isEmpty() || pair.second.isEmpty()) {
        return QString(); // Return an empty string if any part of the pair is empty
    }

    // Combine the strings with " -> " as a separator
    return pair.first + " -> " + pair.second;
}

// Converts a formatted QString back to a QPair<QString, QString>
std::optional<QPair<QString, QString>> RecentFilesPathFormater::stringToPair(const QString& str) {
    // Check if the input string is empty
    if (str.isEmpty()) {
        return std::nullopt; // Return no value if the input string is empty
    }

    // Use the helper method to split the string
    return splitString(str, " -> ");
}

// Helper method to split a string with a specific separator
std::optional<QPair<QString, QString>> RecentFilesPathFormater::splitString(const QString& str, const QString& separator) {
    int separatorIndex = str.indexOf(separator);

    // If the separator is not found or is at the beginning or end of the string, return no value
    if (separatorIndex == -1 || separatorIndex == 0 || separatorIndex == str.length() - separator.length()) {
        return std::nullopt;
    }

    // Extract the two parts of the string
    QString firstPart = str.left(separatorIndex);
    QString secondPart = str.mid(separatorIndex + separator.length());

    // Check if either part is empty
    if (firstPart.isEmpty() || secondPart.isEmpty()) {
        return std::nullopt;
    }

    // Return the reconstructed QPair
    return QPair<QString, QString>(firstPart, secondPart);
}
