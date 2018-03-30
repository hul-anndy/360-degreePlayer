#ifndef VIDEORENDEROPENGLRACTANGLE_H
#define VIDEORENDEROPENGLRACTANGLE_H

#include <QObject>
#include "qtavvideorenderer.h"
#include "libOpenglPano\openglyuvrectangle.h"

class VideoRenderOpenGLRactangle : public QtAVVideoRenderer, public OpenGLYUVRectangle
{
public:
	VideoRenderOpenGLRactangle(QWidget *parent = Q_NULLPTR);
	virtual ~VideoRenderOpenGLRactangle();

private:
	
};

#endif // VIDEORENDEROPENGLRACTANGLE_H
