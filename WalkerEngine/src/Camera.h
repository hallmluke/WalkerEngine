#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Events/Event.h"
#include "Events/MouseEvent.h"

namespace Walker {

    class Camera
    {
    public:
        Camera(glm::vec3 position, float viewportWidth, float viewportHeight);
        Camera(glm::vec3 position, float yaw, float pitch, float viewportWidth, float viewportHeight);
        Camera(float posX, float posY, float posZ, float yaw, float pitch, float viewportWidth, float viewportHeight);

        void OnUpdate(float deltaTime);
        void OnEvent(Event& e);

        void UpdateViewMatrix();
        const glm::mat4& GetViewMatrix() const;

        void UpdateProjectionMatrix();
        void UpdateProjectionMatrix(float nearPlane, float farPlane);
        const glm::mat4& GetProjectionMatrix() const;

        const glm::mat4& GetViewProjectionMatrix() const;

        std::vector<glm::vec4> GetFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view);

    private:
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewProjMatrix;

        glm::vec3 m_Position;
        glm::vec3 m_Front;
        glm::vec3 m_Up;
        glm::vec3 m_Right;
        glm::vec3 m_WorldUp;

        float m_Yaw = -90.0f;
        float m_Pitch = 0.0f;

        float m_MovementSpeed = 5.0f;
        float m_MouseSensitivity = 3.0f;
        float m_Zoom = 45.0f;

        float m_NearPlane = 0.1f;
        float m_FarPlane = 100.0f;

        float m_ViewportWidth;
        float m_ViewportHeight;

        glm::vec2 m_LastMousePosition = { 0, 0 };

        void UpdateCameraVectors();
        void ProcessKeyboard(float deltaTime);
        void ProcessMouseMovement(float xoffset, float yoffset, float deltaTime);
        bool ProcessMouseScroll(MouseScrolledEvent& e);

    };
}