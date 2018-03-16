#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Map;
class ControlPanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Map* worldMap;
    ControlPanel* panel;
    void createMenus();
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void save();
    void load();
};

#endif // MAINWINDOW_H
