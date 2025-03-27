#ifndef EXTERNALIMAGEVIEWERDIALOG_H
#define EXTERNALIMAGEVIEWERDIALOG_H

#include <QDialog>
#include <QImage>
#include <QMenuBar>
#include <QStatusBar>

class ExternalGraphicsView;

class ExternalImageViewerDialog : public QDialog {
    Q_OBJECT

public:
    explicit ExternalImageViewerDialog(const QPixmap &image, const QString &description, QWidget *parent = nullptr);

protected:
    void showEvent(QShowEvent*) override;
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    ExternalGraphicsView *graphicsView;

    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupMenu();
    void saveWindowState();
    void restoreWindowState();
};

#endif // EXTERNALIMAGEVIEWERDIALOG_H
