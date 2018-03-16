#ifndef WORLD
#define WORLD

#include <QJsonObject>
#include <QObject>
#include <QPoint>
#include "tile.h"
#include "constants.h"

class GraphDialog;

class World: public QObject{
    Q_OBJECT

    Tile tileArray[MAX_MAP_X][MAX_MAP_Y];
    int width;
    int height;
    double p1, p2, p3;
    double defaultReward;
    double discountingFactor;

    GraphDialog* graph;
    QPoint currentTile;
    double neighbourValue(int x, int y);
    char policy(int x, int y);
public:
    World(QObject* parent = nullptr);
    double sizeRatio(){return static_cast<double>(width)/static_cast<double>(height);}
    int getWidth() const {return width;}
    int getHeight() const {return height;}
    QPoint getCurrentTile() const {return currentTile;}
    void clearTarget();
    void setTarget(QPoint tile);
    Tile::Type getType(int x, int y) const {return tileArray[x][y].type;}

    void write(QJsonObject &json) const;
    void read(const QJsonObject &json);
public slots:
    void setWidth(int w);
    void setHeight(int h);
    void setDefaultReward(double dr){defaultReward = dr;}
    void setDiscountingFactor(double df);
    void setP1(double _p1);
    void setP2(double _p2);
    void setP3(double _p3);

    void setType(int type);
    void setReward(double reward);
    void solve();

signals:
    void stateChanged();
    void fileLoaded(int h, int w, double dr, double df, double p1, double p2, double p3);
    void targetCleared();
    void newTarget(Tile::Type type, double reward, QPoint pos);
};

#endif // WORLD

