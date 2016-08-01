#include <QtWidgets>
#include <QFileDialog>

#include <QMessageBox>
//#include <QMediaPlayer>
//#include <QVideoWidget>
//#include <QMediaPlaylist>
#include <time.h>

#include "mainwindow.h"
#include "graphicwindow.h"
#include "graphmanager.h"
#include "node.h"
#include "edge.h"

void error(QString text){
    QMessageBox msgBox;
    msgBox.setWindowTitle("Error, error!");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

//Destruktor
MainWindow::~MainWindow()
{
}

//Konstruktor - inicjalizuje główne okno
MainWindow::MainWindow()
{
    //Ustawia widget
    QWidget *widget = new QWidget;
    setCentralWidget(widget);


    //Ustawia parametry okna
    setWindowTitle(tr("Graph"));
    setMinimumSize(600, 480);
    this->showMaximized();
    this->update();

    setStyleSheet("QLineEdit{background-color:#666666;font: bold;color:white} \
                   QListWidget{background-color:#666666;font: bold;color:white} \
                   QComboBox{background: #666666; font:bold;color:white;border: 2px solid #666666;border-radius: 6px;}\
                   QComboBox:on{background: #666666; font:bold;color:white}\
                   QPushButton{background: #666666; font:bold;color:white;border: 2px solid #666666;border-radius: 6px;}\
                   QPushButton::item:selected{background: #888888}\
                   QPushButton:pressed{background: #888888; font:bold;color:white}\
                   QPushButton:disabled{background: #88888}\
                   QLabel{color:white;font:bold}\
                   QMainWindow{background:#666666}\
                   QMenuBar{background:#444444}\
                   QMenuBar::item{background:#444444;color:white;font:bold}\
                   QMenuBar::item:selected{background:#666666}\
                   QMenu{background:#444444;color:white;font:bold}\
                   QMenu::item:selected{background:#666666}");


                  //Tworzy menu
                  createActions();
                  createMenus();
                  createToolbar();

                  createSelector();
                  createEditNode();
                  createEditEdge();

}

//Akcja po kliknięciu File - > New
void MainWindow::newFile()
{
    clearAll();
    //Usuń stare pole do rysowania
    QList<GraphicWindow *> widgets = findChildren<GraphicWindow *>();
    foreach(GraphicWindow * widget, widgets)
    {
        ///tutaj trzeba usunąć wierzchołki...
        delete widget;
    }
    //Utwórz nowe pole do rysowania na podstawie danych z pliku
    graphic = new GraphicWindow(this,this,0);
    this->setCentralWidget(graphic);

    showSelector();
    toolbar->setVisible(true);
}

//Akcja po kliknięciu File - > Open
void MainWindow::open()
{
    //Wywołaj okno Otwórz i pobierz lokalizację pliku
    QString lokalizacja = QFileDialog::getOpenFileName(this,tr("Open.."),"/home/",tr("Pliki graph(*.graph)"));

    //Jeśli nie wybrano to wyjdź z funkcji
    if(lokalizacja.isEmpty())
        return;
    //W innym wypadku:
    clearAll();
    //Usuń stare pole do rysowania
    QList<GraphicWindow *> widgets = findChildren<GraphicWindow *>();
    foreach(GraphicWindow * widget, widgets)
    {
        ///tutaj trzeba usunąć wierzchołki...
        delete widget;
    }
    //Utwórz nowe pole do rysowania na podstawie danych z pliku
    graphic = new GraphicWindow(this,this,lokalizacja);
    this->setCentralWidget(graphic);

    showSelector();
    toolbar->setVisible(true);
}

//Akcja po kliknięciu File - > Save
void MainWindow::save()
{
    //Wywołaj okno Otwórz i pobierz lokalizację pliku
    QString lokalizacja = QFileDialog::getSaveFileName(this,tr("Save.."),"/home/",tr("Pliki graph(*.graph)"));

    //Jeśli nie wybrano to wyjdź z funkcji
    if(lokalizacja.isEmpty())
        return;

    //W przeciwnym wypadku zainicjalizuj graf
    QFile plik(lokalizacja);

    //Jeżeli nie da rady otworzyć pliku to wyjdź z funkcji
    if(!plik.open(QIODevice::WriteOnly | QIODevice::Text)){
        error("Problem z otwarciem pliku");
        return;
    }

    QTextStream out(&plik);
    GraphManager *graph=graphic->getMngr();

    out<<"NODES\n";

    foreach(Node *n,graph->getNodes()){
        out<<n->getName()<<" "
           <<QString::number(n->getWeight())<<" "
           <<QString::number(n->getColor().red())<<" "
           <<QString::number(n->getColor().green())<<" "
           <<QString::number(n->getColor().blue())<<"\n";
    }

    out<<"EDGES\n";

    foreach(Edge *e,graph->getEdges()){
        out<<e->getSource()<<" "
           <<e->getDest()<<" "
           <<QString::number(e->getWeight())<<" "
           <<QString::number(static_cast<int>(e->getDirection()))<<"\n";
    }

}

//Akcja po kliknięciu About - > Twórcy
void MainWindow::twor(){
    //QMediaPlayer* player;
    //player = new QMediaPlayer;
    QMessageBox msgBox;
         msgBox.setWindowTitle("AUTORZY!");
         //msgBox.setIcon(QMessageBox::Information);
         QPixmap exportSuccess("pobrane.jpg");
         msgBox.setIconPixmap(exportSuccess);
         msgBox.setText(trUtf8("Najlepsi z najlepszych z myślą o was zrobili ten cudowny program:"));
         msgBox.setInformativeText(trUtf8("Nikodem Ha Nguyen Anh\nKamil Kiewel\nRobert Liberski\nDaniel Siwik"));
         msgBox.setStandardButtons(QMessageBox::Ok);

         //connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
         //player->setMedia(QUrl::fromLocalFile("john.mp3"));
         //player->setVolume(50);
         //player->play();

    msgBox.exec();
    //player->stop();
}

//Łączy akcje z przyciskami
void MainWindow::createActions()
{
    //Łączy zmienną newAct z akcją newFile
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

    //Łączy zmienną openAct z akcją open
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    //Łączy zmienną saveAct z akcją save
    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    //Łączy zmienną exitAct z akcją QWidget::close - czyli zamknięcie programu
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    //Łączy zmienną tworcy z akcją twor
    tworcy=new QAction(tr("&Tworcy"), this);
    tworcy->setStatusTip(tr("Cosik"));
    connect(tworcy, &QAction::triggered, this, &MainWindow::twor);

}

//Inicjalizuje menu
void MainWindow::createMenus()
{
    //Dodaje menu File
    fileMenu = menuBar()->addMenu(tr("&File"));
    //Dodaje zmienne jako akcje do menu file
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    //Dodaje menu About
    fileMenu = menuBar()->addMenu(tr("&About"));
    //Dodaje pozycję twórcy do menu about
    fileMenu->addAction(tworcy);
}

//inicjalizuje toolbar
void MainWindow::createToolbar(){
    //Stwórz toolbar
    toolbar=addToolBar(tr("Edit"));
    toolbar->setStyleSheet("background:#222222;""color:white");
    //Stwórz comboboxa z algorytmami
    algorithms=new QComboBox();
    //Dodaj tego comboboxa do toolbara
    algorithms->setMinimumHeight(20);
    algorithms->setMaximumHeight(20);
    toolbar->addWidget(algorithms);
    //Dodaj pozycje do comboboxa
    algorytmy.push_back("");           //pierwsza pusta
    algorytmy.push_back("Najkrótsza ścieżka");                      //Najkrótsza ścieżka (Dijkstra)
    algorytmy.push_back("Maksymalny przepływ");                     //Maksymalny przepływ (Ford-Fulkerson)
    algorytmy.push_back("Minimalne drzewo rozpinające");            //Minimalne drzewo rozpinające (Kruskal)
    algorytmy.push_back("Kolorowanie grafu");                       //Kolorowanie grafu
    algorytmy.push_back("Najkrótsza ścieżka");                      //Najkrótsza ścieżka (Bellman-Ford)
    algorytmy.push_back("Minimalne drzewo rozpinające");            //Minimalne drzewo rozpinające (Prim)
    algorytmy.push_back("Znajdowanie ścieżki eulerowskiej");        //Znajdowanie ścieżki eulerowskiej (w czasie liniowym)
    algorytmy.push_back("Znajdowanie mostów i punktów artykulacji");//Znajdowanie mostów i punktów artykulacji (wierzchołków rozdzielających)
    algorytmy.push_back("Przeszukiwanie grafu");                    //Przeszukiwanie grafu (wszerz, wgłąb, uogólnione)
    algorytmy.push_back("Maksymalny przepływ");                     //Maksymalny przepływ - przepychanie wstępne (preflow-push)
    algorithms->addItems(algorytmy);
    //Dodaj przycisk uruchom
    uruchom = new QPushButton("Run algorithm",this);
    uruchom->setMinimumHeight(22);
    uruchom->setMaximumHeight(22);
    connect(uruchom, SIGNAL (released()), this, SLOT(runAlgorithm()));
    toolbar->addWidget(uruchom);
    toolbar->addSeparator();
    //dodaj przycisk "otwórz spis zawartości"
    toolbar->addWidget(new QLabel("           "));
    /*
    openSelector = new QToolButton(this);
    openSelector->setText("Zawartość");
    openSelector->setCheckable(true);
    openSelector->setMinimumHeight(22);
    openSelector->setMaximumHeight(22);
    connect(openSelector, SIGNAL (released()), this, SLOT(showSelector()));
    toolbar->addWidget(openSelector);*/

    toolbar->setVisible(false);
}

//akcja po wciśnięciu "uruchom"
void MainWindow::runAlgorithm(){
    //w ten sposób pobieramy VVVVVVVVVVVVVVVVVVVVVVVVV pozycje z comboboxa
    //algorithms->currentIndex();

    //pobieramy sobie wskaźnik do managera grafu i na nim dzialamy
    GraphicWindow *poleZGrafem = this->findChild<GraphicWindow *>();
    GraphManager *graph = poleZGrafem->getMngr();

    if(algorithms->currentIndex()==0){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Przyklad");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("Wybierz sobie najpierw funkcję typie");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }
    else if(algorithms->currentIndex()==1){ //djikstra
        graph->runDjikstra();
    }
    else if(algorithms->currentIndex()==4){ //kolorowanie grafu
        graph->runKolorowanie();
    }

}

void MainWindow::clearAll(){

    activeNode=NULL;
    activeEdge=NULL;

    if(nodeNameLine->text()!="")
        nodeNameLine->clear();
    if(nodeWeightLine->text()!="")
        nodeWeightLine->clear();
    //if(edgeWeightLine->text()!="")
        //edgeWeightLine->clear();
    //if(addEdgeWeight->text()!="")
        //addEdgeWeight->clear();
    if(addNodeName->text()!="")
        addNodeName->clear();
    if(addNodeWeight->text()!="")
        addNodeWeight->clear();


    if(nodesTable->count()>0){
        nodesTable->clear();
    }
    if(edgesTable->count()>0){
        edgesTable->clear();
    }
    if(sourceNodes->count()>0){
        sourceNodes->clear();
    }
    if(destNodes->count()>0){
        destNodes->clear();
    }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::createEditNode(){
    //Stwórz toolbar
    nodeEdit=new QToolBar(tr("Node edit"),this);
    nodeEdit->setStyleSheet("background:#222222;""color:white");
    this->addToolBar(Qt::LeftToolBarArea,nodeEdit);
    nodeEdit->setMovable(false);

    nodeEdit->addWidget(new QLabel("\nNode editor"));
    nodeEdit->addSeparator();

    nodeNameLabel=new QLabel("Name: ");
    nodeEdit->addWidget(nodeNameLabel);
    nodeNameLine=new QLineEdit();
    nodeEdit->addWidget(nodeNameLine);
    nodeNameSet=new QPushButton("Set",this);
    connect(nodeNameSet, SIGNAL (released()), this, SLOT(setNodeName()));
    nodeEdit->addWidget(nodeNameSet);
    nodeEdit->addSeparator();

    nodeWeightLabel=new QLabel("Weight: ");
    nodeEdit->addWidget(nodeWeightLabel);
    nodeWeightLine=new QLineEdit();
    nodeWeightLine->setValidator( new QIntValidator(0,100));
    nodeEdit->addWidget(nodeWeightLine);
    nodeWeightSet=new QPushButton("Set",this);
    connect(nodeWeightSet, SIGNAL (released()), this, SLOT(setNodeWeight()));
    nodeEdit->addWidget(nodeWeightSet);
    nodeEdit->addSeparator();

    nodeColorEditor=new QPushButton("Choose node color");
    connect(nodeColorEditor, SIGNAL (released()), this, SLOT(setNodeColor()));
    nodeEdit->addWidget(nodeColorEditor);
    nodeEdit->addSeparator();

    closeNodeEdit=new QPushButton("Close editor",this);
    connect(closeNodeEdit, SIGNAL (released()), this, SLOT(hideEditNode()));
    nodeEdit->addWidget(closeNodeEdit);

    hideEditNode();
}
void MainWindow::hideEditNode(){
    nodeEdit->setVisible(false);
    removeNodeButton->setEnabled(false);
}
void MainWindow::showEditNode(){
    hideEditEdge();

    nodeEdit->setVisible(true);
    nodeNameLine->setText(activeNode->getName());
    nodeWeightLine->setText(QString::number(activeNode->getWeight()));

    removeNodeButton->setEnabled(true);
}
void MainWindow::setNodeName(){
    if(sourceNodes->findText(nodeNameLine->text())==-1){
        QString oldName=activeNode->getName();
        nodesTable->currentItem()->setText(" Name: "+nodeNameLine->text()+" Weight: "+QString::number(activeNode->getWeight()));
        activeNode->setName(nodeNameLine->text());

        int pos=sourceNodes->findText(oldName);
        sourceNodes->setItemText(pos,activeNode->getName());
        sourceNodes->update();

        pos=destNodes->findText(oldName);
        destNodes->setItemText(pos,activeNode->getName());
        destNodes->update();

        pos=sourceNodes2->findText(oldName);
        sourceNodes2->setItemText(pos,activeNode->getName());
        sourceNodes2->update();

        pos=destNodes2->findText(oldName);
        destNodes2->setItemText(pos,activeNode->getName());
        destNodes2->update();

        //krawędzie
        for(int i=0;i<edgesTable->count();i++){
            QListWidgetItem *item=edgesTable->item(i);
            QString text=item->text();
            text.replace(" "+oldName+" "," "+activeNode->getName()+" ");
            item->setText(text);
        }

    }
    else if(nodeNameLine->text()==""){
        error("Wprowadź najpierw nazwę");
    }
    else{
        error("Istnieje już wierzchołek z taką nazwą!");
    }
}
void MainWindow::setNodeWeight(){
    if(nodeWeightLine->text()!=""){
        nodesTable->currentItem()->setText(" Name: "+activeNode->getName()+" Weight: "+nodeWeightLine->text());
        activeNode->setWeight(nodeWeightLine->text().toInt());
    }
    else
        error("Wprowadź najpierw wagę");
}
void MainWindow::setNodeColor(){
    QColorDialog *coldiag = new QColorDialog(activeNode->getColor());
    activeNode->setColor(coldiag->getColor());
    delete coldiag;
}
void MainWindow::setActiveNode(Node *n){
    activeNode=n;
}

void MainWindow::createEditEdge(){
    //Stwórz toolbar
    edgeEdit=new QToolBar(tr("Edge edit"),this);
    edgeEdit->setStyleSheet("background:#222222;""color:white");
    this->addToolBar(Qt::LeftToolBarArea,edgeEdit);
    edgeEdit->setMovable(false);

    edgeEdit->addWidget(new QLabel("\nEdge editor"));
    edgeEdit->addSeparator();

    edgeSourceSet=new QPushButton("Set");
    edgeDestSet=new QPushButton("Set");
    edgeDirSet=new QPushButton("Set");
    edgeWeightSet=new QPushButton("Set");
    closeEdgeEdit=new QPushButton("Close editor");

    sourceNodes2 = new QComboBox();
    destNodes2 = new QComboBox();
    dir2 = new QComboBox();
    dir2->insertItem(0,QString("SOURCE_TO_DEST"));
    dir2->insertItem(1,QString("DEST_TO_SOURCE"));
    dir2->insertItem(2,QString("TWO_WAY"));


    connect(edgeSourceSet, SIGNAL (released()), this, SLOT(setEdgeSource()));
    connect(edgeDestSet, SIGNAL (released()), this, SLOT(setEdgeDest()));
    connect(edgeDirSet, SIGNAL (released()), this, SLOT(setEdgeDirection()));
    connect(edgeWeightSet, SIGNAL (released()), this, SLOT(setEdgeWeight()));
    connect(closeEdgeEdit, SIGNAL (released()), this, SLOT(hideEditEdge()));

    edgeEdit->addWidget(new QLabel("Source: "));
    edgeEdit->addWidget(sourceNodes2);
    edgeEdit->addWidget(edgeSourceSet);
    edgeEdit->addSeparator();

    edgeEdit->addWidget(new QLabel("Destination: "));
    edgeEdit->addWidget(destNodes2);
    edgeEdit->addWidget(edgeDestSet);
    edgeEdit->addSeparator();

    edgeEdit->addWidget(new QLabel("Direction: "));
    edgeEdit->addWidget(dir2);
    edgeEdit->addWidget(edgeDirSet);
    edgeEdit->addSeparator();

    edgeEdit->addWidget(new QLabel("Weight: "));
    edgeWeightLine=new QLineEdit();
    edgeWeightLine->setValidator( new QIntValidator(0,100));
    edgeEdit->addWidget(edgeWeightLine);
    edgeEdit->addWidget(edgeWeightSet);
    edgeEdit->addSeparator();

    edgeEdit->addWidget(closeEdgeEdit);

    hideEditEdge();
}
void MainWindow::showEditEdge(){
    hideEditNode();

    edgeEdit->setVisible(true);

    edgeWeightLine->setText(QString::number(activeEdge->getWeight()));

    int sourcepos=sourceNodes2->findText(activeEdge->getSource());
    int destpos=destNodes2->findText(activeEdge->getDest());
    int dirpos=activeEdge->getDirection();

    sourceNodes2->setCurrentIndex(sourcepos);
    destNodes2->setCurrentIndex(destpos);
    dir2->setCurrentIndex(dirpos);

    removeEdgeButton->setEnabled(true);
}
void MainWindow::hideEditEdge(){
    edgeEdit->setVisible(false);
    removeEdgeButton->setEnabled(false);
}
void MainWindow::setEdgeWeight(){
    if(edgeWeightLine->text()!=""){
        QString item=" "+activeEdge->getSource();
        if(activeEdge->getDirection()==SOURCE_TO_DEST){
            item+=" -> ";
        }
        else if(activeEdge->getDirection()==DEST_TO_SOURCE){
            item+=" <- ";
        }
        else if(activeEdge->getDirection()==TWO_WAY){
            item+=" <-> ";
        }
        item+=activeEdge->getDest()+" Weight: "+edgeWeightLine->text();

        edgesTable->currentItem()->setText(item);
        activeEdge->setWeight(edgeWeightLine->text().toInt());
    }
    else
        error("Wprowadź najpierw wagę");
}
void MainWindow::setEdgeSource(){

    for(int i=0;i<edgesTable->count();i++){
        QString text=edgesTable->item(i)->text();
        QStringList list = text.split(" ");
        QString source = list.at(1);
        QString dest = list.at(3);
        if(source==sourceNodes2->currentText()&&destNodes2->currentText()==dest){
            error("Istnieje już taka krawędź");
            return;
        }
        if(dest==sourceNodes2->currentText()&&destNodes2->currentText()==source){
            error("Istnieje już taka krawędź");
            return;
        }
    }

    Node *dest=activeEdge->destNode();
    int weight=activeEdge->getWeight();
    Direction dir=activeEdge->getDirection();

    QListWidgetItem* item = edgesTable->currentItem();
    delete item;

    delete activeEdge;

    graphic->getMngr()->addItem(new Edge(graphic->getMngr()->getNodeByName(sourceNodes2->currentText()),dest,weight,dir,graphic));
    graphic->scene()->addItem(graphic->getMngr()->getLastEdge());
    graphic->getMngr()->edgeToMap(graphic->getMngr()->getLastEdge());
    activeEdge=graphic->getMngr()->getLastEdge();
}
void MainWindow::setEdgeDest(){

    for(int i=0;i<edgesTable->count();i++){
        QString text=edgesTable->item(i)->text();
        QStringList list = text.split(" ");
        QString source = list.at(1);
        QString dest = list.at(3);
        if(source==sourceNodes2->currentText()&&destNodes2->currentText()==dest){
            error("Istnieje już taka krawędź");
            return;
        }
        if(dest==sourceNodes2->currentText()&&destNodes2->currentText()==source){
            error("Istnieje już taka krawędź");
            return;
        }
    }

    Node *source=activeEdge->sourceNode();
    int weight=activeEdge->getWeight();
    Direction dir=activeEdge->getDirection();

    QListWidgetItem* item = edgesTable->currentItem();
    delete item;

    delete activeEdge;

    graphic->getMngr()->addItem(new Edge(source,graphic->getMngr()->getNodeByName(destNodes2->currentText()),weight,dir,graphic));
    graphic->scene()->addItem(graphic->getMngr()->getLastEdge());
    graphic->getMngr()->edgeToMap(graphic->getMngr()->getLastEdge());
    activeEdge=graphic->getMngr()->getLastEdge();
}
void MainWindow::setEdgeDirection(){
    activeEdge->setDirection(static_cast<Direction>(dir2->currentIndex()));
}
void MainWindow::setActiveEdge(Edge *e){
    activeEdge=e;
}

void MainWindow::createSelector(){
    //Stwórz toolbar
    selector=new QToolBar(tr("Selector"),this);
    selector->setStyleSheet("background:#222222;""color:white");
    this->addToolBar(Qt::RightToolBarArea,selector);
    selector->setMovable(false);
    selector->addSeparator();

    //lista wierzchołków
    selector->addWidget(new QLabel("Nodes:"));
    nodesTable=new QListWidget();
    nodesTable->setMaximumWidth(250);
    nodesTable->setMaximumHeight(250);
    connect(nodesTable, SIGNAL(itemPressed(QListWidgetItem *)), this, SLOT(nodeItemClicked(QListWidgetItem *)));
    selector->addWidget(nodesTable);
    removeNodeButton=new QPushButton("Remove selected node");
    connect(removeNodeButton,SIGNAL(released()),this,SLOT(removeNode()));
    selector->addWidget(removeNodeButton);
    selector->addSeparator();

    //panel dodawania wierzchołka
    nodeAddPanel=new QGroupBox();
    QVBoxLayout *layout=new QVBoxLayout();
    layout->addWidget(new QLabel("Name: "));
    addNodeName=new QLineEdit();
    layout->addWidget(addNodeName);
    layout->addWidget(new QLabel("Weight: "));
    addNodeWeight=new QLineEdit();
    addNodeWeight->setValidator( new QIntValidator(0,100));
    layout->addWidget(addNodeWeight);
    newNodeButton=new QPushButton("Add new Node..");
    connect(newNodeButton, SIGNAL (released()), this, SLOT(addNode()));
    layout->addWidget(newNodeButton);
    nodeAddPanel->setLayout(layout);
    selector->addWidget(nodeAddPanel);

    selector->addSeparator();

    //lista krawędzi
    selector->addWidget(new QLabel("\nEdges:"));
    edgesTable=new QListWidget();;
    edgesTable->setMaximumWidth(250);
    edgesTable->setMaximumHeight(250);
    connect(edgesTable, SIGNAL(itemPressed(QListWidgetItem *)), this, SLOT(edgeItemClicked(QListWidgetItem *)));
    selector->addWidget(edgesTable);
    removeEdgeButton=new QPushButton("Remove selected edge");
    connect(removeEdgeButton,SIGNAL(released()),this,SLOT(removeEdge()));
    selector->addWidget(removeEdgeButton);
    selector->addSeparator();

    //panel dodawania krawędzi
    edgeAddPanel=new QGroupBox();
    QVBoxLayout *layout2=new QVBoxLayout();
    layout2->addWidget(new QLabel("Source: "));
    sourceNodes=new QComboBox();
    layout2->addWidget(sourceNodes);
    layout2->addWidget(new QLabel("Destination: "));
    destNodes=new QComboBox();
    layout2->addWidget(destNodes);
    layout2->addWidget(new QLabel("Direction: "));
    directionOfEdge=new QComboBox();
    directionOfEdge->insertItem(0,QString("SOURCE_TO_DEST"));
    directionOfEdge->insertItem(1,QString("DEST_TO_SOURCE"));
    directionOfEdge->insertItem(2,QString("TWO_WAY"));
    layout2->addWidget(directionOfEdge);
    layout2->addWidget(new QLabel("Weight: "));
    addEdgeWeight=new QLineEdit();
    addEdgeWeight->setValidator( new QIntValidator(0,100));
    layout2->addWidget(addEdgeWeight);
    newEdgeButton=new QPushButton("Add new edge..");
    connect(newEdgeButton, SIGNAL (released()), this, SLOT(addEdge()));
    layout2->addWidget(newEdgeButton);
    edgeAddPanel->setLayout(layout2);
    selector->addWidget(edgeAddPanel);

    selector->addSeparator();

    removeNodeButton->setEnabled(false);
    removeEdgeButton->setEnabled(false);

    hideSelector();
}
void MainWindow::showSelector(){
    //if(openSelector->isChecked())
        selector->setVisible(true);
    //else
        //hideSelector();
}
void MainWindow::hideSelector(){
    selector->setVisible(false);
}
void MainWindow::addNode(){
    if(sourceNodes->findText(addNodeName->text())==-1 && addNodeWeight->text()!="" && addNodeName->text()!=""){
        graphic->addNode(addNodeName->text(),addNodeWeight->text().toInt());
    }
    else if(addNodeWeight->text()=="")
        error("Wprowadz najpierw wagę");
    else if(addNodeName->text()=="")
        error("Wprowadź najpierw nazwę");
    else
        error("Istnieje już wierzchołek o takiej nazwie!");

}
void MainWindow::addEdge(){
    if(addEdgeWeight->text()=="")
        error("Wprowadz najpierw wagę");
    else{
        //sprawdź czy istnieje:
        for(int i=0;i<edgesTable->count();i++){
            QString text=edgesTable->item(i)->text();
            QStringList list = text.split(" ");
            QString source = list.at(1);
            QString dest = list.at(3);
            if(source==sourceNodes->currentText()&&destNodes->currentText()==dest){
                error("Istnieje już taka krawędź");
                return;
            }
            if(dest==sourceNodes->currentText()&&destNodes->currentText()==source){
                error("Istnieje już taka krawędź");
                return;
            }
        }


        Node *s=graphic->getMngr()->getNodeByName(sourceNodes->currentText());
        Node *d=graphic->getMngr()->getNodeByName(destNodes->currentText());
        Direction dir=static_cast<Direction>(directionOfEdge->currentText().toInt());
        int weight=addEdgeWeight->text().toInt();
        graphic->addEdge(s,d,weight,dir);

    }
}
void MainWindow::addNode(Node *n){

    QString item=" Name: " +n->getName()+" Weight: "+QString::number(n->getWeight());

    nodesTable->addItem(new QListWidgetItem(item));
    sourceNodes->addItem(QString(n->getName()));
    destNodes->addItem(QString(n->getName()));
    sourceNodes2->addItem(QString(n->getName()));
    destNodes2->addItem(QString(n->getName()));


}
void MainWindow::addEdge(Edge *e){
    QString item=" "+e->getSource();
    if(e->getDirection()==SOURCE_TO_DEST){
        item+=" -> ";
    }
    else if(e->getDirection()==DEST_TO_SOURCE){
        item+=" <- ";
    }
    else if(e->getDirection()==TWO_WAY){
        item+=" <-> ";
    }
    item+=e->getDest()+" Weight: "+QString::number(e->getWeight());
    edgesTable->addItem(new QListWidgetItem(item));
}
void MainWindow::removeNode(){



    QString name=nodesTable->currentItem()->text().split(" ").at(2);

    delete nodesTable->currentItem();

    //znajdź wierzchołek w comboboxach i usuń
    int pos=sourceNodes->findText(name);
    sourceNodes->removeItem(pos);

    pos=destNodes->findText(name);
    destNodes->removeItem(pos);

    pos=sourceNodes2->findText(name);
    sourceNodes2->removeItem(pos);

    pos=destNodes2->findText(name);
    destNodes2->removeItem(pos);

    activeNode->removeThis();
    hideEditNode();
}
void MainWindow::removeEdge(){
    delete edgesTable->currentItem();
    activeEdge->removeThis();
    hideEditEdge();
}
void MainWindow::removeEdge(Edge *e){
    QString sourceNode=e->getSource();
    QString destNode=e->getDest();

    for(int i=0;i<edgesTable->count();i++){
        QString text=edgesTable->item(i)->text();
        QStringList list = text.split(" ");
        QString source = list.at(1);
        QString dest = list.at(3);
        if(source==sourceNode && destNode==dest){
            delete edgesTable->item(i);
            continue;
        }
        if(dest==sourceNode && destNode==source){
            delete edgesTable->item(i);
            continue;
        }
    }
}
void MainWindow::nodeItemClicked(QListWidgetItem *item){
    //pobieramy sobie wskaźnik do managera grafu i na nim dzialamy
    GraphicWindow *poleZGrafem = this->findChild<GraphicWindow *>();
    GraphManager *graph = poleZGrafem->getMngr();

    QStringList list = item->text().split(" ");
    QString name = list.at(2);

    activeNode=graph->getNodeByName(name);
    activeNode->selectThis();
    activeNode->update();
}
void MainWindow::edgeItemClicked(QListWidgetItem *item){
    //pobieramy sobie wskaźnik do managera grafu i na nim dzialamy
    GraphicWindow *poleZGrafem = this->findChild<GraphicWindow *>();
    GraphManager *graph = poleZGrafem->getMngr();

    QStringList list = item->text().split(" ");
    QString source = list.at(1);
    QString dest = list.at(3);

    activeEdge=graph->getEdgeBySourceDest(source,dest);
    activeEdge->select();
    activeEdge->update();
}

