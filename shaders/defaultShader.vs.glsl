#version 330 core

layout(location= 0) in vec3 Vpos;

uniform mat4 VM;
uniform mat4 PM;

void main() {
    gl_Position= PM * VM * vec4(Vpos.x, Vpos.y, Vpos.z, 1.0);
}
