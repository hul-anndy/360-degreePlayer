#include "qtavplayer.h"

#include <QFile>
#include<QtAV/AVError.h>
#include <QDebug>
#include <Qtimer>
#include <QVariant>
#include <QHash>

static const int INT_RESULT_ERROR = -1;
static const float FLOAT_RESULT_ERROR = -1.0f;

const int OPERATION_DELAY = 50;

QtAVPlayer::QtAVPlayer(QObject *parent)
	: QObject(parent)
	, m_nPosition(INT_RESULT_ERROR)
	, m_player(this)
	, m_state(QtAV::AVPlayer::StoppedState)
{
	m_bIgnorePositionChanged = false;
	setRepeat(false);
	m_player.setSeekType(QtAV::AnyFrameSeek);
	connect(&m_player, &QtAV::AVPlayer::positionChanged, this, [this](qint64 position) {
		qDebug() << "AVPlayer::position: " << position;
		if (m_bIgnorePositionChanged)
		{
			return;
		}

		if (m_nPosition != position && m_player.state() != QtAV::AVPlayer::StoppedState)
		{
			m_nPosition = position;
			float f_position = getFloatPosition(position);
			emit positionChanged(f_position);
		}
	
	});
	connect(&m_player, &QtAV::AVPlayer::durationChanged, this, [this](qint64 duration) {
		emit durationChanged(duration);
	});
	connect(&m_player, SIGNAL(stateChanged(QtAV::AVPlayer::State)), this, SIGNAL(stateChanged(QtAV::AVPlayer::State)));
	QObject::connect(&m_player, &QtAV::AVPlayer::stateChanged, [this](QtAV::AVPlayer::State state) 
	{
		qDebug() << "state changed: " << state;
		switch (state)
		{
		case QtAV::AVPlayer::StoppedState:
			//on_stopped();
			break;
		case QtAV::AVPlayer::PlayingState:
			on_playing();
			break;
		case QtAV::AVPlayer::PausedState:
			if (m_bIgnorePositionChanged)
			{
				m_bIgnorePositionChanged = false;
			}
			on_paused();
			break;
		}
		m_state = state;
	});
	QObject::connect(&m_player, &QtAV::AVPlayer::mediaStatusChanged, [this](QtAV::MediaStatus state) {
		qDebug() << "state mediaStatus changed: " << state;

		switch (state)
		{
		case QtAV::NoMedia:
			
			break;
		case QtAV::InvalidMedia:
			
			break;
		case QtAV::BufferingMedia:
			
			break;
		case QtAV::BufferedMedia:
			
			break;
		case QtAV::LoadingMedia:
			
			break;
		case QtAV::LoadedMedia:
			break;
		case QtAV::EndOfMedia:
			on_stopped();
			m_bIgnorePositionChanged = false;
			break;
		case QtAV::StalledMedia:
			m_bIgnorePositionChanged = false;
			//on_stopped();
			break;
		default:
			//on_stopped();
			qDebug() << "mediaStatusChanged: default";
			break;
		}
	});
	
	connect(&m_player, SIGNAL(QtAV::AVPlayer::error(QtAV::AVError&)), this, SLOT(on_player_error(QtAV::AVError&)));
}

QtAVPlayer::~QtAVPlayer()
{

}

void QtAVPlayer::setRepeat(const bool is_repeat)
{
	m_player.setRepeat(is_repeat ? -1 : 0);
}

void QtAVPlayer::setRender(QtAV::VideoRenderer * render)
{
	m_player.setRenderer(render);
}

void QtAVPlayer::setIsSuspended(const bool is_suspended)
{
}

void QtAVPlayer::refresh()
{
	
}

void QtAVPlayer::setMedia(const QString &media_path)
{
	if (QFile::exists(media_path))
	{
		m_bIgnorePositionChanged = false;
		m_strPath = media_path;

		m_player.setBufferMode(QtAV::BufferPackets);
		m_player.play(media_path);

	}
}

float QtAVPlayer::position()
{
	return getFloatPosition(m_player.position());
}
float QtAVPlayer::getFloatPosition(const int position)
{
	float result = (m_player.duration() > 0) ? (float)position / m_player.duration() : FLOAT_RESULT_ERROR;
	if (result < 0)
	{
		qDebug() << "result < 0";
	}
	return result;
}

void QtAVPlayer::play()
{
	m_player.play();
}
void QtAVPlayer::pause(const bool is_pause)
{
	m_player.pause(is_pause);
}
void QtAVPlayer::stop()
{
	m_player.stop();
}
void QtAVPlayer::setTime(int time)
{
	m_player.setPosition(time);
}
void QtAVPlayer::setPosition(float pos)
{
	if (m_player.state() == QtAV::AVPlayer::StoppedState)
	{
		m_bIgnorePositionChanged = true;
		m_fNewPos = pos;
		m_player.play();
	}
	else
	{
		m_bIgnorePositionChanged = true;
		m_fNewPos = pos;
		bool test = m_player.isSeekable();
		m_player.seek(pos);
	}
}

void QtAVPlayer::setVolume(int volume)
{
	QtAV::AudioOutput *p_audio = m_player.audio();
	qreal r_volume = volume*0.01;
	if (p_audio)
	{
		p_audio->setVolume(r_volume);
	}
}

void QtAVPlayer::on_player_error(QtAV::AVError& error)
{
	qDebug() << "player error: " << error.string();
}

void QtAVPlayer::on_opening()
{

}
void QtAVPlayer::on_playing()
{
	if (m_state == QtAV::AVPlayer::StoppedState )
	{
		if (m_bIgnorePositionChanged)
		{
			setPosition(m_fNewPos);
			pause(true);
		}
	}
	else
	{
		m_bIgnorePositionChanged = false;
	}
}
void QtAVPlayer::on_paused()
{

}
void QtAVPlayer::on_stopped()
{
	float f_stop_position = getFloatPosition(m_player.stopPosition());
	emit atStopPosition(f_stop_position);
}
void QtAVPlayer::on_ended()
{

}