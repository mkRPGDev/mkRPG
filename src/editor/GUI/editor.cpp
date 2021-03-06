#include "editor.h"

Editor::Editor(QStringList UNUSED(args), QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    QPalette p(tabBar->palette());
    p.setColor(QPalette::Window, p.color(QPalette::Mid));
    tabBar->setPalette(p);
    hidden->setHidden(true);
    connect(tabBar, SIGNAL(currentTabChanged(int)), this, SLOT(setCurrentTab(int)));

    welcome = new Welcome;
    worldEditor = new WorldTab;
    mapsEditor = new MapsTab;
    objectEditor = new ObjectTab;
    actionEditor = new ActionTab;

    addTab(tr("Welcome"), QPixmap(":Icons/main.png"), welcome);
    //addTab(tr("Game"), QPixmap(":Icons/main.png"), worldEditor);
    addTab(tr("Objects"), QPixmap(":Icons/main.png"), objectEditor);
    addTab(tr("Maps"), QPixmap(":Icons/main.png"), mapsEditor);
    addTab(tr("Actions"), QPixmap(":Icons/main.png"), actionEditor);

    loadDefault();

    tabBar->setTabsEnabled(false);

    connect(welcome, SIGNAL(newGameS()), this, SLOT(on_actionRolePlayGame_triggered()));
    connect(welcome, SIGNAL(openGameS()), this, SLOT(on_actionOpen_triggered()));

    //qDebug() << args.length();
    connect(worldEditor, SIGNAL(editMap()), this, SLOT(editMap()));

    setGame(open("f"));
    //setGame(new Game());
    //importGame("/home/baptiste/ENS/test/snake/game.xml");
}

void Editor::setGame(Game *game){
    currentGame = game;
    mapsEditor->setGame(currentGame);
    worldEditor->setGame(currentGame);
    objectEditor->setGame(currentGame);
    actionEditor->setGame(currentGame);
    tabBar->setTabsEnabled(currentGame);
    tabBar->setCurrentTab(1);
}


void Editor::addTab(const QString &n, const QPixmap &p, TabWidget *w){
    stackedWidget->addWidget(w);
    tabBar->addTabAcces(n,p,w);

}

void Editor::loadDefault(){
    Options &options(Options::options());
    move(options.load<QPoint>(WIN, "Position"));
    resize(options.load<QSize>(WIN, "Size"));
    if(options.load<bool>(WIN, "Maximized")) showMaximized();
}

void Editor::saveDefault(){
    Options &options(Options::options());
    options.save(WIN, "Maximized", isMaximized());
}


void Editor::on_actionOpen_triggered(){
    QString f(QFileDialog::getOpenFileName(this, "Open a game project", QDir::homePath(), "Game file *xml"));
    if(f != ""){
        Game *g = importGame(f);
        if(g) setGame(g);
    }
}

bool Editor::getSaveDirectory(QDir &d, const QString &m){
    QString di(QFileDialog::getExistingDirectory(this, m, QDir::homePath()));
    if(di != ""){
        d = QDir(di);
        if(d.mkdir(currentGame->name()))
            d.cd(currentGame->name());
        else{
            //*
            d.cd(currentGame->name());
            d.removeRecursively();
            d.cdUp();
            d.mkdir(currentGame->name());
            d.cd(currentGame->name());
            /*/
            int k(1);
            while(!d.mkdir(g->name()+QString::number(++k)));
            d.cd(g->name()+QString::number(k));
            //*/
        }
        return true;
    }
    return false;
}

void Editor::on_actionExport_triggered(){
    QDir d;
    if(getSaveDirectory(d, "Export as XML (serveur)"))
        XmlWritter(d,*currentGame, true);
}

void Editor::on_actionSave_as_triggered(){
    QDir d;
    if(getSaveDirectory(d,"Save as XML (editor)"))
        XmlWritter(d,*currentGame, false);
}

void Editor::on_actionImportImage_triggered(){
    QString f = QFileDialog::getOpenFileName(this, "Import image", "", "Image *png *jpg");
    if(f!="" && currentGame!=nullptr){
        currentGame->addImage(new Image(*currentGame, f));
    }
}

void Editor::on_actionRolePlayGame_triggered(){
    NewGame g(this);
    if(g.exec()){
        Game *ga = new Game();
        ga->setName(g.name());
        XmlWritter(g.folder(), *ga, true);
        setGame(ga);
    }
//        newGame(g.name(), g.folder(), g.createFolder());
}

void Editor::newGame(QString name, QString dir, bool createFolder){
    /* name : sans .game */
    QDir d(dir);
    if(createFolder){
        if(!d.mkdir(name)){
            QMessageBox::critical(this, "Game creation", "Impossible to create the folder\n\""+dir+"/"+name+"\".\n Insufficient rights ?");
        }
        d.cd(name);
    }
    // TODO peut échouer...
    d.mkdir("Maps");
    d.mkdir("Entities");
    d.mkdir("Objects");
    d.mkdir("Pictures");
    QString mainFile(d.path() + "/"+name+".game");
    QFile f(mainFile);
    f.open(QFile::WriteOnly);
    QTextStream s(&f);
    s << "<Game name=\"" << name << "\">\n";
    s << "  <Source path=\"world.xml\" />\n";
    s << "  <Ressources path=\"ressources.xml\" />\n";
    s << "</Game>\n";
    f.close();
    f.setFileName(d.path()+"/world.xml");
    f.open(QFile::WriteOnly);
    s << "<World name=\"" << name << "\">\n";
    s << "</World>\n";
    f.close();
    f.setFileName(d.path()+"/ressources.xml");
    f.open(QFile::WriteOnly);
    s << "<Ressources>\n";
    s << "</Ressources>\n";
    f.close();

    open(mainFile);
}


Game* Editor::open(QString UNUSED(fileName)){ // NOTE : temporaire
    Game* g = new Game();

    g->setName("Trop_cool_mon_jeu");

    Image *im;
    CellType *ct1, *ct2, *ct3;

    g->addAction("moveLeft", new Action());
    Image *ra = new Image(*g, ":/Icons/raoul.png");
    g->addImage(ra);
    im = new Image(*g, ":/Icons/herbe.png");
    im->setName("Herbe");
    g->addImage(im);
    ct1 = new CellType(g->world().types().cellType());
    ct1->setName("Herbe");
    ct1->setImage(im);
    //g->world().addCellType(ct1);
    im = new Image(*g, ":/Icons/glace.png");
    im->setName("Glace");
    g->addImage(im);
    ct2 = new CellType(g->world().types().cellType());
    ct2->setName("Glace");
    ct2->setImage(im);
    //g->world().addCellType(ct2);
    im = new Image(*g, ":/Icons/mer.png");
    im->setName("Mer");
    g->addImage(im);
    CellType *ct = new CellType(g->world().types().cellType());
    ct->setName("Eau");
    ct->addEvent("Naufrage");
    ct->addOrder("Saler");
    ct->setImage(im);
    ct->setParam("Profondeur", 75);


    ct3 = new CellType(*ct);
    ct3->setName("Mer");
    //ct3->setImage(im);
    ct3->setParam("Salinité", 12);
    ct3->setParam("Pollution", 70);
    ct3->setParam("boue", 0);
    CellType *ctt = new CellType(*ct3);
    ctt->setName("Atlantique");
    ctt->setParam("Salinité", 8);
    ctt->addEvent("Titanic");
    ctt->addOrder("Traverser");
    ctt->setParam("Requins", 20);
    Map *m = new Map(g->world().types().mapType(), g->world());
    g->world().objects().addMap(m);
    m->setName("Paysage_bucolique");
    int l = m->width();
    int h = m->height();
    for(int i(0); i<l; ++i)
        for(int j(0); j<h; ++j){
            double o = 3.*j/h+(1.8-8.*(i-l/2.)*(i-l/2.)/l/l)*((qrand()%65536)/65536.-.5);
            m->cell(i,j).setCellType(o<1 ? *ct1 : o<2 ? *ct2 : *ct3);
        }
    tabBar->setTabsEnabled(true);

    EntityType *bucheron = new EntityType(g->world().types().entityType());
    bucheron->setName("Bucheron");
    bucheron->setFlag("barbu", true);
    bucheron->setImage(ra);
    Entity *Pierre = new Entity(*bucheron, g->world());
    Pierre->setName("Raoul");
    g->world().objects().addEntity(Pierre);
    ObjectType *ot = new ObjectType(g->world().types().objectType());
    ot->setName("Potion");
    ot->setParam("Amertume", 50);
    ot->setFlag("Fatal", true);
    ot->setImage(im);

    Object *Cafe = new Object(*ot, g->world());
    Cafe->setName("Café");
    Cafe->setParam("Energie", 89);
    Cafe->setParam("Amertume", 10);
    Cafe->setFlag("Fatal", false);
    g->world().objects().addObject(Cafe);

    m = new Map(g->world().types().mapType(), g->world());
    m->setName("Le_grand_large");
    g->world().objects().addMap(m);
    l = m->width();
    h = m->height();
    for(int i(0); i<l; ++i)
        for(int j(0); j<h; ++j){
            double o = 3.*j/h+(1.8-8.*(l/2.)*(i-l/2.)/l/l)*((qrand()%65536)/65536.-.5);
            m->cell(i,j).setCellType(o<1 ? *ct1 : o<2 ? *ct2 : *ct3);
        }

//    // TODO Mettre le dossier ailleurs
//    QDir d(QDir::homePath()+"/XML");
//    if(!d.exists()) assert(QDir::home().mkdir("XML"));
//    if(d.mkdir(g->name()))
//        d.cd(g->name());
//    else{
//        //*
//        d.cd(g->name());
//        d.removeRecursively();
//        d.cdUp();
//        d.mkdir(g->name());
//        d.cd(g->name());
//        /*/
//        int k(1);
//        while(!d.mkdir(g->name()+QString::number(++k)));
//        d.cd(g->name()+QString::number(k));
//        //*/
//    }

    //XmlWritter xml(d,*g);

    return g;
    /* AVANT XML */

    /*QXmlSimpleReader xmlReader;
    //Game* g = new Game;
    XmlHandler h(g);
    xmlReader.setErrorHandler(&h);
    xmlReader.setContentHandler(&h);
    xmlReader.setDTDHandler(&h);
    QFile f(fileName);
    QXmlInputSource s(&f);
    if(!xmlReader.parse(&s)){
        QMessageBox::critical(this, "Impossible opening", "Impossible to read the game \n\""+fileName+"\". Syntax error.");
    }
    return g;*/
}




void Editor::on_actionQuit_triggered(){
    close();
}


void Editor::saveGeom(){
    if(!isMaximized() && !isMinimized()){
        Options &options(Options::options());
        options.save(WIN, "Size", size());
        options.save(WIN, "Position", pos());
    }
}

void Editor::resizeEvent(QResizeEvent *re){
    QMainWindow::resizeEvent(re);
    QTimer::singleShot(100, this, SLOT(saveGeom()));
}

void Editor::moveEvent(QMoveEvent *me){
    QMainWindow::moveEvent(me);
    QTimer::singleShot(100, this, SLOT(saveGeom()));

}

void Editor::closeEvent(QCloseEvent *ce){
    saveDefault();
    QMainWindow::closeEvent(ce);
}

void Editor::editMap(){
    tabBar->setCurrentTab(mapsEditor->index());
    mapsEditor->updateGame();
}


void Editor::setCurrentTab(int i){
    stackedWidget->setCurrentIndex(i);
    dynamic_cast<TabWidget*>(stackedWidget->widget(i))->updateGame();
}
