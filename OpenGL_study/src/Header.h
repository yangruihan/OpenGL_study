#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Common.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Window.h"
#include "Camera.h"
#include "Geometry.h"

#ifdef __APPLE__
#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"
#else
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#endif
