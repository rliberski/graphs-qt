#include "graphmanager.h"

void error2(QString text)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Error!");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

/*create new graph manager*/
GraphManager::GraphManager()
{
    if(nodes.count()>0)
        nodes.clear();
    if(edges.count()>0)
        edges.clear();
}

/*push new node to vector*/
void GraphManager::addItem(Node* n)
{
    nodes.push_back(n);
}

/*push new edge to vector*/
void GraphManager::addItem(Edge* e)
{
    edges.push_back(e);
}

/*remove edge*/
void GraphManager::removeItem(Edge* e)
{
    edges.removeOne(e);
}

/*remove node*/
void GraphManager::removeItem(Node* n)
{
    nodes.removeOne(n);
}

/*return last added node*/
Node* GraphManager::getLastNode()
{
    return nodes.last();
}

/*return last added edge*/
Edge* GraphManager::getLastEdge()
{
    return edges.last();
}

/*return all nodes*/
QVector<Node*> GraphManager::getNodes()
{
    return nodes;
}

/*return all edges */
QVector<Edge*> GraphManager::getEdges()
{
    return edges;
}

/*return node with name given in parameter*/
Node* GraphManager::getNodeByName(QString name)
{
    foreach(Node* n, nodes)
    {
        if(n->getName()==name)
            return n;
    }
    return 0;
}

/*return edge with source and destination names given in parameters*/
Edge* GraphManager::getEdgeBySourceDest(QString source,QString dest)
{
    foreach(Edge* e, edges)
    {
        if(e->getSource()==source && e->getDest()==dest)
            return e;
    }
    return 0;
}

/*unselect all nodes*/
void GraphManager::unselectAllNodes()
{
    foreach(Node *n,nodes)
    {
        n->unselect();
    }
}

/*unselect all edges*/
void GraphManager::unselectAllEdges()
{
    foreach(Edge *e,edges)
    {
        e->unselect();
    }
}

/*return true if nodes list and edges list is empty*/
bool GraphManager::isClean()
{
    if(nodes.isEmpty() && edges.isEmpty())
        return true;
    else
        return false;
}

