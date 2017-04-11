#include <QtGui>
#include <QtCore>

#include "threadedimageloader.h"

ThreadedImageLoader::ThreadedImageLoader(QObject *parent)
    : QThread(parent)
{}

void ThreadedImageLoader::loadImageFromFile(const QString &imagePath)
{
    m_imagePath = imagePath;

    start();
}

void ThreadedImageLoader::run()
{
    emit loaded(QImage(m_imagePath));
}
