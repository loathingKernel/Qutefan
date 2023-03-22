#include "doublelabel.h"
#include "ui_doublelabel.h"

DoubleLabel::DoubleLabel(QWidget *parent) : QWidget(parent), ui(new Ui::DoubleLabel)
{
    ui->setupUi(this);
}

DoubleLabel::DoubleLabel(QWidget *parent, const QString &format) : DoubleLabel(parent)
{
    setFormat(format);
}

DoubleLabel::DoubleLabel(QWidget *parent, const QString &format, int max) : DoubleLabel(parent)
{
    setFormat(format, max);
}

DoubleLabel::~DoubleLabel()
{
    delete ui;
}

void DoubleLabel::setFormat(const QString &format, int max)
{
    m_format = format;
    QFont font = this->font();
    QFontMetrics fm = QFontMetrics(font);
    int width =
        fm.boundingRect(m_format.arg(max)).width() + (fm.horizontalAdvance(QLatin1Char('x')) * 2) +
        (ui->labelCurrent->contentsMargins().left() + ui->labelCurrent->contentsMargins().right());

    ui->labelCurrent->setMinimumWidth(width);
    ui->labelMaximum->setMinimumWidth(width);
}

void DoubleLabel::setValue(int value, bool reset)
{
    if (value > m_maximum || reset)
        m_maximum = value;
    ui->labelCurrent->setText(m_format.arg(value));
    ui->labelMaximum->setText(m_format.arg(m_maximum));
}
