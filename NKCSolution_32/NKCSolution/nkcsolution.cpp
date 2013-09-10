#include "nkcsolution.h"
#include "ControlPanelDlg.h"
#include "StatusDlg.h"
#include <QtWidgets\qmessagebox.h>
#include "ControlThread.h"

NKCSolution::NKCSolution(QWidget *parent)
    : QMainWindow(parent)
{
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("NKC"),
            QObject::tr("NKC couldn't detect any system tray on this system."));
        return;
    }

    appState.init();

    ui.setupUi(this);
    controlThread = new ControlThread(this);
    controlThread->CreateWorkerThread();

    controlPanelDlg = new ControlPanelDlg(this); 
    statusDlg = new StatusDlg(this);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/NKCSolution/MainIcon"));
    trayIcon->setToolTip(tr("NKC"));

    optionAction = new QAction(tr("Options..."), this);
    optionAction->setStatusTip(tr("Setting the Program")); 
    optionAction->setIcon(QIcon(":/NKCSolution/OptionsIcon")); 
    connect(optionAction, SIGNAL(triggered()), this, SLOT(showPanel())); 

    quitAction = new QAction(tr("Exits"), this);
    quitAction->setStatusTip(tr("Exit the program.")); 
    quitAction->setIcon(QIcon(":/NKCSolution/ExitIcon")); 
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close())); 

    showStatusAction = new QAction(tr("Show Status"), this);
    showStatusAction->setStatusTip(tr("Show status dialog on the window")); 
    showStatusAction->setCheckable(true);
    showStatusAction->setChecked(appState.isShowingStatus);
    connect(showStatusAction, SIGNAL(triggered()), this, SLOT(chickShowStatus())); 

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(optionAction);
    trayIconMenu->addAction(showStatusAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
        this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    connect(this, SIGNAL(updatemessage()), this, SLOT(update()));
    connect(this, SIGNAL(nocameramessage()), this, SLOT(shownocamera()));
    connect(this, SIGNAL(failmessage()), this, SLOT(showfaildialog()));

    trayIcon->show();

    update();
}

NKCSolution::~NKCSolution()
{
    controlThread->DestroyThread();
    delete controlThread;
    delete controlPanelDlg;
    delete trayIcon;
    delete trayIconMenu;
    delete quitAction;
    delete optionAction;
    delete showStatusAction;
    delete statusDlg;
}

void NKCSolution::shownocamera()
{   
    QMessageBox::warning(NULL,tr("Warning!"),tr("Fail to open the Camera!"));
}

void NKCSolution::showfaildialog()
{   
    QMessageBox::warning(NULL,tr("Warning!"),tr("Implementation Fail!"));
}

void NKCSolution::update()
{
    QMainWindow::update();


    showStatusAction->setChecked(appState.isShowingStatus);
    showStatusDlg();

    if (controlPanelDlg->isVisible()) 
        controlPanelDlg->update();

    if (statusDlg->isVisible()) 
        statusDlg->update();

    showMessage();
    voiceAlert();
}

void NKCSolution::showPanel()
{
    controlPanelDlg->show();
}

void NKCSolution::chickShowStatus()
{
    appState.isShowingStatus = showStatusAction->isChecked();
    update();
}

void NKCSolution::showStatusDlg()
{
    if (appState.isShowingStatus)
    {
        statusDlg->show();
    }
    else
    {
        statusDlg->hide();
    }
}

void NKCSolution::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        showNormal();
        break;
    case QSystemTrayIcon::MiddleClick:
        showMessage();
        break;
    default:
        ;
    }
}

void NKCSolution::showNormal()
{
    if (controlPanelDlg->isVisible())
    {
        controlPanelDlg->hide();
    }
    else
    {
        controlPanelDlg->show();
    }
}

void NKCSolution::voiceAlert()
{
    if (!appState.isSoundOn) return;

    switch(appState.curSitState)
    {
    case STAY_TERRIBLE:
        appState.soundPlayer.play(0);
        return;
    }

    switch(appState.curAction)
    {
    case LEFT_TURN:
        appState.soundPlayer.play(2);
        return;
    case RIGHT_TURN:
        appState.soundPlayer.play(3);
        return;
    case NOD_HEAD:
        appState.soundPlayer.play(4);
        return;
    }

    switch(appState.curMessage)
    {
    case NO_ACTION_WARNING:
        appState.soundPlayer.play(5);
        return;
    case LANG_TIME_USING:
        appState.soundPlayer.play(6);
        return;
    }
}

void NKCSolution::showMessage()
{
    static bool isWorking = true;
    if (!appState.isWarning || !isWorking) return;

    isWorking = false;
    switch(appState.curMessage)
    {
    case NO_ACTION_WARNING:
        if (appState.warningState.isWarnLangTimeStay) 
        {
            QMessageBox::warning(NULL,tr("Warning!"),tr("You haven't moved your neck for a long time!"));
        }
        break;
    case LANG_TIME_USING:
        if (appState.warningState.isWarnLangTimeUsing) 
        {
            QMessageBox::warning(NULL,tr("Warning!"),tr("You have used the computer for a long time!"));
        }
        break;
    }
    isWorking = true;
}

void NKCSolution::closeEvent(QCloseEvent *event)
{
    QApplication::quit();
}