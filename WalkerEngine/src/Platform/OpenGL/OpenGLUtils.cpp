#include "walkerpch.h"
#include "OpenGLUtils.h"

#include <glad/glad.h>

namespace Walker {

	void OpenGLUtils::LogErrors()
	{
        GLenum err;

        while ((err = glGetError()) != GL_NO_ERROR)
        {
            std::string error;
            switch (err)
            {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            }
            W_CORE_ERROR("OpenGL Error: {0}", error);
        }
	}
}