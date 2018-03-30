#include "abstractopenglwidgetyuv.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QDebug>

const int DEFAULT_IMAGE_WIDTH = 1080;
const int DEFAULT_IMAGE_HEIGHT = 2160;

AbstractOpenGLWidgetYUV::AbstractOpenGLWidgetYUV(QWidget *parent)
	: 
	m_bIsOutputGlError(false),
	m_bIsAnimating(false),
	m_pVideoFrame(NULL),
	m_pImageBuffer(NULL),
	m_colorClear(Qt::black),
	m_pFShader(Q_NULLPTR),
	m_pVShader(Q_NULLPTR),
	m_pProgram(0),
	m_textureY(QOpenGLTexture::Target2D),
	m_textureU(QOpenGLTexture::Target2D),
	m_textureV(QOpenGLTexture::Target2D),
	m_textureUniformY(0),
	m_textureUniformU(0),
	m_textureUniformV(0),
	m_nWidth(0),
	m_nHeight(0)
{
	plane[0] = NULL;
}

AbstractOpenGLWidgetYUV::~AbstractOpenGLWidgetYUV()
{
	makeCurrent();
	releaseGlArray();
	deleteShaderProgram();
	m_glBuffer.destroy();
	m_textureY.destroy();
	m_textureU.destroy();
	m_textureV.destroy();
	delete[] m_pImageBuffer;
	m_pImageBuffer = NULL;
	doneCurrent();
}

void AbstractOpenGLWidgetYUV::outputGlError(const QString& str_tip)
{
	if (m_bIsOutputGlError)
	{
		for (GLenum err; (err = glGetError()) != GL_NO_ERROR;)
		{
			qDebug() << "glGetError code: " << err << "  from: "<< str_tip;
		}
	}
}

void AbstractOpenGLWidgetYUV::setClearColor(const QColor &color)
{
	m_colorClear = color;
	update();
}

void AbstractOpenGLWidgetYUV::setEmpty()
{
	m_mutexImageBuffer.lock();
	m_pVideoFrame = NULL;
	plane[0] = NULL;
	m_mutexImageBuffer.unlock();
}

void AbstractOpenGLWidgetYUV::setImage(const QtAV::VideoFrame* p_video_frame)
{
	m_pVideoFrame = p_video_frame;
	plane[0] = Q_NULLPTR;
	plane[1] = Q_NULLPTR;
	plane[2] = Q_NULLPTR;
	if (!m_bIsAnimating)
	{
		update();
	}
}

void AbstractOpenGLWidgetYUV::setImage(unsigned char* y, unsigned char* u, unsigned char* v, int width, int height, QMutex* mutex)
{
	if (height != m_nHeight || width != m_nWidth)
	{
		setImageSize(width, height);
	}

	m_mutexImageBuffer.lock();
	if (plane[0] != y)
	{
		m_pParamMutex = mutex;
		plane[0] = y;
		plane[1] = u;
		plane[2] = v;
	}
	m_mutexImageBuffer.unlock();

	if (!m_bIsAnimating)
	{
		update();
	}
}

void AbstractOpenGLWidgetYUV::setImageSize(const int width, const int height)
{
	if (height != m_nHeight || width != m_nWidth)
	{
		m_nWidth = width;
		m_nHeight = height;
	}
}

void AbstractOpenGLWidgetYUV::deleteShaderProgram()
{
	if (m_pProgram)
	{
		delete m_pProgram;
		m_pProgram = NULL;
	}
	if (m_pVShader)
	{
		delete m_pVShader;
		m_pVShader = NULL;
	}
	if (m_pFShader)
	{
		delete m_pFShader;
		m_pFShader = NULL;
	}
}

void AbstractOpenGLWidgetYUV::initializeShaderProgram()
{
	deleteShaderProgram();
	QString str_vertex_shader = getVertexShaderString();
	qDebug() << "vertex_shader: " << str_vertex_shader;
	m_pVShader = new QOpenGLShader(QOpenGLShader::Vertex,this);
	bool result = m_pVShader->compileSourceCode(str_vertex_shader);
	qDebug() << "vertex_shader compileSourceCode result: " << result;
	qDebug() << "m_pVShader compileSourceCode log: " << m_pVShader->log();
	QString str_fragment_shader = getFragmentShaderString();
	qDebug() << "str_fragment_shader: " << str_fragment_shader;
	m_pFShader = new QOpenGLShader(QOpenGLShader::Fragment, this);
	result = m_pFShader->compileSourceCode(str_fragment_shader);
	qDebug() << "fragment_shader compileSourceCode result: " << result;
	qDebug() << "m_pFShader compileSourceCode log: " << m_pFShader->log();

	m_pProgram = new QOpenGLShaderProgram();
	result = m_pProgram->addShader(m_pVShader);
	qDebug() << "addShader m_pVShader result: " << result;
	result = m_pProgram->addShader(m_pFShader);
	qDebug() << "addShader m_pFShader result: " << result;
	m_pProgram->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
	m_pProgram->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
	result = m_pProgram->link();
	qDebug() << "m_pProgram->link result: " << result;
	result = m_pProgram->bind();
	qDebug() << "m_pProgram->bind result: " << result;
	int int_result;
	int_result = m_pProgram->uniformLocation("texture_y");
	qDebug() << " m_pProgram->uniformLocation(texture_y): " << int_result;
	m_textureUniformY = int_result;
	int_result = m_pProgram->uniformLocation("texture_u");
	qDebug() << " m_pProgram->uniformLocation(texture_u): " << int_result;
	m_textureUniformU = int_result;
	int_result= m_pProgram->uniformLocation("texture_v");
	qDebug() << " m_pProgram->uniformLocation(texture_v): " << int_result;
	m_textureUniformV = int_result;
	int_result = m_pProgram->uniformLocation("matrix");
	outputGlError("m_pProgram->uniformLocation(\"matrix\"); ");
	m_matrixId = int_result;
	m_pProgram->release();
	outputGlError("m_pProgram->release()");
}

void AbstractOpenGLWidgetYUV::initializeGL()
{
	initializeOpenGLFunctions();
	//initialize default image size
	if (m_nWidth ==0 || m_nHeight ==0)
	{
		setImageSize(DEFAULT_IMAGE_WIDTH, DEFAULT_IMAGE_HEIGHT);
	}
	
	makeObject();

	glEnable(GL_CULL_FACE);
	outputGlError("glEnable(GL_CULL_FACE);");
	glDisable(GL_DEPTH_TEST);
	outputGlError("glDisable(GL_DEPTH_TEST);");

	glEnable(GL_BLEND);
	outputGlError("glEnable(GL_BLEND);");
	glBlendFunc(GL_ONE, GL_ONE);
	outputGlError("glBlendFunc(GL_ONE, GL_ONE);");

	initializeShaderProgram();

	initializeYUVTexture();
	//initialize success
}

void AbstractOpenGLWidgetYUV::initializeYUVTexture()
{
	m_textureY.destroy();
	m_textureY.create();
	m_textureY.bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	m_textureY.release();
	outputGlError("m_textureY.release();");

	m_textureU.destroy();
	m_textureU.create();
	m_textureU.bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	m_textureU.release();
	outputGlError("m_textureU.release();");

	m_textureV.destroy();
	m_textureV.create();
	m_textureV.bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	m_textureV.release();
	outputGlError("m_textureV.release();");
}

void AbstractOpenGLWidgetYUV::bindYUV420Texture()
{
	//y data
	glActiveTexture(GL_TEXTURE0);
	outputGlError("glActiveTexture(GL_TEXTURE0);");
	m_textureY.bind();
	outputGlError("m_textureY.bind();");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_nWidth, m_nHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, plane[0]);
	outputGlError("glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_nWidth, m_nHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, plane[0]);");
	//0 means GL_TEXTURE0
	m_pProgram->setUniformValue(m_textureUniformY, 0);
	outputGlError("m_pProgram->setUniformValue(m_textureUniformY, 0);");

	//u data
	glActiveTexture(GL_TEXTURE1);
	outputGlError("glActiveTexture(GL_TEXTURE1);//激活纹理单元GL_TEXTURE1");
	m_textureU.bind();
	outputGlError("m_textureU.bind();");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_nWidth / 2, m_nHeight / 2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, plane[1]);
	outputGlError("glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_nWidth / 2, m_nHeight / 2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, plane[1]);");
	//1 means GL_TEXTURE1
	m_pProgram->setUniformValue(m_textureUniformU, 1);
	outputGlError("m_pProgram->setUniformValue(m_textureUniformU, 1);");

	//v data
	glActiveTexture(GL_TEXTURE2);
	m_textureV.bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_nWidth / 2, m_nHeight / 2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, plane[2]);
	//2 means GL_TEXTURE2
	m_pProgram->setUniformValue(m_textureUniformV, 2);
}

void AbstractOpenGLWidgetYUV::paintGL()
{
	if (plane[0] || (m_pVideoFrame&&m_pVideoFrame->isValid()))
	{
		glClearColor(m_colorClear.redF(), m_colorClear.greenF(), m_colorClear.blueF(), m_colorClear.alphaF());
		outputGlError("glClearColor(m_colorClear.redF(), m_colorClear.greenF(), m_colorClear.blueF(), m_colorClear.alphaF());");
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		outputGlError("glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);");

		if (m_pVideoFrame)
		{
			if (m_pVideoFrame->isValid())
			{
				m_nWidth = m_pVideoFrame->width();
				m_nHeight = m_pVideoFrame->height();
				plane[0] = m_pVideoFrame->bits(0);
				plane[1] = m_pVideoFrame->bits(1);
				plane[2] = m_pVideoFrame->bits(2);
			}
			else
			{
				plane[0] = NULL;
			}
		}

		//m_mutexImageBuffer.lock();
		if (plane[0])
		{
			m_pProgram->bind();
			outputGlError("m_pProgram->bind();");

			bindYUV420Texture();
			m_glBuffer.bind();

			outputGlError("m_glBuffer.bind();");
			abstractDraw();
			outputGlError("abstractDraw();");

			releaseTexture();
			outputGlError("releaseTexture();");
			m_pProgram->disableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
			m_pProgram->disableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
			m_pProgram->release();
			m_glBuffer.release();
		}
		//m_mutexImageBuffer.unlock();
	}
}

void AbstractOpenGLWidgetYUV::bindTexture()
{
	m_textureY.bind();
	m_textureU.bind();
	m_textureV.bind();
}
void AbstractOpenGLWidgetYUV::releaseTexture()
{
	m_textureV.release();
	m_textureU.release();
	m_textureY.release();
}

void AbstractOpenGLWidgetYUV::resizeGL(int width, int height)
{
	int side = qMin(width, height);
	qDebug() << "width - height:" << width << height;
	glViewport(width, height, width, height);
}

void AbstractOpenGLWidgetYUV::makeObject()
{
	m_glBuffer.destroy();
	releaseGlArray();
	//get vertex data from subclass
	QVector<GLfloat> vertData = getVertexData();

	if (m_glBuffer.create())
	{
		if (m_glBuffer.bind())
		{
			m_glBuffer.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
			m_glBuffer.release();
		}
		else
		{
			qDebug() << "buffer bind fail.";
		}
	}
	else
	{
		qDebug() << "buffer create fail.";
	}
}