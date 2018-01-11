#include "qtavvideorenderer.h"

#include <QtAV/QtAV_Global.h>
#include <QDebug>

using namespace QtAV;
VideoRendererId QTAV_RENDERER_ID = mkid::id32base36_6<'Q', 'T', 'A', 'V', 'R', 'D'>::value;

QtAVVideoRenderer::QtAVVideoRenderer(QObject *parent)
	:m_bFrameChanged(false)
{

}

QtAVVideoRenderer::QtAVVideoRenderer(QtAVVideoRendererPrivate &d)
	: VideoRenderer(d)
{
	setPreferredPixelFormat(VideoFormat::Format_YUV420P);
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
		DPTR_D(QtAVVideoRenderer);
		d.video_frame = frame;
		d.m_bIsFrameChanged = true;
		//TODO: render frame
	}
	
	return result;
}
void QtAVVideoRenderer::drawFrame()
{
	if (m_bFrameChanged)
	{
		qDebug() << "draw frame size: " << m_videoFrame.width() << " height: "<< m_videoFrame.height();
		m_bFrameChanged = false;
	}
}