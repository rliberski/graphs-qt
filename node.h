#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>

class Edge;                                                     //Używane do wszystkiego co ma coś wspólnego z krawędziami
class GraphicWindow;                                            //Używane do wyświetlenia

class Node : public QGraphicsItem{
public:
    Node(GraphicWindow *graphWidget,QString n,int w,QColor c);  //Konstruktor - arg1 - pole, arg2 - nazwa, arg3 - waga, arg4 - kolor
    ~Node();
    void removeThis();
    void addEdge(Edge *edge);                                   //Dodaje nowe połączenie do danego wierzchołka
    void removeEdge(Edge *edge);
    void calculateForces();                                     //??
    bool advance();                                             //??
    QRectF boundingRect() const Q_DECL_OVERRIDE;                //Zwraca obszar kolizji danego wierzchołka
    QPainterPath shape() const Q_DECL_OVERRIDE;                 //??
    void paint(                                                 //Rysuje wierzchołek
            QPainter *painter,
            const QStyleOptionGraphicsItem *option,
            QWidget *widget) Q_DECL_OVERRIDE;

    //getery
    QList<Edge *> edges() const;                                //Zwraca listę połączeń danego wierzchołka
    QString getName();                                          //Zwraca nazwę wierzchołka
    int getWeight();                                            //Zwraca wagę
    QColor getColor();                                          //Zwraca kolor

    //setery
    void setName(QString n);
    void setWeight(int w);
    void setColor(QColor c); //ustawia kolor

    void selectThis();
    void unselect();

protected:
    QVariant itemChange(                                        //Aktualizuje linie jeżeli pozycja wierzchołka się zmieniła
            GraphicsItemChange change,
            const QVariant &value) Q_DECL_OVERRIDE;

private:
    QPointF newPos;                                             //??
    GraphicWindow *graphic;                                     //Pole do rysowania w którym będą rysowane wierzchołki

    QList<Edge *> edgeList;                                     //Lista połączeń danego wierzchołka
    QString name;                                               //Nazwa wierzchołka
    int weight;                                                 //Waga wierzchołka
    QColor color;                                               //Kolor wierzchołka
    bool selected=false;
};

#endif // NODE_H
