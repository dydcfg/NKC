#pragma once
#include <QtWidgets/QDialog> 
#include "Base.h"

class NKCSolution;

class StatusDlg : public QDialog
{
    Q_OBJECT 

public: 
    StatusDlg(NKCSolution *parent = 0); 
    ~StatusDlg(); 

    void show();
    void update();
protected:
    void closeEvent(QCloseEvent *event);
    void paintEvent( QPaintEvent*event);

    void mouseMoveEvent( QMouseEvent* event );
    void mousePressEvent(QMouseEvent *event); 
    void mouseReleaseEvent(QMouseEvent *event); 

    void keyPressEvent(QKeyEvent *keyevent);


private:
    NKCSolution* nsParent;
    int width,height;

    QPixmap* statusPic;
    QPixmap* optionPic;
    QPixmap* backgroundPic;

    QPixmap* statusDisablePic;

    QPixmap* statusNormalPicMid;
    QPixmap* statusNormalPicLeft;
    QPixmap* statusNormalPicRight;
    QPixmap* statusNormalPicNod;

    //QPixmap* statusBadPicMid;
    //QPixmap* statusBadPicLeft;
    //QPixmap* statusBadPicRight;
    //QPixmap* statusBadPicNod;

    QPixmap* statusTerriblePicMid;
    QPixmap* statusTerriblePicLeft;
    QPixmap* statusTerriblePicRight;
    QPixmap* statusTerriblePicNod;

    QPixmap* optionStartPic;
    QPixmap* optionStartTouchPic;
    QPixmap* optionStartPushPic;
    QPixmap* optionStopPic;
    QPixmap* optionStopTouchPic;
    QPixmap* optionStopPushPic;
    QPixmap* backgroundBluePic;

private:
    QPoint dragPressPos;
    bool isMouseDown;
    int optionButtionx, optionButtiony, optionButtionw, optionButtionh;
    int statusPicx, statusPicy,statusPicw,statusPich;
    int dialogX,dialogY;


    bool checkOptionButton(QPoint point);
};