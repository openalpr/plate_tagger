#include <QtWidgets>
#include <QtGui>
#include <QtCore>

#include "threadedimageloader.h"
#include "QProgressIndicator.h"
#include "imagefile.h"
#include "imageview.h"
#include "utils.h"
#include "ui_imageview.h"

ImageView::ImageView(QWidget *parent)
    : QStackedWidget(parent)
    , ui(new Ui::ImageView)
    , m_imageFile(nullptr)
    , m_threadedImageLoader(new ThreadedImageLoader(this))
{
    ui->setupUi(this);

    // the image has been loaded
    connect(m_threadedImageLoader, SIGNAL(loaded(QImage)), this, SLOT(slotImageFileLoaded(QImage)));

    // proxying signals
    connect(ui->imageViewer, SIGNAL(zoomChanged(double)), this, SIGNAL(zoomChanged(double)));
    connect(ui->imageViewer, SIGNAL(platesChanged()),     this, SIGNAL(platesChanged()));
}

ImageView::~ImageView()
{
    delete ui;
}

void ImageView::setImageFile(ImageFile *imageFile)
{
    qDebug("Loading image '%s'", qPrintable(imageFile->fileInfo().canonicalFilePath()));

    if(busy())
        return;

    m_imageFile = imageFile;

    setCurrentIndex(0);
    ui->progress->startAnimation();

    m_threadedImageLoader->loadImageFromFile(imageFile->fileInfo().canonicalFilePath());
}

bool ImageView::busy() const
{
    return m_threadedImageLoader->isRunning();
}

void ImageView::reset()
{
    if(busy())
        return;

    setCurrentIndex(0);

    m_imageFile = nullptr;
    ui->imageViewer->reset();
}

void ImageView::setImage(const QImage &image)
{
    if(!m_imageFile)
        return;

    setCurrentIndex(1);
    ui->progress->stopAnimation();

    ui->imageViewer->setImageFile(m_imageFile);
    ui->imageViewer->setImage(image);
}

void ImageView::slotImageFileLoaded(const QImage &image)
{
    if(image.isNull())
    {
        setCurrentIndex(1);
        Utils::error(tr("Cannot load the specified image. Possibly it's corrupted"), this);
        return;
    }

    setImage(image);
}
