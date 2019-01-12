#include "ui_gputab.h"
#include "gputab_nvctrl.h"

GpuTabNVCtrl::GpuTabNVCtrl(QuteFanNVCtrl* _api, QuteFanNVCtrl::NvGPU* _gpu, QWidget* parent) : GpuTab(parent)
{
    api = _api;
    gpu = _gpu;

    gpu->status = XNVCTRLQueryTargetAttribute(api->dpy,
                                              NV_CTRL_TARGET_TYPE_COOLER,
                                              gpu->handle,
                                              0,
                                              NV_CTRL_THERMAL_COOLER_LEVEL,
                                              &default_level);
    qDebug("Saved defaults");

    ui->spinBoxFixedLevel->setMinimum(30);
    ui->spinBoxFixedLevel->setMaximum(100);
    ui->spinBoxFixedLevel->setValue(default_level);
}

GpuTabNVCtrl::~GpuTabNVCtrl()
{
}

void GpuTabNVCtrl::setGPUDefaults()
{
    gpu->status = XNVCTRLSetTargetAttributeAndGetStatus(api->dpy,
                                                        NV_CTRL_TARGET_TYPE_COOLER,
                                                        gpu->handle,
                                                        0,
                                                        NV_CTRL_THERMAL_COOLER_LEVEL,
                                                        default_level);
    gpu->status = XNVCTRLSetTargetAttributeAndGetStatus(api->dpy,
                                                        NV_CTRL_TARGET_TYPE_GPU,
                                                        gpu->handle,
                                                        0,
                                                        NV_CTRL_GPU_COOLER_MANUAL_CONTROL,
                                                        NV_CTRL_GPU_COOLER_MANUAL_CONTROL_FALSE);
    qDebug("Restored defaults for GPU");
}

void GpuTabNVCtrl::regulateFan()
{
    gpu->status = XNVCTRLQueryTargetAttribute(api->dpy,
                                              NV_CTRL_TARGET_TYPE_THERMAL_SENSOR,
                                              gpu->handle,
                                              0,
                                              NV_CTRL_THERMAL_SENSOR_READING,
                                              &temp);
    if(max_temp < temp) max_temp = temp;
    ui->labelStatusTempCur->setText(QString("%1°C").arg(temp));
    ui->labelStatusTempMax->setText(QString("%1°C").arg(max_temp));

    GpuTab::FanMode mode = getMode();
    if(mode == GpuTab::FanMode::Off) {
        if(mode != last_mode)
            setGPUDefaults();
    } else {
        gpu->status = XNVCTRLSetTargetAttributeAndGetStatus(api->dpy,
                                                            NV_CTRL_TARGET_TYPE_GPU,
                                                            gpu->handle,
                                                            0,
                                                            NV_CTRL_GPU_COOLER_MANUAL_CONTROL,
                                                            NV_CTRL_GPU_COOLER_MANUAL_CONTROL_TRUE);;
        switch(mode) {
        case GpuTab::FanMode::Off:
            break;
        case GpuTab::FanMode::Quiet:
            level = default_level;
            break;
        case GpuTab::FanMode::Fixed:
            level = ui->spinBoxFixedLevel->text().toInt();
            break;
        case GpuTab::FanMode::Linear:
            level = temp + ui->spinBoxLinearOffset->text().toInt();
            break;
        case GpuTab::FanMode::Graph:
            break;
        }
        gpu->status = XNVCTRLSetTargetAttributeAndGetStatus(api->dpy,
                                                            NV_CTRL_TARGET_TYPE_COOLER,
                                                            gpu->handle,
                                                            0,
                                                            NV_CTRL_THERMAL_COOLER_LEVEL,
                                                            level);
    }
    last_mode = mode;


    gpu->status = XNVCTRLQueryTargetAttribute(api->dpy,
                                              NV_CTRL_TARGET_TYPE_COOLER,
                                              gpu->handle,
                                              0,
                                              NV_CTRL_THERMAL_COOLER_CURRENT_LEVEL,
                                              &level);
    if(max_level < level) max_level = level;
    ui->labelStatusFanCur->setText(QString("%1%").arg(level));
    ui->labelStatusFanMax->setText(QString("%1%").arg(max_level));
}

void GpuTabNVCtrl::displayStatus()
{
    char* str;
    gpu->status = XNVCTRLQueryTargetStringAttribute(api->dpy,
                                              NV_CTRL_TARGET_TYPE_GPU,
                                              gpu->handle,
                                              0,
                                              NV_CTRL_STRING_GPU_CURRENT_CLOCK_FREQS,
                                              &str);
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
    max_temp = temp;
    ui->labelStatusTempMax->setText(QString("%1°C").arg(max_temp));
    max_level = level;
    ui->labelStatusFanMax->setText(QString("%1%").arg(max_level));
}
