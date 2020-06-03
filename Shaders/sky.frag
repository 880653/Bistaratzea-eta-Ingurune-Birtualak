#version 120

varying vec3 f_texCoord;
uniform samplerCube cubemap;

// To sample a texel from a cubemap, use "textureCube" function:
//
// vec4 textureCube(samplerCube sampler, vec3 coord);

void main() {

	vec4 texColor = textureCube(cubemap, f_texCoord);

	gl_FragColor = texColor;
}
