#include "SimpleGLSL.h"

namespace simpleglsl {

const char* OpenGLStateHandlerCheckError::translateError (GLenum glError) const
{
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

OpenGLStateHandlerCheckError::OpenGLStateHandlerCheckError(const char *file, int line, const char *function) : 
	_file(file), _line(line), _function(function) {
}

OpenGLStateHandlerCheckError::~OpenGLStateHandlerCheckError() {
	int ret = 0;
	for (;;) {
		const GLenum glError = glGetError();
		if (glError == GL_NO_ERROR)
			break;
		std::cout << "openGL err: " << file << " (" << line << "): " << function;
		std::cout << " " << translateError(glError) << " (" << glError << ")" << std::endl;
		ret++;
	}
	return ret;
}

Shader::Shader (GLSLContext* ctx) :
		_ctx(ctx), _program(0), _initialized(false), _active(false), _time(0)
{
	for (int i = 0; i < SHADER_MAX; ++i) {
		_shader[i] = 0;
	}
}

Shader::~Shader ()
{
	for (int i = 0; i < SHADER_MAX; ++i) {
		_ctx->ctx_glDeleteShader(_shader[i]);
	}
	_ctx->ctx_glDeleteProgram(_program);
}

void Shader::update (uint32_t deltaTime)
{
	_time += deltaTime;
}

bool Shader::load (const std::string& filename, const std::string& source, ShaderType shaderType)
{
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

		ScopedPtr<GLchar> strInfoLog(new GLchar[infoLogLength + 1]);
		_ctx->ctx_glGetShaderInfoLog(_shader[shaderType], infoLogLength, nullptr, strInfoLog);
		std::string errorLog(strInfoLog, static_cast<std::size_t>(infoLogLength));

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

		std::cerr << "compile failure in " << filename << " (type: " << strShaderType << ") shader:"  << std::endl << errorLog << std::endl;
	}

	return true;
}

bool Shader::loadFromFile (const std::string& filename, ShaderType shaderType)
{
	FilePtr filePtr = FS.getFile(FS.getShaderDir() + filename);
	char *buffer;
	int fileLen = filePtr->read((void **) &buffer);
	ScopedArrayPtr<char> p(buffer);
	if (!buffer || fileLen <= 0) {
		std::cerr << "could not load shader " << filename << std::endl;
		return false;
	}

	const std::string src = getSource(shaderType, buffer, fileLen);
	return load(filename, src, shaderType);
}

std::string Shader::getSource (ShaderType shaderType, const char *buffer, int len)
{
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

	std::string append(buffer, len);

	const std::string include = "#include";
	int index = 0;
	for (std::string::iterator i = append.begin(); i != append.end(); ++i, ++index) {
		const char *c = &append[index];
		if (*c != '#') {
			src.append(c, 1);
			continue;
		}
		if (strncmp(include.c_str(), c, include.length())) {
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
				FilePtr filePtr = FS.getFile(FS.getShaderDir() + includeFile);
				char *includeBuffer;
				const int includeLen = filePtr->read((void **) &includeBuffer);
				ScopedArrayPtr<char> p(includeBuffer);
				if (!includeBuffer || includeLen <= 0) {
					std::cerr << "could not load shader include " << includeFile << std::endl;
					break;
				}
				src.append(includeBuffer, includeLen);
				break;
			}
			break;
		}
	}

	return src;
}

bool Shader::loadProgram (const std::string& filename)
{
	const bool vertex = loadFromFile(filename + "_vs.glsl", SHADER_VERTEX);
	if (!vertex)
		return false;

	const bool fragment = loadFromFile(filename + "_fs.glsl", SHADER_FRAGMENT);
	if (!fragment)
		return false;

	createProgramFromShaders();
	fetchAttributes();
	fetchUniforms();
	const bool success = _program != 0;
	_initialized = success;
	return success;
}

void Shader::fetchUniforms ()
{
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

void Shader::fetchAttributes ()
{
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

void Shader::createProgramFromShaders ()
{
	checkError();
	GLint status;
	_program = _ctx->ctx_glCreateProgram();
	checkError();

	const GLuint vert = _shader[SHADER_VERTEX];
	const GLuint frag = _shader[SHADER_FRAGMENT];

	_ctx->ctx_glAttachShader(_program, vert);
	_ctx->ctx_glAttachShader(_program, frag);
	checkError();

	_ctx->ctx_glLinkProgram(_program);
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

int Shader::getAttributeLocation (const std::string& name) const
{
	ShaderVariables::const_iterator i = _attributes.find(name);
	if (i == _attributes.end()) {
		std::cerr << "can't find attribute " << name << std::endl;
		return -1;
	}
	return i->second;
}

int Shader::getUniformLocation (const std::string& name) const
{
	ShaderVariables::const_iterator i = _uniforms.find(name);
	if (i == _uniforms.end()) {
		std::cerr << "can't find uniform " << name << std::endl;
		return -1;
	}
	return i->second;
}

void Shader::setProjectionMatrix (const glm::mat4& projectionMatrix)
{
	_projectionMatrix = projectionMatrix;
}

void Shader::setModelViewMatrix (const glm::mat4& modelViewMatrix)
{
	_modelViewMatrix = modelViewMatrix;
}

bool Shader::activate () const
{
	_ctx->ctx_glUseProgram(_program);
	checkError();
	_active = true;

	return true;
}

void Shader::deactivate () const
{
	if (!_active) {
		return;
	}

	_ctx->ctx_glUseProgram(0);
	checkError();
	_active = false;
	_time = 0;
}

ShaderScope::ShaderScope (const Shader& shader) :
		_shader(shader)
{
	_shader.activate();
}

ShaderScope::~ShaderScope ()
{
	_shader.deactivate();
}

}
