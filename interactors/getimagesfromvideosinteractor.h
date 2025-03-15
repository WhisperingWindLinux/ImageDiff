#ifndef GETIMAGESFROMVIDEOSINTERACTOR_H
#define GETIMAGESFROMVIDEOSINTERACTOR_H

#include <qstring.h>

#include <gui/getimagesfromvideosdialog.h>

class MainWindow;

class GetImagesFromVideosInteractor
{
public:
    GetImagesFromVideosInteractor();

    std::optional<QPair<QString, QString> > grabImagesFromVideos();

private:
};

#endif // GETIMAGESFROMVIDEOSINTERACTOR_H
