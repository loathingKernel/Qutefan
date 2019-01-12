#include "ui_gputab.h"
#include "gputab.h"

GpuTab::GpuTab(QWidget* parent) : QWidget(parent), ui(new Ui::GpuTab)
{
    ui->setupUi(this);
    // Hide unfinished UI elements
    ui->groupBoxOverclock->hide();
    ui->radioButtonGraph->hide();
    ui->pushButtonGraph->hide();
    ui->spinBoxFixedLevel->setSingleStep(5);
    connect(ui->pushButtonReset, SIGNAL(pressed()), this, SLOT(resetMax()));
}

GpuTab::~GpuTab()
{
    delete ui;
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
