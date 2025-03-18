#ifndef HTMLIMAGEPROCESSORSHELPPRESENTER_H
#define HTMLIMAGEPROCESSORSHELPPRESENTER_H

#include <qstring.h>

#include <domain/valueobjects/imageprocessorsinfo.h>


class HtmlImageProcessorsHelpPresenter
{
public:
    HtmlImageProcessorsHelpPresenter() = delete;
    ~HtmlImageProcessorsHelpPresenter() = delete;

    static QString formatToHTML(const QList<ImageProcessorInfo> &processors);

};

#endif // HTMLIMAGEPROCESSORSHELPPRESENTER_H
