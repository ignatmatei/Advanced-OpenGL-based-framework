#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"


namespace m1
{
    class Baloane : public gfxc::SimpleScene
    {
    public:
        Baloane();
        ~Baloane();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color = glm::vec3(1), Texture2D* texture1 = nullptr);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        std::unordered_map<std::string, Texture2D *> mapTextures;

        glm::vec3 lightPosition;
        glm::vec3 lightDirection;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;

        GLint typeOfLight;
        GLfloat angleOX, angleOY;
        GLfloat cutoffAngle;

        glm::vec3 lightPosition2;
        glm::vec3 lightDirection2;

        struct Balloon {
            glm::vec3 initialPosition;
            float radius;
            float height;
            float rotationSpeed;
            float oscillationAmplitude;
            float oscillationSpeed;
            float angle;
            Texture2D* texture;
        };
        std::vector<Balloon> balloons;
    };
}   // namespace m1