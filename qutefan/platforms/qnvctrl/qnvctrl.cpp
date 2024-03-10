#include <QDebug>

#include "qnvctrl.h"

ControlNVCtrl::ControlNVCtrl()
{

}

ControlNVCtrl::~ControlNVCtrl()
{
    if(dpy)
        XCloseDisplay(dpy);
}

bool ControlNVCtrl::available()
{
    dpy = XOpenDisplay(nullptr);
    if (!dpy) {
        qDebug("Cannot open display '%s'", XDisplayName(nullptr));
        return false;
    }

    screen = getNvXScreen(dpy);
    if (screen < 0) {
        qDebug("Unable to find any NVIDIA X screens; aborting.");
        return false;
    }

    return true;
}

void ControlNVCtrl::initialize()
{
    status = XNVCTRLQueryVersion(dpy, &(version.major), &(version.minor));
    if (!status) {
        qDebug("The NV-CONTROL X extension does not exist on '%s'.", XDisplayName(nullptr));
        return;
    }

    status = XNVCTRLQueryTargetCount(dpy, NV_CTRL_TARGET_TYPE_GPU, &gpu_count);
    if (!status) {
        qDebug("Failed to query number of gpus");
        return;
    }

    for(int i = 0; i < gpu_count; i++) {
        NvGPU gpu = NvGPU();
        gpu.handle = i;

        // Get general cooler information
        int len = 0;
        NV_COOLER_DATA *p_cooler_data;
        gpu.status = XNVCTRLQueryTargetBinaryData(
                    dpy, NV_CTRL_TARGET_TYPE_GPU, gpu.handle,
                    0, NV_CTRL_BINARY_DATA_COOLERS_USED_BY_GPU, (unsigned char **)(&p_cooler_data), &len);
        gpu.cooler_count = p_cooler_data->count;

        // Get per cooler information
        for(int c = 0; c < gpu.cooler_count; ++c) {
            NvCooler cooler = NvCooler();
            cooler.handle = p_cooler_data->handle[c];
            gpu.cooler.push_back(cooler);
        }
        free((unsigned char *)p_cooler_data);
        qDebug("GPU %i: Number of coolers = %i", gpu_count, gpu.cooler_count);
        m_gpu.push_back(gpu);
    }
    qDebug("Number of GPUs = %i", gpu_count);
}

const QString ControlNVCtrl::name(NvGPU *gpu)
{
    char *nv_name;
    gpu->status = XNVCTRLQueryTargetStringAttribute(
                dpy, NV_CTRL_TARGET_TYPE_GPU, gpu->handle,
                0, NV_CTRL_STRING_PRODUCT_NAME, &nv_name);
    QString name(nv_name);
    free(nv_name);
    return name;
}

const QString ControlNVCtrl::uuid(NvGPU *gpu)
{
    char *nv_uuid;
    gpu->status = XNVCTRLQueryTargetStringAttribute(
                dpy, NV_CTRL_TARGET_TYPE_GPU, gpu->handle,
                0, NV_CTRL_STRING_GPU_UUID, &nv_uuid);
    QString uuid(nv_uuid);
    free(nv_uuid);
    return uuid;
}

ControlNVCtrl::NvGPU *ControlNVCtrl::getGpuByIndex(int index)
{
    return &m_gpu[index];
}

int ControlNVCtrl::getCoolerCount(NvGPU *gpu)
{
    return gpu->cooler_count;
}

Control::CoolerLimits ControlNVCtrl::getCoolerLimits(NvGPU *gpu)
{
    CoolerLimits limits = {};
    limits.maximum = 100;
    limits.minimum = 30;
    // It seems there is no way to get the defaults for a cooler, so quickly reset it to
    // get some values
    setCoolerManualControl(gpu, false);
    gpu->status = XNVCTRLQueryTargetAttribute(
                dpy, NV_CTRL_TARGET_TYPE_COOLER, gpu->cooler[0].handle,
        0, NV_CTRL_THERMAL_COOLER_LEVEL, reinterpret_cast<int*>(&limits.current));
    return limits;
}

void ControlNVCtrl::setCoolerManualControl(NvGPU *gpu, bool enable)
{
    if (enable) {
        gpu->status = XNVCTRLSetTargetAttributeAndGetStatus(
                    dpy, NV_CTRL_TARGET_TYPE_GPU, gpu->handle,
                    0, NV_CTRL_GPU_COOLER_MANUAL_CONTROL, NV_CTRL_GPU_COOLER_MANUAL_CONTROL_TRUE);
    } else {
        gpu->status = XNVCTRLSetTargetAttributeAndGetStatus(
                    dpy, NV_CTRL_TARGET_TYPE_GPU, gpu->handle,
                    0, NV_CTRL_GPU_COOLER_MANUAL_CONTROL, NV_CTRL_GPU_COOLER_MANUAL_CONTROL_FALSE);
    }
}

Control::CoolerLevels ControlNVCtrl::getCoolerLevels(NvGPU *gpu)
{
    CoolerLevels levels = {};
    for (int c = 0; c < gpu->cooler_count; ++c) {
        int level;
        gpu->status = XNVCTRLQueryTargetAttribute(
                    dpy, NV_CTRL_TARGET_TYPE_COOLER, gpu->cooler[c].handle,
                    0, NV_CTRL_THERMAL_COOLER_CURRENT_LEVEL, &level);
        levels.current.append(level);
    }
    levels.count = gpu->cooler_count;
    return levels;
}

void ControlNVCtrl::setCoolerLevels(NvGPU *gpu, int level)
{
    for (int c = 0; c < gpu->cooler_count; ++c) {
        gpu->status = XNVCTRLSetTargetAttributeAndGetStatus(
                    dpy, NV_CTRL_TARGET_TYPE_COOLER, gpu->cooler[c].handle,
                    0, NV_CTRL_THERMAL_COOLER_LEVEL, level);
    }
}

Control::Temperatures ControlNVCtrl::getGpuTemperatures(NvGPU *gpu)
{
    int len;
    NV_THERMAL_SENSOR_DATA *p_sensor_data;
    gpu->status = XNVCTRLQueryTargetBinaryData(
                dpy, NV_CTRL_TARGET_TYPE_GPU, gpu->handle,
                0, NV_CTRL_BINARY_DATA_COOLERS_USED_BY_GPU, (unsigned char **)(&p_sensor_data), &len);

    Temperatures temps = {};
    for (int s = 0; s < p_sensor_data->count; ++s) {

        int target;
        gpu->status = XNVCTRLQueryTargetAttribute(
                    dpy, NV_CTRL_TARGET_TYPE_THERMAL_SENSOR, p_sensor_data->handle[s],
                    0, NV_CTRL_THERMAL_SENSOR_TARGET, &target);

        int reading;
        gpu->status = XNVCTRLQueryTargetAttribute(
                    dpy, NV_CTRL_TARGET_TYPE_THERMAL_SENSOR, p_sensor_data->handle[s],
                    0, NV_CTRL_THERMAL_SENSOR_READING, &reading);

        switch(target) {
            case NV_CTRL_THERMAL_SENSOR_TARGET_NONE: {
                temps.none = reading;
                break;
            }
            case NV_CTRL_THERMAL_SENSOR_TARGET_GPU: {
                temps.gpu = reading;
                break;
            }
            case NV_CTRL_THERMAL_SENSOR_TARGET_MEMORY: {
                temps.memory = reading;
                break;
            }
            case NV_CTRL_THERMAL_SENSOR_TARGET_POWER_SUPPLY: {
                temps.power_supply = reading;
                break;
            }
            case NV_CTRL_THERMAL_SENSOR_TARGET_BOARD: {
                temps.board = reading;
                break;
            }
            default: {
//                qDebug("%s uknown target %d [handle: %d, reading: %d]",
//                       __PRETTY_FUNCTION__, target, p_sensor_data->handle[s], reading);
                break;
            }
        }
    }
    free((unsigned char *)p_sensor_data);
    return temps;
}

Control::Frequencies ControlNVCtrl::getCurrentClockFrequencies(NvGPU *gpu)
{
    Frequencies freqs = {};

    NV_CLOCK_FREQS p_nv_clocks;
    gpu->status = XNVCTRLQueryTargetAttribute(
                dpy, NV_CTRL_TARGET_TYPE_GPU, gpu->handle,
                0, NV_CTRL_GPU_CURRENT_CLOCK_FREQS, (int *)&p_nv_clocks);
    freqs.core = p_nv_clocks.core;
    freqs.memory = p_nv_clocks.memory;
    freqs.shader = 0;
    return freqs;
}

int ControlNVCtrl::getNvXScreen(Display *dpy)
{
    int defaultScreen, screen;

    defaultScreen = DefaultScreen(dpy);

    if (XNVCTRLIsNvScreen(dpy, defaultScreen)) {
        return defaultScreen;
    }

    for (screen = 0; screen < ScreenCount(dpy); screen++) {
        if (XNVCTRLIsNvScreen(dpy, screen)) {
            qDebug("Default X screen %d is not an NVIDIA X screen. Using X screen %d instead.",
                   defaultScreen, screen);
            return screen;
        }
    }
    return -1;
}

