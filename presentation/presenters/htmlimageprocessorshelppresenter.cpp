#include "htmlimageprocessorshelppresenter.h"


QString HtmlImageProcessorsHelpPresenter::formatToHTML(
                                                const QList<ImageProcessorInfo> &processors
                                                )
{
    QString html = R"(
        <html>
        <head>
            <style>
                table {
                    border-collapse: collapse;
                    width: 100%;
                }
                th, td {
                    border: 1px solid #ddd;
                    padding: 8px;
                }
                th {
                    background-color: #f4f4f4;
                    text-align: left;
                }
                tr:nth-child(even) {
                    background-color: #f9f9f9;
                }
                tr:hover {
                    background-color: #f1f1f1;
                }
            </style>
        </head>
        <body>
            <center><h2 style="line-height: 2;">Image Processing Algorithms</h2></center>
            <table>
                <tr>
                    <th>Name</th>
                    <th>Description</th>
                    <th>Hotkey</th>
                    <th>Type</th>
                </tr>
    )";

    for (const ImageProcessorInfo &processor : processors) {

        QString processorType;
        if (processor.type == ImageProcessorType::Filter) {
            processorType = "Filter";
        } else if (processor.type == ImageProcessorType::Comparator) {
            processorType = "Comparator";
        } else {
            processorType = "";
        }

        html += "<tr>";
        html += "<td>" + processor.name + "</td>";
        html += "<td>" + processor.description + "</td>";
        html += "<td>" + processor.hotkey + "</td>";
        html += "<td>" + processorType  + "</td>";
        html += "</tr>";
    }

    html += R"(
            </table>
        </body>
        </html>
    )";

    return html;
}
