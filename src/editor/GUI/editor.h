#ifndef EDITOR_H
#define EDITOR_H

#include <QtWidgets>
#include "ui_editor.h"
#include "options.h"
#include "newgame.h"
#include "Game/xmlhandler.h"
#include "Tabs/welcome.h"
#include "Game/xmlwritter.h"
#include "Tabs/worldtab.h"
#include "Tabs/MapTab/maptab.h"
#include "Tabs/objecttab.h"
#include "Tabs/actiontab.h"

/*!
 * \file editor.h
 * \brief Definition of the Editor class.
 */

/*!
 * \brief The constant SHARED is used to open
 * shared memory between different instances
 * of the application
 */
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


/*!
 * \brief The Editor class is the main window of the Editor.
 *
 * It is composed of tabs that offer editing facilities.
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
    void on_actionExport_triggered();


private slots:
    void saveGeom();
    void editMap();

    void setCurrentTab(int i);

private:
    void addTab(const QString& n, const QPixmap& p, TabWidget *w);


    void newGame(QString name, QString dir, bool createFolder);
    Game* open(QString fileName);


    void loadDefault();
    void saveDefault();
    void resizeEvent(QResizeEvent *re);
    void moveEvent(QMoveEvent *me);
    void closeEvent(QCloseEvent *ce);

    WorldTab *worldEditor;
    MapsTab *mapsEditor;
    ObjectTab *objectEditor;
    ActionTab *actionEditor;


    Game *currentGame;
};

/* Template de jeu
 *
 * -> ouvrir une page spécifique pour ajuster plein d'option à remplir dans un template.
 *
 */


#endif // EDITOR_H
