#ifndef GETIMAGESFROMVIDEOSINTERACTOR_H
#define GETIMAGESFROMVIDEOSINTERACTOR_H

#include <qstring.h>

#include <presentation/dialogs/getimagesfromvideosdialog.h>
#include <domain/valueobjects/images.h>

class MainWindow;

class GetImagesFromVideosInteractor
{
public:
    GetImagesFromVideosInteractor() = default;
    ~GetImagesFromVideosInteractor() = default;
    
    ImagesPtr get();
};

#endif // GETIMAGESFROMVIDEOSINTERACTOR_H
