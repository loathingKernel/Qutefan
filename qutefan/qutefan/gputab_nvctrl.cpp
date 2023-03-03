#if USE_CHARTS
#include <QtCharts>
#endif

#include "ui_gputab.h"
#include "gputab_nvctrl.h"

GpuTabNVCtrl::GpuTabNVCtrl(QuteFanNVCtrl* _api,
                           QuteFanNVCtrl::NvGPU* _gpu,
                           QSettings* _settings,
                           QWidget* parent) : GpuTab(_settings, parent)
{
    api = _api;
    gpu = _gpu;

    ui->spinBoxFixedLevel->setMinimum(30);
    ui->spinBoxFixedLevel->setMaximum(100);
    ui->spinBoxFixedLevel->setValue(gpu->cooler[0].default_level);

    temp_label = new QLabel("Temperature:", this);
    temp_info = new DualLabel(this);
    ui->formLayoutStatus->insertRow(0, temp_label, temp_info);

    for (int c = 0; c < gpu->cooler_count; ++c) {
        QLabel* _label = new QLabel(QString("Fan %1 level:").arg(gpu->cooler[c].handle));
        DualLabel* _info = new DualLabel(this);
        _info->setCurrent(QString("%1%").arg(gpu->cooler[c].current_level));
        ui->formLayoutStatus->insertRow(c + 1, _label, _info);
        fan_label.push_back(_label);
        fan_info.push_front(_info);
    }

    loadSettings(QString(gpu->uuid));
}

GpuTabNVCtrl::~GpuTabNVCtrl()
{
    delete temp_label;
    delete temp_info;
    for (int i = fan_label.size() - 1; i >= 0 ; --i) {
        delete fan_label[i];
        delete fan_info[i];
    }
}

void GpuTabNVCtrl::saveGpuSettings()
{
    saveSettings(QString(gpu->uuid));
}

void GpuTabNVCtrl::setGPUDefaults()
{
    for(int c = 0; c < gpu->cooler_count; ++c) {
        gpu->status = XNVCTRLSetTargetAttributeAndGetStatus(
                    api->dpy, NV_CTRL_TARGET_TYPE_COOLER, gpu->cooler[c].handle,
                    0, NV_CTRL_THERMAL_COOLER_LEVEL, gpu->cooler[c].default_level);
    }
    gpu->status = XNVCTRLSetTargetAttributeAndGetStatus(
                api->dpy, NV_CTRL_TARGET_TYPE_GPU, gpu->handle,
                0, NV_CTRL_GPU_COOLER_MANUAL_CONTROL, NV_CTRL_GPU_COOLER_MANUAL_CONTROL_FALSE);
    qDebug("Restored defaults for GPU");
}

void GpuTabNVCtrl::regulateFan()
{
    gpu->status = XNVCTRLQueryTargetAttribute(
                api->dpy, NV_CTRL_TARGET_TYPE_THERMAL_SENSOR, gpu->handle,
                0, NV_CTRL_THERMAL_SENSOR_READING, &gpu->current_temp);

    if(gpu->maximum_temp < gpu->current_temp)
        gpu->maximum_temp = gpu->current_temp;
    temp_info->setCurrent(QString("%1°C").arg(gpu->current_temp));
    temp_info->setMaximum(QString("%1°C").arg(gpu->maximum_temp));

    GpuTab::FanMode mode = getMode();
    if(mode == GpuTab::FanMode::Off) {
        if(mode != last_mode)
            setGPUDefaults();
    } else {
        gpu->status = XNVCTRLSetTargetAttributeAndGetStatus(
                    api->dpy, NV_CTRL_TARGET_TYPE_GPU, gpu->handle,
                    0, NV_CTRL_GPU_COOLER_MANUAL_CONTROL, NV_CTRL_GPU_COOLER_MANUAL_CONTROL_TRUE);

        for (int c = 0; c < gpu->cooler_count; ++c) {
            int level;
            switch(mode) {
            case GpuTab::FanMode::Off:
            case GpuTab::FanMode::Graph:
            case GpuTab::FanMode::Quiet:
            default:
                level = gpu->cooler[c].default_level;
                break;
            case GpuTab::FanMode::Fixed:
                level = ui->spinBoxFixedLevel->text().toInt();
                break;
            case GpuTab::FanMode::Linear:
                level = gpu->current_temp + ui->spinBoxLinearOffset->text().toInt();
                break;
            }
            gpu->status = XNVCTRLSetTargetAttributeAndGetStatus(
                        api->dpy, NV_CTRL_TARGET_TYPE_COOLER, gpu->cooler[c].handle,
                        0, NV_CTRL_THERMAL_COOLER_LEVEL, level);
        }
    }
    last_mode = mode;

    for (int c = 0; c < gpu->cooler_count; ++c) {
        gpu->status = XNVCTRLQueryTargetAttribute(
                    api->dpy, NV_CTRL_TARGET_TYPE_COOLER, gpu->cooler[c].handle,
                    0, NV_CTRL_THERMAL_COOLER_CURRENT_LEVEL, &gpu->cooler[c].current_level);

        if(gpu->cooler[c].maximum_level < gpu->cooler[c].current_level)
            gpu->cooler[c].maximum_level = gpu->cooler[c].current_level;
        fan_info[c]->setCurrent(QString("%1%").arg(gpu->cooler[c].current_level));
        fan_info[c]->setMaximum(QString("%1%").arg(gpu->cooler[c].maximum_level));
    }
#if USE_CHARTS
    current_entry->time = QDateTime::currentDateTime();
    current_entry->level = level;
    current_entry->temp = temp;
    current_entry++;
    if(current_entry > &history[599]) current_entry = history;
# endif
}

void GpuTabNVCtrl::displayStatus()
{
    char* str;
    gpu->status = XNVCTRLQueryTargetStringAttribute(
                api->dpy,
                NV_CTRL_TARGET_TYPE_GPU,
                gpu->handle,
                0,
                NV_CTRL_STRING_GPU_CURRENT_CLOCK_FREQS,
                &str
    );
    QStringList list = QStringList(QString(str).split(", "));
    XFree(str);

    QMap<QString, int> clocks;
    for(int i = 0; i < list.size(); i++) {
        clocks.insert(list[i].split("=").first(), list[i].split("=").last().toInt());
    }

    ui->labelStatusCoreCur->setText(QString("%1Mhz").arg(clocks["nvclock"]));
    //ui->labelStatusShaderCur->setText(QString("%1Mhz").arg(clocks["memTransferRate"]));
    ui->labelStatusMemCur->setText(QString("%1Mhz").arg(clocks["memclock"]));
}

void GpuTabNVCtrl::resetMax()
{
    gpu->maximum_temp = gpu->current_temp;
    temp_info->setMaximum(QString("%1°C").arg(gpu->current_temp));
    for (int c = 0; c < gpu->cooler_count; ++c) {
        gpu->cooler[c].maximum_level = gpu->cooler[c].current_level;
        fan_info[c]->setMaximum(QString("%1%").arg(gpu->cooler[c].maximum_level));
    }
}

#if USE_CHARTS
void GpuTabNVCtrl::showChart()
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
