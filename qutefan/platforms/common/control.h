#pragma once

#include <QVector>

class Control
{
public:
    Control() { }
    virtual ~Control() { }

    typedef struct _NvGPUBase {
        unsigned int cooler_count;
    } NvGPUBase;

    virtual bool available()  = 0;
    virtual void initialize() = 0;

    typedef struct _CoolerLevels {
        int count = 0;
        QVector<int> current;
    } CoolerLevels;

    typedef struct _CoolerLimits {
        int minimum;
        int maximum;
        int current;
    } CoolerLimits;

    typedef struct _Temperature {
        int none;
        int gpu;
        int memory;
        int power_supply;
        int board;
        int unknown;
    } Temperatures;

    typedef struct _Frequency {
        int core;
        int memory;
        int shader;
    } Frequencies;
};

