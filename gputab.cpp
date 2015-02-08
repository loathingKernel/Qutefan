#include "gputab.h"
#include "ui_gputab.h"

GpuTab::GpuTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GpuTab)
{
    ui->setupUi(this);

#ifdef Q_OS_LINUX
    QString groupBoxStyle("QGroupBox {border: 1px solid rgba(0, 0, 0, .3); border-radius: 5px; margin-top: 0.5em;}"
                          "QGroupBox::title {subcontrol-origin: margin; left: 10px; padding: 0 3px 0 3px;}");
    ui->groupBoxReadings->setStyleSheet(groupBoxStyle);
#endif
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
