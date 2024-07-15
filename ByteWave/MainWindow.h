#pragma once

#include <QMainWindow>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/QMenuBar.h>
#include <QtWidgets/qlabel.h>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	void HandlePushButton();
	void HandlePushButtonReleased();
	void setLabel1Text(int);

private:
	QPushButton *upButton;
	QLabel* label1;
	QMenuBar *mainMenuBar;
	QMenu* fileMenu;
	QMenu* saveAsMenu;
	const int minWidth = 600;
	const int minHeight = 400;
	int num = 0;
};
