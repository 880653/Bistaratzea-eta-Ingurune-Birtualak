#version 120

attribute vec3 v_position;
attribute vec3 v_normal;
attribute vec2 v_texCoord;

uniform int active_lights_n; // Number of active lights (< MG_MAX_LIGHT)

uniform mat4 modelToCameraMatrix;
uniform mat4 cameraToClipMatrix;
uniform mat4 modelToWorldMatrix;
uniform mat4 modelToClipMatrix;

varying vec3 f_position;
varying vec3 f_viewDirection;
varying vec3 f_normal;
varying vec2 f_texCoord;

void main() {

	f_position = (modelToCameraMatrix * vec4(v_position, 1.0)).xyz;
	f_viewDirection = normalize(vec4(v_position, 0.0) - vec4(theLights[i].position));
	f_texCoord = v_texCoord;
	f_normal = normalize((modelToCameraMatrix * vec4(v_normal, 0.0)).xyz);
	gl_Position = modelToClipMatrix * vec4(v_position, 1.0);
}
