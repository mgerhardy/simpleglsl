#pragma once

#include <string>
#include <map>
#include <glm/glm.hpp>
#include "GLSLContext.h"

#define MAX_SHADER_VAR_NAME 128

namespace simpleglsl {

class OpenGLStateHandlerCheckError {
protected:
	const char* _file;
	const int _line;
	const char* _function;

	const char* translateError (GLenum glError) const;

public:
	OpenGLStateHandlerCheckError(const char *file, int line, const char *function);
	~OpenGLStateHandlerCheckError();
};

#ifdef _DEBUG
#define checkError() do {OpenGLStateHandlerCheckError(__FILE__, __LINE__, __PRETTY_FUNCTION__);} while(0)
#else
#define checkError()
#endif

class GLSLContext {
public:
	GLSLContext(
		GLuint (*_glCreateShader)(GLenum type), 
		void (*_glDeleteShader)(GLuint id), 
		void (*_glShaderSource)(GLuint id, GLuint count, const GLchar **sources, GLuint *len),
		void (*_glCompileShader)(GLuint id),
		void (*_glGetShaderiv)(GLuint id, GLenum field, GLint *dest),
		void (*_glGetShaderInfoLog)(GLuint id, GLuint maxlen, GLuint *len, GLchar *dest),
		GLuint (*_glCreateProgram)(void),
		void (*_glDeleteProgram)(GLuint id),
		void (*_glAttachShader)(GLuint prog, GLuint shader),
		void (*_glDetachShader)(GLuint prog, GLuint shader),
		void (*_glLinkProgram)(GLuint id),
		void (*_glUseProgram)(GLuint id),
		void (*_glGetProgramiv)(GLuint id, GLenum field, GLint *dest),
		void (*_glGetActiveUniform)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name),
		void (*_glGetProgramInfoLog)(GLuint id, GLuint maxlen, GLuint *len, GLchar *dest),
		GLint (*_glGetUniformLocation)(GLuint id, const GLchar *name),
		void (*_glUniform1i)(GLint location, GLint i),
		void (*_glUniform2i)(GLint location, GLint i1, GLint i2),
		void (*_glUniform3i)(GLint location, GLint i1, GLint i2, GLint i3),
		void (*_glUniform4i)(GLint location, GLint i1, GLint i2, GLint i3, GLint i4),
		void (*_glUniform1f)(GLint location, GLfloat f),
		void (*_glUniform2f)(GLint location, GLfloat f1, GLfloat f2),
		void (*_glUniform3f)(GLint location, GLfloat f1, GLfloat f2, GLfloat f3),
		void (*_glUniform4f)(GLint location, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4),
		void (*_glUniform1fv)(GLint location, int count, GLfloat *f),
		void (*_glUniform2fv)(GLint location, int count, GLfloat *f),
		void (*_glUniform3fv)(GLint location, int count, GLfloat *f),
		void (*_glUniform4fv)(GLint location, int count, GLfloat *f),
		void (*_glGetActiveAttrib)(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name),
		GLint (*_glGetAttribLocation)(GLuint id, const GLchar *name),
		void (*_glUniformMatrix2fv)(GLint location, int count, GLboolean transpose, GLfloat *v),
		void (*_glUniformMatrix3fv)(GLint location, int count, GLboolean transpose, GLfloat *v),
		void (*_glUniformMatrix4fv)(GLint location, int count, GLboolean transpose, GLfloat *v),
		void (*_glVertexAttrib4f) (GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	) :
		ctx_glCreateShader(_glCreateShader),
		ctx_glDeleteShader(_glDeleteShader),
		ctx_glShaderSource(_glShaderSource),
		ctx_glCompileShader(_glCompileShader),
		ctx_glGetShaderiv(_glGetShaderiv),
		ctx_glGetShaderInfoLog(_glGetShaderInfoLog),
		ctx_glCreateProgram(_glCreateProgram),
		ctx_glDeleteProgram(_glDeleteProgram),
		ctx_glAttachShader(_glAttachShader),
		ctx_glDetachShader(_glDetachShader),
		ctx_glLinkProgram(_glLinkProgram),
		ctx_glUseProgram(_glUseProgram),
		ctx_glGetActiveUniform(_glGetActiveUniform),
		ctx_glGetProgramiv(_glGetProgramiv),
		ctx_glGetProgramInfoLog(_glGetProgramInfoLog),
		ctx_glGetUniformLocation(_glGetUniformLocation),
		ctx_glUniform1i(_glUniform1i),
		ctx_glUniform2i(_glUniform2i),
		ctx_glUniform3i(_glUniform3i),
		ctx_glUniform4i(_glUniform4i),
		ctx_glUniform1f(_glUniform1f),
		ctx_glUniform2f(_glUniform2f),
		ctx_glUniform3f(_glUniform3f),
		ctx_glUniform4f(_glUniform4f),
		ctx_glUniform1fv(_glUniform1fv),
		ctx_glUniform2fv(_glUniform2fv),
		ctx_glUniform3fv(_glUniform3fv),
		ctx_glUniform4fv(_glUniform4fv),
		ctx_glGetActiveAttrib(_glGetActiveAttrib),
		ctx_glGetAttribLocation(_glGetAttribLocation),
		ctx_glUniformMatrix2fv(_glUniformMatrix2fv),
		ctx_glUniformMatrix3fv(_glUniformMatrix3fv),
		ctx_glUniformMatrix4fv(_glUniformMatrix4fv)
	}

	GLuint (*ctx_glCreateShader)(GLenum type);
	void (*ctx_glDeleteShader)(GLuint id);
	void (*ctx_glShaderSource)(GLuint id, GLuint count, const GLchar **sources, GLuint *len);
	void (*ctx_glCompileShader)(GLuint id);
	void (*ctx_glGetShaderiv)(GLuint id, GLenum field, GLint *dest);
	void (*ctx_glGetShaderInfoLog)(GLuint id, GLuint maxlen, GLuint *len, GLchar *dest);
	GLuint (*ctx_glCreateProgram)(void);
	void (*ctx_glDeleteProgram)(GLuint id);
	void (*ctx_glAttachShader)(GLuint prog, GLuint shader);
	void (*ctx_glDetachShader)(GLuint prog, GLuint shader);
	void (*ctx_glLinkProgram)(GLuint id);
	void (*ctx_glUseProgram)(GLuint id);
	void (*ctx_glGetProgramiv)(GLuint id, GLenum field, GLint *dest);
	void (*ctx_glGetActiveUniform)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
	void (*ctx_glGetProgramInfoLog)(GLuint id, GLuint maxlen, GLuint *len, GLchar *dest);
	GLint (*ctx_glGetUniformLocation)(GLuint id, const GLchar *name);
	void (*ctx_glUniform1i)(GLint location, GLint i);
	void (*ctx_glUniform2i)(GLint location, GLint i1, GLint i2);
	void (*ctx_glUniform3i)(GLint location, GLint i1, GLint i2, GLint i3);
	void (*ctx_glUniform4i)(GLint location, GLint i1, GLint i2, GLint i3, GLint i4);
	void (*ctx_glUniform1f)(GLint location, GLfloat f);
	void (*ctx_glUniform2f)(GLint location, GLfloat f1, GLfloat f2);
	void (*ctx_glUniform3f)(GLint location, GLfloat f1, GLfloat f2, GLfloat f3);
	void (*ctx_glUniform4f)(GLint location, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4);
	void (*ctx_glUniform1fv)(GLint location, int count, GLfloat *f);
	void (*ctx_glUniform2fv)(GLint location, int count, GLfloat *f);
	void (*ctx_glUniform3fv)(GLint location, int count, GLfloat *f);
	void (*ctx_glUniform4fv)(GLint location, int count, GLfloat *f);
	void (*ctx_glGetActiveAttrib)(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
	GLint (*ctx_glGetAttribLocation)(GLuint id, const GLchar *name);
	void (*ctx_glUniformMatrix2fv)(GLint location, int count, GLboolean transpose, GLfloat *v);
	void (*ctx_glUniformMatrix3fv)(GLint location, int count, GLboolean transpose, GLfloat *v);
	void (*ctx_glUniformMatrix4fv)(GLint location, int count, GLboolean transpose, GLfloat *v);
	void (*ctx_glVertexAttrib4f) (GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
};

enum ShaderType {
	SHADER_VERTEX, SHADER_FRAGMENT,

	SHADER_MAX
};

class Shader {
protected:
	GLSLContext* _ctx;
	GLuint _shader[SHADER_MAX];
	GLuint _program;
	bool _initialized;
	mutable bool _active;

	typedef std::map<std::string, int> ShaderVariables;
	ShaderVariables _uniforms;
	ShaderVariables _attributes;

	glm::mat4 _projectionMatrix;
	glm::mat4 _modelViewMatrix;
	mutable uint32_t _time;

	void fetchUniforms ();
	void fetchAttributes ();

	std::string getSource (ShaderType shaderType, const char *buffer, int len);
	void createProgramFromShaders ();
public:
	Shader (GLSLContext* ctx);
	virtual ~Shader ();

	bool load (const std::string& filename, const std::string& source, ShaderType shaderType);
	bool loadFromFile (const std::string& filename, ShaderType shaderType);
	bool loadProgram (const std::string& filename);

	GLuint getShader (ShaderType shaderType) const;

	void setProjectionMatrix (const glm::mat4& matrix);
	void setModelViewMatrix (const glm::mat4& matrix);

	virtual void update (uint32_t deltaTime);
	bool activate () const;
	void deactivate () const;

	int getAttributeLocation (const std::string& name) const;
	int getUniformLocation (const std::string& name) const;

	void setUniformi (const std::string& name, int value) const;
	void setUniformi (int location, int value) const;
	void setUniformi (const std::string& name, int value1, int value2) const;
	void setUniformi (int location, int value1, int value2) const;
	void setUniformi (const std::string& name, int value1, int value2, int value3) const;
	void setUniformi (int location, int value1, int value2, int value3) const;
	void setUniformi (const std::string& name, int value1, int value2, int value3, int value4) const;
	void setUniformi (int location, int value1, int value2, int value3, int value4) const;
	void setUniformf (const std::string& name, float value) const;
	void setUniformf (int location, float value) const;
	void setUniformf (const std::string& name, float value1, float value2) const;
	void setUniformf (int location, float value1, float value2) const;
	void setUniformf (const std::string& name, float value1, float value2, float value3) const;
	void setUniformf (int location, float value1, float value2, float value3) const;
	void setUniformf (const std::string& name, float value1, float value2, float value3, float value4) const;
	void setUniformf (int location, float value1, float value2, float value3, float value4) const;
	void setUniform1fv (const std::string& name, float* values, int offset, int length) const;
	void setUniform1fv (int location, float* values, int offset, int length) const;
	void setUniform2fv (const std::string& name, float* values, int offset, int length) const;
	void setUniform2fv (int location, float* values, int offset, int length) const;
	void setUniform3fv (const std::string& name, float* values, int offset, int length) const;
	void setUniform3fv (int location, float* values, int offset, int length) const;
	void setUniform4fv (const std::string& name, float* values, int offset, int length) const;
	void setUniform4fv (int location, float* values, int offset, int length) const;
	void setUniformMatrix (const std::string& name, glm::mat4& matrix, bool transpose = false) const;
	void setUniformMatrix (int location, glm::mat4& matrix, bool transpose = false) const;
	void setUniformMatrix (const std::string& name, glm::mat3& matrix, bool transpose = false) const;
	void setUniformMatrix (int location, glm::mat3& matrix, bool transpose = false) const;
	void setUniformf (const std::string& name, glm::vec2& values) const;
	void setUniformf (int location, glm::vec2& values) const;
	void setUniformf (const std::string& name, glm::vec3& values) const;
	void setUniformf (int location, glm::vec3& values) const;
	void setUniformf (const std::string& name, Color values) const;
	void setUniformf (int location, Color values) const;
	void setVertexAttribute (const std::string& name, int size, int type, bool normalize, int stride, void* buffer) const;
	void setVertexAttribute (int location, int size, int type, bool normalize, int stride, void* buffer) const;
	void setAttributef (const std::string& name, float value1, float value2, float value3, float value4) const;
	void disableVertexAttribute (const std::string& name) const;
	void disableVertexAttribute (int location) const;
	void enableVertexAttribute (const std::string& name) const;
	void enableVertexAttribute (int location) const;
	bool hasAttribute (const std::string& name) const;
	bool hasUniform (const std::string& name) const;
};

inline GLuint Shader::getShader (ShaderType shaderType) const
{
	return _shader[shaderType];
}

inline void Shader::setUniformi (const std::string& name, int value) const
{
	const int location = getUniformLocation(name);
	setUniformi(location, value);
}

inline void Shader::setUniformi (int location, int value) const
{
	_ctx->ctx_glUniform1i(location, value);
	checkError();
}

inline void Shader::setUniformi (const std::string& name, int value1, int value2) const
{
	const int location = getUniformLocation(name);
	setUniformi(location, value1, value2);
}

inline void Shader::setUniformi (int location, int value1, int value2) const
{
	_ctx->ctx_glUniform2i(location, value1, value2);
	checkError();
}

inline void Shader::setUniformi (const std::string& name, int value1, int value2, int value3) const
{
	const int location = getUniformLocation(name);
	setUniformi(location, value1, value2, value3);
}

inline void Shader::setUniformi (int location, int value1, int value2, int value3) const
{
	_ctx->ctx_glUniform3i(location, value1, value2, value3);
	checkError();
}

inline void Shader::setUniformi (const std::string& name, int value1, int value2, int value3, int value4) const
{
	const int location = getUniformLocation(name);
	setUniformi(location, value1, value2, value3, value4);
}

inline void Shader::setUniformi (int location, int value1, int value2, int value3, int value4) const
{
	_ctx->ctx_glUniform4i(location, value1, value2, value3, value4);
	checkError();
}

inline void Shader::setUniformf (const std::string& name, float value) const
{
	const int location = getUniformLocation(name);
	setUniformf(location, value);
}

inline void Shader::setUniformf (int location, float value) const
{
	_ctx->ctx_glUniform1f(location, value);
	checkError();
}

inline void Shader::setUniformf (const std::string& name, float value1, float value2) const
{
	const int location = getUniformLocation(name);
	setUniformf(location, value1, value2);
}

inline void Shader::setUniformf (int location, float value1, float value2) const
{
	_ctx->ctx_glUniform2f(location, value1, value2);
	checkError();
}

inline void Shader::setUniformf (const std::string& name, float value1, float value2, float value3) const
{
	const int location = getUniformLocation(name);
	setUniformf(location, value1, value2, value3);
}

inline void Shader::setUniformf (int location, float value1, float value2, float value3) const
{
	_ctx->ctx_glUniform3f(location, value1, value2, value3);
	checkError();
}

inline void Shader::setUniformf (const std::string& name, float value1, float value2, float value3, float value4) const
{
	const int location = getUniformLocation(name);
	setUniformf(location, value1, value2, value3, value4);
}

inline void Shader::setUniformf (int location, float value1, float value2, float value3, float value4) const
{
	_ctx->ctx_glUniform4f(location, value1, value2, value3, value4);
	checkError();
}

inline void Shader::setUniform1fv (const std::string& name, float* values, int offset, int length) const
{
	const int location = getUniformLocation(name);
	setUniform1fv(location, values, offset, length);
}

inline void Shader::setUniform1fv (int location, float* values, int offset, int length) const
{
	_ctx->ctx_glUniform1fv(location, length, values);
	checkError();
}

inline void Shader::setUniform2fv (const std::string& name, float* values, int offset, int length) const
{
	const int location = getUniformLocation(name);
	setUniform2fv(location, values, offset, length);
}

inline void Shader::setUniform2fv (int location, float* values, int offset, int length) const
{
	_ctx->ctx_glUniform2fv(location, length / 2, values);
	checkError();
}

inline void Shader::setUniform3fv (const std::string& name, float* values, int offset, int length) const
{
	const int location = getUniformLocation(name);
	setUniform3fv(location, values, offset, length);
}

inline void Shader::setUniform3fv (int location, float* values, int offset, int length) const
{
	_ctx->ctx_glUniform3fv(location, length / 3, values);
	checkError();
}

inline void Shader::setUniform4fv (const std::string& name, float* values, int offset, int length) const
{
	int location = getUniformLocation(name);
	setUniform4fv(location, values, offset, length);
}

inline void Shader::setUniform4fv (int location, float* values, int offset, int length) const
{
	_ctx->ctx_glUniform4fv(location, length / 4, values);
	checkError();
}

inline void Shader::setUniformMatrix (const std::string& name, glm::mat4& matrix, bool transpose) const
{
	const int location = getUniformLocation(name);
	setUniformMatrix(location, matrix, transpose);
}

inline void Shader::setUniformMatrix (int location, glm::mat4& matrix, bool transpose) const
{
	_ctx->ctx_glUniformMatrix4fv(location, 1, transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(matrix));
	checkError();
}

inline void Shader::setUniformMatrix (const std::string& name, glm::mat3& matrix, bool transpose) const
{
	const int location = getUniformLocation(name);
	setUniformMatrix(location, matrix, transpose);
}

inline void Shader::setUniformMatrix (int location, glm::mat3& matrix, bool transpose) const
{
	_ctx->ctx_glUniformMatrix3fv(location, 1, transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(matrix));
	checkError();
}

inline void Shader::setUniformf (const std::string& name, glm::vec2& values) const
{
	setUniformf(name, values.x, values.y);
}

inline void Shader::setUniformf (int location, glm::vec2& values) const
{
	setUniformf(location, values.x, values.y);
}

inline void Shader::setUniformf (const std::string& name, glm::vec3& values) const
{
	setUniformf(name, values.x, values.y, values.z);
}

inline void Shader::setUniformf (int location, glm::vec3& values) const
{
	setUniformf(location, values.x, values.y, values.z);
}

inline void Shader::setUniformf (const std::string& name, Color values) const
{
	setUniformf(name, values[0], values[1], values[2], values[3]);
}

inline void Shader::setUniformf (int location, Color values) const
{
	setUniformf(location, values[0], values[1], values[2], values[3]);
}

inline void Shader::setVertexAttribute (const std::string& name, int size, int type, bool normalize, int stride, void* buffer) const
{
	const int location = getAttributeLocation(name);
	if (location == -1)
		return;
	setVertexAttribute(location, size, type, normalize, stride, buffer);
}

inline void Shader::setVertexAttribute (int location, int size, int type, bool normalize, int stride, void* buffer) const
{
	_ctx->ctx_glVertexAttribPointer(location, size, type, normalize, stride, buffer);
	checkError();
}

inline void Shader::setAttributef (const std::string& name, float value1, float value2, float value3, float value4) const
{
	const int location = getAttributeLocation(name);
	_ctx->ctx_glVertexAttrib4f(location, value1, value2, value3, value4);
	checkError();
}

inline void Shader::disableVertexAttribute (const std::string& name) const
{
	const int location = getAttributeLocation(name);
	if (location == -1)
		return;
	disableVertexAttribute(location);
}

inline void Shader::disableVertexAttribute (int location) const
{
	_ctx->ctx_glDisableVertexAttribArray(location);
	checkError();
}

inline void Shader::enableVertexAttribute (const std::string& name) const
{
	int location = getAttributeLocation(name);
	if (location == -1)
		return;
	enableVertexAttribute(location);
}

inline void Shader::enableVertexAttribute (int location) const
{
	_ctx->ctx_glEnableVertexAttribArray(location);
	checkError();
}

inline bool Shader::hasAttribute (const std::string& name) const
{
	return _attributes.find(name) != _attributes.end();
}

inline bool Shader::hasUniform (const std::string& name) const
{
	return _uniforms.find(name) != _uniforms.end();
}

class ShaderScope {
private:
	const Shader& _shader;
public:
	ShaderScope (const Shader& shader);
	virtual ~ShaderScope ();
};

}
