#include "qutefan_nvctrl.h"


ControllerNVCtrl::ControllerNVCtrl()
{

}

ControllerNVCtrl::~ControllerNVCtrl()
{
    for(int i = 0; i < gpu_count; i ++) {
        XFree(gpu[i].name);
        XFree(gpu[i].uuid);
    }
}

bool ControllerNVCtrl::available()
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

void ControllerNVCtrl::initialize()
{
    status = XNVCTRLQueryVersion(dpy, &(version.major), &(version.minor));
    if (status != True) {
        qDebug("The NV-CONTROL X extension does not exist on '%s'.", XDisplayName(nullptr));
        return;
    }

    status = XNVCTRLQueryTargetCount(dpy, NV_CTRL_TARGET_TYPE_GPU, &gpu_count);
    if (!status) {
        qDebug("Failed to query number of gpus");
        return;
    }

    for(int i = 0; i < gpu_count; i++) {
        NvGPU _gpu = NvGPU();
        _gpu.handle = i;
        _gpu.status = XNVCTRLQueryTargetStringAttribute(
                    dpy, NV_CTRL_TARGET_TYPE_GPU, _gpu.handle,
                    0, NV_CTRL_STRING_PRODUCT_NAME, &_gpu.name);
        if (!_gpu.status) {
            qDebug("Failed to query gpu product name");
            return;
        }
        _gpu.status = XNVCTRLQueryTargetStringAttribute(
                    dpy, NV_CTRL_TARGET_TYPE_GPU, _gpu.handle,
                    0, NV_CTRL_STRING_GPU_UUID, &_gpu.uuid);

        // Get general cooler information
        int len, *cooler_data = NULL;
        _gpu.status = XNVCTRLQueryTargetBinaryData(
                    dpy, NV_CTRL_TARGET_TYPE_GPU, _gpu.handle,
                    0, NV_CTRL_BINARY_DATA_COOLERS_USED_BY_GPU, (unsigned char **)(&cooler_data), &len);
        _gpu.cooler_count = cooler_data[0];

        // Get per cooler information
        for(int j = 1; j <= _gpu.cooler_count; ++j) {
            NvCooler _cooler = NvCooler();

            _cooler.handle = cooler_data[j];

            _gpu.status = XNVCTRLQueryTargetAttribute(
                        dpy, NV_CTRL_TARGET_TYPE_COOLER, _cooler.handle,
                        0, NV_CTRL_THERMAL_COOLER_LEVEL, &_cooler.default_level);

            _gpu.status = XNVCTRLQueryTargetAttribute(
                        dpy, NV_CTRL_TARGET_TYPE_COOLER, _cooler.handle,
                        0, NV_CTRL_THERMAL_COOLER_CURRENT_LEVEL, &_cooler.current_level);
            qDebug("Saved defaults");

            _gpu.cooler.push_back(_cooler);
        }
        delete cooler_data;

        gpu.push_back(_gpu);
    }
}

void ControllerNVCtrl::setCoolerManualControl(NvGPU* _gpu, bool enable)
{
    if (enable) {
        _gpu->status = XNVCTRLSetTargetAttributeAndGetStatus(
                    dpy, NV_CTRL_TARGET_TYPE_GPU, _gpu->handle,
                    0, NV_CTRL_GPU_COOLER_MANUAL_CONTROL, NV_CTRL_GPU_COOLER_MANUAL_CONTROL_TRUE);
    } else {
        _gpu->status = XNVCTRLSetTargetAttributeAndGetStatus(
                    dpy, NV_CTRL_TARGET_TYPE_GPU, _gpu->handle,
                    0, NV_CTRL_GPU_COOLER_MANUAL_CONTROL, NV_CTRL_GPU_COOLER_MANUAL_CONTROL_FALSE);
    }
}

QList<int> ControllerNVCtrl::getCoolerLevel(NvGPU* _gpu, NvCooler *_cooler)
{
    _gpu->status = XNVCTRLQueryTargetAttribute(
                dpy, NV_CTRL_TARGET_TYPE_COOLER, _cooler->handle,
                0, NV_CTRL_THERMAL_COOLER_CURRENT_LEVEL, &_cooler->current_level);

    if(_cooler->maximum_level < _cooler->current_level)
        _cooler->maximum_level = _cooler->current_level;

    return QList<int>({_cooler->current_level, _cooler->maximum_level});
}

void ControllerNVCtrl::setCoolerLevel(NvGPU* _gpu, NvCooler* _cooler, int level)
{
    _gpu->status = XNVCTRLSetTargetAttributeAndGetStatus(
                dpy, NV_CTRL_TARGET_TYPE_COOLER, _cooler->handle,
                0, NV_CTRL_THERMAL_COOLER_LEVEL, level);
}

QList<int> ControllerNVCtrl::getGpuTemperature(NvGPU* _gpu)
{
    _gpu->status = XNVCTRLQueryTargetAttribute(
                dpy, NV_CTRL_TARGET_TYPE_THERMAL_SENSOR, _gpu->handle,
                0, NV_CTRL_THERMAL_SENSOR_READING, &_gpu->current_temp);

    if(_gpu->maximum_temp < _gpu->current_temp)
        _gpu->maximum_temp = _gpu->current_temp;

    return QList<int>({_gpu->current_temp, _gpu->maximum_temp});
}

QMap<QString, int> ControllerNVCtrl::getCurrentClockFreqs(NvGPU * _gpu)
{
    char* str;
    _gpu->status = XNVCTRLQueryTargetStringAttribute(
                dpy, NV_CTRL_TARGET_TYPE_GPU, _gpu->handle,
                0, NV_CTRL_STRING_GPU_CURRENT_CLOCK_FREQS, &str);
    QStringList list = QStringList(QString(str).split(", "));
    XFree(str);

    QMap<QString, int> clocks;
    for(int i = 0; i < list.size(); i++) {
        clocks.insert(list[i].split("=").first(), list[i].split("=").last().toInt());
    }
    return clocks;
}

int ControllerNVCtrl::getNvXScreen(Display *dpy)
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
