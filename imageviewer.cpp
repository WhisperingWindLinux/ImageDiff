#include "imageviewer.h"
#include <QtGui/qevent.h>
#include <QtWidgets/qgraphicsitem.h>
#include <MainWindow.h>
#include <QtWidgets/qstatusbar.h>
#include <qfiledialog.h>
#include <qmainwindow.h>
#include <pixeldifferencecalc.h>
#include <qmessagebox.h>
#include <ImageComparator.h>
#include <qapplication.h>
#include <qprocess.h>

ImageViewer::ImageViewer(MainWindow *parent)
    : QGraphicsView(parent), currentImageIndex(0), scaleFactor(1.0), parent(parent) {
    scene = new QGraphicsScene(this);
    setScene(scene);

    isCompareResultImageShowing = false;

    try {
        loadImages();
    } catch (const  std::runtime_error &e) {
        showErrorAndExit(e.what());
    }

    if (!images[0].isNull()) {
        pixmapItem1 = scene->addPixmap(images[0]);
        pixmapItem2 = scene->addPixmap(images[1]);
        pixmapItem3ComparisonResult = scene->addPixmap(images[2]);
        parent->showStatusMessage(file1);
        pixmapItem2->setVisible(false);
        pixmapItem3ComparisonResult->setVisible(false);
        setSceneRect(pixmapItem1->boundingRect());
    }

    setDragMode(QGraphicsView::ScrollHandDrag);
}

void ImageViewer::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_S) {
        toggleImage();
    } else if (event->key() == Qt::Key_C) {
        showCompareResultImage();
    } else if (event->key() == Qt::Key_D) {
        showCalculatedImageDiff();
    }
    else {
        QGraphicsView::keyPressEvent(event);
    }
}

void ImageViewer::wheelEvent(QWheelEvent *event) {
    if (event->angleDelta().y() > 0) {
        zoomIn();
    } else {
        zoomOut();
    }
}

void ImageViewer::loadImages() {
    file1 = QFileDialog::getOpenFileName(nullptr, "Open First Image", "", "Images (*.png)");
    file2 = QFileDialog::getOpenFileName(nullptr, "Open Second Image", "", "Images (*.png)");

    if (validateFilePath(file1) && validateFilePath(file2)) {
        images[0].load(file1);
        images[1].load(file2);

        if (images[0].size() != images[1].size()) {
            throw std::runtime_error("Error: Images must have the same resolution!");
        }
    } else {
        throw std::runtime_error("Error: Both images must be selected!");
    }

    try {
        ImageComparator comparator(file1, file2);
        file3ComparisonResult = comparator.compareImagesAndSaveResultAsImage();
        qDebug() << "Result saved at:" << file3ComparisonResult;
    } catch (const std::exception &e) {
        QString errorMsg = QString("Error:") + e.what();
        throw std::runtime_error(errorMsg.toStdString());
    }

    if (file3ComparisonResult.isEmpty()) {
        throw std::runtime_error("Error: Unable to compare images!");
    } else {
        images[2].load(file3ComparisonResult);
    }
}

bool ImageViewer::validateFilePath(const QString &filePath) {
    if (filePath.isEmpty()) {
        return false;
    }

    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists() || !fileInfo.isFile()) {
        return false;
    }

    return true;
}

void ImageViewer::toggleImage() {
    if (isCompareResultImageShowing) {
        return;
    }

    QRectF viewRect = mapToScene(viewport()->geometry()).boundingRect();

    if (currentImageIndex == 0) {
        pixmapItem1->setVisible(false);
        pixmapItem2->setVisible(true);
        currentImageIndex = 1;
        parent->showStatusMessage(file2);
    } else {
        pixmapItem1->setVisible(true);
        pixmapItem2->setVisible(false);
        currentImageIndex = 0;
        parent->showStatusMessage(file1);
    }

    centerOn(viewRect.center());
}

void ImageViewer::showCompareResultImage() {
    QRectF viewRect = mapToScene(viewport()->geometry()).boundingRect();

    if (isCompareResultImageShowing) {
        currentImageIndex == 0 ? pixmapItem1->setVisible(true) : pixmapItem2->setVisible(true);
        currentImageIndex == 0 ? parent->showStatusMessage(file1) : parent->showStatusMessage(file2);
        pixmapItem3ComparisonResult->setVisible(false);
        isCompareResultImageShowing = false;
    } else {
        pixmapItem1->setVisible(false);
        pixmapItem2->setVisible(false);
        pixmapItem3ComparisonResult->setVisible(true);
        parent->showStatusMessage(file3ComparisonResult);
        isCompareResultImageShowing = true;
    }

    centerOn(viewRect.center());
}

void ImageViewer::zoomIn() {
    scale(1.25, 1.25);
    scaleFactor *= 1.25;
}

void ImageViewer::zoomOut() {
    scale(0.8, 0.8);
    scaleFactor *= 0.8;
}

void ImageViewer::showCalculatedImageDiff() {
    try {
        auto differences = PixelDifferenceCalc::calculatePixelDifferences(file1, file2);

        QString resultText;
        resultText.append("<b>Pixel Differences by Range:</b><br><br>");

        for (auto it = differences.begin(); it != differences.end(); ++it) {
            resultText.append(QString("Range <font color=\"green\">[%1-%2]</font>: %3 pixels (<font color=\"red\">%4%</font>)<br>")
                                  .arg(it.key().first)
                                  .arg(it.key().second)
                                  .arg(it.value().first)
                                  .arg(QString::number(it.value().second, 'f', 2)));
        }

        QMessageBox msgBox;
        msgBox.setWindowTitle("Pixel Difference Analysis");
        msgBox.setTextFormat(Qt::RichText);
        msgBox.setText(resultText);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

    } catch (const std::runtime_error &e) {
        showErrorAndExit(e.what());
    }
}

void ImageViewer::showErrorAndExit(const QString &errorMessage) {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText(errorMessage);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    msgBox.exec();

    qDebug() << errorMessage;

    throw std::runtime_error(errorMessage.toStdString());
}
