#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>

class QMenu;        //używane w mainwindow
class QAction;      //używane w mainwindow
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

class Node;
class Edge;
class GraphManager;
class GraphicWindow;

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    MainWindow();           //Konstruktor
    ~MainWindow();          //Destruktor
    void showEditNode();
    void showEditEdge();
    void addNode(Node *n);
    void addEdge(Edge *e);

    void removeEdge(Edge *e);
    void setActiveNode(Node *n);
    void setActiveEdge(Edge *e);


private slots:
    void newFile();         //Akcja po kliknięciu File - > New
    void open();            //Akcja po kliknięciu File - > Open
    void save();            //Akcja po kliknięciu File - > Save
    void twor();
    void runAlgorithm();    //Wykonywane przy wciśnięciu przycisku "uruchom algorytm"

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
    void createActions();   //Łączy akcje z przyciskami
    void createMenus();     //Inicjalizuje menu
    void createToolbar();   //Tworzy toolbar
    void createEditNode();
    void createEditEdge();
    void createSelector();

    void clearAll();

    Node* activeNode;
    Edge* activeEdge;
    GraphicWindow *graphic;

    //menu file
    QMenu *fileMenu;        //Menu File
    QAction *newAct;        //Akcja newFile
    QAction *openAct;       //Akcja open
    QAction *saveAct;       //Akcja save
    QAction *exitAct;       //Akcja exit
    QAction *tworcy;        //

    //algorytmy
    QToolBar *toolbar;
    QComboBox *algorithms;
    QStringList algorytmy;
    QPushButton *uruchom;

    //menu edit node
    QToolBar *nodeEdit;
    QLabel *nodeNameLabel;
    QLineEdit *nodeNameLine;
    QPushButton *nodeNameSet;
    QLabel *nodeWeightLabel;
    QLineEdit *nodeWeightLine;
    QPushButton *nodeWeightSet;
    QPushButton *nodeColorEditor;
    QPushButton *closeNodeEdit;

    //menu edit edge
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

    //narzedzia
    QToolBar *selector;
    QListWidget *nodesTable;
    QListWidget *edgesTable;
    //panel dodawania wierzchołka
    QGroupBox *nodeAddPanel;
    QLineEdit *addNodeName;
    QLineEdit *addNodeWeight;
    QPushButton *newNodeButton;
    QPushButton *removeNodeButton;
    //panel dodawania krawędzi
    QGroupBox *edgeAddPanel;
    QComboBox *sourceNodes;
    QComboBox *destNodes;
    QComboBox *directionOfEdge;
    QLineEdit *addEdgeWeight;
    QPushButton *newEdgeButton;
    QPushButton *removeEdgeButton;
};

#endif // MAINWINDOW_H
