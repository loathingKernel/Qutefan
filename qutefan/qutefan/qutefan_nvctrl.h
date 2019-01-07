#ifndef QUTEFAN_NVCTRL_H
#define QUTEFAN_NVCTRL_H

#include "qnvctrl.h"

class QuteFanNVCtrl
{
public:
    QuteFanNVCtrl();
    ~QuteFanNVCtrl();

    bool available();
    void initialize();
};

#endif // QUTEFAN_NVCTRL_H
