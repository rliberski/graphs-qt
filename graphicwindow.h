#ifndef GRAPHICWINDOW_H
#define GRAPHICWINDOW_H

#include <QGraphicsView>
#include "edge.h"

class GraphManager;
class MainWindow;
class Node;
class Edge;

class GraphicWindow : public QGraphicsView{
    Q_OBJECT
public:
    GraphicWindow(QWidget *parent,MainWindow *wind, int a);                                 //Tworzy puste pole do rysowania
    GraphicWindow(QWidget *parent,MainWindow *wind, QString fileName = "");          //Tworzy pole do rysowania z grafem z pliku

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
    void scaleView(qreal scaleFactor);                              //Skaluje pole

    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;            //Obsługa rolki myszy

private:
    GraphManager* mngr;                                             //Interfejs, gdzie przechowujemy dane o grafie
    MainWindow *wndw;
};



#endif // GRAPHICWINDOW_H
