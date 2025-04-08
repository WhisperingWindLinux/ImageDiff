#include "helphtmlformatter.h"


QString HelpHtmlFormatter::formatImageProcessorInfo(const ImageProcessorInfo &info) {
    QString html;

    html += QString("<b>Type:</b> %1.<br>").arg(enumToString(info.type));
    html += QString("<b>Full Name:</b> %1.<br>").arg(info.fullName);
    html += QString("<b>Short Name (Menu Display):</b> %1.<br>").arg(info.name);
    html += QString("<b>Hotkey:</b> %1.<br>").arg(info.hotkey);
    html += QString("<b>Description:</b> %1<br><br>").arg(info.description);

    if (!info.properties.isEmpty()) {
        html += "<h3>Properties</h3>";
        html += "<table border='1' cellspacing='0' cellpadding='5'>";
        html += "<tr><th>Parameter Name</th><th>Parameter Description</th></tr>";

        for (const Property &property : info.properties) {
            html += "<tr>";
            html += QString("<td>%1</td>").arg(property.mPropertyName);
            html += QString("<td>%1</td>").arg(formatProperty(property));
            html += "</tr>";
        }

        html += "</table>";
    }

    return html;
}

QString HelpHtmlFormatter::enumToString(ImageProcessorType type) {
    switch (type) {
    case ImageProcessorType::Comparator:
        return "Comparator";
    case ImageProcessorType::Filter:
        return "Filter";
    default:
        return "Unknown";
    }
}

QString HelpHtmlFormatter::formatProperty(const Property &property) {
    QString result;

    switch (property.mPropertyType) {
    case Property::Type::Integer:
        result += "<b>Variable Type</b>: Integer.<br>";
        result += QString("<b>Description</b>: %1<br>").arg(property.mPropertyDescription);
        result += QString("<b>Default Value</b>: %1.<br>").arg(static_cast<int>(property.mDoubleValue));
        result += QString("<b>Max Value</b>: %1.<br>").arg(static_cast<int>(property.mMax));
        result += QString("<b>Min Value</b>: %1.").arg(static_cast<int>(property.mMin));
        break;

    case Property::Type::Real:
        result += "<b>Variable Type</b>: Real.<br>";
        result += QString("<b>Description</b>: %1<br>").arg(property.mPropertyDescription);
        result += QString("<b>Default Value</b>: %1.<br>").arg(property.mDoubleValue);
        result += QString("<b>Max Value</b>: %1.<br>").arg(property.mMax);
        result += QString("<b>Min Value</b>: %1.").arg(property.mMin);
        break;

    case Property::Type::Alternatives:
        result += "<b>Variable Type</b>: List of String Values.<br>";
        result += QString("<b>Description</b>: %1<br>").arg(property.mPropertyDescription);
        result += QString("<b>Alternative Values</b>: %1.<br>").arg(property.mAlternativesValue.join(", "));
        result += QString("<b>Default Choice</b>: %1.").arg(
            property.mAlternativesValue.value(static_cast<int>(property.mDoubleValue)));
        break;

    case Property::Type::FilePath:
        result += "<b>Variable Type</b>: File Path.<br>";
        result += QString("<b>Description</b>: %1").arg(property.mPropertyDescription);
        break;
    }

    return result;
}
