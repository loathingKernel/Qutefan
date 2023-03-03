#include "qutefan_nvctrl.h"

QuteFanNVCtrl::QuteFanNVCtrl()
{

}

QuteFanNVCtrl::~QuteFanNVCtrl()
{
    for(int i = 0; i < gpu_count; i ++) {
        XFree(gpu[i].name);
        XFree(gpu[i].uuid);
    }
}

bool QuteFanNVCtrl::available()
{
    dpy = XOpenDisplay(nullptr);
    if (!dpy) {
        qDebug("Cannot open display '%s'", XDisplayName(nullptr));
        return false;
    }

    screen = GetNvXScreen(dpy);
    if (screen < 0) {
        qDebug("Unable to find any NVIDIA X screens; aborting.");
        return false;
    }

    return true;
}

void QuteFanNVCtrl::initialize()
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

        gpu.push_back(_gpu);
    }
}

int QuteFanNVCtrl::GetNvXScreen(Display *dpy)
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
