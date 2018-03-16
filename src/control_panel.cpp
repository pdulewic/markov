#include "inc/control_panel.h"
#include "inc/constants.h"
#include "inc/world.h"
#include <QSpinBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QDoubleSpinBox>

void ControlPanel::disableReward(int index){
    if(index < 2)
        reward->setEnabled(false);
    else
        reward->setEnabled(true);
}

ControlPanel::ControlPanel(QWidget* parent): QWidget(parent){
    height = new QSpinBox;
    height->setRange(0,MAX_MAP_Y);
    width = new QSpinBox;
    width->setRange(0,MAX_MAP_X);
    defaultReward = new QDoubleSpinBox;
    defaultReward->setRange(MIN_REWARD,MAX_REWARD);
    defaultReward->setDecimals(REWARD_PRECISION);
    defaultReward->setSingleStep(REWARD_STEP);
    discountingFactor = new QDoubleSpinBox;
    discountingFactor->setRange(MIN_DISCOUNTING,MAX_DISCOUNTING);
    discountingFactor->setDecimals(DISCOUNTING_PRECISION);
    discountingFactor->setSingleStep(DISCOUNTING_STEP);
    p1 = new QDoubleSpinBox;
    p1->setRange(MIN_PROBABILITY,MAX_PROBABILITY);
    p1->setDecimals(PROBABILITY_PRECISION);
    p1->setSingleStep(PROBABILITY_STEP);
    p2 = new QDoubleSpinBox;
    p2->setRange(MIN_PROBABILITY,MAX_PROBABILITY);
    p2->setDecimals(PROBABILITY_PRECISION);
    p2->setSingleStep(PROBABILITY_STEP);
    p3 = new QDoubleSpinBox;
    p3->setRange(MIN_PROBABILITY,MAX_PROBABILITY);
    p3->setDecimals(PROBABILITY_PRECISION);
    p3->setSingleStep(PROBABILITY_STEP);

    QFormLayout* generalLayout = new QFormLayout;
    generalLayout->addRow(tr("Height:"),height);
    generalLayout->addRow(tr("Width:"),width);
    generalLayout->addRow(tr("Default reward:"),defaultReward);
    generalLayout->addRow(tr("Discounting factor:"),discountingFactor);
    generalLayout->addRow(tr("p1:"),p1);
    generalLayout->addRow(tr("p2:"),p2);
    generalLayout->addRow(tr("p3:"),p3);

    QGroupBox* generalBox = new QGroupBox(tr("General"));
    generalBox->setLayout(generalLayout);

    type = new QComboBox;
    type->addItem(tr("Default"));
    type->addItem(tr("Forbidden"));
    type->addItem(tr("Special"));
    type->addItem(tr("Terminal"));
    connect(type,SIGNAL(currentIndexChanged(int)),this,SLOT(disableReward(int)));
    reward = new QDoubleSpinBox;
    reward->setRange(MIN_REWARD, MAX_REWARD);
    reward->setDecimals(REWARD_PRECISION);
    reward->setSingleStep(REWARD_STEP);
    reward->setEnabled(false);

    QFormLayout* tileLayout = new QFormLayout;
    tileLayout->addRow(tr("Type:"),type);
    tileLayout->addRow(tr("Reward:"),reward);
    tileBox = new QGroupBox(tr("Tile:"));
    tileBox->setLayout(tileLayout);
    tileBox->setVisible(false);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(generalBox);
    layout->addWidget(tileBox);
    layout->addStretch();
    setLayout(layout);
}

void ControlPanel::connectWithWorld(const World *world){
    connect(height,SIGNAL(valueChanged(int)),world,SLOT(setHeight(int)));
    connect(width,SIGNAL(valueChanged(int)),world,SLOT(setWidth(int)));
    connect(world,SIGNAL(newTarget(Tile::Type,double,QPoint)),this,SLOT(changeTileBox(Tile::Type,double,QPoint)));
    connect(world,SIGNAL(targetCleared()),this,SLOT(hideTileBox()));
    connect(type,SIGNAL(currentIndexChanged(int)),world,SLOT(setType(int)));
    connect(reward,SIGNAL(valueChanged(double)),world,SLOT(setReward(double)));
    connect(defaultReward,SIGNAL(valueChanged(double)),world,SLOT(setDefaultReward(double)));
    connect(discountingFactor,SIGNAL(valueChanged(double)),world,SLOT(setDiscountingFactor(double)));
    connect(p1,SIGNAL(valueChanged(double)),world,SLOT(setP1(double)));
    connect(p2,SIGNAL(valueChanged(double)),world,SLOT(setP2(double)));
    connect(p3,SIGNAL(valueChanged(double)),world,SLOT(setP3(double)));
    connect(world,SIGNAL(fileLoaded(int,int,double,double,double,double,double)),this,SLOT(changeGeneralValues(int,int,double,double,double,double,double)));
}

void ControlPanel::changeGeneralValues(int h, int w, double dr, double df, double _p1, double _p2, double _p3){
    if(h >= 0 && h <= MAX_MAP_Y)
        height->setValue(h);
    if(w >= 0 && w <= MAX_MAP_X)
        width->setValue(w);
    if(df >= MIN_DISCOUNTING && df <= MAX_DISCOUNTING)
        discountingFactor->setValue(df);
    if(dr >= MIN_REWARD && dr <= MAX_REWARD)
        defaultReward->setValue(dr);
    if(_p1 >= MIN_PROBABILITY && _p1 <= MAX_PROBABILITY)
        p1->setValue(_p1);
    if(_p2 >= MIN_PROBABILITY && _p2 <= MAX_PROBABILITY)
        p2->setValue(_p2);
    if(_p3 >= MIN_PROBABILITY && _p3 <= MAX_PROBABILITY)
        p3->setValue(_p3);
}

void ControlPanel::changeTileBox(Tile::Type t, double r, QPoint pos){
    if(!tileBox->isVisible())
        tileBox->setVisible(true);
    type->setCurrentIndex(t);
    reward->setValue(r);
    QString newTitle = tr("Tile (%1,%2):").arg(pos.x()).arg(pos.y());
    tileBox->setTitle(newTitle);
}

void ControlPanel::hideTileBox(){
    tileBox->setVisible(false);
}
