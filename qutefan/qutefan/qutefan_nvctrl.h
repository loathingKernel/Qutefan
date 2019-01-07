#ifndef QUTEFAN_NVCTRL_H
#define QUTEFAN_NVCTRL_H

#include "qnvctrl.h"


class QuteFanNVCtrl: public QuteFanAPI
{
public:
    QuteFanNVCtrl();
    bool isAvailable();
    void initialize();
};

#endif // QUTEFAN_NVCTRL_H
