#include <QDebug>

#include "qnvml.h"

ControlNVML::ControlNVML() {}

ControlNVML::~ControlNVML()
{
    nvmlShutdown();
    if(m_dpy)
        XCloseDisplay(m_dpy);
}

bool ControlNVML::available()
{
    m_dpy = XOpenDisplay(nullptr);
    if (!m_dpy) {
        qDebug("Cannot open display '%s'", XDisplayName(nullptr));
    }

    m_status = nvmlInit_v2();
    if(!m_status) {
        qDebug("Failed to initialize NVML; aborting");
        return false;
    }

    return true;
}

void ControlNVML::initialize()
{
}

