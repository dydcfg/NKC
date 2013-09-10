#pragma once
#include <qstring.h>
#include <map>
#include <QtMultimedia\qmediaplayer.h>
#include <QtMultimedia\qmediaplaylist.h>
#include "../NKCAlgorithm/ActionDetector.h"

#ifdef _DEBUG
#include <fstream>
#endif

struct Shortcut
{
    bool  fvalid,svalid;
    int ffirst,fsecond,fthird;
    int sfirst,ssecond,sthird;

    Shortcut():fvalid(false),svalid(false),
        ffirst(0),fsecond(0),fthird(0),
        sfirst(0),ssecond(0),sthird(0){}
};

struct ShortcutOption
{
    static const int ActionNum = 3;
    QString name;
    Shortcut shortcuts[ActionNum];
    ShortcutOption(QString str = ""):name(str){}
};

struct SoundState
{
    bool isPlayControlSound;
    bool isPlayWarningSound;
    bool isPlaySitSound;
    SoundState():isPlayControlSound(true),isPlayWarningSound(true),isPlaySitSound(true){}
};

struct WarningState
{
    bool isWarnLangTimeUsing;
    bool isWarnLangTimeStay;
    WarningState():isWarnLangTimeUsing(true),isWarnLangTimeStay(true){}
};

struct SoundPlayer
{
public:
    SoundPlayer():player(NULL)
    {
        player = new QMediaPlayer;
    };
    ~SoundPlayer(){
        delete player;
    };

    void init();
    void play(unsigned int num)
    {
        if (player == NULL || num >= list.size()) return;
        player->setMedia(list[num]);
        player->play();
    }
private:
    QMediaPlayer* player;
    std::vector<QUrl> list;
};

struct AppState
{
    bool isRunning;
    bool hasShortcutOption;
    bool isShowingStatus;
    ShortcutOption shortcutOption;
    std::map<int,QString> keyMap;
    Action curAction;
    SitState curSitState;
    ActMessage curMessage;

    bool isWarning;
    bool isSoundOn;
    SoundState soundState;
    WarningState warningState;
    SoundPlayer  soundPlayer;

    void init();

    void doShortcut(Action curAction);

#ifdef _DEBUG
    std::ofstream dbgFile;
#endif
};


extern AppState appState;