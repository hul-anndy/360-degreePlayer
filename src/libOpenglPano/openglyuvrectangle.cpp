#include "openglyuvrectangle.h"

#include <QMatrix4x4>
#include <QOpenGLShaderProgram>

OpenGLYUVRectangle::OpenGLYUVRectangle(QWidget *parent)
	: AbstractOpenGLWidgetYUV(parent)
	, myVertices(NULL)
{

}

OpenGLYUVRectangle::~OpenGLYUVRectangle()
{

}

void OpenGLYUVRectangle::abstractDraw()
{
	QMatrix4x4 m;
	m.setToIdentity();

	m_pProgram->setUniformValue(m_matrixId, m);
	outputGlError("m_pProgram->setUniformValue(m_matrixId,m)");

	//enable attribute array
	m_pProgram->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
	outputGlError("m_pProgram->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);");
	m_pProgram->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
	outputGlError("m_pProgram->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);");

	m_pProgram->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
	outputGlError("m_pProgram->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));");
	m_pProgram->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
	outputGlError("m_pProgram->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));");
	//glDrawElements(GL_TRIANGLES, (GLsizei)m_nIndexSize, GL_UNSIGNED_SHORT, (GLvoid*)m_pIndecis);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	//glDrawArrays(GL_TRIANGLES, 0, 4);
	outputGlError("glDrawArrays(GL_TRIANGLE_FAN, 0, 4);");
}

QVector<GLfloat> OpenGLYUVRectangle::getVertexData()
{
	QVector<GLfloat> vertData;
	//rectangle coordinates in GL
	int coords[4][3] =
	{  { -1, +1, 0 },{ -1, -1, 0 }, { +1, -1, 0 } ,{ +1, +1, 0 } };
	for (int i = 0; i < 4; ++i) {
		// vertex position
		vertData.append((GLfloat)coords[i][0]);
		vertData.append((GLfloat)coords[i][1]);
		vertData.append((GLfloat)coords[i][2]);

		// texture coordinate
		vertData.append(i == 2 || i == 3);
		vertData.append(i ==1 || i == 2);
	}

	return vertData;
}
QString OpenGLYUVRectangle::getVertexShaderString()
{
	QString result =
		"attribute highp vec4 vertex;"
		"attribute mediump vec2  texCoord;"
		"varying mediump vec2  texc;"
		"uniform mediump mat4 matrix;"
		"void main(void)"
		"{"
		"	highp vec4 vertex_uz = vertex;"
		"   gl_Position = matrix * vertex_uz;"
		"	texc = texCoord;"
		"}";
	return result;
}
QString OpenGLYUVRectangle::getFragmentShaderString()
{
	QString result =
		"uniform sampler2D texture_y;"
		"uniform sampler2D texture_u;"
		"uniform sampler2D texture_v;"
		"varying mediump vec2 texc;"
		"void main(void)"
		"{"
		"	mediump vec3 yuv;"
		"	highp vec3 rgb;"
		"	highp float f1 = -0.39465;"
		"	highp float f2 = 2.03211;"
		"	highp float f3 = 1.13983;"
		"	highp float f4 = -0.58060;"
		"	yuv.x = 1.164*(texture2D(texture_y, texc).r - 0.0625);"
		"	yuv.y = texture2D(texture_u, texc).r - 0.5;"
		"	yuv.z = texture2D(texture_v, texc).r - 0.5;"
		"	rgb = mat3( 1,       1,         1, \
						0,f1, f2, \
						f3, f4,  0) * yuv;"
		"   gl_FragColor = vec4(rgb, 1);"
		"}";
	return result;
}

void OpenGLYUVRectangle::releaseGlArray()
{
	if (myVertices)
	{
		delete[] myVertices;
		myVertices = NULL;
		m_nVerticesCount = 0;
	}
}