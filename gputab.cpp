#include "gputab.h"
#include "ui_gputab.h"

GpuTab::GpuTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GpuTab)
{
    ui->setupUi(this);
}

GpuTab::~GpuTab()
{
    delete ui;
}

void GpuTab::setTempValue(QString value)
{
    ui->labelTempValue->setText(value);
}

void GpuTab::setLevelValue(QString value)
{
    ui->labelLevelValue->setText(value);
}
