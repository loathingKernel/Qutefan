#include "ui_gputab.h"
#include "gputab_nvapi.h"

GpuTabNvAPI::GpuTabNvAPI(ControlNvAPI* api,
                         ControlNvAPI::NvGPU* gpu,
                         QSettings* settings,
                         QWidget* parent) : GpuTab(settings, parent)
{
    m_api = api;
    m_gpu = gpu;

    ControlNvAPI::CoolerLimits limits = m_api->getCoolerLimits(m_gpu);
    ui->spinBoxFixedLevel->setMinimum(limits.minimum);
    ui->spinBoxFixedLevel->setMaximum(limits.maximum);
    ui->spinBoxFixedLevel->setValue(limits.current);

    ControlNvAPI::CoolerLevels levels = m_api->getCoolerLevels(m_gpu);
    addCoolers(levels);
    loadSettings(QString(m_api->uuid(m_gpu)));
}

GpuTabNvAPI::~GpuTabNvAPI()
{

}

void GpuTabNvAPI::saveGpuSettings()
{
    saveSettings(QString(m_api->uuid(m_gpu)));
}

void GpuTabNvAPI::setGPUDefaults()
{
    for(unsigned int c = 0; c < m_gpu->cooler_count; ++c) {
        ControlNvAPI::CoolerLimits limits = m_api->getCoolerLimits(m_gpu);
        m_api->setCoolerLevels(m_gpu, limits.minimum);
        m_api->setCoolerManualControl(m_gpu, false);
    }
    qDebug("Restored defaults for GPU");
}

void GpuTabNvAPI::regulateFans()
{
    ControlNvAPI::Temperature temps = m_api->getGpuTemperatures(m_gpu);
    m_temp_info->setValue(temps.gpu);

    GpuTab::FanMode mode = getMode();
    if(mode == GpuTab::FanMode::Off) {
        if(mode != last_mode)
            setGPUDefaults();
    } else {
        int level = 0;
        m_api->setCoolerManualControl(m_gpu, true);
        switch(mode) {
        case GpuTab::FanMode::Off:
            break;
        case GpuTab::FanMode::Quiet: {
            ControlNvAPI::CoolerLimits limits = m_api->getCoolerLimits(m_gpu);
            level = limits.minimum;
            break;
        }
        case GpuTab::FanMode::Fixed: {
            level = ui->spinBoxFixedLevel->text().toInt();
        }   break;
        case GpuTab::FanMode::Linear: {
            level = temps.gpu + ui->spinBoxLinearOffset->text().toInt();
        }   break;
        case GpuTab::FanMode::Graph: {
        }    break;
        }
        m_api->setCoolerLevels(m_gpu, level);
    }
    last_mode = mode;

    ControlNvAPI::CoolerLevels levels = m_api->getCoolerLevels(m_gpu);
    for (int c = 0; c < levels.count; ++c) {
        m_fan_info[c]->setValue(levels.current[c]);
    }
}

void GpuTabNvAPI::displayFrequencies()
{
    ControlNvAPI::Frequency frequency = m_api->getCurrentClockFrequencies(m_gpu);
    ui->labelStatusCoreCur->setText(QString("%1Mhz").arg(frequency.core));
    ui->labelStatusMemCur->setText(QString("%1Mhz").arg(frequency.memory));
    ui->labelStatusShaderCur->setText(QString("%1Mhz").arg(frequency.shader));
}

void GpuTabNvAPI::resetMaximums()
{
    ControlNvAPI::Temperature temps = m_api->getGpuTemperatures(m_gpu);
    m_temp_info->setValue(temps.gpu, true);
    ControlNvAPI::CoolerLevels levels = m_api->getCoolerLevels(m_gpu);
    for (int c = 0; c < levels.count; ++c) {
        m_fan_info[c]->setValue(levels.current[c], true);
    }
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
