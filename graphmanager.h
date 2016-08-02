#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H

#include <QVector>
#include <QMessageBox>

#include "node.h"
#include "edge.h"

class Node;
class Edge;

class GraphManager{
public:
    GraphManager();

    void addItem(Node* n);
    void addItem(Edge* e);
    void removeItem(Edge* e);
    void removeItem(Node* n);

    Node* getLastNode();
    Edge* getLastEdge();

    QVector<Node*> getNodes();
    QVector<Edge*> getEdges();

    Node* getNodeByName(QString name);
    Edge* getEdgeBySourceDest(QString source,QString dest);

    void unselectAllNodes();
    void unselectAllEdges();
    bool isClean();

private:
    QVector<Node*> nodes;
    QVector<Edge*> edges;
};

#endif // GRAPHMANAGER_H








