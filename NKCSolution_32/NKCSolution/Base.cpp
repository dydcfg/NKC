#include "Base.h"
#include "windows.h"
#include <qobject.h>
#include <QRegExp>

AppState appState;

void AppState::init()
{
    hasShortcutOption = false;
    isRunning = true;
    isSoundOn = true;
    isWarning = true;
    isShowingStatus = true;
    curAction = NOTRACKING;
    curSitState = STAY_GOOD;
    keyMap[0] = QObject::tr("None");
    keyMap[VK_CONTROL] = QObject::tr("Ctrl");
    keyMap[VK_SHIFT]   = QObject::tr("Alt");
    keyMap[VK_MENU]    = QObject::tr("Shift");

    for(int i = 0; i < 26; ++i)
    {
        char name[2];
        name[0] = 'A' + i;
        name[1] = 0;
        keyMap[0x41 + i] = QObject::tr(name);
    }
    for(int i = 0; i < 10; ++i)
    {
        char name[2];
        name[0] = '0' + i;
        name[1] = 0;
        keyMap[0x30 + i] = QObject::tr(name);
    }
    keyMap[VK_SPACE]  = QObject::tr("Space");
    keyMap[VK_TAB]    = QObject::tr("Tab");
    keyMap[VK_RETURN] = QObject::tr("Enter");
    keyMap[VK_ESCAPE] = QObject::tr("Escape");
    keyMap[VK_BACK]   = QObject::tr("Backspace");

    keyMap[VK_UP]    = QObject::tr("^");//¡ü
    keyMap[VK_RIGHT] = QObject::tr("->");//¡ú
    keyMap[VK_DOWN]  = QObject::tr("v");//¡ý
    keyMap[VK_LEFT]  = QObject::tr("<-");//¡û    

    keyMap[VK_ADD]       = QObject::tr("+");
    keyMap[VK_SUBTRACT]  = QObject::tr("-");
    keyMap[VK_MULTIPLY]  = QObject::tr("*");
    keyMap[VK_DIVIDE]    = QObject::tr("/");
    keyMap[VK_DECIMAL]   = QObject::tr(".");

    keyMap[VK_NUMPAD0]    = QObject::tr("NumPad 0");
    keyMap[VK_NUMPAD1]    = QObject::tr("NumPad 1");
    keyMap[VK_NUMPAD2]    = QObject::tr("NumPad 2");
    keyMap[VK_NUMPAD3]    = QObject::tr("NumPad 3");
    keyMap[VK_NUMPAD4]    = QObject::tr("NumPad 4");
    keyMap[VK_NUMPAD5]    = QObject::tr("NumPad 5");
    keyMap[VK_NUMPAD6]    = QObject::tr("NumPad 6");
    keyMap[VK_NUMPAD7]    = QObject::tr("NumPad 7");
    keyMap[VK_NUMPAD8]    = QObject::tr("NumPad 8");
    keyMap[VK_NUMPAD9]    = QObject::tr("NumPad 9");

    keyMap[VK_F1]    = QObject::tr("F1");
    keyMap[VK_F2]    = QObject::tr("F2");
    keyMap[VK_F3]    = QObject::tr("F3");
    keyMap[VK_F4]    = QObject::tr("F4");
    keyMap[VK_F5]    = QObject::tr("F5");
    keyMap[VK_F6]    = QObject::tr("F6");
    keyMap[VK_F7]    = QObject::tr("F7");
    keyMap[VK_F8]    = QObject::tr("F8");
    keyMap[VK_F9]    = QObject::tr("F9");
    keyMap[VK_F10]   = QObject::tr("F10");
    keyMap[VK_F11]   = QObject::tr("F11");
    keyMap[VK_F12]   = QObject::tr("F12");

    soundPlayer.init();
#ifdef _DEBUG
    dbgFile.open("debugFile.txt");
#endif
}

void AppState::doShortcut(Action curAction)
{
    if (!hasShortcutOption || curAction > NOD_HEAD) return;
    //return;
    Shortcut &tcut = shortcutOption.shortcuts[curAction];
    // Simulating a Alt+Tab keystroke

    if (tcut.ffirst != 0) keybd_event(tcut.ffirst, 0, KEYEVENTF_EXTENDEDKEY, 0);
    if (tcut.fsecond != 0) keybd_event(tcut.fsecond, 0, KEYEVENTF_EXTENDEDKEY, 0);
    if (tcut.fthird != 0) keybd_event(tcut.fthird, 0, KEYEVENTF_EXTENDEDKEY, 0);
    if (tcut.fthird != 0) keybd_event(tcut.fthird, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    if (tcut.fsecond != 0) keybd_event(tcut.fsecond, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    if (tcut.ffirst != 0)  keybd_event(tcut.ffirst, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);

    if (tcut.svalid == false) return;
    if (tcut.sfirst != 0) keybd_event(tcut.sfirst, 0, KEYEVENTF_EXTENDEDKEY, 0);
    if (tcut.ssecond != 0) keybd_event(tcut.ssecond, 0, KEYEVENTF_EXTENDEDKEY, 0);
    if (tcut.sthird != 0) keybd_event(tcut.sthird, 0, KEYEVENTF_EXTENDEDKEY, 0);
    if (tcut.sthird != 0) keybd_event(tcut.sthird, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    if (tcut.ssecond != 0) keybd_event(tcut.ssecond, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    if (tcut.sfirst != 0)  keybd_event(tcut.sfirst, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
}

void SoundPlayer::init()
{
    player->setVolume(50);
    list.push_back(QUrl::fromLocalFile("sound/default.wav"));
    list.push_back(QUrl::fromLocalFile("sound/01.wav"));
    list.push_back(QUrl::fromLocalFile("sound/02.wav"));
    list.push_back(QUrl::fromLocalFile("sound/03.wav"));
    list.push_back(QUrl::fromLocalFile("sound/04.wav"));
    list.push_back(QUrl::fromLocalFile("sound/05.wav"));
    list.push_back(QUrl::fromLocalFile("sound/06.wav"));
    list.push_back(QUrl::fromLocalFile("sound/07.wav"));

}