#include "edge.h"

/*set new edge with given parameters*/
Edge::Edge(Node *sourceNode, Node *destNode, int w, Direction d, GraphicWindow *graphicWindow)
    :source(sourceNode), dest(destNode), weight(w), direction(d), graphic(graphicWindow)
{
    setAcceptedMouseButtons(0);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);

    sourceName = sourceNode -> getName();
    destName = destNode -> getName();

    source -> addEdge(this);
    dest -> addEdge(this);

    adjust();

    graphic -> newEdge(this);
}

Edge::~Edge()
{
    //if(source) delete source;
    //if(dest) delete dest;
}

/*remove edge from view, from edges list and from graph manager*/
void Edge::removeThis()
{
    source -> removeEdge(this);
    dest -> removeEdge(this);

    graphic -> scene() -> removeItem(this);
    graphic -> getMngr() -> removeItem(this);
    graphic -> getWndw() -> removeEdge(this);
}

/*adjust edge*/
void Edge::adjust()
{
    if (!source || !dest)
        return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();
    prepareGeometryChange();

    if (length > qreal(20.))
    {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
    }
    else
    {
        sourcePoint = destPoint = line.p1();
    }
}

/*return bounding rectangle*/
QRectF Edge::boundingRect() const
{
    if (!source || !dest)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + 10) / 2.0; // penWidth + arrowSize

    //A->B
    if(source != dest)
        return QRectF(sourcePoint,
                QSizeF(destPoint.x() - sourcePoint.x(),destPoint.y() - sourcePoint.y()))
                .normalized()
                .adjusted(-extra, -extra, extra, extra);
    //A->A
    else
        return QRectF(sourcePoint,
                QSizeF(70,70))
                .normalized()
                .adjusted(-extra, -extra, extra, extra);

    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

/*select edge*/
void Edge::select()
{
    graphic->unselectAll();
    graphic->showEditEdge(this);
    selected=true;
    update();
}

/*unselect edge*/
void Edge::unselect()
{
    selected=false;
    update();
}

/*paint edge*/
void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);

    if(selected)
        painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    else
        painter->setPen(QPen(QColor(200,200,200,255), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    //A->B
    if (source!=dest)
    {
        if (qFuzzyCompare(line.length(), qreal(0.)))
            return;

        painter->drawLine(line);

        QPointF center;
        center.setX((line.x1()+line.x2())/2);
        center.setY((line.y1()+line.y2())/2);
        QRadialGradient gradient(-3, -3, 10);
        gradient.setColorAt(0, Qt::darkGray);
        gradient.setColorAt(1, Qt::black);
        painter->setBrush(gradient);
        painter->drawEllipse(center.x() -5,center.y() -5, 10, 10);

        QRectF textRect(center.x()-5, center.y()-5, 10, 10);
        QString message = QString::number(weight);
        QFont font = painter->font();
        font.setBold(true);
        font.setPointSize(5);
        painter->setFont(font);
        painter->drawText(textRect,Qt::AlignCenter, message);

        //arrows
        double Pi = 3.14159265359;
        double DoublePi = 6.28318531;
        double angle = ::acos(line.dx() / line.length());
        if (line.dy() >= 0)
            angle = DoublePi - angle;
        if(selected)
            painter->setBrush(Qt::red);
        else
            painter->setBrush(QColor(200,200,200,255));

        QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * 10,     //* arrowSize
                                                      cos(angle + Pi / 3) * 10);
        QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * 10,
                                                      cos(angle + Pi - Pi / 3) * 10);
        QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * 10,
                                                  cos(angle - Pi / 3) * 10);
        QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * 10,
                                                  cos(angle - Pi + Pi / 3) * 10);

        if(direction==DEST_TO_SOURCE)
        {
            painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
        }
        if(direction==SOURCE_TO_DEST)
        {
            painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
        }
        if(direction==TWO_WAY)
        {
            painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
            painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
        }
    }
    //A->A
    else{
        painter->drawArc(source->pos().x()-13,dest->pos().y()-13, 80,80,-210*16,330*16);

        QRadialGradient gradient(-3, -3, 10);
        gradient.setColorAt(0, Qt::darkGray);
        gradient.setColorAt(1, Qt::black);
        painter->setBrush(gradient);
        painter->drawEllipse(source->pos().x()+51,source->pos().y()+51, 10, 10);
        QRectF textRect(source->pos().x()+51,source->pos().y()+51, 10, 10);
        QString message = QString::number(weight);
        QFont font = painter->font();
        font.setBold(true);
        font.setPointSize(5);
        painter->setFont(font);
        painter->drawText(textRect,Qt::AlignCenter, message);
    }
}

/*return source node*/
Node *Edge::getSourceNode() const
{
    return source;
}

/*return destination node*/
Node *Edge::getDestNode() const
{
    return dest;
}

/*return name of source node*/
QString Edge::getSource()
{
    return sourceName;
}

/*return name of destination node*/
QString Edge::getDest()
{
    return destName;
}

/*return weight of edge*/
int Edge::getWeight()
{
    return weight;
}

/*return direction of edge*/
Direction Edge::getDirection()
{
    return direction;
}

/*set new source*/
void Edge::setSource(Node *n)
{
    this->source->removeEdge(this);
    source=n;
    sourceName=n->getName();
    n->addEdge(this);
    update();
}

/*set new destination*/
void Edge::setDest(Node *n)
{
    this->dest->removeEdge(this);
    dest=n;
    destName=n->getName();
    n->addEdge(this);
    update();
}

/*set new weight*/
void Edge::setWeight(int w)
{
    weight=w;
    update();
}

/*set new direction*/
void Edge::setDirection(Direction d)
{
    direction=d;
    update();
}


