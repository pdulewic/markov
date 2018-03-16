#include "inc/world.h"
#include "inc/tile.h"
#include "inc/graph_dialog.h"
#include <QJsonArray>
#include <QVector>
#include <iostream>

#include <QDebug>

double World::neighbourValue(int x, int y){
    double up, down, left, right;
    if(y+1 == height || tileArray[x][y+1].type == Tile::FORBIDDEN)
        up = tileArray[x][y].utility;
    else
        up = tileArray[x][y+1].utility;

    if(y-1 < 0 || tileArray[x][y-1].type == Tile::FORBIDDEN)
        down = tileArray[x][y].utility;
    else
        down = tileArray[x][y-1].utility;

    if(x+1 == width || tileArray[x+1][y].type == Tile::FORBIDDEN)
        right = tileArray[x][y].utility;
    else
        right = tileArray[x+1][y].utility;

    if(x-1 < 0 || tileArray[x-1][y].type == Tile::FORBIDDEN)
        left = tileArray[x][y].utility;
    else
        left = tileArray[x-1][y].utility;

    double a[4];
    a[0] = p2*left + p1*up + p3*right  + (1.0 - p1 - p2 - p3) * down;
    a[1] = p2*up + p1*right + p3*down  + (1.0 - p1 - p2 - p3) * left;
    a[2] = p2*right + p1*down + p3*left  + (1.0 - p1 - p2 - p3) * up;
    a[3] = p2*down + p1*left + p3*up  + (1.0 - p1 - p2 - p3) * right;
    double max = a[0] > a[1] ? a[0] : a[1];
    max = max > a[2] ? max : a[2];
    max = max > a[3] ? max : a[3];
    return max;
}

char World::policy(int x, int y){
    qDebug() << "elo, type jest " << static_cast<int>(tileArray[x][y].type);
    if(tileArray[x][y].type == Tile::TERMINAL){
        qDebug() << "TERMINAL!";
        return 'x';
    }
    if(tileArray[x][y].type == Tile::FORBIDDEN){
        qDebug() << "FORBIDDEN!";
        return 'x';
    }

    double up, down, left, right;
    if(y+1 == height || tileArray[x][y+1].type == Tile::FORBIDDEN)
        up = tileArray[x][y].utility;
    else
        up = tileArray[x][y+1].utility;

    if(y-1 < 0 || tileArray[x][y-1].type == Tile::FORBIDDEN)
        down = tileArray[x][y].utility;
    else
        down = tileArray[x][y-1].utility;

    if(x+1 == width || tileArray[x+1][y].type == Tile::FORBIDDEN)
        right = tileArray[x][y].utility;
    else
        right = tileArray[x+1][y].utility;

    if(x-1 < 0 || tileArray[x-1][y].type == Tile::FORBIDDEN)
        left = tileArray[x][y].utility;
    else
        left = tileArray[x-1][y].utility;

    double a[4];
    a[0] = p2*left + p1*up + p3*right  + (1.0 - p1 - p2 - p3) * down;
    a[1] = p2*up + p1*right + p3*down  + (1.0 - p1 - p2 - p3) * left;
    a[2] = p2*right + p1*down + p3*left  + (1.0 - p1 - p2 - p3) * up;
    a[3] = p2*down + p1*left + p3*up  + (1.0 - p1 - p2 - p3) * right;

    double max = -1000000.0;
    int index;
    for(int i=0; i<4; ++i){
        if(a[i]>max){
            max = a[i];
            index = i;
        }
    }
    char arrows[4] = {'^', '>', 'v', '<'};
    return arrows[index];
}

World::World(QObject *parent): QObject(parent), width(0), height(0), currentTile(-1,-1) {
    for(int i=0; i<MAX_MAP_X; ++i)
        for(int j=0; j<MAX_MAP_Y; ++j)
            tileArray[i][j] = Tile();
}

void World::clearTarget(){
    currentTile = QPoint(-1,-1);
    emit targetCleared();
}

void World::setTarget(QPoint tile){
    if(tile == QPoint(-1,-1))
        qDebug() << "Invalid tile index in World::setTarget!";
    currentTile = tile;
    Tile tmp = tileArray[tile.x()][tile.y()];
    emit newTarget(tmp.type, tmp.reward, tile);
}

void World::setWidth(int w){
    if(w >= 0 && w <= MAX_MAP_X){
        width = w;
        emit stateChanged();
    }
}

void World::setHeight(int h){
    if(h >= 0 && h <= MAX_MAP_Y){
        height = h;
        emit stateChanged();
    }
}

void World::setDiscountingFactor(double df){
    if(df >= 0.0 && df <= 1.0)
        discountingFactor = df;
}

void World::setP1(double _p1){
    if(_p1 >= 0.0 && _p1 <= 1.0)
        p1 = _p1;
}

void World::setP2(double _p2){
    if(_p2 >= 0.0 && _p2 <= 1.0)
p2 = _p2;
}

void World::setP3(double _p3){
    if(_p3 >= 0.0 && _p3 <= 1.0)
        p3 = _p3;
}


void World::setType(int type){
    if(currentTile == QPoint(-1,-1))
        return;
    if(tileArray[currentTile.x()][currentTile.y()].type != static_cast<Tile::Type>(type)){
        tileArray[currentTile.x()][currentTile.y()].type = static_cast<Tile::Type>(type);
        emit stateChanged();
    }
}

void World::setReward(double reward){
    if(currentTile == QPoint(-1,-1))
        return;
    tileArray[currentTile.x()][currentTile.y()].reward = reward;
}

void World::solve(){
    QVector<double> iterations;
    iterations.push_back(0);
    QVector<QVector<double>> history;
    //std::vector<Tile[MAX_MAP_X][MAX_MAP_Y]> history;
    for(int i=0; i<width; ++i){       
        for(int j=0; j<height; ++j){
            history.push_back(QVector<double>());
            if(tileArray[i][j].type == Tile::TERMINAL)
                tileArray[i][j].utility = tileArray[i][j].reward;
            else
                tileArray[i][j].utility = 0.0;
            history[i*height + j].push_back(tileArray[i][j].utility);
        }
    }



    for(int t=0; t<ITERATIONS; ++t){
        iterations.push_back(t+1);
        Tile tmp[MAX_MAP_X][MAX_MAP_Y];
        for(int i=0; i<width; ++i){
            for(int j=0; j<height; ++j){
                tmp[i][j] = tileArray[i][j];
                if(tileArray[i][j].type == Tile::FORBIDDEN || tileArray[i][j].type == Tile::TERMINAL)
                    continue;
                double reward = defaultReward;
                if(tileArray[i][j].type == Tile::SPECIAL)
                    reward = tileArray[i][j].reward;
                tmp[i][j].utility = reward + discountingFactor * neighbourValue(i,j);
            }
        }
        for(int i=0; i<width; ++i){
            for(int j=0; j<height; ++j){
                tileArray[i][j] = tmp[i][j];
                history[i*height + j].push_back(tmp[i][j].utility);
            }
        }
    }
    for(int i=height-1; i>=0; --i){
        for(int j=0; j<width; ++j)
            std::cout << tileArray[j][i].utility << "\t";
        std::cout << std::endl;
    }
    std::cout << "-------------------" <<std::endl;

    for(int i=height-1; i>=0; --i){
        for(int j=0; j<width; ++j){
            int x = j;
            int y = i;
            if(tileArray[x][y].type == Tile::FORBIDDEN ||  static_cast<int>(tileArray[j][i].type) == 3){
                std::cout << "x\t";
                continue;
            }
            double up, down, left, right;
            if(y+1 == height || tileArray[x][y+1].type == Tile::FORBIDDEN)
                up = tileArray[x][y].utility;
            else
                up = tileArray[x][y+1].utility;

            if(y-1 < 0 || tileArray[x][y-1].type == Tile::FORBIDDEN)
                down = tileArray[x][y].utility;
            else
                down = tileArray[x][y-1].utility;

            if(x+1 == width || tileArray[x+1][y].type == Tile::FORBIDDEN)
                right = tileArray[x][y].utility;
            else
                right = tileArray[x+1][y].utility;

            if(x-1 < 0 || tileArray[x-1][y].type == Tile::FORBIDDEN)
                left = tileArray[x][y].utility;
            else
                left = tileArray[x-1][y].utility;

            double a[4];
            a[0] = p2*left + p1*up + p3*right  + (1.0 - p1 - p2 - p3) * down;
            a[1] = p2*up + p1*right + p3*down  + (1.0 - p1 - p2 - p3) * left;
            a[2] = p2*right + p1*down + p3*left  + (1.0 - p1 - p2 - p3) * up;
            a[3] = p2*down + p1*left + p3*up  + (1.0 - p1 - p2 - p3) * right;
            qDebug() << tr("For state(%1, %2): up = %3, right = %4, down = %5, left = %6").arg(x).arg(y).arg(a[0]).arg(a[1]).arg(a[2]).arg(a[3]);
            double max = -1000000.0;
            int index;
            for(int i=0; i<4; ++i){
                if(a[i]>max){
                    max = a[i];
                    index = i;
                }
            }
            char arrows[4] = {'^', '>', 'v', '<'};
            std::cout << arrows[index] << "\t";
        }
        std::cout << std::endl;
    }

    graph = new GraphDialog(iterations,history,height);
    graph->show();
}

void World::write(QJsonObject &json) const{
    json["width"] = width;
    json["height"] = height;
    json["defaultReward"] = defaultReward;
    json["discountingFactor"] = discountingFactor;
    json["p1"] = p1;
    json["p2"] = p2;
    json["p3"] = p3;
    QJsonArray forbidden;
    QJsonArray special;
    QJsonArray terminal;
    QJsonArray position;
    QJsonObject specialTile;
    QJsonObject terminalTile;
    for(int i=0; i<width; ++i){
        for(int j=0; j<height; ++j){
            switch(tileArray[i][j].type){
            case Tile::DEFAULT:
                break;
            case Tile::FORBIDDEN:
                position = {i,j};
                forbidden.append(position);
                break;
            case Tile::SPECIAL:
                specialTile["reward"] = tileArray[i][j].reward;
                position = {i,j};
                specialTile["position"] = position;
                special.append(specialTile);
                break;
            case Tile::TERMINAL:
                terminalTile["reward"] = tileArray[i][j].reward;
                position = {i,j};
                terminalTile["position"] = position;
                terminal.append(terminalTile);
                break;
            }
        }
    }
    json["forbidden"] = forbidden;
    json["special"] = special;
    json["terminal"] = terminal;
}

void World::read(const QJsonObject &json){
    for(int i=0; i<width; ++i){
        for(int j=0; j<height; ++j){
            tileArray[i][j].type = Tile::DEFAULT;
            tileArray[i][j].reward = 0.0;
        }
    }
    if(json.contains("height") && json["height"].isDouble())
        height = json["height"].toDouble();
    if(json.contains("width") && json["width"].isDouble())
        width = json["width"].toDouble();
    if(json.contains("defaultReward") && json["defaultReward"].isDouble())
        defaultReward = json["defaultReward"].toDouble();
    if(json.contains("discountingFactor") && json["discountingFactor"].isDouble())
        discountingFactor = json["discountingFactor"].toDouble();
    if(json.contains("p1") && json["p1"].isDouble())
        p1 = json["p1"].toDouble();
    if(json.contains("p2") && json["p2"].isDouble())
        p2 = json["p2"].toDouble();
    if(json.contains("p3") && json["p3"].isDouble())
        p3 = json["p3"].toDouble();
    if(json.contains("forbidden") && json["forbidden"].isArray()){
        QJsonArray forbidden = json["forbidden"].toArray();
        for(int i=0; i< forbidden.size(); ++i){
            QJsonArray pos = forbidden[i].toArray();
            int x = pos[0].toDouble();
            int y = pos[1].toDouble();
            tileArray[x][y].type = Tile::FORBIDDEN;
        }
    }
    if(json.contains("special") && json["special"].isArray()){
        QJsonArray special = json["special"].toArray();
        for(int i=0; i<special.size(); ++i){
            QJsonObject specialTile = special[i].toObject();
            QJsonArray pos = specialTile["position"].toArray();
            int x = pos[0].toDouble();
            int y = pos[1].toDouble();
            tileArray[x][y].type = Tile::SPECIAL;
            tileArray[x][y].reward = specialTile["reward"].toDouble();
        }
    }
    if(json.contains("terminal") && json["terminal"].isArray()){
        QJsonArray terminal = json["terminal"].toArray();
        for(int i=0; i<terminal.size(); ++i){
            QJsonObject terminalTile = terminal[i].toObject();
            QJsonArray pos = terminalTile["position"].toArray();
            int x = pos[0].toDouble();
            int y = pos[1].toDouble();
            tileArray[x][y].type = Tile::TERMINAL;
            tileArray[x][y].reward = terminalTile["reward"].toDouble();
        }
    }
    emit fileLoaded(height, width, defaultReward, discountingFactor, p1, p2, p3);
    emit stateChanged();
}
