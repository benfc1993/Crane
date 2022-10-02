#pragma once

#include "Crane/Camera/Camera.h"
#include <glm/glm.hpp>


namespace Crane {
    class SceneCamera : public Camera
    {
    public:
        enum class ProjectionType { Perspective = 0, Orthographic = 1 };
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void SetOrthographic(float size, float nearClip, float farclip);
        void SetPerspective(float verticalFov, float nearClip, float farclip);

        void SetViewportSize(uint32_t width, uint32_t height);

        void SetPerspectiveVerticalFov(float verticalFov) { m_PerspectiveVerticalFov = verticalFov; RecalculateProjection(); }
        float GetPerspectiveVerticalFov() const { return m_PerspectiveVerticalFov; }

        void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
        float GetPerspectiveNearClip() const { return m_PerspectiveNear; }

        void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }
        float GetPerspectiveFarClip() const { return  m_PerspectiveFar; }


        float GetOrthographicSize() const { return m_OrthographicSize; }
        void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }

        void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
        float GetOrthographicNearClip() const { return m_OrthographicNear; }

        void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }
        float GetOrthographicFarClip() const { return  m_OrthographicFar; }

        ProjectionType GetProjectionType() const { return m_ProjectionType; }
        void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }
    private:
        void RecalculateProjection();

    private:
        ProjectionType m_ProjectionType = ProjectionType::Orthographic;
        float m_PerspectiveVerticalFov = glm::radians(45.0f);
        float m_PerspectiveNear = 0.001f, m_PerspectiveFar = 1000.0f;

        float m_OrthographicSize = 10.0f;
        float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

        float m_AspectRatio = 0.0f;
    };
}
