#version 120

varying vec2 tex_coord;
uniform vec4 colour;
uniform sampler2D texture;


void main(void) {
    gl_FragColor = vec4(1, 1, 1, texture2D(texture, tex_coord).r) * colour;
}
