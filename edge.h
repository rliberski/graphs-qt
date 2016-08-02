#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>
#include <QPainter>

#include "node.h"
#include "graphicwindow.h"
#include "graphmanager.h"
#include "mainwindow.h"

class Node;
class GraphicWindow;

enum Direction{
    SOURCE_TO_DEST = 0,
    DEST_TO_SOURCE = 1,
    TWO_WAY = 2
};

class Edge : public QGraphicsItem
{
public:
    Edge(Node *getSourceNode,
         Node *getDestNode,
         int w,
         Direction d,
         GraphicWindow *graphicWindow);
    ~Edge();

    void removeThis();           
    void select();
    void unselect();
    void adjust();

    /*getters & setters*/
    Node *getSourceNode() const;
    Node *getDestNode() const;
    QString getSource();
    QString getDest();
    int getWeight();
    Direction getDirection();
    void setSource(Node *s);
    void setDest(Node *d);
    void setWeight(int w);
    void setDirection(Direction d);

protected:
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(
            QPainter *painter,
            const QStyleOptionGraphicsItem *option,
            QWidget *widget) Q_DECL_OVERRIDE;

private:
    Node *source;
    Node *dest;
    QString sourceName;
    QString destName;
    QPointF sourcePoint;
    QPointF destPoint;
    int weight;
    Direction direction;
    bool selected=false;
    GraphicWindow *graphic;
};

#endif // EDGE_H
