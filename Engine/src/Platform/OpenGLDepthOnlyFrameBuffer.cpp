#include "engpch.h"


#include "OpenGLDepthOnlyFrameBuffer.h"

#include "Engine/Core.h"
#include "Engine/Log.h"
#include "Engine/Renderer/RenderCommand.h"
#include "glad/glad.h"

namespace Engine
{
	OpenGLDepthOnlyFrameBuffer::OpenGLDepthOnlyFrameBuffer(int bufferSize)
	{
		size = bufferSize;
		// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
		
		glGenFramebuffers(1, &FramebufferName);
		// Always check that our framebuffer is ok
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			ENGINE_CORE_ASSERT(false, "DEPTH ONLY FRAMEBUFFER FAILED TO INITIALIZE");

		// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
		glGenTextures(1, &depthTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, bufferSize, bufferSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float clampColor[4] = { 1,1,1,1 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

		glDrawBuffer(GL_NONE); // No color buffer is drawn to.
		glReadBuffer(GL_NONE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
	}

	void OpenGLDepthOnlyFrameBuffer::Bind()
	{
		glViewport(0, 0, size, size);
		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		glClear(GL_DEPTH_BUFFER_BIT);
			
	}

	void OpenGLDepthOnlyFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	OpenGLDepthOnlyFrameBuffer::~OpenGLDepthOnlyFrameBuffer()
	{
		
	}

	void OpenGLDepthOnlyFrameBuffer::BindDepthTexture()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
	}
}
