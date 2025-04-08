#ifndef LASTDISPLAYEDCOMPARISONRESULT_H
#define LASTDISPLAYEDCOMPARISONRESULT_H

#include <qpixmap.h>


struct LastDisplayedComparisonResult {
public:
    void set(const QPixmap &image, const QString &description) {
        this->mImage = image;
        this->mDescription = description;
    }

    void clear() {
        mImage = std::nullopt;
        mDescription = "";
    }

    bool hasLastDisplayedComparisonResult() {
        return (mImage != std::nullopt);
    }

    QPixmap getImage() const {
        if (!mImage.has_value()) {
            throw std::runtime_error("The application is in an inconsistent state. "
                                     "Please report the following information to the "
                                     "app developer: an empty QPixmap was requested in "
                                     "the function LastDisplayedComparisonResult::getImage.");
        }
        return mImage.value();
    }

    QString getDescription() const {
        return mDescription;
    }

private:
    std::optional<QPixmap> mImage;
    QString mDescription;
};

#endif // LASTDISPLAYEDCOMPARISONRESULT_H
