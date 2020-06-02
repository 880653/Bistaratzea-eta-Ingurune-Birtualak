#version 120

varying vec3 f_texCoord;
uniform samplerCube cubemap;

// To sample a texel from a cubemap, use "textureCube" function:
//
// vec4 textureCube(samplerCube sampler, vec3 coord);

void main() {
	gl_FragColor = vec4(1.0);
}
