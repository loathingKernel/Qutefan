#pragma once

#include <QtGlobal>
#include <QVector>

#include <X11/Xlib.h>

#include "control.h"
#include "include/nvml.h"

class ControlNVML : public Control
{
public:
    Display *m_dpy;
    int m_screen;

    bool m_status;

    ControlNVML();
    ~ControlNVML();

    bool available();
    void initialize();
};
