#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output values to fragment shader
out vec3 world_position;
out vec3 world_normal;
out vec2 texcoord;

void main()
{
    vec3 modified_position = v_position;

    // Pass v_texture_coord as output to fragment shader
    texcoord = v_texture_coord;

    // Apply stretching deformation to the lower half of the sphere
    if (v_position.y >= -0.5 && v_position.y < 0.0) {
        modified_position.y *= 2.0; // Stretch the y-coordinate
    }

    // Transform the modified position
    world_position = (Model * vec4(modified_position, 1)).xyz;
    world_normal = normalize(mat3(Model) * normalize(v_normal));

    gl_Position = Projection * View * Model * vec4(modified_position, 1.0);
}