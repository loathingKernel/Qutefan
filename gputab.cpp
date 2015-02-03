#include "gputab.h"

GpuTab::GpuTab(QWidget *parent) :
    QWidget(parent)
{
    gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(6);
    gridLayout->setContentsMargins(11, 11, 11, 11);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));
    verticalSpacerTab = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout->addItem(verticalSpacerTab, 2, 0, 1, 2);

    horizontalLayoutLevel = new QHBoxLayout(this);
    horizontalLayoutLevel->setSpacing(6);
    horizontalLayoutLevel->setObjectName(QStringLiteral("horizontalLayoutLevel"));
    labelLevel = new QLabel(parent);
    labelLevel->setObjectName(QStringLiteral("labelLevel"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(labelLevel->sizePolicy().hasHeightForWidth());
    labelLevel->setSizePolicy(sizePolicy);

    horizontalLayoutLevel->addWidget(labelLevel);

    labelLevelValue = new QLabel(this);
    labelLevelValue->setObjectName(QStringLiteral("labelLevelValue"));
    QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(labelLevelValue->sizePolicy().hasHeightForWidth());
    labelLevelValue->setSizePolicy(sizePolicy1);
    labelLevelValue->setMinimumSize(QSize(32, 0));
    labelLevelValue->setFrameShape(QFrame::StyledPanel);
    labelLevelValue->setFrameShadow(QFrame::Sunken);

    horizontalLayoutLevel->addWidget(labelLevelValue);

    horizontalSpacerLevel = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayoutLevel->addItem(horizontalSpacerLevel);


    gridLayout->addLayout(horizontalLayoutLevel, 3, 1, 1, 1);

    horizontalLayoutTemp = new QHBoxLayout(this);
    horizontalLayoutTemp->setSpacing(6);
    horizontalLayoutTemp->setObjectName(QStringLiteral("horizontalLayoutTemp"));
    labelTemp = new QLabel(parent);
    labelTemp->setObjectName(QStringLiteral("labelTemp"));
    sizePolicy.setHeightForWidth(labelTemp->sizePolicy().hasHeightForWidth());
    labelTemp->setSizePolicy(sizePolicy);

    horizontalLayoutTemp->addWidget(labelTemp);

    labelTempValue = new QLabel(this);
    labelTempValue->setObjectName(QStringLiteral("labelTempValue"));
    sizePolicy1.setHeightForWidth(labelTempValue->sizePolicy().hasHeightForWidth());
    labelTempValue->setSizePolicy(sizePolicy1);
    labelTempValue->setMinimumSize(QSize(32, 0));
    labelTempValue->setFrameShape(QFrame::StyledPanel);
    labelTempValue->setFrameShadow(QFrame::Sunken);

    horizontalLayoutTemp->addWidget(labelTempValue);

    horizontalSpacerTemp = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayoutTemp->addItem(horizontalSpacerTemp);


    gridLayout->addLayout(horizontalLayoutTemp, 3, 0, 1, 1);

    gridLayoutMode = new QGridLayout(this);
    gridLayoutMode->setSpacing(6);
    gridLayoutMode->setObjectName(QStringLiteral("gridLayoutMode"));
    horizontalSpacerMode = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayoutMode->addItem(horizontalSpacerMode, 2, 4, 1, 1);

    radioButtonOff = new QRadioButton(this);
    radioButtonOff->setObjectName(QStringLiteral("radioButtonOff"));
    radioButtonOff->setChecked(true);

    gridLayoutMode->addWidget(radioButtonOff, 0, 0, 1, 2);

    radioButtonQuiet = new QRadioButton(this);
    radioButtonQuiet->setObjectName(QStringLiteral("radioButtonQuiet"));

    gridLayoutMode->addWidget(radioButtonQuiet, 1, 0, 1, 2);

    labelUpdateIntervalUnit = new QLabel(this);
    labelUpdateIntervalUnit->setObjectName(QStringLiteral("labelUpdateIntervalUnit"));

    gridLayoutMode->addWidget(labelUpdateIntervalUnit, 4, 3, 1, 1);

    spinBoxUpdateInterval = new QSpinBox(this);
    spinBoxUpdateInterval->setObjectName(QStringLiteral("spinBoxUpdateInterval"));
    spinBoxUpdateInterval->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    spinBoxUpdateInterval->setMinimum(1);
    spinBoxUpdateInterval->setMaximum(120);

    gridLayoutMode->addWidget(spinBoxUpdateInterval, 4, 2, 1, 1);

    labelUpdateInterval = new QLabel(this);
    labelUpdateInterval->setObjectName(QStringLiteral("labelUpdateInterval"));
    labelUpdateInterval->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    gridLayoutMode->addWidget(labelUpdateInterval, 4, 0, 1, 2);

    radioButtonLinear = new QRadioButton(this);
    radioButtonLinear->setObjectName(QStringLiteral("radioButtonLinear"));

    gridLayoutMode->addWidget(radioButtonLinear, 2, 0, 1, 1);

    radioButtonGraph = new QRadioButton(this);
    radioButtonGraph->setObjectName(QStringLiteral("radioButtonGraph"));

    gridLayoutMode->addWidget(radioButtonGraph, 3, 0, 1, 1);

    pushButtonGraphEdit = new QPushButton(this);
    pushButtonGraphEdit->setObjectName(QStringLiteral("pushButtonGraphEdit"));

    gridLayoutMode->addWidget(pushButtonGraphEdit, 3, 1, 1, 2);

    labelLinearOffset = new QLabel(this);
    labelLinearOffset->setObjectName(QStringLiteral("labelLinearOffset"));
    labelLinearOffset->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    gridLayoutMode->addWidget(labelLinearOffset, 2, 3, 1, 1);

    spinBoxLinearOffset = new QSpinBox(this);
    spinBoxLinearOffset->setObjectName(QStringLiteral("spinBoxLinearOffset"));
    spinBoxLinearOffset->setMinimum(-20);
    spinBoxLinearOffset->setMaximum(20);

    gridLayoutMode->addWidget(spinBoxLinearOffset, 2, 1, 1, 2);


    gridLayout->addLayout(gridLayoutMode, 1, 0, 1, 2);

    retranslateGpuTab();
}

void GpuTab::retranslateGpuTab()
{
    labelLevel->setText(QApplication::translate("QuteFan", "Fan level:", 0));
    labelLevelValue->setText(QApplication::translate("QuteFan", "0%", 0));
    labelTemp->setText(QApplication::translate("QuteFan", "Temperature:", 0));
    labelTempValue->setText(QApplication::translate("QuteFan", "0\302\260C", 0));
    radioButtonOff->setText(QApplication::translate("QuteFan", "Off", 0));
    radioButtonQuiet->setText(QApplication::translate("QuteFan", "Quiet", 0));
    labelUpdateIntervalUnit->setText(QApplication::translate("QuteFan", "sec", 0));
    labelUpdateInterval->setText(QApplication::translate("QuteFan", "Update interval:", 0));
    radioButtonLinear->setText(QApplication::translate("QuteFan", "Linear", 0));
    radioButtonGraph->setText(QApplication::translate("QuteFan", "Graph", 0));
    pushButtonGraphEdit->setText(QApplication::translate("QuteFan", "Edit", 0));
    labelLinearOffset->setText(QApplication::translate("QuteFan", "Offset", 0));
}
