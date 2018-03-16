#include "inc/mainwindow.h"
#include "inc/map.h"
#include "inc/control_panel.h"
#include <QLabel>
#include <QMenuBar>
#include <QFileDialog>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent){
    worldMap = new Map;
    worldMap->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    worldMap->setMouseTracking(true);

    panel = new ControlPanel;
    panel->connectWithWorld(worldMap->getWorld());

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(worldMap);
    layout->addWidget(panel);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
    createMenus();
}

MainWindow::~MainWindow()
{

}

void MainWindow::createMenus(){
    QMenu* fileMenu = menuBar()->addMenu(tr("File"));

    QAction* saveAct = new QAction(tr("Save"),this);
    connect(saveAct,SIGNAL(triggered(bool)),this,SLOT(save()));
    fileMenu->addAction(saveAct);

    QAction* loadAct = new QAction(tr("Load"),this);
    connect(loadAct,SIGNAL(triggered(bool)),this,SLOT(load()));
    fileMenu->addAction(loadAct);

    QMenu* toolsMenu = menuBar()->addMenu(tr("Tools"));

    QAction* solveAct = new QAction(tr("Solve MDP"),this);
    connect(solveAct,SIGNAL(triggered(bool)),worldMap,SLOT(solve()));
    toolsMenu->addAction(solveAct);
}

void MainWindow::save(){
    QString filename = QFileDialog::getSaveFileName(this,tr("Save as"),".",tr("JSON files(.json)"));
    if(filename.isEmpty())
        return;
    if(!filename.endsWith(".json"))
        filename.append(".json");
    worldMap->saveWorld(filename);
}

void MainWindow::load(){
    QString filename = QFileDialog::getOpenFileName(this,tr("Open file"),".",tr("JSON Files (*.json)"));
    if(filename.isEmpty())
        return;
    worldMap->loadWorld(filename);
}
