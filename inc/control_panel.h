#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include "tile.h"
#include <QWidget>

class QSpinBox;
class World;
class QGroupBox;
class QComboBox;
class QDoubleSpinBox;

class ControlPanel: public QWidget{
    Q_OBJECT

    QSpinBox* height;
    QSpinBox* width;
    QDoubleSpinBox* defaultReward;
    QDoubleSpinBox* discountingFactor;
    QDoubleSpinBox* p1;
    QDoubleSpinBox* p2;
    QDoubleSpinBox* p3;

    QComboBox* type;
    QDoubleSpinBox* reward;

    QGroupBox* tileBox;
private slots:
    void disableReward(int index);
public:
    ControlPanel(QWidget* parent = nullptr);
    void connectWithWorld(const World* world);

public slots:
    void changeGeneralValues(int h, int w, double dr, double df, double _p1, double _p2, double _p3);
    void changeTileBox(Tile::Type t, double r, QPoint pos);
    void hideTileBox();
};

#endif // CONTROL_PANEL_H
