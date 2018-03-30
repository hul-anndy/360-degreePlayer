#ifndef ABSTRACTOPENGLYUV_H
#define ABSTRACTOPENGLYUV_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <QOpenGLBuffer>
#include <QMutex>
#include <QOpenGLTexture>

#include <atomic>

#include <QtAV/VideoFrame.h>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram);
QT_FORWARD_DECLARE_CLASS(QOpenGLShader);

class AbstractOpenGLWidgetYUV : public QOpenGLWidget, public QOpenGLFunctions
{
	Q_OBJECT

public:
	static const int  PROGRAM_VERTEX_ATTRIBUTE = 0;
	static const int  PROGRAM_TEXCOORD_ATTRIBUTE = 1;
	explicit AbstractOpenGLWidgetYUV(QWidget *parent = Q_NULLPTR);
	virtual ~AbstractOpenGLWidgetYUV();

	void setIsAnimating(const bool is_animating){
		m_bIsAnimating = is_animating;
	}

	void setClearColor(const QColor &color);
	void setEmpty();
	void setIsOutputGlError(const bool is_output_gl_error) { m_bIsOutputGlError = is_output_gl_error; };

public slots:
	void setImage(unsigned char* y, unsigned char* u, unsigned char* v, int width, int height, QMutex* mutex);
	void setImage(const QtAV::VideoFrame* p_video_frame);

protected:
	void initializeGL() Q_DECL_OVERRIDE;
	void initializeYUVTexture();
	void initializeShaderProgram();

	void makeObject();

	void bindTexture();
	void releaseTexture();
	
	void paintGL() Q_DECL_OVERRIDE;
	virtual void resizeGL(int width, int height) Q_DECL_OVERRIDE;

	void outputGlError(const QString& str_tip);


	virtual void releaseGlArray() {};
	virtual void abstractDraw() = 0;
	
	virtual QVector<GLfloat> getVertexData() = 0;
	virtual QString getVertexShaderString() = 0;
	virtual QString getFragmentShaderString() = 0;

protected:
	QOpenGLShader *m_pVShader;
	QOpenGLShader *m_pFShader;
	QOpenGLShaderProgram *m_pProgram;
	GLuint m_matrixId;

	const QtAV::VideoFrame* m_pVideoFrame;

protected:
	void setImageSize(const int width, const int height);
	void bindYUV420Texture();
	void deleteShaderProgram();

private:
	std::atomic_bool m_bIsOutputGlError;
	std::atomic_bool m_bIsAnimating;

	QMutex* m_pParamMutex;
	unsigned char* m_pImageBuffer;
	QMutex m_mutexImageBuffer;

	const unsigned char *plane[3];

	int m_nHeight;
	int m_nWidth;

	QColor m_colorClear;

	GLuint m_textureUniformY;
	GLuint m_textureUniformU;
	GLuint m_textureUniformV;

	QOpenGLTexture m_textureY;
	QOpenGLTexture m_textureU;
	QOpenGLTexture m_textureV;

	QOpenGLBuffer m_glBuffer;
};

#endif // ABSTRACTOPENGLYUV_H
