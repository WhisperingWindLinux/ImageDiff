#ifndef LASTDISPLAYEDCOMPARISONRESULT_H
#define LASTDISPLAYEDCOMPARISONRESULT_H

#include <qpixmap.h>


struct LastDisplayedComparisonResult {

public:
    void set(const QPixmap &image, const QString &description) {
        this->image = image;
        this->description = description;
    }

    void clear() {
        image = std::nullopt;
        description = "";
    }

    bool hasLastDisplayedComparisonResult() {
        return (image != std::nullopt);
    }

    QPixmap getImage() const {
        if (!image.has_value()) {
            throw std::runtime_error("The application is in an inconsistent state. "
                                     "Please report the following information to the "
                                     "app developer: an empty QPixmap was requested in "
                                     "the function LastDisplayedComparisonResult::getImage.");
        }
        return image.value();
    }

    QString getDescription() const {
        return description;
    }

private:
    std::optional<QPixmap> image;
    QString description;
};





#endif // LASTDISPLAYEDCOMPARISONRESULT_H
