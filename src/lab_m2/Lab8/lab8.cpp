#include "lab_m2/Lab8/lab8.h"

#include <vector>
#include <iostream>

#include "pfd/portable-file-dialogs.h"

using namespace std;
using namespace m2;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab8::Lab8()
{
    outputMode = 0;
    gpuProcessing = false;
    saveScreenToImage = false;
    window->SetSize(600, 600);
}


Lab8::~Lab8()
{
}


void Lab8::Init()
{
    // Load default texture fore imagine processing
    originalImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "cube", "pos_x.png"), nullptr, "image", true, true);
    processedImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "cube", "pos_x.png"), nullptr, "newImage", true, true);

    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Lab8", "shaders");

    // Create a shader program for particle system
    {
        Shader *shader = new Shader("ImageProcessing");
        shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "FragmentShader.glsl"), GL_FRAGMENT_SHADER);

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


void Lab8::FrameStart()
{
}


void Lab8::Update(float deltaTimeSeconds)
{
    ClearScreen();

    auto shader = shaders["ImageProcessing"];
    shader->Use();

    if (saveScreenToImage)
    {
        window->SetSize(originalImage->GetWidth(), originalImage->GetHeight());
    }

    int flip_loc = shader->GetUniformLocation("flipVertical");
    glUniform1i(flip_loc, saveScreenToImage ? 0 : 1);

    int screenSize_loc = shader->GetUniformLocation("screenSize");
    glm::ivec2 resolution = window->GetResolution();
    glUniform2i(screenSize_loc, resolution.x, resolution.y);

    int outputMode_loc = shader->GetUniformLocation("outputMode");
    glUniform1i(outputMode_loc, outputMode);

    int locTexture = shader->GetUniformLocation("textureImage");
    glUniform1i(locTexture, 0);

    auto textureImage = (gpuProcessing == true) ? originalImage : processedImage;
    textureImage->BindToTextureUnit(GL_TEXTURE0);

    RenderMesh(meshes["quad"], shader, glm::mat4(1));

    if (saveScreenToImage)
    {
        saveScreenToImage = false;

        GLenum format = GL_RGB;
        if (originalImage->GetNrChannels() == 4)
        {
            format = GL_RGBA;
        }

        glReadPixels(0, 0, originalImage->GetWidth(), originalImage->GetHeight(), format, GL_UNSIGNED_BYTE, processedImage->GetImageData());
        processedImage->UploadNewData(processedImage->GetImageData());
        SaveImage("shader_processing_" + std::to_string(outputMode));

        float aspectRatio = static_cast<float>(originalImage->GetWidth()) / originalImage->GetHeight();
        window->SetSize(static_cast<int>(600 * aspectRatio), 600);
    }
}


void Lab8::FrameEnd()
{
    DrawCoordinateSystem();
}


void Lab8::OnFileSelected(const std::string &fileName)
{
    if (fileName.size())
    {
        std::cout << fileName << endl;
        originalImage = TextureManager::LoadTexture(fileName, nullptr, "image", true, true);
        processedImage = TextureManager::LoadTexture(fileName, nullptr, "newImage", true, true);

        float aspectRatio = static_cast<float>(originalImage->GetWidth()) / originalImage->GetHeight();
        window->SetSize(static_cast<int>(600 * aspectRatio), 600);
    }
}


void Lab8::GrayScale()
{
    unsigned int channels = originalImage->GetNrChannels();
    unsigned char* data = originalImage->GetImageData();
    unsigned char* newData = processedImage->GetImageData();

    if (channels < 3)
        return;

    glm::ivec2 imageSize = glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight());

    for (int i = 0; i < imageSize.y; i++)
    {
        for (int j = 0; j < imageSize.x; j++)
        {
            int offset = channels * (i * imageSize.x + j);

            // Reset save image data
            char value = static_cast<char>(data[offset + 0] * 0.2f + data[offset + 1] * 0.71f + data[offset + 2] * 0.07);
            memset(&newData[offset], value, 3);
        }
    }

    processedImage->UploadNewData(newData);
}


void Lab8::SaveImage(const std::string &fileName)
{
    cout << "Saving image! ";
    processedImage->SaveToFile((fileName + ".png").c_str());
    cout << "[Done]" << endl;
}


void Lab8::OpenDialog()
{
    std::vector<std::string> filters =
    {
        "Image Files", "*.png *.jpg *.jpeg *.bmp",
        "All Files", "*"
    };

    auto selection = pfd::open_file("Select a file", ".", filters).result();
    if (!selection.empty())
    {
        std::cout << "User selected file " << selection[0] << "\n";
        OnFileSelected(selection[0]);
    }
}

void Lab8::Blur(int radius)
{

    const int w = originalImage->GetWidth();
    const int h = originalImage->GetHeight();
    const unsigned int channels = originalImage->GetNrChannels();

    unsigned char* src = originalImage->GetImageData();
    unsigned char* dst = processedImage->GetImageData();

    if (channels < 3) return; // need at least RGB

    auto clampi = [](int v, int lo, int hi) {
        return (v < lo) ? lo : (v > hi) ? hi : v;
    };

    const int kernelSize = 2 * radius + 1;
    const int samples = kernelSize * kernelSize;

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int sumR = 0, sumG = 0, sumB = 0, sumA = 0;

            for (int ky = -radius; ky <= radius; ky++)
            {
                int sy = clampi(y + ky, 0, h - 1);

                for (int kx = -radius; kx <= radius; kx++)
                {
                    int sx = clampi(x + kx, 0, w - 1);
                    int off = (sy * w + sx) * (int)channels;

                    sumR += src[off + 0];
                    sumG += src[off + 1];
                    sumB += src[off + 2];
                    if (channels == 4) sumA += src[off + 3];
                }
            }

            int outOff = (y * w + x) * (int)channels;
            dst[outOff + 0] = (unsigned char)(sumR / samples);
            dst[outOff + 1] = (unsigned char)(sumG / samples);
            dst[outOff + 2] = (unsigned char)(sumB / samples);
            if (channels == 4)
                dst[outOff + 3] = (unsigned char)(sumA / samples);
        }
    }

    processedImage->UploadNewData(dst);
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab8::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input
}


void Lab8::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F || key == GLFW_KEY_ENTER || key == GLFW_KEY_SPACE)
    {
        OpenDialog();
    }

    if (key == GLFW_KEY_E)
    {
        gpuProcessing = !gpuProcessing;
        if (gpuProcessing == false)
        {
            outputMode = 0;
        }
        cout << "Processing on GPU: " << (gpuProcessing ? "true" : "false") << endl;
    }

    if (key - GLFW_KEY_0 >= 0 && key <= GLFW_KEY_5)
    {
        outputMode = key - GLFW_KEY_0;

        if (gpuProcessing == false)
        {
           if (outputMode == 0) {
               GrayScale();
           } else if (outputMode == 1) {
            // Blur
            int blurRadius = 3;
            Blur(blurRadius);
           } else if (outputMode == 2) {
        }
    }

    if (key == GLFW_KEY_S && mods & GLFW_MOD_CONTROL)
    {
        if (!gpuProcessing)
        {
            SaveImage("processCPU_" + std::to_string(outputMode));
        } else {
            saveScreenToImage = true;
        }
    }
}

}
void Lab8::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab8::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab8::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab8::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab8::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab8::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
