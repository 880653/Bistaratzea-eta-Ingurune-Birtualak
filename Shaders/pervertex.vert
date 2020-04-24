#version 120


uniform mat4 modelToCameraMatrix;
uniform mat4 cameraToClipMatrix;
uniform mat4 modelToWorldMatrix;
uniform mat4 modelToClipMatrix;

uniform int active_lights_n; // Number of active lights (< MG_MAX_LIGHT)
uniform vec3 scene_ambient;  // rgb

uniform struct light_t {
	vec4 position;    // Camera space
	vec3 diffuse;     // rgb
	vec3 specular;    // rgb
	vec3 attenuation; // (constant, lineal, quadratic)
	vec3 spotDir;     // Camera space
	float cosCutOff;  // cutOff cosine
	float exponent;
} theLights[4];     // MG_MAX_LIGHTS

uniform struct material_t {
	vec3  diffuse;
	vec3  specular;
	float alpha;
	float shininess;
} theMaterial;

attribute vec3 v_position; // Model space
attribute vec3 v_normal;   // Model space
attribute vec2 v_texCoord;

varying vec4 f_color;
varying vec2 f_texCoord;


void main() {

	vec4 normal = modelToCameraMatrix * vec4(v_normal, 0.0);

	vec3 lag = vec3(0.0);

	//char m_type;

	for(int i = 0; i<4; i++){

		vec4 l = normalize(-1*theLights[i].position);

		//difusoa
		vec3 diffuse = theMaterial.diffuse * theLights[i].diffuse;

		//erp: erpinaren posizioa
		vec4 erp = vec4(v_position, 0.0);

		//arg: argiaren posizioa
		vec4 arg = vec4(theLights[i].position);

		//v: erpinetik kamerara doan bektore unitarioa
		vec4 v = normalize(erp-arg);

		//r 
		vec4 r = (2*(normal*l)*normal) - l;
		
		//espekularra
		vec3 spec = pow(max(0, dot(r, v)), theMaterial.shininess) * theMaterial.specular * theLights[i].specular;

		lag = lag + max(0, dot(normal, l))*(diffuse + spec);

	}

	vec3 argia = scene_ambient + lag;

	f_color = vec4(argia, 1.0);
	gl_Position = modelToClipMatrix * vec4(v_position, 1);

	f_texCoord = v_texCoord;



}
