#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "imagefile.h"

class QImage;

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    /*
     *  Reset all labels
     */
    void reset();

    /*
     *  Update labels based on the currently selected image
     */
    void updateImageInformationFromIndex();
    void updateImagePlatesInformationFromIndex();

    /*
     *  Update labels with a number of tagged images
     */
    void updateTaggedAndPlatesInformation();

    /*
     *  Move the currently selected image file to negatives
     *  if necessary
     */
    void moveFileFromCurrentIndex();

private slots:
    // File menu
    void slotOpenDirectory();
    void slotQuit();

    // Edit menu
    void slotOptions();

    // Help menu
    void slotUsage();
    void slotAbout();
    void slotAboutQt();

    // other slots
    void slotPrevious();
    void slotJumpTo();
    void slotNext();
    void slotZoomChanged(double zoom);
    void slotPlatesChanged();

private:
    Ui::MainWindow *ui;
    ImageFileList m_imageFiles;
    int m_currentIndex;
    bool m_firstRun;
};

#endif // MAINWINDOW_H
