#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform sampler2D height_map;

// Output values to fragment shader
out vec3 world_position;
out vec3 world_normal;
out vec2 texture_coord;

void main()
{
    // Sample the height map texture
    float height = texture(height_map, v_texture_coord).r;

    // Adjust the vertex position based on the height map
    vec3 adjusted_position = v_position;
    adjusted_position.y = height;

    // Calculate world position and normal
    world_position = (Model * vec4(adjusted_position, 1)).xyz;
    world_normal = normalize(mat3(Model) * normalize(v_normal));

    // Texture coordinates
    texture_coord = v_texture_coord;

    // Calculate final vertex position
    gl_Position = Projection * View * Model * vec4(adjusted_position, 1.0);
}