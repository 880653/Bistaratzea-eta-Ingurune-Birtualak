#version 120

uniform mat4 modelToCameraMatrix; // M
uniform mat4 cameraToClipMatrix;  // P
// uniform mat4 modelToClipMatrix;   // MP

// uniform int active_lights_n; // < MG_MAX_LIGHT

// uniform struct light_t {
//   vec4 position;   // Camera space
//   vec4 ambient;
//   vec4 diffuse;
//   vec4 specular;
// } theLights[8];    // MG_MAX_LIGHTS

// uniform struct material_t {
//  vec4  ambient;
//  vec4  diffuse;
//  vec4  specular;
//  float shininess;
// } theMaterial;

attribute vec4 v_position;
// attribute vec3 v_normal;
// attribute vec2 v_texCoord;

varying vec4 f_color;

void main() {

  f_color = vec4(1,1,1,1);
  gl_Position = cameraToClipMatrix * modelToCameraMatrix * vec4(V_position, 1);
  //gl_Position = v_position;
}
