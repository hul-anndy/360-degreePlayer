#ifndef QTAVPLAYER_H
#define QTAVPLAYER_H

#include <QObject>
#include <QtAV/QtAV>

#include "qtavvideorenderer.h"

class QtAVPlayer : public QObject
{
	Q_OBJECT

signals :
	void positionChanged(float);
	void durationChanged(qint64);
	void atStopPosition(float);
	void stateChanged(QtAV::AVPlayer::State);

public:
	QtAVPlayer(QObject *parent = Q_NULLPTR);
	virtual ~QtAVPlayer();

	void setIsSuspended(const bool is_suspended);
	void setIgnoremPositionChanged(const bool is_ignore) {
		m_bIgnorePositionChanged = is_ignore;
	};

	void refresh();
	void setMedia(const QString &media_path);

	QtAV::AVPlayer *player() { return &m_player; };
	QtAV::AVPlayer::State state() { return m_player.state(); }
	float position();

	void setRepeat(const bool is_repeat);

	void setRender(QtAV::VideoRenderer* render);

public slots:
	void play();
	void pause(const bool is_pause = true);
	void stop();
	void setTime(int time);
	void setPosition(float pos);

	void setVolume(int volume);

private:
	float getFloatPosition(const int position);

private slots:
	void on_opening();
	void on_playing();
	void on_paused();
	void on_stopped();
	void on_ended();
	void on_player_error(QtAV::AVError&);

private:
	bool m_bIgnorePositionChanged;
	float m_fNewPos;
	float m_fStartLimit;
	QtAV::AVPlayer m_player;
	QtAV::AVPlayer::State m_state;
	//QtAVVideoRenderer m_render;
	QString m_strPath;
	qint64 m_nPosition;
};

#endif // QTAVPLAYER_H
