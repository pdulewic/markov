#include "inc/graph_dialog.h"
#include "inc/qcustomplot.h"
#include "inc/constants.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QVector>
#include <QDateTime>
#include <QString>
#include <QPushButton>
#include <random>

void GraphDialog::save(){
    long unixTime = QDateTime::currentMSecsSinceEpoch();
    QString filename = QString::number(unixTime);
    filename.append(".pdf");
    plot->savePdf(filename);
}

GraphDialog::GraphDialog(QVector<double> iterations, QVector<QVector<double> > history, int height, QWidget *parent): QDialog(parent){
    plot = new QCustomPlot;
    plot->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    plot->setInteraction(QCP::iRangeDrag, true);
    plot->setInteraction(QCP::iRangeZoom, true);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, NUMBER_OF_COLORS-1);

    std::random_device rd2;
    std::mt19937 gen2(rd2());
    std::uniform_int_distribution<> dis2(1, 3);

    for(int i=0; i<history.size(); ++i){
        plot->addGraph();
        plot->graph(i)->setData(iterations,history[i]);
        QPen pen(colors[dis(gen)]);
        pen.setStyle(static_cast<Qt::PenStyle>(dis2(gen2)));
        plot->graph(i)->setPen(pen);
        plot->graph(i)->setName(tr("(%1, %2)").arg(i/height).arg(i%height));
    }

    plot->xAxis->setLabel("Iteracje");
    plot->yAxis->setLabel("Uzytecznosci stanow");
    plot->xAxis->setRange(0, ITERATIONS+2);
    plot->yAxis->rescale();
    plot->legend->setVisible(true);
    plot->replot();

    QPushButton* saveButton = new QPushButton(tr("Save"));
    connect(saveButton,SIGNAL(clicked(bool)),this,SLOT(save()));


    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(plot);
    layout->addWidget(saveButton);
    setLayout(layout);
    resize(DIALOG_X, DIALOG_Y);
}
