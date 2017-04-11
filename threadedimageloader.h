#ifndef THREADEDIMAGELOADER_H
#define THREADEDIMAGELOADER_H

#include <QThread>
#include <QString>

class QImage;

/*
 *  Thread to asynchronously load images. Allocates an image on a heap. Signal receiver
 *  MUST delete it
 */
class ThreadedImageLoader : public QThread
{
    Q_OBJECT

public:
    ThreadedImageLoader(QObject *parent = nullptr);

    void loadImageFromFile(const QString &imagePath);

protected:
    virtual void run() override;

signals:
    void loaded(const QImage &);

private:
    QString m_imagePath;
};

#endif // THREADEDIMAGELOADER_H
