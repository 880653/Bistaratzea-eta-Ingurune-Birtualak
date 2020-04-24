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
	vec3 normal = normalize((modelToCameraMatrix * vec4(v_position, 1.0)).xyz);
	vec3 spec = vec3(0.0, 0.0, 0.0);
	vec3 itot = vec3(0.0, 0.0, 0.0);
	vec3 argia = vec3(0.0, 0.0, 0.0);
	vec3 lag, r, v, p;
	float d;

	for(int i=0; i<active_lights_n; i++){

		//Direkzionala
		if(theLights[i].position.w == 0.0f){

			lag = normalize(-1.0 * theLights[i].position.xyz);

			r = normalize(2*(dot(normal, lag))*normal) - lag;

			v = normalize((modelToCameraMatrix * vec4((-1.0 * v_position), -1.0)).xyz);
			
			spec = pow(max(0, dot(r,v)), theMaterial.shininess) * (theMaterial.specular * theLights[i].specular);
			
			itot = max(0, dot(normal, lag)) * ((theMaterial.diffuse * theLights[i].diffuse) + spec);
			
			argia = argia + itot;
		}
		//Posizionala
		else if(theLights[i].cosCutOff == 0.0f){

			p = (modelToCameraMatrix * vec4(v_position, 1.0)).xyz;

			lag = normalize(theLights[i].position.xyz - p);
			
			r = normalize(2*(dot(normal, lag))*normal) - lag;
			
			v = normalize((modelToCameraMatrix * vec4((-1.0 * v_position), -1.0)).xyz);
			
			spec = pow(max(0, dot(r,v)), theMaterial.shininess) * (theMaterial.specular * theLights[i].specular);
			
			d = 1/(theLights[i].attenuation[0] + (theLights[i].attenuation[1] * length(theLights[i].position.xyz - p)) + theLights[i].attenuation[2] * pow(length(theLights[i].position.xyz - p), 2));
			
			itot = (d * max(0, dot(normal, lag)) * ((theMaterial.diffuse * theLights[i].diffuse) + spec));
			
			argia = argia + itot;
		}


	}

	argia = scene_ambient + lag;

	f_color = vec4(argia, 1.0);

	gl_Position = modelToClipMatrix * vec4(v_position, 1);

	f_texCoord = v_texCoord;
}
