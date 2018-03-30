#ifndef PANOPLAYER_H
#define PANOPLAYER_H

#include <QtWidgets/QMainWindow>
#include "ui_panoplayer.h"

class PanoPlayer : public QMainWindow
{
	Q_OBJECT

public:
	PanoPlayer(QWidget *parent = 0);
	~PanoPlayer();

private:
	Ui::PanoPlayerClass ui;
};

#endif // PANOPLAYER_H
