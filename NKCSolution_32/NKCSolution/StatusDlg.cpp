#include "StatusDlg.h"
#include <QtGui\qevent.h>
#include <QtWidgets\qapplication.h>
#include <QtWidgets\qdesktopwidget.h>
#include <QtGui\qpainter.h>
#include "nkcsolution.h"
#include "windows.h"

StatusDlg::StatusDlg(NKCSolution *parent) 
{ 
    nsParent = parent;
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::Popup|Qt::Tool);
    int scnwid = QApplication::desktop()->width();
    int scnhei = QApplication::desktop()->height();

    width = 200;
    height = 100;
    optionButtionw = 44;
    optionButtionh = 44;
    optionButtionx = 128;
    optionButtiony = (height - optionButtionh ) / 2;

    statusPicw = 128;
    statusPich = 128;
    statusPicx = 10;
    statusPicy = (height - statusPich ) / 2;

    resize(width, height);
    dialogX = scnwid - width - 100;
    dialogY = 50;
    move(dialogX, dialogY);

    isMouseDown = false;

    statusDisablePic       = new QPixmap(":/NKCSolution/StatusDisable");

    statusNormalPicMid     = new QPixmap(":/NKCSolution/GoodStatusNormal");
    statusNormalPicLeft    = new QPixmap(":/NKCSolution/GoodHeadTurnLeft");
    statusNormalPicRight   = new QPixmap(":/NKCSolution/GoodHeadTurnRight");
    statusNormalPicNod     = new QPixmap(":/NKCSolution/GoodHeadNod");

    //statusBadPicMid        = new QPixmap(":/NKCSolution/StatusNormal");
    //statusBadPicLeft       = new QPixmap(":/NKCSolution/HeadTurnLeft");
    //statusBadPicRight      = new QPixmap(":/NKCSolution/HeadTurnRight");
    //statusBadPicNod        = new QPixmap(":/NKCSolution/HeadNod");

    statusTerriblePicMid   = new QPixmap(":/NKCSolution/TerribleStatusNormal");
    statusTerriblePicLeft  = new QPixmap(":/NKCSolution/TerribleHeadTurnLeft");
    statusTerriblePicRight = new QPixmap(":/NKCSolution/TerribleHeadTurnRight");
    statusTerriblePicNod   = new QPixmap(":/NKCSolution/TerribleHeadNod");

    optionStartPic      = new QPixmap(":/NKCSolution/Start");
    optionStartTouchPic = new QPixmap(":/NKCSolution/StartTouch");
    optionStartPushPic  = new QPixmap(":/NKCSolution/StartPush");
    optionStopPic       = new QPixmap(":/NKCSolution/Stop");
    optionStopTouchPic  = new QPixmap(":/NKCSolution/StopTouch");
    optionStopPushPic   = new QPixmap(":/NKCSolution/StopPush");

    backgroundBluePic   = new QPixmap(":/NKCSolution/Background");

    setAttribute(Qt::WA_TranslucentBackground, true);
    setMouseTracking(true);;

    update();
}


StatusDlg::~StatusDlg(void)
{
    delete statusNormalPicMid;
    delete statusNormalPicLeft;
    delete statusNormalPicRight;
    delete statusNormalPicNod;

    //delete statusBadPicMid;
    //delete statusBadPicLeft;
    //delete statusBadPicRight;
    //delete statusBadPicNod;

    delete statusTerriblePicMid;
    delete statusTerriblePicLeft;
    delete statusTerriblePicRight;
    delete statusTerriblePicNod;

    delete statusDisablePic;
    delete optionStartPic;
    delete optionStartTouchPic;
    delete optionStartPushPic;
    delete optionStopPic;
    delete optionStopTouchPic;
    delete optionStopPushPic;
    delete backgroundBluePic;
}

void StatusDlg::update()
{
    QDialog::update();
    if (appState.isRunning)
    {
        optionPic = optionStopPic; 
    }
    else
    {
        optionPic = optionStartPic; 
    }
    switch(appState.curAction)
    {
    case NOTRACKING:
        statusPic = statusDisablePic; 
        break;

    case LEFT_TURN:
        if (appState.curSitState == STAY_GOOD)
            statusPic = statusNormalPicLeft;
        if (appState.curSitState == STAY_TERRIBLE)
            statusPic = statusTerriblePicLeft;
        break;

    case RIGHT_TURN:
        if (appState.curSitState == STAY_GOOD)
            statusPic = statusNormalPicRight;
        if (appState.curSitState == STAY_TERRIBLE)
            statusPic = statusTerriblePicRight;
        break;

    case STAY_MID:
        if (appState.curSitState == STAY_GOOD)
            statusPic = statusNormalPicMid;
        if (appState.curSitState == STAY_TERRIBLE)
            statusPic = statusTerriblePicMid;
        break;

    case NOD_HEAD:
        if (appState.curSitState == STAY_GOOD)
            statusPic = statusNormalPicNod;
        if (appState.curSitState == STAY_TERRIBLE)
            statusPic = statusTerriblePicNod;
        break;

    default:
        break;
    }
    backgroundPic = backgroundBluePic;

    repaint();
}
void StatusDlg::show()
{
    QDialog::show();
    update();
}
void StatusDlg::paintEvent( QPaintEvent*event) 
{ 
    QPainter painter(this); 

    painter.drawPixmap(0, 0, width, height, *backgroundPic); 
    painter.drawPixmap(optionButtionx,optionButtiony,optionButtionw,optionButtionh, *optionPic); 
    if (appState.isRunning)
    {
        painter.drawPixmap(statusPicx, statusPicy,statusPicw,statusPich, *statusPic); 
    }
} 
void StatusDlg::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}


void StatusDlg::mouseMoveEvent( QMouseEvent* event )
{ 
    if (checkOptionButton(event->pos()))
    {
        if (appState.isRunning)
        {
            optionPic = optionStopTouchPic; 
        }
        else
        {
            optionPic = optionStartTouchPic; 
        }
    }
    else
    {
        if (appState.isRunning)
        {
            optionPic = optionStopPic; 
        }
        else
        {
            optionPic = optionStartPic; 
        }
    }
    

    if (isMouseDown) {
        QPoint mov = event->pos() - dragPressPos;
        int distance = mov.manhattanLength();
        if (distance > QApplication::startDragDistance()) {
            dialogX += mov.x();
            dialogY += mov.y();
            move(dialogX,dialogY);
        }
    }
    
    repaint();
} 

void StatusDlg::mousePressEvent(QMouseEvent *event) 
{ 
    dragPressPos = event->pos();
    isMouseDown = true;
    if (checkOptionButton(event->pos()))
    {
        if (appState.isRunning)
        {
            optionPic = optionStopPushPic; 
        }
        else
        {
            optionPic = optionStartPushPic;      
        } 
        repaint();
    }
} 

void StatusDlg::mouseReleaseEvent(QMouseEvent *event) 
{ 
    isMouseDown = false;
    if (checkOptionButton(event->pos()))
    {
        appState.isRunning = !appState.isRunning;
        nsParent->update();
    }
} 

void StatusDlg::keyPressEvent(QKeyEvent *keyevent)
{
}

bool StatusDlg::checkOptionButton(QPoint point)
{
    if (point.x() > optionButtionx && 
        point.x() < optionButtionx + optionButtionw && 
        point.y() > optionButtiony && 
        point.y() < optionButtiony + optionButtionh )
    {
        return true;
    }
    return false;
}