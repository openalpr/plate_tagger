#include <QtWidgets>
#include <QtGui>

#include "squeezedlabel.h"

SqueezedLabel::SqueezedLabel(QWidget *parent)
    : QLabel(parent)
    , m_metrics(nullptr)
{
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    setTextFormat(Qt::PlainText);

    ensurePolished();
}

SqueezedLabel::~SqueezedLabel()
{
    delete m_metrics;
}

void SqueezedLabel::setText(const QString &text)
{
    m_text = text;
    updateText();
}

void SqueezedLabel::updateText()
{
    if(!m_metrics)
        m_metrics = new QFontMetrics(font());

    QLabel::setText(m_metrics->elidedText(m_text, Qt::ElideMiddle, width()));
}

void SqueezedLabel::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)

    updateText();
}
