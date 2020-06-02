#version 120

// Bump mapping with many lights.

// all attributes in model space
attribute vec3 v_position;
attribute vec3 v_normal;
attribute vec2 v_texCoord;
attribute vec3 v_TBN_t;
attribute vec3 v_TBN_b;

uniform mat4 modelToCameraMatrix;
uniform mat4 modelToWorldMatrix;
uniform mat4 cameraToClipMatrix;
uniform mat4 modelToClipMatrix;

uniform int active_lights_n; // Number of active lights (< MG_MAX_LIGHT)

uniform struct light_t {
	vec4 position;    // Camera space
	vec3 diffuse;     // rgb
	vec3 specular;    // rgb
	vec3 attenuation; // (constant, lineal, quadratic)
	vec3 spotDir;     // Camera space
	float cosCutOff;  // cutOff cosine
	float exponent;
} theLights[4];     // MG_MAX_LIGHTS

// All bump computations are performed in tangent space; therefore, we need to
// convert all light (and spot) directions and view directions to tangent space
// and pass them the fragment shader.

// output varying
varying vec2 f_texCoord;
varying vec3 f_viewDirection;     // tangent space
varying vec3 f_lightDirection[4]; // tangent space
varying vec3 f_spotDirection[4];  // tangent space
varying vec3 f_position;

void main() {
	// get 3x3 modelview matrix
	mat3 MV3x3 = mat3(modelToCameraMatrix);

	// object space -> camera space
	vec3 n = MV3x3 * v_normal;
	vec3 t = MV3x3 * v_TBN_t;
	vec3 b = MV3x3 * v_TBN_b;

	f_position = MV3x3 * v_position;
	vec3 f_texCoord3 = MV3x3 * vec3(v_texCoord, 1.0);
	f_texCoord = vec2(f_texCoord3[0], f_texCoord3[1]);

	// matrix to transform from camera space to tangent space
	mat3 cameraToTangent = transpose(mat3(t, b, n));


	vec3 l;
	
	for(int i=0; i<active_lights_n; i++){

		//Direkzionala
		if(theLights[i].position.w == 0.0f){

			l = -1.0 * theLights[i].position.xyz;

		}
		//Posizionala
		else if(theLights[i].cosCutOff == 0.0f){

			l = theLights[i].position.xyz - f_position;
			
		}
		//Fokua
		else{

			l = theLights[i].position.xyz - f_position;
			
		}
		// camera space -> tangent space
		f_lightDirection[i] = cameraToTangent * l;

		f_viewDirection = (cameraToTangent * vec3(-1.0 * v_position));

		// f_spotDirection[i]



	}

	
	gl_Position = modelToClipMatrix * vec4(v_position, 1.0);

}
