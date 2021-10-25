#include "ui_gputab.h"
#include "gputab_nvapi.h"

GpuTabNvAPI::GpuTabNvAPI(QuteFanNvAPI* _api,
                         QuteFanNvAPI::NvGPU* _gpu,
                         QSettings* _settings,
                         QWidget* parent) : GpuTab(_settings, parent)
{
    api = _api;
    gpu = _gpu;
    nvapi = api->nvapi;

    gpu->status = nvapi->GPU_GetCoolerSettings(gpu->handle, 0, &gpu->coolerSettings);
    default_cooler_levels.cooler[0].level = gpu->coolerSettings.cooler[0].currentLevel;
    default_cooler_levels.cooler[0].policy = gpu->coolerSettings.cooler[0].currentPolicy;
    qDebug("Saved defaults");

    ui->spinBoxFixedLevel->setMinimum(gpu->coolerSettings.cooler[0].defaultMin);
    ui->spinBoxFixedLevel->setMaximum(gpu->coolerSettings.cooler[0].defaultMax);
    ui->spinBoxFixedLevel->setValue(gpu->coolerSettings.cooler[0].currentLevel);

    gpu->status = nvapi->GPU_GetPCIIdentifiers(gpu->handle,
                                               &gpu->deviceId, &gpu->subSystemId, &gpu->revisionId, &gpu->extDeviceId);

    QStringList id_list = {
        QString::number(gpu->deviceId, 16),
        QString::number(gpu->subSystemId, 16),
        QString::number(gpu->revisionId, 16),
        QString::number(gpu->extDeviceId, 16),
    };
    uuid = new QString(id_list.join("-"));
    loadSettings(uuid);
}

GpuTabNvAPI::~GpuTabNvAPI()
{
}

void GpuTabNvAPI::saveGpuSettings()
{
    saveSettings(uuid);
}

void GpuTabNvAPI::setGPUDefaults()
{
    gpu->status = nvapi->GPU_SetCoolerLevels(gpu->handle, 0, &default_cooler_levels);
    qDebug("Restored defaults for GPU");
}

void GpuTabNvAPI::regulateFan()
{
    gpu->status = nvapi->GPU_GetThermalSettings(gpu->handle, 0, &gpu->thermalSettings);
    if(max_temp < gpu->thermalSettings.sensor[0].currentTemp)
        max_temp = gpu->thermalSettings.sensor[0].currentTemp;
    ui->labelStatusTempCur->setText(QString("%1°C").arg(gpu->thermalSettings.sensor[0].currentTemp));
    ui->labelStatusTempMax->setText(QString("%1°C").arg(max_temp));


    GpuTab::FanMode mode = getMode();
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
        gpu->status = nvapi->GPU_SetCoolerLevels(gpu->handle, 0, &newCoolerLevels);
    }
    last_mode = mode;


    gpu->status = nvapi->GPU_GetCoolerSettings(gpu->handle, 0, &gpu->coolerSettings);
    if(max_level < gpu->coolerSettings.cooler[0].currentLevel)
        max_level = gpu->coolerSettings.cooler[0].currentLevel;
    ui->labelStatusFanCur->setText(QString("%1%").arg(gpu->coolerSettings.cooler[0].currentLevel));
    ui->labelStatusFanMax->setText(QString("%1%").arg(max_level));
}

void GpuTabNvAPI::displayStatus()
{
    gpu->status = nvapi->GPU_GetAllClocks(gpu->handle, &gpu->clocks);

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

#if USE_CHARTS
void GpuTabNvAPI::showChart()
{
    if(ui->groupBoxCharts->isVisible()) {
        return;
    }

    QLineSeries *series = new QLineSeries();
    //TODO: fix this after testing
    for (unsigned long i = 0; i < 600; i++)
        if (history[i].level != 0)
            series->append(history[i].time.toMSecsSinceEpoch(), history[i].level);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle("Fan Speed");
    chart->setMargins(QMargins(0,0,0,0));
    //chart->setBackgroundRoundness(0);

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setRange(QDateTime::currentDateTime().addSecs(-300), QDateTime::currentDateTime());
    axisX->setTickCount(10);
    axisX->setFormat("HH:mm:ss");
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setMax(max_level + 10);
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Duty Cycle (%)");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    //popup->resize(820, 600);
    ui->groupBoxCharts->resize(640, 480);
    chartView->resize(ui->groupBoxCharts->size());
    ui->groupBoxCharts->layout()->addWidget(chartView);
    ui->groupBoxCharts->show();
    //popup->show();
}
#endif
