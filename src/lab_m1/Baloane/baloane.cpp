#include "lab_m1/Baloane/baloane.h"
#include "lab_m1/Baloane/object2D.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about FrameStart, Update, FrameEnd
 *  and the order in which they are called, see world.cpp.
 */


Baloane::Baloane()
{
}


Baloane::~Baloane()
{
}


void Baloane::Init()
{
    GetSceneCamera()->SetPosition(glm::vec3(0, 15, 15));
    GetSceneCamera()->Update();

    std::vector<std::string> textureKeys = {
      "pepsi", "red", "fanta", "sprite", "coca", "mountain", "monster"
    };

    // Load height map texture
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "heightmap256.png").c_str(), GL_REPEAT);
        mapTextures["heightMap"] = texture;

        // If heightmap is not found, print an error message
        if (texture->GetTextureID() == 0)
        {
            std::cout << "Heightmap texture not found!" << std::endl;
        }
        else
        {
            std::cout << "Heightmap texture loaded!" << std::endl;
        }
    }

    // Create a sphere mesh for the sun
    {
        Mesh* mesh = new Mesh("sun");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane"), "plane256.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Textures
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "crate.jpg").c_str(), GL_REPEAT);
        mapTextures["crate"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "textures", "earth.png").c_str(), GL_REPEAT);
        mapTextures["earth"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "textures", "coca.png").c_str(), GL_REPEAT);
        mapTextures["coca"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "textures", "fanta.png").c_str(), GL_REPEAT);
        mapTextures["fanta"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "textures", "monster.png").c_str(), GL_REPEAT);
        mapTextures["monster"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "textures", "mountain.png").c_str(), GL_REPEAT);
        mapTextures["mountain"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "textures", "pepsi.png").c_str(), GL_REPEAT);
        mapTextures["pepsi"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "textures", "red.png").c_str(), GL_REPEAT);
        mapTextures["red"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "textures", "sprite.jpeg").c_str(), GL_REPEAT);
        mapTextures["sprite"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "textures", "monke.jpg").c_str(), GL_REPEAT);
        mapTextures["monke"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "textures", "mylove.jpg").c_str(), GL_REPEAT);
        mapTextures["mylove"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "textures", "poh.png").c_str(), GL_REPEAT);
        mapTextures["poh"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "textures", "target.jpg").c_str(), GL_REPEAT);
        mapTextures["target"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "textures", "topg.jpg").c_str(), GL_REPEAT);
        mapTextures["topg"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "textures", "warcriminal.jpg").c_str(), GL_REPEAT);
        mapTextures["warcriminal"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "textures", "weed.jpg").c_str(), GL_REPEAT);
        mapTextures["weed"] = texture;
    }


    // Balloon shader
    {
        Shader* shader = new Shader("BalloonShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "shaders", "VertexShaderBalloon.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "shaders", "FragmentShaderBalloon.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Sphere shader
    {
        Shader* shader = new Shader("SphereShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "shaders", "VertexShaderSphere.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "shaders", "FragmentShaderSphere.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Sun shader
    {
        Shader* shader = new Shader("SunShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "shaders", "VertexShaderSun.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "shaders", "FragmentShaderSun.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Terrain shader
    {
        Shader* shader = new Shader("TerrainShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "shaders", "VertexShaderTerrain.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Baloane", "shaders", "FragmentShaderTerrain.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }


    // Light & material properties
    {
        lightPosition = glm::vec3(0, 50, -50);
        lightDirection = glm::vec3(0, -1, 0);
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;

        typeOfLight = 0;
        angleOX = 0;
        angleOY = 0;
        cutoffAngle = 25.f;
    }

    // Initialize balloons
    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < 5; ++i) {
        Balloon balloon;
        balloon.initialPosition = glm::vec3(0, 0, 0);
        balloon.radius = static_cast<float>(rand() % 13 + 2);
        balloon.height = static_cast<float>(rand() % 2 + 9);
        balloon.rotationSpeed = static_cast<float>(rand() % 3 + 20);
        balloon.oscillationAmplitude = static_cast<float>(rand() % 2 + 1);
        balloon.oscillationSpeed = static_cast<float>(rand() % 6 + 5) / 10.0f;
        balloon.angle = static_cast<float>(rand() % 360);
        // Random texture
        balloon.texture = mapTextures[textureKeys[rand() % textureKeys.size()]];
        balloons.push_back(balloon);
    }
}


void Baloane::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Baloane::Update(float deltaTimeSeconds)
{
    // Render the sun
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 50, -50)); // Position far away in the sky
        modelMatrix = glm::scale(modelMatrix, glm::vec3(10.0f)); // Large size for the sun
        RenderSimpleMesh(meshes["sun"], shaders["SunShader"], modelMatrix, glm::vec3(1.0f, 0.5f, 0.0f)); // Yellow-orange color
    }

    // Render ground
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(1, 0.01f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(50, 5, 50));
        RenderSimpleMesh(meshes["plane"], shaders["TerrainShader"], modelMatrix, glm::vec3(1,1,0.45), mapTextures["earth"]);
    }

    // Update balloons
    for (auto& balloon : balloons) {
        balloon.angle += balloon.rotationSpeed * deltaTimeSeconds;
        if (balloon.angle > 360.0f) {
            balloon.angle -= 360.0f;
        }

        float x = balloon.radius * cos(glm::radians(balloon.angle));
        float z = balloon.radius * sin(glm::radians(balloon.angle));
        float y = balloon.height + balloon.oscillationAmplitude * sin(balloon.oscillationSpeed * glfwGetTime());

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.8f));
        RenderSimpleMesh(meshes["sphere"], shaders["SphereShader"], modelMatrix, glm::vec3(1, 0, 0), balloon.texture);

        // Render the box
        glm::mat4 modelMatrix1 = glm::mat4(1);
        modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(x, y - 2.3f, z));
        modelMatrix1 = glm::scale(modelMatrix1, glm::vec3(1, 0.5f, 0.5f));
        RenderSimpleMesh(meshes["box"], shaders["BalloonShader"], modelMatrix1, glm::vec3(1, 0, 0), mapTextures["crate"]);

        // Render the sticks
        glm::mat4 modelMatrix2 = glm::mat4(1);
        modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(x - 0.47f, y - 1.8f, z - 0.22f));
        modelMatrix2 = glm::scale(modelMatrix2, glm::vec3(0.05f, 1.0f, 0.05f));
        RenderSimpleMesh(meshes["box"], shaders["BalloonShader"], modelMatrix2, glm::vec3(1, 0, 0), mapTextures["crate"]);

        glm::mat4 modelMatrix3 = glm::mat4(1);
        modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(x + 0.47f, y - 1.8f, z - 0.22f));
        modelMatrix3 = glm::scale(modelMatrix3, glm::vec3(0.05f, 1.0f, 0.05f));
        RenderSimpleMesh(meshes["box"], shaders["BalloonShader"], modelMatrix3, glm::vec3(1, 0, 0), mapTextures["crate"]);

        glm::mat4 modelMatrix4 = glm::mat4(1);
        modelMatrix4 = glm::translate(modelMatrix4, glm::vec3(x - 0.47f, y - 1.8f, z + 0.22f));
        modelMatrix4 = glm::scale(modelMatrix4, glm::vec3(0.05f, 1.0f, 0.05f));
        RenderSimpleMesh(meshes["box"], shaders["BalloonShader"], modelMatrix4, glm::vec3(1, 0, 0), mapTextures["crate"]);

        glm::mat4 modelMatrix5 = glm::mat4(1);
        modelMatrix5 = glm::translate(modelMatrix5, glm::vec3(x + 0.47f, y - 1.8f, z + 0.22f));
        modelMatrix5 = glm::scale(modelMatrix5, glm::vec3(0.05f, 1.0f, 0.05f));
        RenderSimpleMesh(meshes["box"], shaders["BalloonShader"], modelMatrix5, glm::vec3(1, 0, 0), mapTextures["crate"]);
    }
}


void Baloane::FrameEnd()
{
    DrawCoordinateSystem();
}


void Baloane::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, Texture2D* texture1)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light properties
    int light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

    int light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color)
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    GLint type = glGetUniformLocation(shader->program, "type_of_light");
    glUniform1i(type, typeOfLight);

    GLint cut_off_angle = glGetUniformLocation(shader->program, "cut_off_angle");
    glUniform1f(cut_off_angle, cutoffAngle);

    // Bind the texture if available
    if (texture1)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }

    // Height map texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mapTextures["heightMap"]->GetTextureID());
    glUniform1i(glGetUniformLocation(shader->program, "height_map"), 1);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

    // Set shader uniforms for the second light properties
    int light_position2 = glGetUniformLocation(shader->program, "light_position2");
    glUniform3f(light_position2, lightPosition2.x, lightPosition2.y, lightPosition2.z);

    int light_direction2 = glGetUniformLocation(shader->program, "light_direction2");
    glUniform3f(light_direction2, lightDirection2.x, lightDirection2.y, lightDirection2.z);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see input_controller.h.
 */


void Baloane::OnInputUpdate(float deltaTime, int mods)
{
}


void Baloane::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F)
    {
        if (typeOfLight == 0)
            typeOfLight = 1;
        else
            typeOfLight = 0;
    }
}


void Baloane::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Baloane::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Baloane::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Baloane::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Baloane::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Baloane::OnWindowResize(int width, int height)
{
}
