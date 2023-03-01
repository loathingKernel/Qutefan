#include "dual_label.h"
#include "ui_dual_label.h"

DualLabel::DualLabel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DualLabel)
{
    ui->setupUi(this);

    QFont font = this->font();
    QFontMetrics fm = QFontMetrics(font);
    int width =
            fm.boundingRect("100%").width() + (fm.horizontalAdvance(QLatin1Char('x')) * 2) +
            (ui->labelCurrent->contentsMargins().left() + ui->labelCurrent->contentsMargins().right());

    ui->labelCurrent->setMinimumWidth(width);
    ui->labelMaximum->setMinimumWidth(width);
}

DualLabel::~DualLabel()
{
    delete ui;
}

void DualLabel::setCurrent(const QString &level)
{
    ui->labelCurrent->setText(level);

}

void DualLabel::setMaximum(const QString &level)
{
    ui->labelMaximum->setText(level);
}
