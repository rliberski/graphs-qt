#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

class Node;
class GraphicWindow;

enum Direction{
    SOURCE_TO_DEST = 0,
    DEST_TO_SOURCE = 1,
    TWO_WAY = 2
};

class Edge : public QGraphicsItem{
public:
    Edge(Node *sourceNode,                              //Konstruktor -
         Node *destNode,                                //arg1 i arg 2 - wskaźniki do wierzchołków
         int w,                                         //arg3 - waga, arg4 - kierunek
         Direction d,
         GraphicWindow *g);
    ~Edge();
    void removeThis();
    Node *sourceNode() const;                           //Zwraca wierzchołek źródłowy
    Node *destNode() const;                             //Zwraca wierzchołek docelowy
    void adjust();                                      //Aktualizuje linię (?)

    //getery
    QString getSource();
    QString getDest();
    int getWeight();
    Direction getDirection();

    //setery
    void setSource(Node *s);
    void setDest(Node *d);
    void setWeight(int w);
    void setDirection(Direction d);

    void select();
    void unselect();

    void paint(                                         //Rysuje krawędź
            QPainter *painter,
            const QStyleOptionGraphicsItem *option,
            QWidget *widget) Q_DECL_OVERRIDE;

protected:
    QRectF boundingRect() const Q_DECL_OVERRIDE;        //Zwraca obszar kolizji

private:
    Node *source, *dest;                                //Wskaźniki do swoich wierzchołków
    QString sourceName;
    QString destName;
    QPointF sourcePoint;                                //Współrzędna wierzchołka 1
    QPointF destPoint;                                  //Współrzędna wierzchołka 2
    int waga;                                           //Waga krawędzi
    Direction direction;                                //Kierunek
    bool selected=false;
    GraphicWindow *graphic;
};

#endif // EDGE_H
