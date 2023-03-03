#pragma once

#include <QWidget>

namespace Ui {
class DualLabel;
}

class DoubleLabel : public QWidget
{
    Q_OBJECT

public:
    explicit DoubleLabel(QWidget *parent = nullptr);
    ~DoubleLabel();

    void setCurrent(const QString &);
    void setMaximum(const QString &);

private:
    Ui::DualLabel *ui;
};

