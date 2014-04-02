#include <QtCore>
#include <QtScript>
#include <QDesktopServices>

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "o2.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
, o2(new O2(this))
{
    ui->setupUi(this);

    QFile f(":/goog_client_secret.json");
    if (!f.open (QFile::ReadOnly)) {
        qWarning("Failed to open :/goog_client_secret.json");
        return;
    }

    connect(o2, SIGNAL(linkedChanged()), this, SLOT(onLinkedChanged()));
    connect(o2, SIGNAL(linkingFailed()), this, SLOT(onLinkingFailed()));
    connect(o2, SIGNAL(linkingSucceeded()), this, SLOT(onLinkingSucceeded()));
    connect(o2, SIGNAL(openBrowser(QUrl)), this, SLOT(onOpenBrowser(QUrl)));
    connect(o2, SIGNAL(closeBrowser()), this, SLOT(onCloseBrowser()));

    QByteArray baData = f.readAll ();
    QString temp = QString("var o = %1;").arg(QString(baData));
    QScriptEngine e;

    e.evaluate (temp);
    temp = e.evaluate ("o.installed.client_id").toString ();
    o2->setClientId (temp);

    e.evaluate (temp);
    temp = e.evaluate ("o.installed.client_secret").toString ();
    o2->setClientSecret (temp);

    o2->setGrantFlow (O2::GrantFlowAuthorizationCode);
    o2->setScope ("https://www.google.com/m8/feeds");

    o2->link ();

    f.close ();
}//MainWindow::MainWindow

MainWindow::~MainWindow()
{
    o2->unlink ();
    delete ui;
}//MainWindow::~MainWindow

void
MainWindow::onLinkedChanged()
{
    qDebug("Link has changed");
}//MainWindow::onLinkedChanged

void
MainWindow::onLinkingFailed()
{
    qDebug("Linking failed");
}//MainWindow::onLinkingFailed

void
MainWindow::onLinkingSucceeded()
{
    qDebug("Linking succeeded");
}//MainWindow::onLinkingSucceeded

void
MainWindow::onOpenBrowser(const QUrl &url)
{
    QString dest = "https://accounts.google.com/o/oauth2/auth" + url.toString();
    QDesktopServices::openUrl (dest);
}//MainWindow::onOpenBrowser

void
MainWindow::onCloseBrowser()
{
    qDebug("Close browser");
}//MainWindow::onCloseBrowser
