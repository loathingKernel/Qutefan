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

int GpuTab::getFixedLevel()
{
    return ui->spinBoxFixedLevelValue->text ().toInt();
}

int GpuTab::getLinearOffset()
{
    return ui->spinBoxLinearOffsetValue->text().toInt();
}

GpuTab::FanMode GpuTab::getMode()
{
    if(ui->radioButtonOff->isChecked())     return FanMode::Off;
    if(ui->radioButtonQuiet->isChecked())   return FanMode::Quiet;
    if(ui->radioButtonFixed->isChecked())   return FanMode::Fixed;
    if(ui->radioButtonLinear->isChecked())  return FanMode::Linear;
    if(ui->radioButtonGraph->isChecked())   return FanMode::Graph;

    return FanMode::Off;
}
