#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec2 texcoord;

out vec4 vPosition;
out vec3 my_color;

out vec3 world_position;
out vec3 world_normal;

void main()
{
	// TODO : pass v_texture_coord as output to Fragment Shader
	
	world_position = (Model * vec4(v_position,1)).xyz;
	world_normal = normalize( mat3(Model) * v_normal );

	texcoord = v_texture_coord;
	my_color = v_color;
	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
