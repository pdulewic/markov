#ifndef GRAPH_DIALOG_H
#define GRAPH_DIALOG_H

#include <QDialog>
#include <QString>
#include <QVector>

class QLabel;
class QCustomPlot;

class GraphDialog: public QDialog{
    Q_OBJECT

    QCustomPlot* plot;
private slots:
    void save();
public:
    GraphDialog(QVector<double> iterations, QVector<QVector<double>> history, int height, QWidget* parent = nullptr);
};

#endif // GRAPH_DIALOG_H
