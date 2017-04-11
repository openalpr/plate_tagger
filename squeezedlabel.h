#ifndef SQUEEZEDLABEL_H
#define SQUEEZEDLABEL_H

#include <QLabel>

class QFontMetrics;

/*
 *  Label displaying dots if not enough space for all characters like
 *  long_image_...file.jpg
 */
class SqueezedLabel : public QLabel
{
    Q_OBJECT

public:
    explicit SqueezedLabel(QWidget *parent = 0);
    ~SqueezedLabel();

    void setText(const QString &text);

private slots:
    void updateText();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    QFontMetrics *m_metrics;
    QString m_text;
};

#endif // SQUEEZEDLABEL_H
