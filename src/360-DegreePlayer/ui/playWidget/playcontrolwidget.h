#ifndef PLAYCONTROLWIDGET_H
#define PLAYCONTROLWIDGET_H

#include <QWidget>
#include "ui_playcontrolwidget.h"

class playControlWidget : public QWidget
{
	Q_OBJECT

public:
	playControlWidget(QWidget *parent = 0);
	~playControlWidget();

private:
	Ui::PlayControlWidget ui;
};

#endif // PLAYCONTROLWIDGET_H
