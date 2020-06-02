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

void main() {

	// Base color
	vec4 baseColor = texture2D(texture0, f_texCoord);

	// Decode the tangent space normal (from[0..1] to [-1..+1])
	vec3 N = texture2D(bumpmap, f_texCoord).rgb * 2.0 - 1.0;

	// Compute ambient, diffuse and specular contribution
	vec4 f_color = vec4(1.0);
	
	// Final color
	gl_FragColor = f_color * baseColor;
}
