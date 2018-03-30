#include "qtavvideorenderer.h"

#include <QtAV/QtAV_Global.h>
#include <QDebug>
#include "libOpenglPano/abstractopenglwidgetyuv.h"

using namespace QtAV;
VideoRendererId QTAV_RENDERER_ID = mkid::id32base36_6<'Q', 'T', 'A', 'V', 'R', 'D'>::value;

QtAVVideoRenderer::QtAVVideoRenderer(QWidget *parent)
	:m_bFrameChanged(false)
	, m_pReceiver(Q_NULLPTR)
{

}

QtAVVideoRenderer::~QtAVVideoRenderer()
{
}

void QtAVVideoRenderer::setReceiver(AbstractOpenGLWidgetYUV * receiver)
{
	m_pReceiver = receiver;
}

QtAV::VideoRendererId QtAVVideoRenderer::id() const
{
	return QTAV_RENDERER_ID;
}
bool QtAVVideoRenderer::isSupported(VideoFormat::PixelFormat pixfmt) const
{
	//qDebug() << "TestVideoRenderer::isSupported: " << pixfmt;
	return true;
}

bool QtAVVideoRenderer::receiveFrame(const VideoFrame& frame)
{
	bool result = true;
	if (!frame.isValid())
	{
		qDebug() << "frame is not valid";
		result = false;
	}
	else
	{
		//DPTR_D will lead to crash with destruction
		/*DPTR_D(QtAVVideoRenderer);
		d.video_frame = frame;
		d.m_bIsFrameChanged = true;*/
		m_videoFrame = frame;
		if (m_pReceiver)
		{
			m_pReceiver->setImage(&m_videoFrame);
		}
	}
	
	return result;
}
void QtAVVideoRenderer::drawFrame()
{
	//This may useless
	if (m_bFrameChanged)
	{
		qDebug() << "draw frame size: " << m_videoFrame.width() << " height: "<< m_videoFrame.height();
		m_bFrameChanged = false;
	}
}