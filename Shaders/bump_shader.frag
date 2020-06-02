#version 120

uniform int active_lights_n; // Number of active lights (< MG_MAX_LIGHT)
uniform vec3 scene_ambient; // Scene ambient light

struct material_t {
	vec3  diffuse;
	vec3  specular;
	float alpha;
	float shininess;
};

struct light_t {
	vec4 position;    // Camera space
	vec3 diffuse;     // rgb
	vec3 specular;    // rgb
	vec3 attenuation; // (constant, lineal, quadratic)
	vec3 spotDir;     // Camera space
	float cosCutOff;  // cutOff cosine
	float exponent;
};

uniform light_t theLights[4];
uniform material_t theMaterial;

uniform sampler2D texture0;
uniform sampler2D bumpmap;

varying vec2 f_texCoord;
varying vec3 f_viewDirection;     // tangent space
varying vec3 f_lightDirection[4]; // tangent space
varying vec3 f_spotDirection[4];  // tangent space
varying vec3 f_position;

void main() {

	// Base color
	vec4 baseColor = texture2D(texture0, f_texCoord);

	// Decode the tangent space normal (from[0..1] to [-1..+1])
	vec3 N = texture2D(bumpmap, f_texCoord).rgb * 2.0 - 1.0;



	// Compute ambient, diffuse and specular contribution
	vec3 normal = normalize(N);
	vec3 spec = vec3(0.0, 0.0, 0.0);
	vec3 itot = vec3(0.0, 0.0, 0.0);
	vec3 argia = vec3(0.0, 0.0, 0.0);
	vec3 l, r, v, p;
	float d, cspot;

	//vec3 texel = texture2D(bumpmap, f_texCoord).rgb;

	for(int i=0; i<active_lights_n; i++){
		l = normalize(f_lightDirection[i]);

		//Direkzionala
		if(theLights[i].position.w == 0.0f){

			r = normalize(2*(dot(normal, l))*normal - l);

			v = normalize(f_viewDirection);

			spec = pow(max(0, dot(r,v)), theMaterial.shininess) * (theMaterial.specular * theLights[i].specular);
			
			itot = max(0, dot(normal, l)) * ((theMaterial.diffuse * theLights[i].diffuse) + spec);
			
			argia = argia + itot;
		}
		//Posizionala
		else if(theLights[i].cosCutOff == 0.0f){

			p = f_position;
			
			r = normalize(2*(dot(normal, l))*normal - l);
			
			v = normalize(f_viewDirection);

			spec = pow(max(0, dot(r,v)), theMaterial.shininess) * (theMaterial.specular * theLights[i].specular);
			
			d = 1/(theLights[i].attenuation[0] + (theLights[i].attenuation[1] * length(theLights[i].position.xyz - p)) + theLights[i].attenuation[2] * pow(length(theLights[i].position.xyz - p), 2));
			
			itot = (d * max(0, dot(normal, l)) * ((theMaterial.diffuse * theLights[i].diffuse) + spec));
			
			argia = argia + itot;
		}
		//Fokua
		else{
			
			
			r = normalize(2*(dot(normal, l))*normal - l);
			
			v = normalize(f_viewDirection);

			spec = pow(max(0, dot(r,v)), theMaterial.shininess) * (theMaterial.specular * theLights[i].specular);
			
			cspot = max(dot(-1.0 * l, normalize(theLights[i].spotDir)), 0);

			if(cspot > theLights[i].cosCutOff){

				itot = pow(cspot, theLights[i].exponent) * max(0, dot(normal, l)) * ((theMaterial.diffuse * theLights[i].diffuse) + spec);
			
				argia = argia + itot;
			}
		}
	}

	vec3 totala = scene_ambient + argia;

	vec4 f_color = vec4(totala, 1.0);
	

	// Final color
	gl_FragColor = f_color * baseColor;
}
