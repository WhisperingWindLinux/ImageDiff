#ifndef GETIMAGESFROMVIDEOSINTERACTOR_H
#define GETIMAGESFROMVIDEOSINTERACTOR_H

#include <qstring.h>
#include <presentation/dialogs/getimagesfromvideosdialog.h>
#include <data/storage/filedialoghandler.h>


typedef std::optional<QPair<QString, QString>> OptionalStringPair;

class MainWindow;

class GetImagesFromVideosInteractor
{
public:
    GetImagesFromVideosInteractor() = default;
    ~GetImagesFromVideosInteractor() = default;
    
    OptionalStringPair getImagePaths();
};

#endif // GETIMAGESFROMVIDEOSINTERACTOR_H
