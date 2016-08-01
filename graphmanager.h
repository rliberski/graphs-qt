#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H

#include <QVector>
#include <string>
#include <map>
#include <vector>

using namespace std;

class Node;
class Edge;

class GraphManager{
public:
    //Metody używane w programie
    GraphManager();

    void addItem(Node* n);                  //funkcja dodająca wierzchołek
    void addItem(Edge* e);                  //funkcja dodająca krawędź

    void removeEdge(Edge* e);
    void removeNode(Node* n);

    Node* getLastNode();                    //zwraca ostatni dodany wierzchołek
    Edge* getLastEdge();                    //zwraca ostatnio dodaną krawędź

    QVector<Node*> getNodes();              //zwraca listę wierzchołków
    QVector<Edge*> getEdges();              //zwraca listę krawędzi

    map<string,vector<int>> getNodMap();                       //zwraca mapę wierzchołków
    map<pair<string,string>,pair<int,string>> getEgdeMap();    //zwraca mapę krawędzi

    void nodToMap(Node *n);                 //konwertuje Node na wierzchołek do mapy
    void edgeToMap(Edge *n);                //konwertuje Edge na krawędź do mapy

    Node* getNodeByName(QString name);
    Edge* getEdgeBySourceDest(QString source,QString dest);

    void unselectAllNodes();
    void unselectAllEdges();
    bool isClean();
    void runDjikstra();
    void runKolorowanie();

private:
    QVector<Node*> nodes;                    //Lista wierzchołków używana w programie
    QVector<Edge*> edges;                    //Lista krawędzi używana w programie

    map<string,vector<int>> nodMap;                     //Przekazywana mapa wierzchołków
    map<pair<string,string>,pair<int,string>> edgeMap;  //Przekazywana mapa krawędzi


};

#endif // GRAPHMANAGER_H


//Krótki opis
//
//Graf podajemy przez mapę nodMap i mapę edgeMap
//
//nodMap ma parę argumentów:
//  string - nazwa wierzchołka
//  vector<int> - 4 liczby, kolejno: waga, barwa czerwona, barwa zielona, barwa niebieska
//
//edgeMap ma parę argumentów, które też są parami, kolejno:
//  pair<string,string>:
//      string pierwszy - nazwa wierzchołka początkowego
//      string drugi - nazwa wierzchołka docelowego
//  pair<int,string>:
//      int - waga krawędzi
//      string - kierunek krawędzi:
//                  SOURCE_TO_DEST
//                  albo DEST_TO_SOURCE
//                  albo TWO_WAY
//







