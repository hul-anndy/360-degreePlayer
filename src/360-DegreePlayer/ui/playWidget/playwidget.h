#ifndef PLAYWIDGET_H
#define PLAYWIDGET_H

#include <QWidget>
#include "ui_playwidget.h"
#include "shared/mediaPlayer/qtavplayer.h"
#include "shared/mediaPlayer/qtavvideorenderer.h"

class PlayWidget : public QWidget
{
	Q_OBJECT

public:
	PlayWidget(QWidget *parent = 0);
	~PlayWidget();

private:
	Ui::PlayWidget ui;
	QtAVPlayer* m_pPlayer;
	QtAVVideoRenderer* m_pRender;
};

#endif // PLAYWIDGET_H
