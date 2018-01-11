#include "panoplayer.h"

PanoPlayer::PanoPlayer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_pPlayer = QSharedPointer<QtAVPlayer>(new QtAVPlayer());
	m_pRender = QSharedPointer<QtAV::VideoOutput>(new QtAV::VideoOutput());
	ui.verticalLayout->addWidget(m_pRender->widget());
	m_pPlayer->setRender(m_pRender.data());
	m_pPlayer->setMedia("D:\\v10_source\\out\\YIVR_Stitch_0001_360_171228204940_360.MP4");
}

PanoPlayer::~PanoPlayer()
{

}
