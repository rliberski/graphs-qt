#include "graphmanager.h"
#include "node.h"
#include "edge.h"

#include <QLibrary>
#include <QMessageBox>

#include <windows.h>

void errorerror(QString text){
    QMessageBox msgBox;
    msgBox.setWindowTitle("Error, error!");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

GraphManager::GraphManager(){
    if(nodes.count()>0)
        nodes.clear();
    if(edges.count()>0)
        edges.clear();
}

//funkcja dodająca wierzchołek
void GraphManager::addItem(Node* n)
{
    nodes.push_back(n);
}

//funkcja dodająca krawędź
void GraphManager::addItem(Edge* e)
{
    edges.push_back(e);
}

void GraphManager::removeEdge(Edge* e){
    edges.removeOne(e);
    edgeMap.clear();
    foreach(Edge* e,edges){
        edgeToMap(e);
    }
}

void GraphManager::removeNode(Node* n){
    nodes.removeOne(n);
    nodMap.clear();
    foreach(Node* n,nodes){
        nodToMap(n);
    }
}


//zwraca ostatni dodany wierzchołek
Node* GraphManager::getLastNode()
{
    return nodes.last();
}

//zwraca ostatnio dodaną krawędź
Edge* GraphManager::getLastEdge()
{
    return edges.last();
}

//zwraca listę wierzchołków
QVector<Node*> GraphManager::getNodes()
{
    return nodes;
}

//zwraca listę krawędzi
QVector<Edge*> GraphManager::getEdges()
{
    return edges;
}


map<string,vector<int>> GraphManager::getNodMap()
{
    return nodMap;
}


map<pair<string,string>,pair<int,string>> GraphManager::getEgdeMap()
{
   return edgeMap;
}


void GraphManager::nodToMap(Node *n)
{
    vector<int> dane;

    dane.push_back(n->getWeight());
    dane.push_back(n->getColor().red());
    dane.push_back(n->getColor().green());
    dane.push_back(n->getColor().blue());
    nodMap.insert(pair<string,vector<int>>(n->getName().toStdString(),dane));

}


void GraphManager::edgeToMap(Edge *n)
{

  string source=n->getSource().toStdString();
  string dest=n->getDest().toStdString();

  pair<string,string> nodesName(source,dest);

  int waga=n->getWeight();

  Direction dir=n->getDirection();
  string kierunek;

  if((int)dir==0){
    kierunek="SOURCE_TO_DEST";
  }
  else if((int)dir==1){
    kierunek="DEST_TO_SOURCE";
  }
  else if((int)dir==2){
    kierunek="TWO_WAY";
  }

  pair<int,string> weightAndDir(waga,kierunek);

  edgeMap.insert(pair<pair<string,string>,pair<int,string>>(nodesName,weightAndDir));

}

Node* GraphManager::getNodeByName(QString name){
    foreach(Node* n, nodes){
        if(n->getName()==name)
            return n;
    }
    return 0;
}

Edge* GraphManager::getEdgeBySourceDest(QString source,QString dest){
    foreach(Edge* e, edges){
        if(e->getSource()==source && e->getDest()==dest)
            return e;
    }
    return 0;
}

void GraphManager::unselectAllNodes(){
    foreach(Node *n,nodes){
        n->unselect();
    }
}
void GraphManager::unselectAllEdges(){
    foreach(Edge *e,edges){
        e->unselect();
    }
}

bool GraphManager::isClean(){
    if(nodes.isEmpty() && edges.isEmpty())
        return true;
    else
        return false;
}

void GraphManager::runDjikstra(){

    /*
    QLibrary library ("Dijkstra.dll");
    if (!library.load()){
        errorerror("Kenot ołpen dijkstra.dll");
    }
    if (library.load()){
        typedef void (*Dijkstra)(map<string,vector<int>>,map<pair<string,string>,pair<int,string>>, int);

        Dijkstra djkstr = (Dijkstra) library.resolve("Dijkstra");
        if (djkstr)
            djkstr(nodMap,edgeMap,0);
        else{
            errorerror("Kenot ołpen dijkstra fankszyn");
        }
    }

    typedef map<string,vector<int>> (*fun)(map<string,vector<int>>,map<pair<string,string>,pair<int,string>>,int);

    HMODULE hModule = LoadLibrary (L"Dijkstra.dll");
    if (hModule == NULL)
        errorerror("Kenot ołpen dijkstra.dll");
    else{
        fun hFun = (fun)GetProcAddress (hModule, "Dijkstra");
        if (hFun == NULL)
            errorerror("Kenot ołpen dijkstra fankszyn");
        else
            hFun(nodMap,edgeMap,0);
        FreeLibrary (hModule);
    }

    */

}

void GraphManager::runKolorowanie(){

/*
    typedef void (*fun)(map<string,vector<int>>*,map<pair<string,string>,pair<int,string>>);

    HMODULE hModule = LoadLibrary (L"kolorowanie.dll");
    if (hModule == NULL)
        errorerror("Kenot ołpen kolorowanie.dll");
    else{
        fun hFun = (fun)GetProcAddress (hModule, "kolorowanie");
        if (hFun == NULL)
            errorerror("Kenot ołpen kolorowanie fankszyn");
        else
            hFun(&nodMap,edgeMap);
        FreeLibrary (hModule);
    }
*/
}

