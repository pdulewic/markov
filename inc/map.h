#ifndef MAP
#define MAP

#include <QGraphicsView>

class QGraphicsRectItem;
class QGraphicsScene;
class World;

class Map: public QGraphicsView{
    Q_OBJECT

    QGraphicsScene* scene;
    World* world;
    QGraphicsRectItem* target;
    QGraphicsRectItem* hover;


    int squareSize;
    int xStart;
    int yStart;

private slots:
    void refreshView();
public:
    Map(QWidget* parent = 0);
    bool saveWorld(const QString filename) const;
    bool loadWorld(const QString filename);
    const World* getWorld() const {return world;}
public slots:
    void solve();

protected:
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent *event);



};

#endif // MAP

