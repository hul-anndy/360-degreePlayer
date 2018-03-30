#ifndef OPENGLYUVRECTANGLE_H
#define OPENGLYUVRECTANGLE_H

#include "abstractopenglwidgetyuv.h"
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram);

class OpenGLYUVRectangle : public AbstractOpenGLWidgetYUV
{
	Q_OBJECT

public:
	explicit OpenGLYUVRectangle(QWidget *parent = Q_NULLPTR);
	virtual ~OpenGLYUVRectangle();

protected:
	virtual void abstractDraw() Q_DECL_OVERRIDE;
	virtual void releaseGlArray() Q_DECL_OVERRIDE;

	virtual QVector<GLfloat> getVertexData() Q_DECL_OVERRIDE;
	virtual QString getVertexShaderString() Q_DECL_OVERRIDE;
	virtual QString getFragmentShaderString() Q_DECL_OVERRIDE;

private:
	int m_nIndexSize;
	int m_nVerticesCount;

	float* myVertices;

	uint16_t *m_pIndecis;
};

#endif // OPENGLYUVRECTANGLE_H
