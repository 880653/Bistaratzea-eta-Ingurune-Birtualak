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
uniform sampler2D specmap;    // specular map

varying vec3 f_position;      // camera space
varying vec3 f_viewDirection; // camera space
varying vec3 f_normal;        // camera space
varying vec2 f_texCoord;

vec4 f_color;

void main() {
	vec3 normal = normalize(f_normal);
	vec3 spec = vec3(0.0, 0.0, 0.0);
	vec3 itot = vec3(0.0, 0.0, 0.0);
	vec3 argia = vec3(0.0, 0.0, 0.0);
	vec3 lag, r, v, p;
	float d, cspot;
	
	vec4 texel4 = texture2D(specmap, f_texCoord);
	vec3 texel = vec3(texel4[0], texel4[1], texel4[2]);



	for(int i=0; i<active_lights_n; i++){

		//Direkzionala
		if(theLights[i].position.w == 0.0f){

			lag = normalize(-1.0 * theLights[i].position.xyz);

			r = normalize(2*(dot(normal, lag))*normal - lag);

			v = normalize(f_viewDirection);

			spec = pow(max(0, dot(r,v)), theMaterial.shininess) * (texel * theLights[i].specular);
			
			itot = max(0, dot(normal, lag)) * ((theMaterial.diffuse * theLights[i].diffuse) + spec);
			
			argia = argia + itot;
		}
		//Posizionala
		else if(theLights[i].cosCutOff == 0.0f){

			p = f_position;

			lag = normalize(theLights[i].position.xyz - p);
			
			r = normalize(2*(dot(normal, lag))*normal - lag);
			
			v = normalize(f_viewDirection);

			spec = pow(max(0, dot(r,v)), theMaterial.shininess) * (texel * theLights[i].specular);
			
			d = 1/(theLights[i].attenuation[0] + (theLights[i].attenuation[1] * length(theLights[i].position.xyz - p)) + theLights[i].attenuation[2] * pow(length(theLights[i].position.xyz - p), 2));
			
			itot = (d * max(0, dot(normal, lag)) * ((theMaterial.diffuse * theLights[i].diffuse) + spec));
			
			argia = argia + itot;
		}
		//Fokua
		else{
			p = f_position;

			lag = normalize(theLights[i].position.xyz - p);
			
			r = normalize(2*(dot(normal, lag))*normal - lag);
			
			v = normalize(f_viewDirection);

			spec = pow(max(0, dot(r,v)), theMaterial.shininess) * (texel * theLights[i].specular);
			
			cspot = max(dot(-1.0 * lag, normalize(theLights[i].spotDir)), 0);

			if(cspot > theLights[i].cosCutOff){

				itot = pow(cspot, theLights[i].exponent) * max(0, dot(normal, lag)) * ((theMaterial.diffuse * theLights[i].diffuse) + spec);
			
				argia = argia + itot;
			}
		}
	}

	vec3 totala = scene_ambient + argia;

	f_color = vec4(totala, 1.0);

	vec4 texColor = texture2D(texture0, f_texCoord);

	gl_FragColor = f_color * texColor;
}
