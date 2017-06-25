#ifndef DIRECTORYLOADER_H
#define DIRECTORYLOADER_H

#include <QStringList>
#include <QDialog>

#include "imagefile.h"

class QDirIterator;

namespace Ui
{
    class DirectoryLoader;
}

/*
 *  Directory loader with a spinning progress indicator. Calls accept()
 *  when some files has been found
 */
class DirectoryLoader : public QDialog
{
    Q_OBJECT

public:
    explicit DirectoryLoader(const QString &dirPath, QWidget *parent = 0);
    ~DirectoryLoader();

    /*
     *  Found image files
     */
    ImageFileList imageFiles() const;

protected:
    virtual void keyPressEvent(QKeyEvent *e) override;

private slots:
    void slotLoadNext();

private:
    Ui::DirectoryLoader *ui;
    enum class Step { Images, Plates } m_step;
    QDirIterator *m_iterator;
    ImageFileList m_imageFiles;
    int m_heapPlates;
    QStringList m_lostPlates;
};

inline
ImageFileList DirectoryLoader::imageFiles() const
{
    return m_imageFiles;
}

#endif // DIRECTORYLOADER_H
