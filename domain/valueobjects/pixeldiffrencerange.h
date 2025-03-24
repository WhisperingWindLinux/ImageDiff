#ifndef PIXELDIFFRENCERANGE_H
#define PIXELDIFFRENCERANGE_H

// Structure to store information about pixel difference ranges.
// For more information see the PixelsAbsoluteValueComparator class

struct PixelDifferenceRange {
    int minDifference;    // Minimum value of the range
    int maxDifference;    // Maximum value of the range
    int pixelCount;       // Number of pixels in this range
    double percentage;    // Percentage of pixels in this range

    PixelDifferenceRange(int minDiff, int maxDiff)
        : minDifference(minDiff), maxDifference(maxDiff), pixelCount(0), percentage(0.0) {}
};

#endif // PIXELDIFFRENCERANGE_H
