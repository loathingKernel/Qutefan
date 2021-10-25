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
        qDebug("The NV-CONTROL X extension does not exist on '%s'.",
                XDisplayName(nullptr));
        return;
    }

    status = XNVCTRLQueryTargetCount(dpy, NV_CTRL_TARGET_TYPE_GPU, &gpu_count);
    if (!status) {
        qDebug("Failed to query number of gpus");
        return;
    }

    for(int i = 0; i < gpu_count; i++) {
        gpu[i].handle = i;
        gpu[i].status = XNVCTRLQueryTargetStringAttribute(
                    dpy, NV_CTRL_TARGET_TYPE_GPU, gpu[i].handle,
                    0, NV_CTRL_STRING_PRODUCT_NAME, &gpu[i].name);
        if (!gpu[i].status) {
            qDebug("Failed to query gpu product name");
            return;
        }
        gpu[i].status = XNVCTRLQueryTargetStringAttribute(
                    dpy, NV_CTRL_TARGET_TYPE_GPU, gpu[i].handle,
                    0, NV_CTRL_STRING_GPU_UUID, &gpu[i].uuid);
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
            qDebug("Default X screen %d is not an NVIDIA X screen.  "
                   "Using X screen %d instead.",
                   defaultScreen, screen);
            return screen;
        }
    }
    return -1;
}
