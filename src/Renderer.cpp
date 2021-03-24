#include "Renderer.h"
#include "glm/gtc/constants.hpp"

void Renderer::init(std::shared_ptr<Window> window)
{
	_window = window;

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        ERROR("Failed to initialize glad");
    }

    const WindowInfos windowInfos = _window->windowInfos();
    glViewport(0, 0, windowInfos.x, windowInfos.y);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::prePass()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::drawMesh(Mesh& mesh)
{
    GLenum renderMode = GL_TRIANGLES;
    switch (mesh.renderMode)
    {
        case LINES:
            renderMode = GL_LINES;
            break;
        case TRIANGLES:
            renderMode = GL_TRIANGLES;
            break;
    }
    glBindVertexArray(mesh.VAO);
    glDrawElements(renderMode, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Renderer::useShader(Shader& shader, Camera& camera, Transform& transform)
{
    glm::mat4 model {1.0f};
    model = glm::translate(model, transform.position);
    model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3{transform.scale});
    const WindowInfos windowInfos = _window->windowInfos();
    const glm::mat4 projection = glm::infinitePerspective(glm::radians(camera.FOV), static_cast<float>(windowInfos.x)/windowInfos.y, 0.1f);
    const glm::mat4 view = glm::lookAt(camera.transform.position, camera.transform.position+camera.front, camera.up);

    shader.use();
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
}

void Renderer::initMesh(Mesh& mesh)
{
    glGenVertexArrays(1, &mesh.VAO);

    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.NBO);
    glGenBuffers(1, &mesh.EBO);

    glBindVertexArray(mesh.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
		glBufferData(
			GL_ARRAY_BUFFER,
			mesh.vertices.size()*sizeof(GLfloat),
			mesh.vertices.data(),
			GL_STATIC_DRAW
		);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)nullptr);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.NBO);
		glBufferData(
			GL_ARRAY_BUFFER,
			mesh.normals.size()*sizeof(GLfloat),
			mesh.normals.data(),
			GL_STATIC_DRAW
		);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)nullptr);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			mesh.indices.size()*sizeof(GLuint),
			mesh.indices.data(),
			GL_STATIC_DRAW
		);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLuint), (GLvoid*)nullptr);
		glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    mesh.initialized = true;
}

void Renderer::freeMesh(Mesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.VAO);
    glDeleteBuffers(1, &mesh.VBO);
    glDeleteBuffers(1, &mesh.NBO);
    glDeleteBuffers(1, &mesh.EBO);
}
