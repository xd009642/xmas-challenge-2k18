#version 120
attribute vec4 vert;
varying vec2 tex_coord;  

void main(void) {
    gl_Position = vec4(vert.xy, 0, 1);
    tex_coord = vert.zw;
}
