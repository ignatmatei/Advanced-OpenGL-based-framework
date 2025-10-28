#version 330

// Input
in vec3 world_position;
in vec3 world_normal;
in vec2 texcoord;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;
uniform vec3 light_position2;
uniform vec3 light_direction2;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// Declare any other uniforms
uniform vec3 object_color;
uniform int type_of_light;
uniform float cut_off_angle;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    // Define ambient, diffuse and specular light components
    vec3 N = normalize(world_normal);
    vec3 L = normalize(light_position - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);
    vec3 L2 = normalize(light_position2 - world_position);
    vec3 H2 = normalize(L2 + V);

    float ambient_light = 0.25;
    float diffuse_light = material_kd * max(dot(normalize(N), L), 0);
    float specular_light = 0;
    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(normalize(N), H), 0), material_shininess);
    }

    float diffuse_light2 = material_kd * max(dot(normalize(N), L2), 0);
    float specular_light2 = 0;
    if (diffuse_light2 > 0)
    {
        specular_light2 = material_ks * pow(max(dot(normalize(N), H2), 0), material_shininess);
    }

    float light = 0;

    if (type_of_light == 1)
    {
        float cut_off_rad = radians(cut_off_angle);
        float spot_light = dot(-L, light_direction);
        float spot_light_limit = cos(cut_off_rad);

        if (spot_light > spot_light_limit)
        {
            // Quadratic attenuation
            float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
            float light_att_factor = pow(linear_att, 2);
            light = ambient_light + light_att_factor * (diffuse_light + specular_light);
        }
        else
        {
            light = ambient_light;  // There is no spot light, but there is light from other objects
        }
    }
    else
    {
        light = ambient_light + diffuse_light + specular_light;
    }

    // Add contribution of the second light source
    light += diffuse_light2 + specular_light2;

    vec3 colour = object_color;

    out_color = vec4(colour, 1);
}