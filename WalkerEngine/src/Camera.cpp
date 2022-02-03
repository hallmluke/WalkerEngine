#include "walkerpch.h"
#include "Camera.h"

#include "Input.h"
#include "KeyCodes.h"
#include "MouseCodes.h"

namespace Walker {

    Camera::Camera(glm::vec3 position)
    {
        m_Position = position;
        m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        UpdateCameraVectors();
        UpdateViewMatrix();
    }

    Camera::Camera(glm::vec3 position, float yaw, float pitch)
    {
        m_Position = position;
        m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        m_Yaw = yaw;
        m_Pitch = pitch;
        UpdateCameraVectors();
        UpdateViewMatrix();
    }

    Camera::Camera(float posX, float posY, float posZ, float yaw, float pitch)
    {
        m_Position = glm::vec3(posX, posY, posZ);
        m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        m_Yaw = yaw;
        m_Pitch = pitch;
        UpdateCameraVectors();
        UpdateViewMatrix();
    }


    std::vector<glm::vec4> Camera::GetFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view)
    {
        const auto inv = glm::inverse(proj * view);

        std::vector<glm::vec4> frustumCorners;
        frustumCorners.reserve(8);

        for (unsigned int x = 0; x < 2; ++x)
        {
            for (unsigned int y = 0; y < 2; ++y)
            {
                for (unsigned int z = 0; z < 2; ++z)
                {
                    const glm::vec4 pt =
                        inv * glm::vec4(
                            2.0f * x - 1.0f,
                            2.0f * y - 1.0f,
                            2.0f * z - 1.0f,
                            1.0f);
                    frustumCorners.push_back(pt / pt.w);
                }
            }
        }

        return frustumCorners;
    }

    void Camera::ProcessMouseMovement(float xoffset, float yoffset, float deltaTime)
    {
        xoffset *= m_MouseSensitivity;
        yoffset *= m_MouseSensitivity;

        m_Yaw += xoffset * deltaTime;
        m_Pitch += yoffset * deltaTime;

        m_Pitch = std::min(m_Pitch, 89.0f);
        m_Pitch = std::max(m_Pitch, -89.0f);
    }

    void Camera::ProcessKeyboard(float deltaTime)
    {
        float velocity = m_MovementSpeed * deltaTime;

        if (Input::IsKeyPressed(Key::Up) || Input::IsKeyPressed(Key::W)) {
            glm::vec3 movement = m_Front * velocity;
            m_Position += movement;
        }
        if (Input::IsKeyPressed(Key::Down) || Input::IsKeyPressed(Key::S)) {
            glm::vec3 movement = m_Front * velocity;
            m_Position -= movement;
        }
        if (Input::IsKeyPressed(Key::Left) || Input::IsKeyPressed(Key::A)) {
            glm::vec3 movement = m_Right * velocity;
            m_Position -= movement;
        }
        if (Input::IsKeyPressed(Key::Right) || Input::IsKeyPressed(Key::D)) {
            glm::vec3 movement = m_Right * velocity;
            m_Position += movement;
        }
    }

    void Camera::ProcessMouseScroll(float yoffset)
    {
        m_Zoom -= yoffset;
        m_Zoom = std::max(1.0f, m_Zoom);
        m_Zoom = std::min(45.0f, m_Zoom);
    }

    void Camera::UpdateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front.y = sin(glm::radians(m_Pitch));
        front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        m_Front = glm::normalize(front);

        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp)); 
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));
    }

    void Camera::OnUpdate(float deltaTime)
    {
        glm::vec2 mouseOffset = Input::GetMousePosition() - m_LastMousePosition;
        ProcessMouseMovement(mouseOffset.x, mouseOffset.y, deltaTime);
        m_LastMousePosition = Input::GetMousePosition();
        ProcessKeyboard(deltaTime);
        UpdateCameraVectors();
        UpdateViewMatrix();
    }

    void Camera::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(ProcessMouseScroll));
    }

    void Camera::UpdateViewMatrix()
    {
        m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
        m_ViewProjMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    const glm::mat4& Camera::GetViewMatrix() const
    {
        return m_ViewMatrix;
    }

    void Camera::UpdateProjectionMatrix(float aspectRatio)
    {
        UpdateProjectionMatrix(aspectRatio, m_NearPlane, m_FarPlane);
    }

    void Camera::UpdateProjectionMatrix(float aspectRatio, float nearPlane, float farPlane)
    {
        m_ProjectionMatrix = glm::perspective(glm::radians(m_Zoom), aspectRatio, nearPlane, farPlane);
    }

    const glm::mat4& Camera::GetProjectionMatrix() const
    {
        return m_ProjectionMatrix;
    }

    const glm::mat4& Camera::GetViewProjectionMatrix() const
    {
        return m_ViewProjMatrix;
    }

}
