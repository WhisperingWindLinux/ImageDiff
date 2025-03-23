#ifndef IIMAGEVALIDATIONRULES_H
#define IIMAGEVALIDATIONRULES_H

#include <optional>

class QString;

// Since we will be comparing the images pixel by pixel, we need to ensure
// that they have the same characteristics (color depth, format, etc.).
class IImageValidationRules
{
public:
    // If all checks pass, return nullopt
    virtual std::optional<QString> isValid() = 0;
};

#endif // IIMAGEVALIDATIONRULES_H
