#ifndef GPUTAB_H
#define GPUTAB_H

#include <QApplication>
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>
#include <QSpinBox>

class GpuTab : public QWidget
{
    Q_OBJECT
public:
    explicit GpuTab(QWidget *parent = 0);

    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacerTab;

    QHBoxLayout *horizontalLayoutLevel;
    QLabel *labelLevel;
    QLabel *labelLevelValue;
    QSpacerItem *horizontalSpacerLevel;

    QHBoxLayout *horizontalLayoutTemp;
    QLabel *labelTemp;
    QLabel *labelTempValue;
    QSpacerItem *horizontalSpacerTemp;

    QGridLayout *gridLayoutMode;
    QSpacerItem *horizontalSpacerMode;

    QRadioButton *radioButtonOff;
    QRadioButton *radioButtonQuiet;

    QRadioButton *radioButtonLinear;
    QLabel *labelLinearOffset;
    QSpinBox *spinBoxLinearOffset;

    QRadioButton *radioButtonGraph;
    QPushButton *pushButtonGraphEdit;

    QLabel *labelUpdateIntervalUnit;
    QSpinBox *spinBoxUpdateInterval;
    QLabel *labelUpdateInterval;


    void retranslateGpuTab();

signals:

public slots:

};

#endif // GPUTAB_H
