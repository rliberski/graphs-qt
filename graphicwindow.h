#ifndef GRAPHICWINDOW_H
#define GRAPHICWINDOW_H

#include <QGraphicsView>
#include <math.h>
#include <iostream>
#include <QFile>
#include <time.h>
#include <QTextStream>
#include <QKeyEvent>
#include <QMetaEnum>

#include "mainwindow.h"
#include "graphmanager.h"
#include "node.h"
#include "edge.h"

class GraphManager;
class MainWindow;
enum Direction;

class GraphicWindow : public QGraphicsView{
    Q_OBJECT
public:
    GraphicWindow(QWidget *parent,MainWindow *wind, int a);
    GraphicWindow(QWidget *parent,MainWindow *wind, QString fileName = "");

    GraphManager* getMngr();
    MainWindow* getWndw();
    void unselectAll();
    void showEditNode(Node *n);
    void showEditEdge(Edge *e);
    void newNode(Node *n);
    void newEdge(Edge *e);
    void addNode(QString name,int weight);
    void addEdge(Node *source,Node *dest,int weight,Direction dir);

protected:
    void scaleView(qreal scaleFactor);
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

private:
    GraphManager* mngr;
    MainWindow *wndw;
};

#endif // GRAPHICWINDOW_H
