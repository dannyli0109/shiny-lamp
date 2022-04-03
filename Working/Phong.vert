#version 450

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec3 a_tangent;
layout (location = 3) in vec3 a_bitangent;
layout (location = 4) in vec4 a_color;
layout (location = 5) in vec2 a_uv;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;

out vec2 v_uv;
out mat3 v_tbn;
out vec3 v_modelPosition;


void main()
{
    vec3 t = normalize(vec3(u_modelMatrix * vec4(a_tangent, 0.0)));
    vec3 b = normalize(vec3(u_modelMatrix * vec4(a_bitangent, 0.0)));
    vec3 n = normalize(vec3(transpose(inverse(u_modelMatrix)) * vec4(a_normal, 0.0)));

    v_tbn = mat3(t, b, n);
    v_modelPosition = (u_modelMatrix * vec4(a_position, 1)).xyz;
    v_uv = a_uv;    
    gl_Position = (u_projectionMatrix * u_viewMatrix * u_modelMatrix) * vec4(a_position, 1);
}