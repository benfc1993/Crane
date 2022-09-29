#include "crpch.h"

#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Crane {
    SceneCamera::SceneCamera()
    {
        RecalculateProjection();
    }

    void SceneCamera::SetOrthographic(float size, float nearClip, float farclip)
    {
        m_ProjectionType = ProjectionType::Orthographic;

        m_OrthographicSize = size;
        m_OrthographicNear = nearClip;
        m_OrthographicFar = farclip;

        RecalculateProjection();
    }
    void SceneCamera::SetPerspective(float verticalFov, float nearClip, float farclip)
    {
        m_ProjectionType = ProjectionType::Perspective;

        m_PerspectiveVerticalFov = glm::radians(verticalFov);
        m_PerspectiveNear = nearClip;
        m_PerspectiveNear = farclip;

        RecalculateProjection();
    }

    void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
    {
        m_AspectRatio = (float)width / (float)height;

        RecalculateProjection();
    }

    void SceneCamera::RecalculateProjection()
    {
        if (m_ProjectionType == ProjectionType::Perspective)
        {
            m_Projection = glm::perspective(
                m_PerspectiveVerticalFov,
                m_AspectRatio,
                m_PerspectiveNear,
                m_PerspectiveFar);
        }

        if (m_ProjectionType == ProjectionType::Orthographic)
        {
            float orthoLeft = -m_OrthographicSize * 0.5f * m_AspectRatio;
            float orthoRight = m_OrthographicSize * 0.5f * m_AspectRatio;
            float orthoBottom = -m_OrthographicSize * 0.5f;
            float orthoTop = m_OrthographicSize * 0.5f;

            m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
        }
    }

}

