#include "pixelsabsolutevalueformatter.h"

#include <qlocale.h>
#include <business/imageanalysis/comporators/helpers/mathhelper.h>


QString PixelsAbsolutValueFormatter::formatResultToHtml(QList<PixelDifferenceRange> &result,
                                                        QString comporatorFullName
                                                        )
{
    QLocale locale = QLocale::system();

    QString html;

    html += QString("<h2 style=\"line-height: 2;\">%1</h2>").arg(comporatorFullName);

    html += R"(
        <table border="1" style="
            border-collapse: collapse;
            width: 100%;
            text-align: center;
            table-layout: fixed;">
        <thead>
        <tr style="
            background-color: #f2f2f2;
            font-weight: bold;">
        <th style="padding-left: 10px; padding-right: 10px;">Range</th>
        <th style="padding-left: 10px; padding-right: 10px;">Pixel Count</th>
        <th style="padding-left: 10px; padding-right: 10px;">Pixel Count (%)</th>
        </tr>
        </thead>
        <tbody>
    )";

    for (auto it = result.begin(); it != result.end(); ++it) {
        QString range = QString("%1...%2")
        .arg(it->minDifference)
            .arg(it->maxDifference);
        QString pixelCount = locale.toString(it->pixelCount);

        QString pixelPercentage = MathHelper::formatPercentageValue(it->percentage);

        html += "<tr>";
        html += QString("<td align=\"right\" style=\"padding-left: 10px; padding-right: 10px;\">%1</td>").arg(range);
        html += QString("<td align=\"right\" style=\"padding-left: 10px; padding-right: 10px;\">%1</td>").arg(pixelCount);
        html += QString("<td align=\"right\" style=\"padding-left: 10px; padding-right: 10px;\">%1</td>").arg(pixelPercentage);
        html += "</tr>";
    }

    html += "</tbody>";
    html += "</table>";

    html += "<br /><br />";
    html += "Range contains the absolute values by which the pixels differ in any of the R, G, or B components.";

    return html;
}
