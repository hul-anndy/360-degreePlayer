#ifndef QTAVVIDEORENDERER_H
#define QTAVVIDEORENDERER_H

#include <QObject>
#include "QtAV/VideoRenderer.h"
#include "QtAV/private/mkid.h"
#include "qtavvideorendererprivate.h"

class QtAVVideoRenderer : public QtAV::VideoRenderer
{
public:
	QtAVVideoRenderer(QObject *parent = Q_NULLPTR);

	virtual QtAV::VideoRendererId id() const Q_DECL_OVERRIDE;
	virtual bool isSupported(QtAV::VideoFormat::PixelFormat pixfmt) const Q_DECL_OVERRIDE;

protected:
	virtual bool receiveFrame(const QtAV::VideoFrame& frame) Q_DECL_OVERRIDE;
	virtual void drawFrame() Q_DECL_OVERRIDE;
	QtAVVideoRenderer(QtAVVideoRendererPrivate &d);

private:
	QtAV::VideoFrame m_videoFrame;
	bool m_bFrameChanged;
};

#endif // QTAVVIDEORENDERER_H
