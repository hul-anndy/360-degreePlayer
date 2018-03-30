#include "playwidget.h"

PlayWidget::PlayWidget(QWidget *parent)
	: QWidget(parent)
	, m_pPlayer(NULL)
{
	ui.setupUi(this);
	m_pPlayer = new QtAVPlayer;
	m_pRender = new QtAVVideoRenderer;
	m_pRender->setReceiver(ui.rectangleOpenGLWidget);
	m_pPlayer->setRender(m_pRender);
	m_pPlayer->setMedia("D:\\test.MP4");
}

PlayWidget::~PlayWidget()
{
	if (m_pPlayer)
	{
		m_pPlayer->setRender(Q_NULLPTR);
		if (m_pPlayer->state() != QtAV::AVPlayer::StoppedState)
		{
			m_pPlayer->stop();
		}
		delete m_pPlayer;
		m_pPlayer = NULL;
	}
	if (m_pRender)
	{
		delete m_pRender;
		m_pRender = Q_NULLPTR;
	}
}

