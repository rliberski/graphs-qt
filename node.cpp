#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include "node.h"
#include "edge.h"
#include "graphicwindow.h"
#include "graphmanager.h"
#include "mainwindow.h"

//Konstruktor - arg1 - pole, arg2 - nazwa, arg3 - waga, arg4 - kolor
Node::Node(GraphicWindow *GraphicWindow,QString n, int w, QColor c)
    : graphic(GraphicWindow),name(n),weight(w),color(c)
{

    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);

    graphic->newNode(this);

}

Node::~Node(){

}

void Node::removeThis(){
    graphic->scene()->removeItem(this);
    graphic->getMngr()->removeNode(this);
    foreach(Edge *e,edgeList){
        graphic->getWndw()->removeEdge(e);
        graphic->getMngr()->removeEdge(e);
        graphic->scene()->removeItem(e);
        removeEdge(e);
    }
}

//Dodaje nowe połączenie do danego wierzchołka
void Node::addEdge(Edge *edge)
{
    //Dodaje połączenie do listy połączeń
    edgeList << edge;
    //Ustala położenie linii  (?)
    edge->adjust();
}

void Node::removeEdge(Edge *edge)
{
    edgeList.removeOne(edge);
    edge->adjust();
}

////// bez tego niby działa ale nie wiem czy można to wyjebać
void Node::calculateForces()
{
    if (!scene() || scene()->mouseGrabberItem() == this) {
        newPos = pos();
        return;
    }

    // Sum up all forces pushing this item away
    qreal xvel = 0;
    qreal yvel = 0;
    foreach (QGraphicsItem *item, scene()->items()) {
        Node *node = qgraphicsitem_cast<Node *>(item);
        if (!node)
            continue;

        QPointF vec = mapToItem(node, 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();
        double l = 2.0 * (dx * dx + dy * dy);
        if (l > 0) {
            xvel += (dx * 150.0) / l;
            yvel += (dy * 150.0) / l;
        }
    }

    // Now subtract all forces pulling items together
    double weight = (edgeList.size() + 1) * 10;
    foreach (Edge *edge, edgeList) {
        QPointF vec;
        if (edge->sourceNode() == this)
            vec = mapToItem(edge->destNode(), 0, 0);
        else
            vec = mapToItem(edge->sourceNode(), 0, 0);
        xvel -= vec.x() / weight;
        yvel -= vec.y() / weight;
    }

    if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
        xvel = yvel = 0;

    QRectF sceneRect = scene()->sceneRect();
    newPos = pos() + QPointF(xvel, yvel);
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));

}

////// nie mam pojęcia
bool Node::advance()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}

//Zwraca obszar kolizji danego wierzchołka
QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

////// nie mam pojęcia ale bez tego mi nie działa
QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

//Rysuje wierzchołek
void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    //Cień
    QRadialGradient gradient(-3, -3, 10);
    gradient.setColorAt(0,QColor(200,200,200,200));
    gradient.setColorAt(0,QColor(200,200,200,0));
    painter->setPen(Qt::NoPen);
    painter->setBrush(gradient);
    painter->drawEllipse(-7, -7, 20, 20);

    //Koło
    /*
    if (option->state & QStyle::State_Sunken) {
        //Jeżeli wciśnięty
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, color.light(130));
        gradient.setColorAt(0, QColor(color.red()-30,color.green()-30,color.blue()-30).light(120));
    } else {*/
        //Jeżeli nie wciśnięty
        gradient.setColorAt(0, color);
        gradient.setColorAt(1, QColor(color.red()-30,color.green()-30,color.blue()-30));
    //}
    if(selected){
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, color.light(130));
        gradient.setColorAt(0, QColor(color.red()-30,color.green()-30,color.blue()-30).light(120));
    }
    painter->setBrush(gradient);

    //Obrys koła
    painter->setPen(QPen(QColor(200,200,200,255), 0));
    painter->drawEllipse(-10, -10, 20, 20);

    //Nazwa wierzchołka
    QRectF textRect(-10, -10, 20, 20);
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(6);
    painter->setFont(font);
    painter->setPen(Qt::white);
    painter->drawText(textRect,Qt::AlignCenter, name+":"+QString::number(weight));

    update();

}

//Aktualizuje linie jeżeli pozycja wierzchołka się zmieniła
QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

/////////////getery
//Zwraca listę połączeń danego wierzchołka
QList<Edge *> Node::edges() const
{
    return edgeList;
}

//Zwraca nazwę wierzchołka
QString Node::getName()
{
    return name;
}

//Zwraca wagę
int Node::getWeight(){
    return weight;
}

//Zwraca kolor
QColor Node::getColor(){
    return color;
}

void Node::setName(QString n){
    name=n;
    this->update();
}

void Node::setWeight(int w){
    weight=w;
    this->update();
}

void Node::setColor(QColor c){
    color=c;
    this->update();
}


void Node::selectThis(){
    graphic->unselectAll();
    graphic->showEditNode(this);
    selected=true;
    update();
}

void Node::unselect(){
    selected=false;
    update();
}

