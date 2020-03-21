#include <cstdio>
#include <cstring>
#include <cmath>
#include <tuple>
#include "shader.h"
#include "constants.h"
#include "tools.h"
#include "shaderUtils.h"
#include "lightManager.h"
#include "texture.h"
#include "material.h"
#include "renderState.h"

static void init_uniform(std::map<std::string, Uniform> & h,
						 const char * uname,
						 Uniform::type utype,
						 const std::string & program_name,
						 GLuint program_handler,
						 bool is_default = true) {
	bool p;
	Uniform u(uname, utype);
	std::map<std::string, Uniform>::iterator it;
	std::tie(it, p) = h.insert(std::make_pair(u.m_name, u));
	if (!p) {
		if (is_default) {
			fprintf(stderr, "[W] Ignoring duplicated uniform %s.\n", u.m_name.c_str());
			return;
		} else {
			fprintf(stderr, "[W] Replacing duplicated uniform %s.\n", u.m_name.c_str());
		}
	}
	it->second.m_handler = GetProgramUniform(program_name.c_str(), program_handler,
											 it->second.m_name.c_str(), !is_default);
}

void ShaderProgram::initDefaultUniforms() {

	static char buffer[1024];
	// Fill uniforms for all possible lights
	for (int i = 0; i < 4; ++i) { // MG_MAX_LIGHTS
		sprintf(buffer, "theLights[%d].position", i);
		init_uniform(m_uniforms, buffer, Uniform::type::uvec4, m_name, m_program);

		sprintf(buffer, "theLights[%d].diffuse", i);
		init_uniform(m_uniforms, buffer, Uniform::type::uvec3, m_name, m_program);

		sprintf(buffer, "theLights[%d].specular", i);
		init_uniform(m_uniforms, buffer, Uniform::type::uvec3, m_name, m_program);

		sprintf(buffer, "theLights[%d].attenuation", i);
		init_uniform(m_uniforms, buffer, Uniform::type::uvec3, m_name, m_program);

		sprintf(buffer, "theLights[%d].spotDir", i);
		init_uniform(m_uniforms, buffer, Uniform::type::uvec3, m_name, m_program);

		sprintf(buffer, "theLights[%d].cosCutOff", i);
		init_uniform(m_uniforms, buffer, Uniform::type::ufloat, m_name, m_program);

		sprintf(buffer, "theLights[%d].exponent", i);
		init_uniform(m_uniforms, buffer, Uniform::type::ufloat, m_name, m_program);
	}
	init_uniform(m_uniforms, "active_lights_n", Uniform::type::uint, m_name, m_program);
	init_uniform(m_uniforms, "scene_ambient", Uniform::type::uvec3, m_name, m_program);
	init_uniform(m_uniforms, "theMaterial.diffuse", Uniform::type::uvec3, m_name, m_program);
	init_uniform(m_uniforms, "theMaterial.specular", Uniform::type::uvec3, m_name, m_program);
	init_uniform(m_uniforms, "theMaterial.shininess", Uniform::type::ufloat, m_name, m_program);
	init_uniform(m_uniforms, "theMaterial.alpha", Uniform::type::ufloat, m_name, m_program);

	init_uniform(m_uniforms, "texture0", Uniform::type::usampler, m_name, m_program);
	//init_uniform(m_uniforms, "bumpmap", Uniform::type::usampler, m_name, m_program);
	//init_uniform(m_uniforms, "cubemap", Uniform::type::usampler, m_name, m_program);

	init_uniform(m_uniforms, "modelToCameraMatrix", Uniform::type::umat4, m_name, m_program);
	init_uniform(m_uniforms, "modelToWorldMatrix", Uniform::type::umat4, m_name, m_program);
	init_uniform(m_uniforms, "cameraToClipMatrix", Uniform::type::umat4, m_name, m_program);
	init_uniform(m_uniforms, "modelToClipMatrix", Uniform::type::umat4, m_name, m_program);

}

void ShaderProgram::loadShaders(const std::string & shaderName,
								 const std::string & vShaderFile,
								 const std::string & fShaderFile) {
	fprintf(stderr, "[I] Shader %s\n", shaderName.c_str());
	fprintf(stderr, "[I] compiling %s ...", vShaderFile.c_str());
	m_vShaderName = vShaderFile;
	m_vShader = LoadShader(GL_VERTEX_SHADER, vShaderFile.c_str());
	fprintf(stderr, "OK\n");
	fprintf(stderr, "[I] compiling %s ...", fShaderFile.c_str());
	m_fShaderName = fShaderFile;
	m_fShader = LoadShader(GL_FRAGMENT_SHADER, fShaderFile.c_str());
	fprintf(stderr, "OK\n");
	fprintf(stderr, "[I] linking ...");
	m_program = CreateProgramFromObjects(shaderName.c_str(),
										 m_vShader,
										 m_fShader);
	fprintf(stderr, "OK (program: %d)\n", m_program);
}

ShaderProgram::~ShaderProgram() {}

ShaderProgram::ShaderProgram(const std::string & shaderName,
							 const std::string & vShaderFile,
							 const std::string & fShaderFile) :
	m_name(shaderName),
	m_vShader(0),
	m_fShader(0),
	m_program(0) {
	loadShaders(shaderName, vShaderFile, fShaderFile);
	initDefaultUniforms();
}

ShaderProgram::ShaderProgram(const std::string & shaderName,
							 const std::string & vShaderFile,
							 const std::string & fShaderFile,
							 const std::vector<Uniform> & uniforms,
							 const std::vector<std::string> & capabilities) :
	m_name(shaderName),
	m_vShader(0),
	m_fShader(0),
	m_program(0) {
	loadShaders(shaderName, vShaderFile, fShaderFile);
	for(auto it = uniforms.begin(); it != uniforms.end(); ++it)
		init_uniform(m_uniforms, it->m_name.c_str(), it->m_type, m_name.c_str(), m_program, false);
	for(auto it = capabilities.begin(); it != capabilities.end(); ++it)
		m_capabilities.insert(*it);
	initDefaultUniforms();
}

const char *ShaderProgram::getName() const { return m_name.c_str(); }
const std::string &ShaderProgram::getNameString() const { return m_name; }

void ShaderProgram::activate() {

	GLenum errorCode;

	glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &(m_oldProgram));
	glUseProgram(m_program);
	errorCode = glGetError();
	if (errorCode != GL_NO_ERROR) {
		fprintf (stderr, "[E] ShaderProgram::activate program %d: %s\n",
				 m_program, gluErrorString(errorCode));
		exit(1);
	}
}

void ShaderProgram::deactivate() {
	GLenum errorCode;

	glUseProgram(m_oldProgram);
	errorCode = glGetError();
	if (errorCode != GL_NO_ERROR) {
		fprintf (stderr, "[E] ShaderProgram::deactivate program %d: %s\n",
				 m_program, gluErrorString(errorCode));
		exit(1);
	}
}

void ShaderProgram::add_capability(const std::string & cap) {
	m_capabilities.insert(cap);
}

void ShaderProgram::add_uniform(const Uniform &u) {
	if(!u.m_name.length()) {
		fprintf (stderr, "[W] ShaderProgram::add_uniform program \'%s\': empty uniform.\n",
				 m_name.c_str());
		return;
	}
	if(u.m_type == Uniform::type::uerror) {
		fprintf (stderr, "[W] ShaderProgram::add_uniform program \'%s\': invalid uniform \'%s\'.\n",
				 m_name.c_str(), u.m_name.c_str());
		return;
	}
	init_uniform(m_uniforms, u.m_name.c_str(), u.m_type, m_name, m_program, false);
}

bool ShaderProgram::has_capability(const std::string & cap) const {
	return m_capabilities.find(cap) != m_capabilities.end();
}


template<class V> void ShaderProgram::send_uniform(const std::string uname, const V & value) {
	auto it = m_uniforms.find(uname);
	if (it == m_uniforms.end()) {
		fprintf(stderr, "[E] no uniform %s in shader %s\n", uname.c_str(), m_name.c_str());
		exit(-1);
	}
	if (it->second.m_handler == -1) return; // If no handler, do nothing
	bool res = it->second.send(value);
	if (!res) {
		std::string error = uname + ": " + Uniform::last_error_str;
		fprintf(stderr, "[E] type mismatch sending uniform %s of shader %s: %s\n",
				uname.c_str(), m_name.c_str(), error.c_str());
		exit(-1);
	}
}

void ShaderProgram::beforeDraw() {

	Material *mat;
	Texture *tex;
	RenderState *rs = RenderState::instance();
	static char buffer[1024];

	this->send_uniform("modelToCameraMatrix", rs->top(RenderState::modelview));
	this->send_uniform("modelToWorldMatrix", rs->top(RenderState::model));
	this->send_uniform("cameraToClipMatrix", rs->top(RenderState::projection));
	this->send_uniform("modelToClipMatrix", rs->top(RenderState::modelview_projection));

	this->send_uniform("scene_ambient", rs->getSceneAmbient());

	int i = 0;
	for(LightManager::iterator it = LightManager::instance()->begin(),
			end = LightManager::instance()->end(); it != end; ++it) {
		Light *theLight = *it;
		if (!theLight->isOn()) continue;
		if (i == 4) { // MG_MAX_LIGHTS
			fprintf(stderr, "[W] too many active lights. Discarding the rest\n");
			break;
		}
		sprintf(buffer, "theLights[%d].position", i);
		this->send_uniform(buffer, theLight->getPositionEye_4fv());
		sprintf(buffer, "theLights[%d].diffuse", i);
		this->send_uniform(buffer, theLight->getDiffuse());
		sprintf(buffer, "theLights[%d].specular", i);
		this->send_uniform(buffer, theLight->getSpecular());
		sprintf(buffer, "theLights[%d].attenuation", i);
		this->send_uniform(buffer, theLight->getAttenuationVector());
		if (theLight->isSpot()) {
			sprintf(buffer, "theLights[%d].spotDir", i);
			this->send_uniform(buffer, theLight->getSpotDirectionEye());
			sprintf(buffer, "theLights[%d].cosCutOff", i);
			this->send_uniform(buffer, cosf(theLight->getSpotCutoff()));
			sprintf(buffer, "theLights[%d].exponent", i);
			this->send_uniform(buffer,theLight->getSpotExponent());
		} else {
			sprintf(buffer, "theLights[%d].cosCutOff", i);
			this->send_uniform(buffer, 0.0f);
		}
		++i;
	}
	this->send_uniform("active_lights_n", i);

	mat = rs->getFrontMaterial();
	if (mat != 0) {
		this->send_uniform("theMaterial.diffuse", mat->getDiffuse());
		this->send_uniform("theMaterial.specular", mat->getSpecular());
		this->send_uniform("theMaterial.shininess", mat->getShininess());
		this->send_uniform("theMaterial.alpha", mat->getAlpha());
		tex = mat->getTexture();
		if (tex != 0) {
			// Set texture to unit 0
			tex->bindGLUnit(Constants::gl_texunits::texture);
			this->send_uniform("texture0", Constants::gl_texunits::texture); // Texture unit 0
		}
	}
	if(this->has_capability("sc")){
		this->send_uniform("sc", rs->getSc());
	}
}

void ShaderProgram::print() const {
	printf("Shader %s (vertex \'%s\', fragment \'%s\') cap {", m_name.c_str(),
		   m_vShaderName.c_str(), m_fShaderName.c_str());
	for (auto it = m_capabilities.begin(); it != m_capabilities.end(); ++it)
		printf("\'%s\',", it->c_str());
	printf("}\n");
	for (auto it = m_uniforms.begin(); it != m_uniforms.end(); ++it) {
		const Uniform &u = it->second;
		printf("  {name: \'%s\', type: \'%s\', handler: %d\n", u.m_name.c_str(), Uniform::type_str(u.m_type), u.m_handler);
	}
}


///////////////////////////////////////////////////////////////////
// JSON parsing functions

static bool json_string(Json::Value & o, std::string & res) {
	if (o.isNull()) return false;
	if (!o.isString()) return false;
	res = o.asString();
	return true;
}

static int populate_uniforms(Json::Value & juniforms,
							  std::vector<Uniform> & uv,
							  std::string & error) {
	std::string name;
	std::string type;

	if (juniforms.isNull())
		return 0;

	if (!juniforms.isArray()) {
		error = "Invalid syntax.";
		return -1;
	}
	int n = juniforms.size();
	for (int i = 0; i < n; ++i) {
		Json::Value & ju = juniforms[i];
		if (!json_string(ju["name"], name)) {
			error = "Some uniforms have no name.";
			return -1;
		}
		if (!json_string(ju["type"], type)) {
			error = "Uniform " + name + " has no type.";
			return -1;
		}
		Uniform u(name, type);
		if (u.m_type == Uniform::type::uerror) {
			error = "Uniform " + name + " has invalid type.";
			return -1;
		}
		uv.push_back(u);
	}
	return uv.size();
}

int populate_capabilities(Json::Value & jcaps,
						  std::vector<std::string > & vcap,
						  std::string & error) {
	std::string name;

	if (jcaps.isNull()) return 0;
	if (!jcaps.isArray()) {
		error = "Invalid syntax.";
		return -1;
	}
	int n = jcaps.size();
	for (int i = 0; i < n; ++i) {
		Json::Value & jcap = jcaps[i];
		if (!json_string(jcap, name)) {
			error = "Some uniforms have no name.";
			return -1;
		}
		vcap.push_back(name);
	}
	return vcap.size();
}

ShaderProgram *ShaderProgram::from_json(Json::Value & shaderjs) {

	std::string name;
	std::string vertex_shader;
	std::string fragment_shader;

	if (!json_string(shaderjs["name"], name)) {
		std::string error = "[E]: shader has no name.";
		exit(-1);
	}
	if (!json_string(shaderjs["vshader"], vertex_shader)) {
		std::string error = "[E]: shader " + name + " has no vertex shader.";
		exit(-1);
	}
	if (!json_string(shaderjs["fshader"], fragment_shader)) {
		std::string error = "[E]: shader " + name + " has no fragment shader.";
		exit(-1);
	}
	std::vector<Uniform> uv;
	std::string error;
	int uniform_n = populate_uniforms(shaderjs["uniforms"], uv, error);
	if (uniform_n == -1) {
		fprintf(stderr, "[E] reading uniforms of shader %s: %s\n",
				name.c_str(), error.c_str());
		exit(-1);
	}
	std::vector<std::string> vcap;
	int caps_n = populate_capabilities(shaderjs["capabilities"], vcap, error);
	if (caps_n == -1) {
		fprintf(stderr, "[E] reading capabilities of shader %s: %s\n",
				name.c_str(), error.c_str());
		exit(-1);
	}
	return new ShaderProgram(name, vertex_shader, fragment_shader, uv, vcap);
}

ShaderProgram *ShaderProgram::from_json(const std::string &src) {
	Json::Value shaderjs;
	std::stringstream is(src);
	is >> shaderjs;
	return from_json(shaderjs);
}
