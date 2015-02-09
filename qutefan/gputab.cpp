#include "gputab.h"
#include "ui_gputab.h"

GpuTab::GpuTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GpuTab)
{
    ui->setupUi(this);

#ifdef Q_OS_LINUX
    QString groupBoxStyle("QGroupBox {border: 1px solid rgba(0, 0, 0, .3); border-radius: 2px; margin-top: 0.5em;}"
                          "QGroupBox::title {subcontrol-origin: margin; left: 10px; padding: 0 3px 0 3px;}");
    ui->groupBoxReadings->setStyleSheet(groupBoxStyle);
#endif
}

GpuTab::~GpuTab()
{
    delete ui;
}

void GpuTab::setTempValues(QString value, QString max)
{
    ui->labelTempValue->setText(value);
    ui->labelTempMax->setText(max);
}

void GpuTab::setLevelValues(QString value, QString max)
{
    ui->labelLevelValue->setText(value);
    ui->labelLevelMax->setText(max);
}

void GpuTab::setFixedLevelLimits(int min, int max, int initial, int step)
{
    ui->spinBoxFixedLevel->setMinimum(min);
    ui->spinBoxFixedLevel->setMaximum(max);
    ui->spinBoxFixedLevel->setValue(initial);
    ui->spinBoxFixedLevel->setSingleStep(step);
}

int GpuTab::getFixedLevel()
{
    return ui->spinBoxFixedLevel->text ().toInt();
}

int GpuTab::getLinearOffset()
{
    return ui->spinBoxLinearOffset->text().toInt();
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
