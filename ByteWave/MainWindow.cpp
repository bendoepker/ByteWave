#include "MainWindow.h"
#include "Audio/portaudio.h"
#include <string>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	this->setMinimumSize(QSize(minWidth, minHeight));

	upButton = new QPushButton("Next Device", this);
	upButton->setGeometry(QRect(QPoint(100, 100), QPoint(200, 150)));
	connect(upButton, &QPushButton::pressed, this, &MainWindow::HandlePushButton);
	connect(upButton, &QPushButton::released, this, &MainWindow::HandlePushButtonReleased);

	mainMenuBar = new QMenuBar(this);
	fileMenu = mainMenuBar->addMenu("File");
	saveAsMenu = fileMenu->addMenu("Save As");

	label1 = new QLabel("Hello", this);
	label1->setGeometry(QRect(QPoint(500, 500), QPoint(800, 800)));
}

MainWindow::~MainWindow()
{}

void MainWindow::setLabel1Text(int text)
{
	label1->setText(QVariant(text).toString());
}

/*
	!! THIS CODE IS ERROR PRONE WHEN DEVICES ARE NOT PRESENT !!
	!!                      REMOVE ASAP                      !!
*/
void MainWindow::HandlePushButton()
{
	const PaDeviceInfo *device = Pa_GetDeviceInfo(PaDeviceIndex(num));

	label1->setText(QVariant(device->name).toString() + " " + QVariant(Pa_GetDeviceCount()).toString() + " " + QVariant(num).toString());

	num < Pa_GetDeviceCount() - 1 ? num++ : num = 0;
}

void MainWindow::HandlePushButtonReleased()
{
}