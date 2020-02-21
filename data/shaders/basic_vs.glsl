#version 410

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 texture_coord;

uniform mat4 model, view, proj;

//out vec3 normal;
//out vec2 st;

out float dist;

void main() {
//    st = texture_coord;
//    normal = vertex_normal;
//    gl_Position = proj * view * vec4 (vertex_position, 1.0);
    gl_Position = proj * view * model * vec4(vertex_position, 1.0);
    dist = vertex_position.z;//1.0 - (-pos_eye.z / 10.0);
}