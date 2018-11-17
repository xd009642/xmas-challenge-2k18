#version 120

in vec3 vert;

void main(void) {
    gl_Position = vec3(vert, 1);
}
