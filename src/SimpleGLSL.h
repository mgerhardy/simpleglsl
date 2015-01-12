#pragma once

#include <iostream>
#include <string>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>

#ifndef GLenum
#error "No GL header included before including this header"
#endif

namespace glsl {

#ifndef VERTEX_POSTFIX
#define VERTEX_POSTFIX "_vs.glsl"
#endif
#ifndef FRAGMENT_POSTFIX
#define FRAGMENT_POSTFIX "_fs.glsl"
#endif

#ifndef MAX_SHADER_VAR_NAME
#define MAX_SHADER_VAR_NAME 128
#endif

class CheckErrorState {
protected:
	const char* _file;
	const int _line;
	const char* _function;

	inline const char* translateError(GLenum glError) const {
#define GL_ERROR_TRANSLATE(e) case e: return #e;
		switch (glError) {
		GL_ERROR_TRANSLATE(GL_INVALID_ENUM)
		GL_ERROR_TRANSLATE(GL_INVALID_VALUE)
		GL_ERROR_TRANSLATE(GL_INVALID_OPERATION)
		GL_ERROR_TRANSLATE(GL_OUT_OF_MEMORY)
#ifdef GL_STACK_OVERFLOW
		GL_ERROR_TRANSLATE(GL_STACK_OVERFLOW)
#endif
#ifdef GL_STACK_UNDERFLOW
		GL_ERROR_TRANSLATE(GL_STACK_UNDERFLOW)
#endif
		default:
			return "UNKNOWN";
		}
#undef GL_ERROR_TRANSLATE
	}

public:
	CheckErrorState(const char *file, int line, const char *function) :
			_file(file), _line(line), _function(function) {
	}

	~CheckErrorState() {
		for (;;) {
			const GLenum glError = glGetError();
			if (glError == GL_NO_ERROR)
				break;
			std::cout << "openGL err: " << _file << " (" << _line << "): " << _function;
			std::cout << " " << translateError(glError) << " (" << glError << ")" << std::endl;
		}
	}
};

#ifdef _DEBUG
#define checkError() do {CheckErrorState(__FILE__, __LINE__, __PRETTY_FUNCTION__);} while(0)
#else
#define checkError()
#endif

/**
 * Extend this class and hand it over to your shaders - should just be a singleton.
 */
class Context {
	friend class Shader;
public:
	virtual ~Context() {
	}

	/**
	 * @brief Call init() to set your gl function pointers
	 */
	void init(
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
		) {
		ctx_glCreateShader = _glCreateShader;
		ctx_glDeleteShader = _glDeleteShader;
		ctx_glShaderSource = _glShaderSource;
		ctx_glCompileShader = _glCompileShader;
		ctx_glGetShaderiv = _glGetShaderiv;
		ctx_glGetShaderInfoLog = _glGetShaderInfoLog;
		ctx_glCreateProgram = _glCreateProgram;
		ctx_glDeleteProgram = _glDeleteProgram;
		ctx_glAttachShader = _glAttachShader;
		ctx_glDetachShader = _glDetachShader;
		ctx_glLinkProgram = _glLinkProgram;
		ctx_glUseProgram = _glUseProgram;
		ctx_glGetActiveUniform = _glGetActiveUniform;
		ctx_glGetProgramiv = _glGetProgramiv;
		ctx_glGetProgramInfoLog = _glGetProgramInfoLog;
		ctx_glGetUniformLocation = _glGetUniformLocation;
		ctx_glUniform1i = _glUniform1i;
		ctx_glUniform2i = _glUniform2i;
		ctx_glUniform3i = _glUniform3i;
		ctx_glUniform4i = _glUniform4i;
		ctx_glUniform1f = _glUniform1f;
		ctx_glUniform2f = _glUniform2f;
		ctx_glUniform3f = _glUniform3f;
		ctx_glUniform4f = _glUniform4f;
		ctx_glUniform1fv = _glUniform1fv;
		ctx_glUniform2fv = _glUniform2fv;
		ctx_glUniform3fv = _glUniform3fv;
		ctx_glUniform4fv = _glUniform4fv;
		ctx_glGetActiveAttrib = _glGetActiveAttrib;
		ctx_glGetAttribLocation = _glGetAttribLocation;
		ctx_glUniformMatrix2fv = _glUniformMatrix2fv;
		ctx_glUniformMatrix3fv = _glUniformMatrix3fv;
		ctx_glUniformMatrix4fv = _glUniformMatrix4fv;
		ctx_glVertexAttrib4f = _glVertexAttrib4f;
	}

	virtual std::string loadShaderFile(const std::string& filename) const = 0;

protected:
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
	void (*ctx_glVertexAttrib4f)(GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
};

enum ShaderType {
	SHADER_VERTEX, SHADER_FRAGMENT,

	SHADER_MAX
};

class Shader {
protected:
	Context* _ctx;
	GLuint _shader[SHADER_MAX];
	GLuint _program;
	bool _initialized;
	mutable bool _active;

	typedef std::unordered_map<std::string, int> ShaderVariables;
	ShaderVariables _uniforms;
	ShaderVariables _attributes;

	mutable uint32_t _time;

	int getAttributeLocation(const std::string& name) const {
		ShaderVariables::const_iterator i = _attributes.find(name);
		if (i == _attributes.end()) {
			std::cerr << "can't find attribute " << name << std::endl;
			return -1;
		}
		return i->second;
	}

	int getUniformLocation(const std::string& name) const {
		ShaderVariables::const_iterator i = _uniforms.find(name);
		if (i == _uniforms.end()) {
			std::cerr << "can't find uniform " << name << std::endl;
			return -1;
		}
		return i->second;
	}

	void fetchUniforms() {
		char name[MAX_SHADER_VAR_NAME];
		int numUniforms = 0;
		_ctx->ctx_glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &numUniforms);
		checkError();

		_uniforms.clear();
		for (int i = 0; i < numUniforms; i++) {
			GLsizei length;
			GLint size;
			GLenum type;
			_ctx->ctx_glGetActiveUniform(_program, i, MAX_SHADER_VAR_NAME - 1, &length, &size, &type, name);
			const int location = _ctx->ctx_glGetUniformLocation(_program, name);
			_uniforms[name] = location;
		}
	}

	void fetchAttributes() {
		char name[MAX_SHADER_VAR_NAME];
		int numAttributes = 0;
		_ctx->ctx_glGetProgramiv(_program, GL_ACTIVE_ATTRIBUTES, &numAttributes);
		checkError();

		_attributes.clear();
		for (int i = 0; i < numAttributes; i++) {
			GLsizei length;
			GLint size;
			GLenum type;
			_ctx->ctx_glGetActiveAttrib(_program, i, MAX_SHADER_VAR_NAME - 1, &length, &size, &type, name);
			const int location = _ctx->ctx_glGetAttribLocation(_program, name);
			_attributes[name] = location;
		}
	}

	std::string getSource(ShaderType shaderType, const std::string& buffer) const {
		std::string src;
#ifdef GL_ES_VERSION_2_0
		src.append("#version 120\n");
		if (shaderType == SHADER_FRAGMENT) {
			src.append("#ifdef GL_ES\n");
			src.append("precision mediump float;\n");
			src.append("precision mediump int;\n");
			src.append("#endif\n");
		}
#else
		src.append("#version 120\n#define lowp\n#define mediump\n#define highp\n");
#endif

		std::string append(buffer);

		const std::string include = "#include";
		int index = 0;
		for (std::string::iterator i = append.begin(); i != append.end(); ++i, ++index) {
			const char *c = &append[index];
			if (*c != '#') {
				src.append(c, 1);
				continue;
			}
			if (::strncmp(include.c_str(), c, include.length())) {
				src.append(c, 1);
				continue;
			}
			for (; i != append.end(); ++i, ++index) {
				const char *cStart = &append[index];
				if (*cStart != '"')
					continue;

				++index;
				++i;
				for (; i != append.end(); ++i, ++index) {
					const char *cEnd = &append[index];
					if (*cEnd != '"')
						continue;

					const std::string includeFile(cStart + 1, cEnd);
					const std::string& includeBuffer = _ctx->loadShaderFile(includeFile);
					if (!includeBuffer.empty()) {
						std::cerr << "could not load shader include " << includeFile << std::endl;
						break;
					}
					src.append(includeBuffer);
					break;
				}
				break;
			}
		}

		return src;
	}

	void createProgramFromShaders() {
		checkError();
		_program = _ctx->ctx_glCreateProgram();
		checkError();

		const GLuint vert = _shader[SHADER_VERTEX];
		const GLuint frag = _shader[SHADER_FRAGMENT];

		_ctx->ctx_glAttachShader(_program, vert);
		_ctx->ctx_glAttachShader(_program, frag);
		checkError();

		_ctx->ctx_glLinkProgram(_program);
		GLint status;
		_ctx->ctx_glGetProgramiv(_program, GL_LINK_STATUS, &status);
		checkError();
		if (status == GL_TRUE)
			return;
		GLint infoLogLength;
		_ctx->ctx_glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		_ctx->ctx_glGetProgramInfoLog(_program, infoLogLength, nullptr, strInfoLog);
		std::cerr << "linker failure: " << strInfoLog << std::endl;
		_ctx->ctx_glDeleteProgram(_program);
		_program = 0;
		delete[] strInfoLog;
	}
public:
	Shader(Context* ctx) :
			_ctx(ctx), _program(0), _initialized(false), _active(false), _time(0) {
		for (int i = 0; i < SHADER_MAX; ++i) {
			_shader[i] = 0;
		}
	}

	virtual ~Shader() {
		for (int i = 0; i < SHADER_MAX; ++i) {
			_ctx->ctx_glDeleteShader(_shader[i]);
		}
		_ctx->ctx_glDeleteProgram(_program);
	}

	bool load(const std::string& name, const std::string& source, ShaderType shaderType) {
		const GLenum glType = shaderType == SHADER_VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
		checkError();

		_shader[shaderType] = _ctx->ctx_glCreateShader(glType);
		const char *s = source.c_str();
		_ctx->ctx_glShaderSource(_shader[shaderType], 1, (const GLchar**) &s, nullptr);
		_ctx->ctx_glCompileShader(_shader[shaderType]);

		GLint status;
		_ctx->ctx_glGetShaderiv(_shader[shaderType], GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE || glGetError() != GL_NO_ERROR) {
			GLint infoLogLength;
			_ctx->ctx_glGetShaderiv(_shader[shaderType], GL_INFO_LOG_LENGTH, &infoLogLength);

			std::unique_ptr < GLchar > strInfoLog(new GLchar[infoLogLength + 1]);
			_ctx->ctx_glGetShaderInfoLog(_shader[shaderType], infoLogLength, nullptr, strInfoLog);
			const std::string errorLog(strInfoLog, static_cast<std::size_t>(infoLogLength));

			std::string strShaderType;
			switch (glType) {
			case GL_VERTEX_SHADER:
				strShaderType = "vertex";
				break;
			case GL_FRAGMENT_SHADER:
				strShaderType = "fragment";
				break;
			default:
				strShaderType = "unknown";
				break;
			}

			std::cerr << "compile failure in " << name << " (type: " << strShaderType << ") shader:" << std::endl << errorLog << std::endl;
			return false;
		}

		return true;
	}

	bool loadFromFile(const std::string& filename, ShaderType shaderType) {
		const std::string& buffer = _ctx->loadShaderFile(filename);
		if (buffer.empty()) {
			std::cerr << "could not load shader " << filename << std::endl;
			return false;
		}

		const std::string& src = getSource(shaderType, buffer);
		return load(filename, src, shaderType);
	}

	/**
	 * @brief Loads a vertex and fragment shader for the given base filename.
	 *
	 * The filename is hand over to your @c Context implementation with the appropriate filename postfixes
	 *
	 * @see VERTEX_POSTFIX
	 * @see FRAGMENT_POSTFIX
	 */
	bool loadProgram(const std::string& filename) {
		const bool vertex = loadFromFile(filename + VERTEX_POSTFIX, SHADER_VERTEX);
		if (!vertex)
			return false;

		const bool fragment = loadFromFile(filename + FRAGMENT_POSTFIX, SHADER_FRAGMENT);
		if (!fragment)
			return false;

		createProgramFromShaders();
		fetchAttributes();
		fetchUniforms();
		const bool success = _program != 0;
		_initialized = success;
		return success;
	}

	/**
	 * @brief Returns the raw shader handle
	 */
	GLuint getShader(ShaderType shaderType) const {
		return _shader[shaderType];
	}

	/**
	 * @brief Ticks the shader
	 */
	virtual void update(uint32_t deltaTime) {
		_time += deltaTime;
	}

	/**
	 * @brief Bind the shader program
	 *
	 * @return @c true if is is useable now, @c false if not
	 *
	 * @see
	 */
	virtual bool activate() const {
		_ctx->ctx_glUseProgram(_program);
		checkError();
		_active = true;
		return _active;
	}

	virtual bool deactivate() const {
		if (!_active) {
			return false;
		}

		_ctx->ctx_glUseProgram(0);
		checkError();
		_active = false;
		_time = 0;
		return _active;
	}

	void setUniformi(const std::string& name, int value) const;
	void setUniformi(int location, int value) const;
	void setUniformi(const std::string& name, int value1, int value2) const;
	void setUniformi(int location, int value1, int value2) const;
	void setUniformi(const std::string& name, int value1, int value2, int value3) const;
	void setUniformi(int location, int value1, int value2, int value3) const;
	void setUniformi(const std::string& name, int value1, int value2, int value3, int value4) const;
	void setUniformi(int location, int value1, int value2, int value3, int value4) const;
	void setUniformf(const std::string& name, float value) const;
	void setUniformf(int location, float value) const;
	void setUniformf(const std::string& name, float value1, float value2) const;
	void setUniformf(int location, float value1, float value2) const;
	void setUniformf(const std::string& name, float value1, float value2, float value3) const;
	void setUniformf(int location, float value1, float value2, float value3) const;
	void setUniformf(const std::string& name, float value1, float value2, float value3, float value4) const;
	void setUniformf(int location, float value1, float value2, float value3, float value4) const;
	void setUniform1fv(const std::string& name, float* values, int offset, int length) const;
	void setUniform1fv(int location, float* values, int offset, int length) const;
	void setUniform2fv(const std::string& name, float* values, int offset, int length) const;
	void setUniform2fv(int location, float* values, int offset, int length) const;
	void setUniform3fv(const std::string& name, float* values, int offset, int length) const;
	void setUniform3fv(int location, float* values, int offset, int length) const;
	void setUniform4fv(const std::string& name, float* values, int offset, int length) const;
	void setUniform4fv(int location, float* values, int offset, int length) const;
	void setUniformMatrix(const std::string& name, glm::mat4& matrix, bool transpose = false) const;
	void setUniformMatrix(int location, glm::mat4& matrix, bool transpose = false) const;
	void setUniformMatrix(const std::string& name, glm::mat3& matrix, bool transpose = false) const;
	void setUniformMatrix(int location, glm::mat3& matrix, bool transpose = false) const;
	void setUniformf(const std::string& name, const glm::vec2& values) const;
	void setUniformf(int location, const glm::vec2& values) const;
	void setUniformf(const std::string& name, const glm::vec3& values) const;
	void setUniformf(int location, const glm::vec3& values) const;
	void setUniformf(const std::string& name, const glm::vec4& values) const;
	void setUniformf(int location, const glm::vec4& values) const;
	void setVertexAttribute(const std::string& name, int size, int type, bool normalize, int stride, void* buffer) const;
	void setVertexAttribute(int location, int size, int type, bool normalize, int stride, void* buffer) const;
	void setAttributef(const std::string& name, float value1, float value2, float value3, float value4) const;
	void disableVertexAttribute(const std::string& name) const;
	void disableVertexAttribute(int location) const;
	void enableVertexAttribute(const std::string& name) const;
	void enableVertexAttribute(int location) const;
	bool hasAttribute(const std::string& name) const;
	bool hasUniform(const std::string& name) const;
};

inline void Shader::setUniformi(const std::string& name, int value) const {
	const int location = getUniformLocation(name);
	setUniformi(location, value);
}

inline void Shader::setUniformi(int location, int value) const {
	_ctx->ctx_glUniform1i(location, value);
	checkError();
}

inline void Shader::setUniformi(const std::string& name, int value1, int value2) const {
	const int location = getUniformLocation(name);
	setUniformi(location, value1, value2);
}

inline void Shader::setUniformi(int location, int value1, int value2) const {
	_ctx->ctx_glUniform2i(location, value1, value2);
	checkError();
}

inline void Shader::setUniformi(const std::string& name, int value1, int value2, int value3) const {
	const int location = getUniformLocation(name);
	setUniformi(location, value1, value2, value3);
}

inline void Shader::setUniformi(int location, int value1, int value2, int value3) const {
	_ctx->ctx_glUniform3i(location, value1, value2, value3);
	checkError();
}

inline void Shader::setUniformi(const std::string& name, int value1, int value2, int value3, int value4) const {
	const int location = getUniformLocation(name);
	setUniformi(location, value1, value2, value3, value4);
}

inline void Shader::setUniformi(int location, int value1, int value2, int value3, int value4) const {
	_ctx->ctx_glUniform4i(location, value1, value2, value3, value4);
	checkError();
}

inline void Shader::setUniformf(const std::string& name, float value) const {
	const int location = getUniformLocation(name);
	setUniformf(location, value);
}

inline void Shader::setUniformf(int location, float value) const {
	_ctx->ctx_glUniform1f(location, value);
	checkError();
}

inline void Shader::setUniformf(const std::string& name, float value1, float value2) const {
	const int location = getUniformLocation(name);
	setUniformf(location, value1, value2);
}

inline void Shader::setUniformf(int location, float value1, float value2) const {
	_ctx->ctx_glUniform2f(location, value1, value2);
	checkError();
}

inline void Shader::setUniformf(const std::string& name, float value1, float value2, float value3) const {
	const int location = getUniformLocation(name);
	setUniformf(location, value1, value2, value3);
}

inline void Shader::setUniformf(int location, float value1, float value2, float value3) const {
	_ctx->ctx_glUniform3f(location, value1, value2, value3);
	checkError();
}

inline void Shader::setUniformf(const std::string& name, float value1, float value2, float value3, float value4) const {
	const int location = getUniformLocation(name);
	setUniformf(location, value1, value2, value3, value4);
}

inline void Shader::setUniformf(int location, float value1, float value2, float value3, float value4) const {
	_ctx->ctx_glUniform4f(location, value1, value2, value3, value4);
	checkError();
}

inline void Shader::setUniform1fv(const std::string& name, float* values, int offset, int length) const {
	const int location = getUniformLocation(name);
	setUniform1fv(location, values, offset, length);
}

inline void Shader::setUniform1fv(int location, float* values, int offset, int length) const {
	_ctx->ctx_glUniform1fv(location, length, values);
	checkError();
}

inline void Shader::setUniform2fv(const std::string& name, float* values, int offset, int length) const {
	const int location = getUniformLocation(name);
	setUniform2fv(location, values, offset, length);
}

inline void Shader::setUniform2fv(int location, float* values, int offset, int length) const {
	_ctx->ctx_glUniform2fv(location, length / 2, values);
	checkError();
}

inline void Shader::setUniform3fv(const std::string& name, float* values, int offset, int length) const {
	const int location = getUniformLocation(name);
	setUniform3fv(location, values, offset, length);
}

inline void Shader::setUniform3fv(int location, float* values, int offset, int length) const {
	_ctx->ctx_glUniform3fv(location, length / 3, values);
	checkError();
}

inline void Shader::setUniform4fv(const std::string& name, float* values, int offset, int length) const {
	int location = getUniformLocation(name);
	setUniform4fv(location, values, offset, length);
}

inline void Shader::setUniform4fv(int location, float* values, int offset, int length) const {
	_ctx->ctx_glUniform4fv(location, length / 4, values);
	checkError();
}

inline void Shader::setUniformMatrix(const std::string& name, glm::mat4& matrix, bool transpose) const {
	const int location = getUniformLocation(name);
	setUniformMatrix(location, matrix, transpose);
}

inline void Shader::setUniformMatrix(int location, glm::mat4& matrix, bool transpose) const {
	_ctx->ctx_glUniformMatrix4fv(location, 1, transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(matrix));
	checkError();
}

inline void Shader::setUniformMatrix(const std::string& name, glm::mat3& matrix, bool transpose) const {
	const int location = getUniformLocation(name);
	setUniformMatrix(location, matrix, transpose);
}

inline void Shader::setUniformMatrix(int location, glm::mat3& matrix, bool transpose) const {
	_ctx->ctx_glUniformMatrix3fv(location, 1, transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(matrix));
	checkError();
}

inline void Shader::setUniformf(const std::string& name, const glm::vec2& values) const {
	setUniformf(name, values.x, values.y);
}

inline void Shader::setUniformf(int location, const glm::vec2& values) const {
	setUniformf(location, values.x, values.y);
}

inline void Shader::setUniformf(const std::string& name, const glm::vec3& values) const {
	setUniformf(name, values.x, values.y, values.z);
}

inline void Shader::setUniformf(int location, const glm::vec3& values) const {
	setUniformf(location, values.x, values.y, values.z);
}

inline void Shader::setUniformf(const std::string& name, const glm::vec4& values) const {
	setUniformf(name, values.x, values.y, values.z, values.w);
}

inline void Shader::setUniformf(int location, const glm::vec4& values) const {
	setUniformf(location, values.x, values.y, values.z, values.w);
}

inline void Shader::setVertexAttribute(const std::string& name, int size, int type, bool normalize, int stride, void* buffer) const {
	const int location = getAttributeLocation(name);
	if (location == -1)
		return;
	setVertexAttribute(location, size, type, normalize, stride, buffer);
}

inline void Shader::setVertexAttribute(int location, int size, int type, bool normalize, int stride, void* buffer) const {
	_ctx->ctx_glVertexAttribPointer(location, size, type, normalize, stride, buffer);
	checkError();
}

inline void Shader::setAttributef(const std::string& name, float value1, float value2, float value3, float value4) const {
	const int location = getAttributeLocation(name);
	_ctx->ctx_glVertexAttrib4f(location, value1, value2, value3, value4);
	checkError();
}

inline void Shader::disableVertexAttribute(const std::string& name) const {
	const int location = getAttributeLocation(name);
	if (location == -1)
		return;
	disableVertexAttribute(location);
}

inline void Shader::disableVertexAttribute(int location) const {
	_ctx->ctx_glDisableVertexAttribArray(location);
	checkError();
}

inline void Shader::enableVertexAttribute(const std::string& name) const {
	int location = getAttributeLocation(name);
	if (location == -1)
		return;
	enableVertexAttribute(location);
}

inline void Shader::enableVertexAttribute(int location) const {
	_ctx->ctx_glEnableVertexAttribArray(location);
	checkError();
}

inline bool Shader::hasAttribute(const std::string& name) const {
	return _attributes.find(name) != _attributes.end();
}

inline bool Shader::hasUniform(const std::string& name) const {
	return _uniforms.find(name) != _uniforms.end();
}

class ShaderScope {
private:
	const Shader& _shader;
public:
	ShaderScope(const Shader& shader) :
			_shader(shader) {
		_shader.activate();
	}

	virtual ~ShaderScope() {
		_shader.deactivate();
	}
};

#undef checkError
#undef VERTEX_POSTFIX
#undef FRAGMENT_POSTFIX
#undef MAX_SHADER_VAR_NAME

}
