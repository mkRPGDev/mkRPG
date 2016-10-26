#ifndef EDITOR_H
#define EDITOR_H

#include <QtWidgets>
#include "ui_editor.h"
#include "options.h"
#include "newgame.h"
#include "Game/xmlhandler.h"
#include "Tabs/welcome.h"
#include "Tabs/worldeditor.h"
#include "Tabs/mapseditor.h"

const QString SHARED = "SharedFileRPGEditor";

/*
 * Modes :
 * 0. Accueil
 * 1. Vue générale
 * 2. Cartes (édition)
 * 3. Objets (édition)
 * 4. Perso (édition)
 * 5. Ressources (édition)
 *
 * Règles ?
 *
 * k. Aide ?
 *
 */



class Editor : public QMainWindow, private Ui::Editor
{
    Q_OBJECT

public:
    explicit Editor(QStringList args, QWidget *parent = 0);

private slots:
    void on_actionRolePlayGame_triggered();
    void on_actionOpen_triggered();
    void on_actionQuit_triggered();


private slots:
    void saveGeom();

private:
    void addTab(const QString& n, const QPixmap& p, QWidget* w);


    void newGame(QString name, QString dir, bool createFolder);
    Game* open(QString fileName);


    void loadDefault();
    void saveDefault();
    void resizeEvent(QResizeEvent *re);
    void moveEvent(QMoveEvent *me);
    void closeEvent(QCloseEvent *ce);

    WorldEditor *worldEditor;
    MapsEditor *mapsEditor;

};

/* Template de jeu
 *
 * -> ouvrir une page spécifique pour ajuster plein d'option à remplir dans un template.
 *
 */

class BHandler : public QXmlDefaultHandler
{
public:
    bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts){
        qDebug() << "ElemD" << localName << atts.count() << atts.localName(0) << atts.value(0); return true;}
    bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName){qDebug() << "ElemF"  << localName; return true;}

};

#endif // EDITOR_H
