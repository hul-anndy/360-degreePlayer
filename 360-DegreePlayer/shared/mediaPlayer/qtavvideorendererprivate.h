#ifndef QTAVVIDEORENDERERPRIVATE_H
#define QTAVVIDEORENDERERPRIVATE_H

#include <QtAV/private/VideoRenderer_p.h>
#include <atomic>

class QtAVVideoRendererPrivate : public QtAV::VideoRendererPrivate
{
public:
	QtAVVideoRendererPrivate();
	virtual ~QtAVVideoRendererPrivate();

	std::atomic_bool m_bIsFrameChanged;
private:
	
};

#endif // QTAVVIDEORENDERERPRIVATE_H
