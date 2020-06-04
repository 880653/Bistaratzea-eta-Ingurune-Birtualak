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

uniform vec3 campos; // Camera position in world space

uniform sampler2D texture0;   // Texture
uniform samplerCube envmap;   // Environment map (cubemap)

varying vec3 f_position;      // camera space
varying vec3 f_viewDirection; // camera space
varying vec3 f_normal;        // camera space
varying vec2 f_texCoord;
varying vec3 f_positionw;    // world space
varying vec3 f_normalw;      // world space


void main() {

	vec3 normal = normalize(f_normal);
	vec3 spec = vec3(0.0, 0.0, 0.0);
	vec3 itot = vec3(0.0, 0.0, 0.0);
	vec3 argia = vec3(0.0, 0.0, 0.0);
	vec3 lag, r, v, p;
	float d, cspot;
	vec4 texColor;

	for(int i=0; i<active_lights_n; i++){

		//Direkzionala
		if(theLights[i].position.w == 0.0f){

			lag = normalize(-1.0 * campos);

			r = normalize(2*(dot(f_normalw, lag))*f_normalw - lag);

			r = vec3(r.x, r.y, -1 * r.z);

			texColor = textureCube(envmap, r);
			
			v = normalize(f_viewDirection);

			spec = pow(max(0, dot(r,v)), theMaterial.shininess) * (theMaterial.specular * theLights[i].specular);
			
			itot = max(0, dot(normal, lag)) * ((theMaterial.diffuse * theLights[i].diffuse) + spec);
			
			argia = argia + itot;
		}
		//Posizionala
		else if(theLights[i].cosCutOff == 0.0f){

			p = f_position;

			lag = normalize(campos - f_positionw);
			
			r = normalize(2*(dot(f_normalw, lag))*f_normalw - lag);

			r = vec3(r.x, r.y, -1 * r.z);

			texColor = textureCube(envmap, r);

			v = normalize(f_viewDirection);

			spec = pow(max(0, dot(r,v)), theMaterial.shininess) * (theMaterial.specular * theLights[i].specular);
			
			d = 1/(theLights[i].attenuation[0] + (theLights[i].attenuation[1] * length(theLights[i].position.xyz - p)) + theLights[i].attenuation[2] * pow(length(theLights[i].position.xyz - p), 2));
			
			itot = (d * max(0, dot(normal, lag)) * ((theMaterial.diffuse * theLights[i].diffuse) + spec));
			
			argia = argia + itot;
		}
		//Fokua
		else{
			p = f_position;

			lag = normalize(campos - f_positionw);
			
			r = normalize(2*(dot(f_normalw, lag))*f_normalw - lag);

			r = vec3(r.x, r.y, -1 * r.z);

			texColor = textureCube(envmap, r);
			
			v = normalize(f_viewDirection);

			spec = pow(max(0, dot(r,v)), theMaterial.shininess) * (theMaterial.specular * theLights[i].specular);
			
			cspot = max(dot(-1.0 * lag, normalize(theLights[i].spotDir)), 0);

			if(cspot > theLights[i].cosCutOff){

				itot = pow(cspot, theLights[i].exponent) * max(0, dot(normal, lag)) * ((theMaterial.diffuse * theLights[i].diffuse) + spec);
			
				argia = argia + itot;
			}
		}
	}
	

	vec3 totala = scene_ambient + argia;

	vec4 f_color = vec4(totala, 1.0);

	gl_FragColor = f_color * texColor;
}
