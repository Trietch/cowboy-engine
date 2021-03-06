#pragma once

#include <GL/gl.h>
#include <QOpenGLContext>
#include <QOpenGLWidget>
#include <cstdint>
#include <memory>

#include "Shader.h"
#include "ECS.h"

enum RDenum
{
	RD_COLOR_PASS,
	RD_DEPTHMAP_PASS,
	RD_SHOWNORMAL_PASS,
	RD_NORMAL,
	RD_DEBUG,
	RD_NULL
};


class DrawableComponent;
class CameraComponent;
class LightComponent;
class Shader;
class ECS_Manager;
class SkeletonComponent;

class Renderer
{
public:
	explicit Renderer(std::shared_ptr<ECS_Manager> __ECS_manager);
	void initGl();
	void resizeGl(int __w, int __h);
	void draw(int __qtFramebuffer);

	void drawDrawable(DrawableComponent* __drawableComponent);
	static void initDrawable(DrawableComponent* __drawableComponent);
	static void freeDrawable(DrawableComponent* __drawableComponent);

	void setActiveCamera(CameraComponent* __cameraComponent);
	void addLight(LightComponent* __lightComponent);

	void switchPolygonmode();
	void switchShowNormals();
	CameraComponent* getActiveCamera() const;
	
	// Debug bone
	void setSkeleton(SkeletonComponent* skeleton);
	void switchBone(std::int64_t diff);
	void switchAnim(std::uint8_t dir);
	void resetAnim();


private:
	void _initScreenQuad();
	void _depthMapPass();
	void _colorPass(int __qtFramebuffer);
	void _showNormalsPass();
	void _screenbufferInit(int __w, int __h);
	void _useShader(DrawableComponent* __drawableComponent);
	void _useShaderLightSpace(DrawableComponent* __drawableComponent);

	std::shared_ptr<ECS_Manager> _ECS_manager = nullptr;
	CameraComponent* _activeCamera = nullptr;
	std::vector<LightComponent*> _lights;

	bool _init = false;
	GLenum _polygonMode = GL_FILL;
	int _glWidth = 0;
	int _glHeight = 0;

	// STATES
	RDenum _rdState = RD_NULL;
	LightComponent* _currentLightDepthMapPass = nullptr;

	// Depth Map framebuffer
	Shader* _depthMapShader = nullptr;
	std::vector<GLuint> _depthMapFBOs;
	std::vector<GLuint> _depthMapTextures;
	std::uint64_t _depthShadowWidth = 1024;
	std::uint64_t _depthShadowHeight = 1024;
	float _shadowFarPlane = 64.0f;

	// Screen quad framebuffer
	Shader* _screenquadShader = nullptr;
	GLuint _screenquadFBO;
	GLuint _screenquadRBO;
	GLuint _screenquadVAO;
	GLuint _screenquadVBO;
	GLuint _textureColorbuffer;

	Shader* _normalShader = nullptr;
	bool _showNormals = false;

	// Bone debug
	SkeletonComponent* _skeleton = nullptr;
};

