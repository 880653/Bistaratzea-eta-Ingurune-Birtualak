#version 120

uniform int active_lights_n; // Number of active lights (< MG_MAX_LIGHT)
uniform vec3 scene_ambient; // Scene ambient light

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

uniform sampler2D texture0;

varying vec3 f_position;      // camera space
varying vec3 f_viewDirection; // camera space
varying vec3 f_normal;        // camera space
varying vec2 f_texCoord;

vec4 f_color;


void main() {


	vec3 normal = (modelToCameraMatrix * vec4(v_position, 1.0)).xyz;
	vec3 spec = vec3(0.0, 0.0, 0.0);
	vec3 itot = vec3(0.0, 0.0, 0.0);
	vec3 argia = vec3(0.0, 0.0, 0.0);

	for(int i=0; i<active_lights_n; i++){

		//Direkzionala
		if(theLights[i].position.w == 0.0){
			vec3 lag = normalize(-1.0 * theLights[i].position.xyz);
			vec3 r = (2*(dot(normal, lag))*normal) - lag;
			vec3 v = normalize(vec4(v_position, 0.0) - vec4(theLights[i].position));
			spec = pow(max(0, dot(r,v)), theMaterial.shininess) * (theMaterial.specular * theLights[i].specular);
			itot = max(0, dot(normal, lag)) * ((theMaterial.diffuse * theLights[i].diffuse) + spec);
			argia = argia + itot;
		}
		vec3 j = scene_ambient + argia;
		f_color = vec4(j, 1.0);
		vec4 texColor = texture2D(texture0, f_texCoord);
		gl_Position = modelToClipMatrix * vec4(v_position, 1);

	}
	f_texCoord = v_texCoord;

	vec3 argia = scene_ambient + lag;

	f_color = vec4(argia, 1.0);
	gl_Position = modelToClipMatrix * vec4(v_position, 1);

	f_texCoord = v_texCoord;

	gl_FragColor = f_color*texColor;

}
