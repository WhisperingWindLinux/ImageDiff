#ifndef ONCROPIMAGESLISTENER_H
#define ONCROPIMAGESLISTENER_H

#include "domain/valueobjects/images.h"

class OnCropImageListener {
public:
    virtual void onImagesCropped(ImagesPtr images) = 0;
};

#endif // ONCROPIMAGESLISTENER_H
