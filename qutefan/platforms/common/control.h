#pragma once

#include <QVector>

class Control
{
public:
    Control() { }
    virtual ~Control() { }

    typedef struct _NvGPUBase {
        unsigned int cooler_count = 0;
    } NvGPUBase;

    virtual bool available()  = 0;
    virtual void initialize() = 0;

    typedef struct _CoolerLevels {
        unsigned int count = 0;
        QVector<unsigned int> current;
    } CoolerLevels;

    typedef struct _CoolerLimits {
        unsigned int minimum;
        unsigned int maximum;
        unsigned int current;
    } CoolerLimits;

    typedef struct _Temperature {
        unsigned int none;
        unsigned int gpu;
        unsigned int memory;
        unsigned int power_supply;
        unsigned int board;
        unsigned int unknown;
    } Temperatures;

    typedef struct _Frequency {
        unsigned int core;
        unsigned int memory;
        unsigned int shader;
        unsigned int video;
        unsigned int power;
    } Frequencies;

    enum LimitType {
        CONTROL_LIMIT_FREQUENCY = 1,
        CONTROL_LIMIT_WATTAGE = 2,
        CONTROL_LIMIT_RPM = 3,
    };

    typedef struct _Limits {
        unsigned int maximum;
        unsigned int minimum;
        unsigned int current;
        int denominator;
        LimitType type;
    } Limits;
};

