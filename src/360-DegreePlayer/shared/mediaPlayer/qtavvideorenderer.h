#ifndef QTAVVIDEORENDERER_H
#define QTAVVIDEORENDERER_H

#include <QObject>
#include "QtAV/VideoRenderer.h"
#include "QtAV/private/mkid.h"
#include "qtavvideorendererprivate.h"

QT_FORWARD_DECLARE_CLASS(AbstractOpenGLWidgetYUV)

class QtAVVideoRenderer : public QtAV::VideoRenderer
{

	//DPTR_DECLARE_PRIVATE(QtAVVideoRenderer)
public:
	QtAVVideoRenderer(QWidget *parent = Q_NULLPTR);
	virtual ~QtAVVideoRenderer();

	void setReceiver(AbstractOpenGLWidgetYUV* receiver);

	virtual QtAV::VideoRendererId id() const Q_DECL_OVERRIDE;
	virtual bool isSupported(QtAV::VideoFormat::PixelFormat pixfmt) const Q_DECL_OVERRIDE;

protected:
	virtual bool receiveFrame(const QtAV::VideoFrame& frame) Q_DECL_OVERRIDE;
	virtual void drawFrame() Q_DECL_OVERRIDE;

private:
	QtAV::VideoFrame m_videoFrame;
	bool m_bFrameChanged;
	AbstractOpenGLWidgetYUV* m_pReceiver;
};

#endif // QTAVVIDEORENDERER_H
