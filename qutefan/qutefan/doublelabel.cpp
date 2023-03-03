#include "doublelabel.h"
#include "ui_doublelabel.h"

DoubleLabel::DoubleLabel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DualLabel)
{
    ui->setupUi(this);

    QFont font = this->font();
    QFontMetrics fm = QFontMetrics(font);
    int width =
            fm.boundingRect("xxxxx").width() + (fm.horizontalAdvance(QLatin1Char('x')) * 2) +
            (ui->labelCurrent->contentsMargins().left() + ui->labelCurrent->contentsMargins().right());

    ui->labelCurrent->setMinimumWidth(width);
    ui->labelMaximum->setMinimumWidth(width);
}

DoubleLabel::~DoubleLabel()
{
    delete ui;
}

void DoubleLabel::setCurrent(const QString &level)
{
    ui->labelCurrent->setText(level);

}

void DoubleLabel::setMaximum(const QString &level)
{
    ui->labelMaximum->setText(level);
}
