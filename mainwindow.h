#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QtWidgets>
#include <QFileDialog>
#include <QMessageBox>

#include "graphicwindow.h"
#include "graphmanager.h"
#include "node.h"
#include "edge.h"

class QMenu;
class QAction;
class QLabel;
class QToolBar;
class QPushButton;
class QToolButton;
class QLineEdit;
class QComboBox;
class QColorDialog;
class QListWidget;
class QListWidgetItem;
class QGroupBox;
class QVBoxLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
    void showEditNode();
    void showEditEdge();

    void addNode(Node *n);
    void addEdge(Edge *e);

    void removeEdge(Edge *e);
    void setActiveNode(Node *n);
    void setActiveEdge(Edge *e);

private slots:
    void newFile();
    void open();
    void save();

    void hideEditNode();
    void setNodeName();
    void setNodeWeight();
    void setNodeColor();

    void hideEditEdge();
    void setEdgeWeight();
    void setEdgeSource();
    void setEdgeDest();
    void setEdgeDirection();

    void showSelector();
    void hideSelector();
    void addNode();
    void addEdge();
    void removeNode();
    void removeEdge();
    void nodeItemClicked(QListWidgetItem *item);
    void edgeItemClicked(QListWidgetItem *item);

private:
    void createActions();
    void createMenus();
    void createToolbar();
    void createEditNode();
    void createEditEdge();
    void createSelector();
    void createGraphicWindow(QString directory);
    void clearAll();

    Node* activeNode;
    Edge* activeEdge;
    GraphicWindow *graphic;

    /*file menu*/
    QMenu *fileMenu;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;

    /*node editor*/
    QToolBar *nodeEdit;
    QLabel *nodeNameLabel;
    QLineEdit *nodeNameLine;
    QPushButton *nodeNameSet;
    QLabel *nodeWeightLabel;
    QLineEdit *nodeWeightLine;
    QPushButton *nodeWeightSet;
    QPushButton *nodeColorEditor;
    QPushButton *closeNodeEdit;

    /*edge editor*/
    QToolBar *edgeEdit;
    QLineEdit *edgeWeightLine;
    QPushButton *edgeSourceSet;
    QPushButton *edgeDestSet;
    QPushButton *edgeDirSet;
    QPushButton *edgeWeightSet;
    QPushButton *closeEdgeEdit;
    QComboBox *sourceNodes2;
    QComboBox *destNodes2;
    QComboBox *dir2;

    /*selector */
    QToolBar *selector;
    QListWidget *nodesTable;
    QListWidget *edgesTable;
    //node add panel
    QGroupBox *nodeAddPanel;
    QLineEdit *addNodeName;
    QLineEdit *addNodeWeight;
    QPushButton *newNodeButton;
    QPushButton *removeNodeButton;
    //edge add panel
    QGroupBox *edgeAddPanel;
    QComboBox *sourceNodes;
    QComboBox *destNodes;
    QComboBox *directionOfEdge;
    QLineEdit *addEdgeWeight;
    QPushButton *newEdgeButton;
    QPushButton *removeEdgeButton;
};

#endif // MAINWINDOW_H
