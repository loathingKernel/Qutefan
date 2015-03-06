#include "gputab.h"
#include "ui_gputab.h"

GpuTab::GpuTab(QWidget* parent) :
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

GpuTab::GpuTab(QNvAPI* api, QNvAPI::NvGPU* gpu, QWidget* parent) :
    GpuTab(parent)
{
    mode = AccessMode::nvapi;

    nvapi = api;
    nvgpu = gpu;

    nvgpu->status = nvapi->GPU_GetCoolerSettings(nvgpu->handle, 0, &nvgpu->coolerSettings);
    nvDefaultCoolerLevels.cooler[0].level = nvgpu->coolerSettings.cooler[0].currentLevel;
    nvDefaultCoolerLevels.cooler[0].policy = nvgpu->coolerSettings.cooler[0].currentPolicy;
    qDebug("Saved defaults");

    ui->spinBoxFixedLevel->setMinimum(nvgpu->coolerSettings.cooler[0].defaultMin);
    ui->spinBoxFixedLevel->setMaximum(nvgpu->coolerSettings.cooler[0].defaultMax);
    ui->spinBoxFixedLevel->setValue(nvgpu->coolerSettings.cooler[0].currentLevel);
    ui->spinBoxFixedLevel->setSingleStep(5);
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

void GpuTab::setGPUDefaults()
{
    nvgpu->status = nvapi->GPU_SetCoolerLevels(nvgpu->handle, 0, &nvDefaultCoolerLevels);
    qDebug("Restored defaults for GPU");
}

void GpuTab::regulateFan()
{
    nvgpu->status = nvapi->GPU_GetThermalSettings(nvgpu->handle, 0, &nvgpu->thermalSettings);
    if(nvMaxTemp < nvgpu->thermalSettings.sensor[0].currentTemp)
        nvMaxTemp = nvgpu->thermalSettings.sensor[0].currentTemp;
    ui->labelTempValue->setText(QString("%1°C").arg(nvgpu->thermalSettings.sensor[0].currentTemp));
    ui->labelTempMax->setText(QString("%1°C").arg(nvMaxTemp));


    GpuTab::FanMode mode = getMode();
    if(mode == GpuTab::FanMode::Off) {
        if(mode != lastMode)
            setGPUDefaults();
    } else {
        NV_GPU_COOLER_LEVELS newCoolerLevels;
        newCoolerLevels.cooler[0].policy = 1;
        switch(mode) {
        case GpuTab::FanMode::Quiet:
            newCoolerLevels.cooler[0].level = nvgpu->coolerSettings.cooler[0].defaultMin;
            break;
        case GpuTab::FanMode::Fixed:
            newCoolerLevels.cooler[0].level = ui->spinBoxFixedLevel->text().toInt();
            break;
        case GpuTab::FanMode::Linear:
            newCoolerLevels.cooler[0].level = nvgpu->thermalSettings.sensor[0].currentTemp + ui->spinBoxLinearOffset->text().toInt();
            break;
        case GpuTab::FanMode::Graph:
            break;
        }
        nvgpu->status = nvapi->GPU_SetCoolerLevels(nvgpu->handle, 0, &newCoolerLevels);
    }
    lastMode = mode;


    nvgpu->status = nvapi->GPU_GetCoolerSettings(nvgpu->handle, 0, &nvgpu->coolerSettings);
    if(nvMaxLevel < nvgpu->coolerSettings.cooler[0].currentLevel)
        nvMaxLevel = nvgpu->coolerSettings.cooler[0].currentLevel;
    ui->labelLevelValue->setText(QString("%1%").arg(nvgpu->coolerSettings.cooler[0].currentLevel));
    ui->labelLevelMax->setText(QString("%1%").arg(nvMaxLevel));
}
