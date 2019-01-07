#include "gputab_nvapi.h"
#include "ui_gputab.h"


GpuTabNvAPI::GpuTabNvAPI(QuteFanNvAPI* _api, QuteFanNvAPI::NvGPU* _gpu, QWidget* parent) : GpuTab(parent)
{
    api = _api;
    gpu = _gpu;

    gpu->status = api->nvapi->GPU_GetCoolerSettings(gpu->handle, 0, &gpu->coolerSettings);
    default_cooler_levels.cooler[0].level = gpu->coolerSettings.cooler[0].currentLevel;
    default_cooler_levels.cooler[0].policy = gpu->coolerSettings.cooler[0].currentPolicy;
    qDebug("Saved defaults");

    ui->spinBoxFixedLevel->setMinimum(gpu->coolerSettings.cooler[0].defaultMin);
    ui->spinBoxFixedLevel->setMaximum(gpu->coolerSettings.cooler[0].defaultMax);
    ui->spinBoxFixedLevel->setValue(gpu->coolerSettings.cooler[0].currentLevel);
}

GpuTabNvAPI::~GpuTabNvAPI(){
    delete ui;
}

void GpuTabNvAPI::setGPUDefaults()
{
    gpu->status = api->nvapi->GPU_SetCoolerLevels(gpu->handle, 0, &default_cooler_levels);
    qDebug("Restored defaults for GPU");
}

void GpuTabNvAPI::regulateFan()
{
    gpu->status = api->nvapi->GPU_GetThermalSettings(gpu->handle, 0, &gpu->thermalSettings);
    if(max_temp < gpu->thermalSettings.sensor[0].currentTemp)
        max_temp = gpu->thermalSettings.sensor[0].currentTemp;
    ui->labelStatusTempCur->setText(QString("%1°C").arg(gpu->thermalSettings.sensor[0].currentTemp));
    ui->labelStatusTempMax->setText(QString("%1°C").arg(max_temp));


    GpuTabNvAPI::FanMode mode = getMode();
    if(mode == GpuTab::FanMode::Off) {
        if(mode != last_mode)
            setGPUDefaults();
    } else {
        NV_GPU_COOLER_LEVELS newCoolerLevels;
        newCoolerLevels.cooler[0].policy = 1;
        switch(mode) {
        case GpuTab::FanMode::Off:
            break;
        case GpuTab::FanMode::Quiet:
            newCoolerLevels.cooler[0].level = gpu->coolerSettings.cooler[0].defaultMin;
            break;
        case GpuTab::FanMode::Fixed:
            newCoolerLevels.cooler[0].level = ui->spinBoxFixedLevel->text().toInt();
            break;
        case GpuTab::FanMode::Linear:
            newCoolerLevels.cooler[0].level = gpu->thermalSettings.sensor[0].currentTemp + ui->spinBoxLinearOffset->text().toInt();
            break;
        case GpuTab::FanMode::Graph:
            break;
        }
        gpu->status = api->nvapi->GPU_SetCoolerLevels(gpu->handle, 0, &newCoolerLevels);
    }
    last_mode = mode;


    gpu->status = api->nvapi->GPU_GetCoolerSettings(gpu->handle, 0, &gpu->coolerSettings);
    if(max_level < gpu->coolerSettings.cooler[0].currentLevel)
        max_level = gpu->coolerSettings.cooler[0].currentLevel;
    ui->labelStatusFanCur->setText(QString("%1%").arg(gpu->coolerSettings.cooler[0].currentLevel));
    ui->labelStatusFanMax->setText(QString("%1%").arg(max_level));
}

void GpuTabNvAPI::displayStatus()
{
    gpu->status = api->nvapi->GPU_GetAllClocks(gpu->handle, &gpu->clocks);

    if(gpu->clocks.clock[30] != 0) {
        ui->labelStatusCoreCur->setText(QString("%1Mhz").arg(gpu->clocks.clock[30]/2000.0, 0, 'f', 1));
        ui->labelStatusShaderCur->setText(QString("%1Mhz").arg(gpu->clocks.clock[30]/1000.0, 0, 'f', 1));
    } else {
        ui->labelStatusCoreCur->setText(QString("%1Mhz").arg(gpu->clocks.clock[0]/1000.0, 0, 'f', 1));
        ui->labelStatusShaderCur->setText(QString("%1Mhz").arg(gpu->clocks.clock[14]/1000.0, 0, 'f', 1));
    }
    ui->labelStatusMemCur->setText(QString("%1Mhz").arg(gpu->clocks.clock[8]/2000.0, 0, 'f', 1));
}

void GpuTabNvAPI::resetMax()
{
    max_temp = gpu->thermalSettings.sensor[0].currentTemp;
    ui->labelStatusTempMax->setText(QString("%1°C").arg(max_temp));
    max_level = gpu->coolerSettings.cooler[0].currentLevel;
    ui->labelStatusFanMax->setText(QString("%1%").arg(max_level));
}
