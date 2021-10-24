#include "ui_gputab.h"
#include "gputab.h"

GpuTab::GpuTab(QSettings* _settings, QWidget* parent) : QWidget(parent), ui(new Ui::GpuTab)
{
    ui->setupUi(this);

    settings = _settings;

    // Hide unfinished UI elements
    ui->groupBoxOverclock->hide();
    ui->radioButtonGraph->hide();
    ui->pushButtonGraph->hide();
    ui->pushButtonChart->hide();

    ui->groupBoxCharts->hide();
    ui->spinBoxFixedLevel->setSingleStep(5);

    connect(ui->pushButtonChart, SIGNAL(pressed()), this, SLOT(showChart()));
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

void GpuTab::saveSettings(QString* uuid)
{
    settings->beginGroup(*uuid);
    settings->setValue("mode", static_cast<int>(getMode()));
    settings->setValue("fixed", ui->spinBoxFixedLevel->value());
    settings->setValue("linear", ui->spinBoxLinearOffset->value());
    settings->endGroup();
}

void GpuTab::loadSettings(QString* uuid)
{
    settings->beginGroup(*uuid);
    int mode = settings->value("mode", 0).toInt();
    if (mode == static_cast<int>(FanMode::Off)) ui->radioButtonOff->setChecked(true);
    if (mode == static_cast<int>(FanMode::Quiet)) ui->radioButtonQuiet->setChecked(true);
    if (mode == static_cast<int>(FanMode::Fixed)) ui->radioButtonFixed->setChecked(true);
    if (mode == static_cast<int>(FanMode::Linear)) ui->radioButtonLinear->setChecked(true);
    if (mode == static_cast<int>(FanMode::Graph)) ui->radioButtonGraph->setChecked(true);
    ui->spinBoxFixedLevel->setValue(settings->value("fixed", 0).toInt());
    ui->spinBoxLinearOffset->setValue(settings->value("linear", 0).toInt());
    settings->endGroup();
}
