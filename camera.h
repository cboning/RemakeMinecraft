#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
    public:
        glm::vec3 cameraPos = glm::vec3(0.0f,80.0f, 0.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        float yaw = -90.0f; // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
        float pitch = 0.0f;
        float fov = 108.0f;

        void moveview(float offsetx, float offsety);
};


void Camera::moveview(float offsetx, float offsety)
{
    yaw += offsetx;
    pitch += offsety;

    if (pitch > 89.9f)
        pitch = 89.9f;
    if (pitch < -89.9f)
        pitch = -89.9f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}