#include "GUI/editor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#if defined(Q_OS_MAC)
    QDir dir(argv[0]); // e.g. appdir/Contents/MacOS/appname
    assert(dir.cdUp());
    assert(dir.cdUp());
    assert(dir.cd("Plugins")); // e.g. appdir/Contents/PlugIns
    QCoreApplication::setLibraryPaths(QStringList(dir.absolutePath()));
#endif
#ifndef Q_OS_WIN32
    QSharedMemory crash(SHARED);
    if(crash.attach())crash.detach();
#endif
#ifdef UNIQUE_WINDOW
    QSharedMemory shared(SHARED);
    bool second = shared.attach();
    if(second){
        partage.lock();
        char *d = (char*)shared.data();
        while(d[0]){
            partage.unlock();
            QThread::msleep(100);
            partage.lock();
            d = (char*)shared.data();
        }
        d[0] = 42;
        d[1] = argc;
        int k = argc + 2;
        for(int i(0); i<argc; ++i){
            d[i+2] = k;
            for(int j(0); argv[i][j]; d[k++] = argv[i][j++]);
            d[k++] = 0;
        }
        shared.unlock();
        return 42;
    }
    shared.create(65536);
    shared.lock();
    char * d = (char*) shared.data();
    d[0] = 0;
    shared.unlock();
#endif

    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("MkRPG");
    QCoreApplication::setOrganizationDomain("MkRPG.bp");
    QCoreApplication::setApplicationName("RPGEditor");
    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&translator);

    QStringList args;
    for(int i(1); i<argc; ++i)
        args.append(QString(argv[i]));
    Editor w(args);
    w.show();
    int r = a.exec();
#ifdef UNIQUE_WINDOW
    shared.unlock();
#endif
    return r;
}
