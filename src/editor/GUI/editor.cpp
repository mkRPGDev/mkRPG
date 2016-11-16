#include "editor.h"

Editor::Editor(QStringList args, QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    QPalette p(tabBar->palette());
    p.setColor(QPalette::Window, p.color(QPalette::Mid));
    tabBar->setPalette(p);
    hidden->setHidden(true);
    connect(tabBar, SIGNAL(currentTabChanged(int)), stackedWidget, SLOT(setCurrentIndex(int)));

    worldEditor = new WorldEditor;
    mapsEditor = new MapsEditor;
    objectEditor = new ObjectEditor;

    addTab(tr("Welcome"), QPixmap(":Icons/main.png"), new Welcome);
    addTab(tr("Game"), QPixmap(":Icons/main.png"), worldEditor);
    addTab(tr("Maps"), QPixmap(":Icons/main.png"), mapsEditor);
    addTab(tr("Objects"), QPixmap(":Icons/main.png"), objectEditor);

    loadDefault();

    tabBar->setTabsEnabled(false);

    //qDebug() << args.length();

    Game * g = open("f");



    mapsEditor->setGame(g);
    worldEditor->setGame(g);
    objectEditor->setGame(g);

}


void Editor::addTab(const QString &n, const QPixmap &p, QWidget *w){
    stackedWidget->addWidget(w);
    tabBar->addTabAcces(n,p);

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
    QString f(QFileDialog::getOpenFileName(this, "Open a game project", QDir::homePath(), "Game file *game"));
    if(f != "")
        mapsEditor->setGame(open(f));
}

void Editor::on_actionRolePlayGame_triggered(){
    NewGame g(this);
    if(g.exec())
        newGame(g.name(), g.folder(), g.createFolder());
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


Game* Editor::open(QString fileName){ // NOTE : temporaire
    Game* g = new Game();
    Map *m = new Map(g, g->world());
    g->world()->addMap(m);
    g->setCurrentMap(m);
    m->setName("Paysage bucolique");
    Image *im;
    CellType *ct1, *ct2, *ct3;


    im = new Image(g, g, ":/Icons/herbe.png");
    g->addImage(im);
    ct1 = new CellType(g, g->world());
    ct1->setName("Herbe");
    ct1->setImage(im);
    g->world()->addCellType(ct1);
    m->cell(10,10).setCellType(ct1);
    im = new Image(g, g, ":/Icons/glace.png");
    g->addImage(im);
    ct2 = new CellType(g, g->world());
    ct2->setName("Glace");
    ct2->setImage(im);
    g->world()->addCellType(ct2);
    m->cell(11,11).setCellType(ct2);
    im = new Image(g, g, ":/Icons/mer.png");
    g->addImage(im);
    ct3 = new CellType(g, g->world());
    ct3->setName("Mer");
    ct3->setImage(im);
    g->world()->addCellType(ct3);
    int l = m->width();
    int h = m->height();
    for(int i(0); i<l; ++i)
        for(int j(0); j<h; ++j){
            double o = 3.*j/h+(1.8-8.*(i-l/2.)*(i-l/2.)/l/l)*((qrand()%65536)/65536.-.5);
            m->cell(i,j).setCellType(o<1 ? ct1 : o<2 ? ct2 : ct3);
        }
    tabBar->setTabsEnabled(true);

    m = new Map(g, g->world());
    m->setName("Le grand large");
    g->world()->addMap(m);
    l = m->width();
    h = m->height();
    for(int i(0); i<l; ++i)
        for(int j(0); j<h; ++j){
            double o = 3.*j/h+(1.8-8.*(l/2.)*(i-l/2.)/l/l)*((qrand()%65536)/65536.-.5);
            m->cell(i,j).setCellType(o<1 ? ct1 : o<2 ? ct2 : ct3);
        }
    tabBar->setTabsEnabled(true);
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
