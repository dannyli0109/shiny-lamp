#version 450

out vec4 f_color;

in vec2 v_uv;
in mat3 v_tbn;
in vec3 v_modelPosition;

uniform sampler2D u_diffuseTexture;
uniform bool u_hasNormal;
uniform sampler2D u_normalTexture;
uniform sampler2D u_specularTexture;
uniform vec3 u_tint;
uniform vec3 u_cameraPos;

void main()
{
	vec4 normalTap = texture(u_normalTexture, v_uv);
	vec3 mapNormal = normalTap.xyz * 2 - 1;
	vec3 normal = u_hasNormal ? normalize(vec4(v_tbn * mapNormal, 0)).xyz : v_tbn[2];
	vec3 lightPos = vec3(0, 3, 5);
	vec3 kd = texture(u_diffuseTexture, v_uv).xyz;
	vec3 ks = texture(u_specularTexture, v_uv).xyz;
	
	vec3 toLight = normalize(lightPos - v_modelPosition);
	float d2 = dot(lightPos - v_modelPosition, lightPos - v_modelPosition); 
	float cosTheta = dot(normal, toLight);
	vec3 ld = kd * (50 / d2) * max(0, cosTheta);

	vec3 toCamera = normalize(u_cameraPos - v_modelPosition);
	vec3 h = normalize(toCamera + toLight);
	float spec = dot(normal, h);
	vec3 ls = ks * (50 / d2) * pow(max(0, spec), 150);
	f_color = vec4(u_tint, 1) * vec4(ld + ls, 1);
}