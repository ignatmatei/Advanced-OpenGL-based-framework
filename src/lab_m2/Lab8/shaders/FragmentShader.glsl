#version 330

// Input
in vec2 texture_coord;

// Uniforms
uniform sampler2D textureImage;
uniform ivec2 screenSize;
uniform int flipVertical;
uniform int outputMode = 4;   // see modes above

// Output
layout(location = 0) out vec4 out_color;

// Flip texture if needed
vec2 textureCoord = vec2(
    texture_coord.x,
    (flipVertical != 0) ? 1.0 - texture_coord.y : texture_coord.y
);

float luminance(vec3 color)
{
    return 0.21 * color.r + 0.71 * color.g + 0.07 * color.b;
}

vec4 grayscale()
{
    vec4 color = texture(textureImage, textureCoord);
    float gray = luminance(color.rgb);
    return vec4(gray, gray, gray, 1.0);
}

vec4 blur(int blurRadius)
{
    vec2 texel = 1.0 / vec2(screenSize);
    vec4 sum = vec4(0.0);

    for (int i = -blurRadius; i <= blurRadius; i++)
    {
        for (int j = -blurRadius; j <= blurRadius; j++)
        {
            sum += texture(textureImage, textureCoord + vec2(i, j) * texel);
        }
    }

    float samples = float((2 * blurRadius + 1) * (2 * blurRadius + 1));
    return sum / samples;
}

float sobelIntensity(sampler2D tex, vec2 coord)
{
    vec2 texel = 1.0 / vec2(screenSize);

    float Gx[9] = float[](
        -1,  0,  1,
        -2,  0,  2,
        -1,  0,  1
    );

    float Gy[9] = float[](
        -1, -2, -1,
         0,  0,  0,
         1,  2,  1
    );

    vec2 offsets[9] = vec2[](
        vec2(-1, -1), vec2(0, -1), vec2(1, -1),
        vec2(-1,  0), vec2(0,  0), vec2(1,  0),
        vec2(-1,  1), vec2(0,  1), vec2(1,  1)
    );

    float sx = 0.0;
    float sy = 0.0;

    for (int i = 0; i < 9; i++)
    {
        float gray = luminance(
            texture(tex, coord + offsets[i] * texel).rgb
        );

        sx += gray * Gx[i];
        sy += gray * Gy[i];
    }

    return length(vec2(sx, sy));
}

vec4 simplifiedCanny()
{
    // 1. Blur
    vec4 blurred = blur(3);

    // 2. Grayscale
    float gray = luminance(blurred.rgb);

    // Fake grayscale texture sampling for Sobel
    // (reusing original texture coordinates)
    float edgeStrength = sobelIntensity(textureImage, textureCoord);

    // 3. Threshold
    float threshold = 0.3;   
    float edge = edgeStrength > threshold ? 1.0 : 0.0;

    return vec4(edge, edge, edge, 1.0);
}


vec4 medianHuang(int radius)
{
    vec2 texel = 1.0 / vec2(screenSize);
    int hist[256];
    for (int i = 0; i < 256; i++)
        hist[i] = 0;
    int count = 0;

    for (int y = -radius; y <= radius; y++)
    {
        for (int x = -radius; x <= radius; x++)
        {
            vec3 col = texture(textureImage,
                               textureCoord + vec2(x, y) * texel).rgb;

            float gray = clamp(luminance(col), 0.0, 1.0);
            int intensity = int(gray * 255.0 + 0.5);

            hist[intensity]++;
            count++;
        }
    }

    int acc = 0;
    int median = 0;
    int target = count / 2;

    for (int i = 0; i < 256; i++)
    {
        acc += hist[i];
        if (acc >= target)
        {
            median = i;
            break;
        }
    }

    float m = float(median) / 255.0;
    return vec4(m, m, m, 1.0);
}

void main()
{
    switch (outputMode)
    {
        case 1:
            out_color = grayscale();
            break;

        case 2:
            out_color = blur(1);
            break;

        case 3:
        {
            float e = sobelIntensity(textureImage, textureCoord);
            out_color = vec4(e, e, e, 1.0);
            break;
        }

        case 4:
            out_color = simplifiedCanny();
            break;
        case 5:
            out_color = medianHuang(1);
            break;
        default:
            out_color = texture(textureImage, textureCoord);
            break;
    }
}
