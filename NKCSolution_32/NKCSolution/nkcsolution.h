#ifndef NKCSOLUTION_H
#define NKCSOLUTION_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets\qsystemtrayicon.h>
#include "Base.h"
#include "ui_nkcsolution.h"


class QMenu;
class ControlPanelDlg;
class ControlThread;
class StatusDlg;
class QAction;


class NKCSolution : public QMainWindow
{
	Q_OBJECT

public:
	NKCSolution(QWidget *parent = 0);
	~NKCSolution();

signals: 
	void updatemessage();
    void nocameramessage();
    void failmessage();

	public slots: 
		void update();
        void shownocamera();
        void showfaildialog();
private:

	void showNormal();
	void showMessage();
    void voiceAlert();


	private slots: 
		void iconActivated(QSystemTrayIcon::ActivationReason reason);
		void showPanel();
		void chickShowStatus();
protected:

	void closeEvent(QCloseEvent *event);

private:
	Ui::NKCSolutionClass ui;

	QSystemTrayIcon *trayIcon;
	QMenu           *trayIconMenu;
	ControlPanelDlg *controlPanelDlg;
	StatusDlg       *statusDlg;

	QAction         *quitAction;
	QAction         *optionAction;
	QAction         *showStatusAction;
	ControlThread   *controlThread;
private:
	void             showStatusDlg();
};

#endif // NKCSOLUTION_H
