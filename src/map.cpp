#include "inc/map.h"
#include "inc/constants.h"
#include "inc/world.h"
#include <QGraphicsScene>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMouseEvent>
#include <QGraphicsRectItem>

#include <QDebug>


void Map::refreshView(){
    scene->clear();
    hover = new QGraphicsRectItem(0,0,0,0);
    hover->setBrush(QBrush(QColor(0,0,255,HOVER_OPACITY)));
    scene->addItem(hover);

    if(world == nullptr)
        return;
    if(world->getWidth() == 0 || world->getHeight() == 0)
        squareSize = 0;
    else if(world->sizeRatio() > RESOLUTION_RATIO)
        squareSize = (RESOLUTION_X - MARGIN_SIZE)/world->getWidth();
    else
        squareSize = (RESOLUTION_Y - MARGIN_SIZE)/world->getHeight();
    xStart = (RESOLUTION_X - squareSize*world->getWidth())/2;
    yStart = (RESOLUTION_Y - squareSize*world->getHeight())/2;
    for(int i=0; i<world->getWidth(); ++i){
        for(int j=0; j<world->getHeight(); ++j){
            scene->addRect(xStart+i*squareSize, yStart+j*squareSize, squareSize, squareSize);
            int jIndex = world->getHeight()-1-j;
            QGraphicsSimpleTextItem* text = new QGraphicsSimpleTextItem;
            QFont font;
            font.setPixelSize(static_cast<int>(squareSize*0.8));
            text->setFont(font);
            switch (world->getType(i,jIndex)) {
            case Tile::DEFAULT:
                break;
            case Tile::FORBIDDEN:
                text->setText(tr("F"));
                break;
            case Tile::SPECIAL:
                text->setText(tr("B"));
                break;
            case Tile::TERMINAL:
                text->setText(tr("T"));
                break;
            default:
                text->setText(tr("SUM-TING-WONG"));
                break;
            }
            int margin = squareSize / 10;
            text->setPos(xStart+i*squareSize + 2.5*margin, yStart+j*squareSize + margin/2);
            scene->addItem(text);
        }
    }
    target = new QGraphicsRectItem(0,0,0,0);
    target->setBrush(QBrush(QColor(0,0,255,TARGET_OPACITY)));
    QPoint targetPos = world->getCurrentTile();
    if(targetPos != QPoint(-1,-1) && targetPos.x() < world->getWidth() && targetPos.y() < world->getHeight())
        target->setRect(xStart+targetPos.x()*squareSize, yStart+(world->getHeight()-1-targetPos.y())*squareSize,
                        squareSize,squareSize);
    scene->addItem(target);
}

Map::Map(QWidget *parent): QGraphicsView(parent), world(nullptr), squareSize(0){
    scene = new QGraphicsScene(0,0,RESOLUTION_X,RESOLUTION_Y);
    scene->setBackgroundBrush(QBrush(QColor(255,255,255)));
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(RESOLUTION_X,RESOLUTION_Y);

    world = new World();
    world->setHeight(5);
    world->setWidth(15);
    refreshView();

    connect(world,SIGNAL(stateChanged()),this,SLOT(refreshView()));
}

bool Map::saveWorld(const QString filename) const{
    QFile saveFile(filename);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonObject mainObject;
    world->write(mainObject);

    QJsonDocument saveDoc(mainObject);
    saveFile.write(saveDoc.toJson());
    saveFile.close();
    return true;
}

bool Map::loadWorld(const QString filename){
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
        return false;

    QByteArray saveData = file.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    world->read(loadDoc.object());

    file.close();
    return true;
}

void Map::solve(){
    world->solve();
}

void Map::mouseMoveEvent(QMouseEvent *event){
    if(squareSize == 0)
        return;
    QPoint mouse = event->pos();
    if(mouse.x() < xStart || mouse.y() < yStart ||
            mouse.x() > xStart + world->getWidth()*squareSize ||
            mouse.y() > yStart + world->getHeight()*squareSize)
        hover->setRect(0,0,0,0);
    else
        hover->setRect(xStart + ((mouse.x() - xStart) / squareSize) * squareSize,
                    yStart + ((mouse.y() - yStart) / squareSize) * squareSize,
                    squareSize,squareSize);
}

void Map::mousePressEvent(QMouseEvent *event){
    if(squareSize == 0)
        return;
    QPoint mouse = event->pos();
    if(mouse.x() < xStart || mouse.y() < yStart ||
            mouse.x() > xStart + world->getWidth()*squareSize ||
            mouse.y() > yStart + world->getHeight()*squareSize){
        target->setRect(0,0,0,0);
        world->clearTarget();
        return;
    }
    QPoint currentTile((mouse.x() - xStart) / squareSize, (mouse.y() - yStart) / squareSize);
    target->setRect(xStart+currentTile.x()*squareSize, yStart+currentTile.y()*squareSize,
                    squareSize,squareSize);
    currentTile.ry() = world->getHeight() - 1 - currentTile.y();
    world->setTarget(currentTile);
}
