#include "MainWindow.h"
#include "Audio/portaudio.h"
#include <string>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	this->setMinimumSize(QSize(minWidth, minHeight));

	mainMenuBar = new QMenuBar(this);
	fileMenu = mainMenuBar->addMenu("File");
	saveAsMenu = fileMenu->addMenu("Save As");

	deviceInfoTable = new QTableWidget(this);
	FillTable();
	deviceInfoTable->move(QPoint(500, 500));
}

MainWindow::~MainWindow()
{}

void MainWindow::FillTable()
{
	int numDevices = Pa_GetDeviceCount();
	deviceInfoTable->setFixedSize(QSize(500, 500));
	deviceInfoTable->setColumnCount(4);
	deviceInfoTable->setRowCount(numDevices);
	QTableWidgetItem *device = new QTableWidgetItem();
	QStringList HeaderList;
	HeaderList << "Device Name" << "Sample Rate" << "Max Input Channels" << "Max Output Channels";

	deviceInfoTable->setHorizontalHeaderLabels(HeaderList);

	for (int i = 0; i < numDevices; i++){
		QTableWidgetItem* deviceName = new QTableWidgetItem();
		deviceName->setText(QVariant(Pa_GetDeviceInfo(i)->name).toString());
		deviceInfoTable->setItem(i, 0, deviceName);

		QTableWidgetItem* deviceSampleRate = new QTableWidgetItem();
		deviceSampleRate->setText(QVariant(Pa_GetDeviceInfo(i)->defaultSampleRate).toString());
		deviceInfoTable->setItem(i, 1, deviceSampleRate);

		QTableWidgetItem* deviceMaxInput = new QTableWidgetItem();
		deviceMaxInput->setText(QVariant(Pa_GetDeviceInfo(i)->maxInputChannels).toString());
		deviceInfoTable->setItem(i, 2, deviceMaxInput);

		QTableWidgetItem* deviceMaxOutput = new QTableWidgetItem();
		deviceMaxOutput->setText(QVariant(Pa_GetDeviceInfo(i)->maxOutputChannels).toString());
		deviceInfoTable->setItem(i, 3, deviceMaxOutput);

	}

	deviceInfoTable->resizeColumnsToContents();
}