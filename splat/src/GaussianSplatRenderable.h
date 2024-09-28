#pragma once
#include "GaussianData.h"
#include "tools/shader.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

class CGaussianSplatRenderable {
public:
	CGaussianSplatRenderable();
	~CGaussianSplatRenderable();
	void setAttenuation(bool vAttenuationMode) {
		m_AttenuationMode = vAttenuationMode;
	}
	void setModelMat(const glm::mat4& vModelMat) {
		m_ModelMat = vModelMat;
	}
	void loadFile(const std::string& vFilename);
	void changeMode(bool vShouldrawPointCloud);
	void render(float vDeltaTime, const glm::vec4 vFocalAndTan, const glm::mat4& vProjectionMat, const glm::mat4& vViewMat, const glm::vec4& vViewport, const glm::vec2& vNearFar, float vScale = 1.0);
private:
	CGaussianData m_Data;
	unsigned int m_VAO;
	unsigned int m_VBOPos;
	unsigned int m_VBOSH;
	unsigned int m_VBOCov;
	unsigned int m_EBO;
	unsigned int m_PositionSSBO;
	unsigned int m_DepthSSBO;
	unsigned int m_DepthSSBO2;
	unsigned int m_IndexSSBO;
	unsigned int m_IndexSSBO2;
	unsigned int m_HistogramSSBO;
	unsigned int m_AtomicCounter;
	unsigned int m_Width;
	unsigned int m_Height;
	unsigned int m_ColorTexture;
	unsigned int m_DepthTexture;
	unsigned int framebuffer;
	unsigned int quadVAO, quadVBO;
	unsigned int m_NumBlocksPerWorkgroup = 32;
	float m_Scale = 1.0;
	bool m_AttenuationMode = true;
	bool m_IsPointCloud = false;
	std::vector<uint32_t> m_CounterVec;
	glm::mat4 m_ModelMat;
	Shader screenShader;
	Shader depthShader;
	Shader m_SplatProgram;
	Shader m_PresortProgram;
	Shader m_HistogramProgram;
	Shader m_SortProgram;
	uint32_t LOCAL_SIZE;
	uint32_t NUM_WORKGROUPS;
	uint32_t RADIX_SORT_BINS;
};