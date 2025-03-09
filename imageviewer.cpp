#include "imageviewer.h"
#include <QtGui/qevent.h>
#include <QtWidgets/qgraphicsitem.h>
#include <QtWidgets/qstatusbar.h>
#include <qfiledialog.h>
#include <qmainwindow.h>
#include <pixeldifferencecalc.h>
#include <qmessagebox.h>
#include <ImageComparator.h>
#include <qapplication.h>
#include <qprocess.h>
#include <PixelsBrightnessComparator.h>
#include <ContrastComporator.h>
#include <ColorsSaturationComporator.h>

ImageViewer::ImageViewer(MainWindow *parent)
    : QGraphicsView(parent),
    currentImageIndex(0),
    isComparisonImageShowing(false),
    scaleFactor(1.0),
    parent(parent)
{
    scene = new QGraphicsScene(this);
    setScene(scene);

    setDragMode(QGraphicsView::ScrollHandDrag);
}

ImageViewer::~ImageViewer() {
    if (imageViewInteractor != nullptr) {
        delete imageViewInteractor;
        imageViewInteractor = nullptr;
    }
    if (firstImagePixmap != nullptr) {
        delete firstImagePixmap;
        firstImagePixmap = nullptr;
    }
    if (secondImagePixmap != nullptr) {
        delete secondImagePixmap;
        secondImagePixmap = nullptr;
    }
    if (comparisonImagePixmap != nullptr) {
        delete comparisonImagePixmap;
        comparisonImagePixmap = nullptr;
    }

    if (parent != nullptr) {
        QString status = "";
        parent->showStatusMessage(status);
    }
}

void ImageViewer::keyPressEvent(QKeyEvent *event) {
    QGraphicsView::keyPressEvent(event);
}

void ImageViewer::wheelEvent(QWheelEvent *event) {
    if (event->angleDelta().y() > 0) {
        zoomIn();
    } else {
        zoomOut();
    }
}

bool ImageViewer::loadImages() {
    try {
        QString firstImagePath = QFileDialog::getOpenFileName(nullptr, "Open First Image", "", "Images (*.png)");
        QString secondImagePath = QFileDialog::getOpenFileName(nullptr, "Open Second Image", "", "Images (*.png)");

        imageViewInteractor = new ImageViewInteractor(firstImagePath, secondImagePath);
        imageViewInteractor->loadImages();

        QPixmap pixmap1 = imageViewInteractor->getFirstImagePixmap();
        QPixmap pixmap2 = imageViewInteractor->getSecondImagePixmap();
        QPixmap pixmap3 = imageViewInteractor->getComparisonImagePixmap();

        QString file1Path = imageViewInteractor->getFirstImagePath();

        firstImagePixmap = scene->addPixmap(pixmap1);
        secondImagePixmap = scene->addPixmap(pixmap2);
        comparisonImagePixmap = scene->addPixmap(pixmap3);
        parent->showStatusMessage(file1Path);
        secondImagePixmap->setVisible(false);
        comparisonImagePixmap->setVisible(false);
        setSceneRect(firstImagePixmap->boundingRect());
        return true;
    }
    catch (const  std::runtime_error &e) {
        showError(e.what());
        return false;
    }
}

void ImageViewer::toggleImage() {

    if (isComparisonImageShowing) {
        return;
    }

    QRectF viewRect = mapToScene(viewport()->geometry()).boundingRect();

    if (currentImageIndex == 0) {
        firstImagePixmap->setVisible(false);
        secondImagePixmap->setVisible(true);
        currentImageIndex = 1;
        QString file2Path = imageViewInteractor->getSecondImagePath();
        parent->showStatusMessage(file2Path);
    } else {
        firstImagePixmap->setVisible(true);
        secondImagePixmap->setVisible(false);
        currentImageIndex = 0;
        QString file1Path = imageViewInteractor->getFirstImagePath();
        parent->showStatusMessage(file1Path);
    }

    centerOn(viewRect.center());
}

void ImageViewer::showDifferenceAsImage() {

    QRectF viewRect = mapToScene(viewport()->geometry()).boundingRect();

    if (isComparisonImageShowing) {
        QString file1Path = imageViewInteractor->getFirstImagePath();
        QString file2Path = imageViewInteractor->getSecondImagePath();

        currentImageIndex == 0 ?
            firstImagePixmap->setVisible(true)
                               : secondImagePixmap->setVisible(true);

        currentImageIndex == 0 ?
            parent->showStatusMessage(file1Path)
                               : parent->showStatusMessage(file2Path);

        comparisonImagePixmap->setVisible(false);
        isComparisonImageShowing = false;
    } else {
        QString file3Path = imageViewInteractor->getComparisonImagePath();
        firstImagePixmap->setVisible(false);
        secondImagePixmap->setVisible(false);
        comparisonImagePixmap->setVisible(true);
        parent->showStatusMessage(file3Path);
        isComparisonImageShowing = true;
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

void ImageViewer::showAbsolutePixelsValueDifference() {
    try {
        QString file1Path = imageViewInteractor->getFirstImagePath();
        QString file2Path = imageViewInteractor->getSecondImagePath();
        auto differences = PixelDifferenceCalc::calculatePixelDifferences(file1Path, file2Path);

        QString resultText;
        resultText.append("<b>Pixel Shade Differences by Range</b><br><br>");

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
        showError(e.what());
    }
}

void ImageViewer::showPixelsBrigthnessDifference() {
    try {
        QString file1Path = imageViewInteractor->getFirstImagePath();
        QString file2Path = imageViewInteractor->getSecondImagePath();
        auto differences = PixelsBrightnessComparator::compareImages(file1Path, file2Path);

        QString resultText = PixelsBrightnessComparator::formatResultToHtml(differences);
        QMessageBox msgBox;
        msgBox.setWindowTitle("Pixel Difference Analysis");
        msgBox.setTextFormat(Qt::RichText);
        msgBox.setText(resultText);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

    } catch (const std::runtime_error &e) {
        showError(e.what());
    }
}

void ImageViewer::showPixeslContrastDifference() {
    try {
        QString file1Path = imageViewInteractor->getFirstImagePath();
        QString file2Path = imageViewInteractor->getSecondImagePath();
        auto differences = ContrastComporator::compareImages(file1Path, file2Path);

        QString resultText = ContrastComporator::formatResultToHtml(differences);
        QMessageBox msgBox;
        msgBox.setWindowTitle("Pixel Difference Analysis");
        msgBox.setTextFormat(Qt::RichText);
        msgBox.setText(resultText);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

    } catch (const std::runtime_error &e) {
        showError(e.what());
    }
}

void ImageViewer::showPixelsSaturationDifference() {
    try {
        QString file1Path = imageViewInteractor->getFirstImagePath();
        QString file2Path = imageViewInteractor->getSecondImagePath();

        std::unique_ptr<ColorsSaturationComporator> comparator =
            std::make_unique<ColorsSaturationComporator>(file1Path, file2Path);
        auto differences = comparator->compareImages();

        QString resultText = ColorsSaturationComporator::formatResultToHtml(differences);
        QMessageBox msgBox;
        msgBox.setWindowTitle("Pixel Difference Analysis");
        msgBox.setTextFormat(Qt::RichText);
        msgBox.setText(resultText);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    } catch (const std::runtime_error &e) {
        showError(e.what());
    }
}

void ImageViewer::showError(const QString &errorMessage) {
    qDebug() << errorMessage;

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText(errorMessage);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}
