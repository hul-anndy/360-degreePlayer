#ifndef PANOPLAYER_H
#define PANOPLAYER_H

#include <QtWidgets/QMainWindow>
#include "ui_panoplayer.h"
#include "shared/mediaPlayer/qtavplayer.h"

class PanoPlayer : public QMainWindow
{
	Q_OBJECT

public:
	PanoPlayer(QWidget *parent = 0);
	~PanoPlayer();

private:
	Ui::PanoPlayerClass ui;
	QSharedPointer<QtAVPlayer> m_pPlayer;
	QSharedPointer<QtAV::VideoOutput> m_pRender;
};

#endif // PANOPLAYER_H
