#include "MainWindow.h"
#include <sailfishapp.h>

QCoreApplication *
createAppObject(int &argc, char **argv)
{
    return SailfishApp::application (argc, argv);
}//createAppObject

MainWindow::MainWindow(QObject *parent)
: QmlMainWindow(parent)
{
    m_view = SailfishApp::createView();
}//MainWindow::MainWindow
