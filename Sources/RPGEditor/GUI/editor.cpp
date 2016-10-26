#include "editor.h"

Editor::Editor(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    QPalette p(tabs->palette());
    p.setColor(QPalette::Window, p.color(QPalette::Shadow));
    tabs->setPalette(p);
    hidden->setHidden(true);
    mapsEditor = new MapsEditor;
    stackedWidget->addWidget(mapsEditor);
    stackedWidget->setCurrentIndex(0);
}

void Editor::on_actionQuit_triggered(){
    close();
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


Game* Editor::open(QString fileName){
    Game* g = new Game();
    g->world()->addMap(new Map(g));
    return g;
    /* AVANT XML */

    QXmlSimpleReader xmlReader;
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
    return g;
}
