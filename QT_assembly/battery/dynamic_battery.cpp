#include "dynamic_battery.h"
#include "ui_dynamic_battery.h"

Dynamic_battery::Dynamic_battery(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Dynamic_battery)
{
    ui->setupUi(this);
    this->initForm();
}

Dynamic_battery::~Dynamic_battery()
{
    delete ui;
}

void Dynamic_battery::initForm()
{
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),ui->battery,SLOT(setValue(int)));
    ui->horizontalSlider->setValue(30);
}

