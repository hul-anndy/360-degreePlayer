#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include "ui_mainpage.h"

class MainPage : public QWidget
{
	Q_OBJECT

public:
	MainPage(QWidget *parent = 0);
	~MainPage();

private:
	Ui::MainPage ui;
};

#endif // MAINPAGE_H
