#version 120

varying vec2 coord;
uniform vec4 colour;
uniform sampler2D texture;


void main(void) {
    gl_FragColor = vec4(1, 1, 1, texture2D(tex, coord).r) * colour;
}
