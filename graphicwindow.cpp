#include <math.h>
#include <iostream>
#include <QFile>
#include <time.h>
#include <QTextStream>
#include <QKeyEvent>
#include <QMetaEnum>

#include "graphicwindow.h"
#include "mainwindow.h"
#include "graphmanager.h"
#include "node.h"
#include "edge.h"

using namespace std;

//Tworzy puste pole do rysowania
GraphicWindow::GraphicWindow(QWidget *parent,MainWindow *wind,int a): QGraphicsView(parent)
{
    //Stwórz interfejs
    mngr = new GraphManager();
    wndw = wind;

    int skala=-800;               //Skala rysowania pola

    //Tworzy pole i ustawia jego parametry
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(skala, skala, -skala, -skala);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(1), qreal(1));
    setMinimumSize(skala/-2,skala/-2);

    this->setStyleSheet("background-color: #222222");

}

//Tworzy pole do rysowania z grafem z pliku
GraphicWindow::GraphicWindow(QWidget *parent,MainWindow *wind,QString fileName): QGraphicsView(parent)
{
    //Jesli nie wskazano pliku to wyjdz z funkcji
    if(fileName.isEmpty())
        return;

    //W przeciwnym wypadku zainicjalizuj graf
    QFile plik(fileName);

    //Jeżeli nie da rady otworzyć pliku to wyjdź z funkcji
    if(!plik.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }

    //Stwórz interfejs
    mngr = new GraphManager();
    wndw = wind;

    int skala=-800;               //Skala rysowania pola

    //Tworzy pole i ustawia jego parametry
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(skala, skala, -skala, -skala);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(1), qreal(1));
    setMinimumSize(skala/-2,skala/-2);

    this->setStyleSheet("background-color: #222222");

    //Wczytywanie z pliku

    //plik ma dwie części: NODES i EDGES
    //jeśli wczytana linia to "NODES" to w następnych pobieramy wierzchołki
    //jeśli wczytana linia to "EDGES" to w następnych pobieramy krawędzi
    //zapisujemy do zmiennej 'nextline' co mamy pobrać z pliku i tworzymy obiekt
    //
    //easy
    //
    //Format wierzchołka:
    //Nazwa Waga KolorRed KolorGreen KolorBlue
    //np: A 6 200 0 0
    //
    //Format krawędzi:
    //Skąd Dokąd Waga Kierunek (patrz enum Direction w edge.h)
    //np: A B 3 SOURCE_TO_DEST
    //
    //Wartości oddzielone spacjami, do krawędzi trzeba jeszcze dorobić kierunek.
    //Kierunek może być skąd->dokąd, gorzej z krawędziami które są skierowane w obie strony
    //
    //PS !!!!!!!!!
    //Trzeba dodać też warunek przy dodawaniu wierzchołka, żeby każdy wierzchołek miał unikalną nazwę

    QString nextline="";
    QTextStream in(&plik);

    while(!in.atEnd()){
        QString line = in.readLine();
        if(line=="NODES"){
            nextline="Node";
        }
        else if(line=="EDGES"){
            nextline="Edge";
        }
        else if(nextline=="Node"){
            QStringList list = line.split(" ");
            QString nazwa = list.at(0);
            int waga = list.at(1).toInt();
            int kolorred = list.at(2).toInt();
            int kolorgreen = list.at(3).toInt();
            int kolorblue = list.at(4).toInt();
            QColor kolor(kolorred,kolorgreen,kolorblue,255);

            mngr->addItem(new Node(this,nazwa,waga,kolor));
            scene->addItem(mngr->getLastNode());
            mngr->nodToMap(mngr->getLastNode());

        }
        else if(nextline=="Edge"){
            QStringList list = line.split(" ");

            Node* sourc;
            Node* des;

            foreach(Node *n, mngr->getNodes()) {
                if (n->getName() == list.at(0))
                    sourc = n;
                if (n->getName() == list.at(1))
                    des = n;
            }

            int waga = list.at(2).toInt();
            Direction dir = (Direction)list.at(3).toInt();

            mngr->addItem(new Edge(sourc,des,waga,dir,this));
            scene->addItem(mngr->getLastEdge());
            mngr->edgeToMap(mngr->getLastEdge());
        }
    }

    plik.close();

    //Ustalanie pozycji
    srand (time(NULL));
    foreach(Node* n, mngr->getNodes())
    {
        int pom=rand()%401+200;
        int pom2=rand()%401+200;
        n->setPos(-pom,-pom2); 
    }

}

//Skaluje pole
void GraphicWindow::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

//zwraca graf
GraphManager* GraphicWindow::getMngr(){
    return mngr;
}

MainWindow* GraphicWindow::getWndw(){
    return wndw;
}

void GraphicWindow::unselectAll(){
    mngr->unselectAllNodes();
    mngr->unselectAllEdges();
}
void GraphicWindow::showEditNode(Node *n){
    wndw->setActiveNode(n);
    wndw->showEditNode();
}
void GraphicWindow::showEditEdge(Edge *e){
    wndw->setActiveEdge(e);
    wndw->showEditEdge();
}
void GraphicWindow::newNode(Node *n){
    wndw->addNode(n);
}

void GraphicWindow::newEdge(Edge *e){
    wndw->addEdge(e);
}

void GraphicWindow::addNode(QString name,int weight){

    Node *newNode =new Node(this,name,weight,Qt::red);
    mngr->addItem(newNode);
    scene()->addItem(newNode);
    mngr->nodToMap(newNode);

    //Ustalanie pozycji
    srand (time(NULL));
    int pom=rand()%401+200;
    int pom2=rand()%401+200;
    newNode->setPos(-pom,-pom2);
    newNode->update();
}

void GraphicWindow::addEdge(Node *source,Node *dest,int weight,Direction dir){
    Edge *newEdge=new Edge(source,dest,weight,dir,this);
    mngr->addItem(newEdge);
    scene()->addItem(newEdge);
    mngr->edgeToMap(newEdge);

    newEdge->update();
}

//Obsługa rolki myszy
void GraphicWindow::wheelEvent(QWheelEvent *event)
{
    //Wywołaj funkcję skalującą pole
    scaleView(pow((double)2, event->delta() / 240.0));
}


