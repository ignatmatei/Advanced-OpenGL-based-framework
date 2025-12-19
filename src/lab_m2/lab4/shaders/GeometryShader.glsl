#version 330

// Input and output topologies
layout(lines) in;
layout(triangle_strip, max_vertices = 256) out;

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 control_p0, control_p1, control_p2, control_p3;
uniform int no_of_instances;
uniform int no_of_generated_points;
uniform float max_translate;
uniform float max_rotate;

// Input
in int instance[2];

// --------------------------------------------------
// Helpers
// --------------------------------------------------
vec3 rotateY(vec3 point, float u)
{
    float x = point.x * cos(u) - point.z * sin(u);
    float z = point.x * sin(u) + point.z * cos(u);
    return vec3(x, point.y, z);
}

vec3 translateX(vec3 point, float t)
{
    return vec3(point.x + t, point.y, point.z);
}

// --------------------------------------------------
// Cubic Bezier
// --------------------------------------------------
vec3 bezier(float t)
{
    float u = 1.0 - t;

    return  control_p0 * (u * u * u) +
            control_p1 * (3.0 * t * u * u) +
            control_p2 * (3.0 * t * t * u) +
            control_p3 * (t * t * t);
}

// --------------------------------------------------
// Main
// --------------------------------------------------
void main()
{
    const int SURFACE_TYPE_ROTATION     = 0;
    const int SURFACE_TYPE_TRANSLATION  = 1;

    int surface_type = SURFACE_TYPE_ROTATION;

    float du = 1.0 / float(no_of_generated_points - 1);

    int inst = instance[0];

    float alpha = float(inst) / float(max(no_of_instances - 1, 1));

    float angle = alpha * max_rotate;
    float offset = alpha * max_translate;

    for (int i = 0; i < no_of_generated_points - 1; i++)
    {
        float t0 = float(i) * du;
        float t1 = float(i + 1) * du;

        vec3 p0 = bezier(t0);
        vec3 p1 = bezier(t1);

        if (surface_type == SURFACE_TYPE_ROTATION)
        {
            p0 = rotateY(p0, angle);
            p1 = rotateY(p1, angle);
        }
        else
        {
            p0 = translateX(p0, offset);
            p1 = translateX(p1, offset);
        }

        gl_Position = Projection * View * vec4(p0, 1.0);
        EmitVertex();

        gl_Position = Projection * View * vec4(p1, 1.0);
        EmitVertex();
    }

    EndPrimitive();
}
