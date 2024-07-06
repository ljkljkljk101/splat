#include "GaussianSplatRenderable.h"

void CGaussianSplatRenderable::loadFile(const std::string& vFilename) {
	m_Data.readFromFile(vFilename);

	NUM_WORKGROUPS = (m_Data.getNum() + m_NumBlocksPerWorkgroup * LOCAL_SIZE - 1) / (m_NumBlocksPerWorkgroup * LOCAL_SIZE);
	m_CounterVec.resize(1, 0);

	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);
	{
		//init VAO
		int loc;

		glGenBuffers(1, &m_VBOPos);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOPos);
		glBufferData(GL_ARRAY_BUFFER, sizeof(SGaussianShaderDataPos) * m_Data.getNum(), m_Data.getDataPos().data(), GL_STATIC_DRAW);

		loc = glGetAttribLocation(m_SplatProgram.ID, "position");
		glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, sizeof(SGaussianShaderDataPos), 0);
		glEnableVertexAttribArray(loc);


		glGenBuffers(1, &m_VBOSH);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOSH);
		glBufferData(GL_ARRAY_BUFFER, sizeof(SGaussianShaderDataSH) * m_Data.getNum(), m_Data.getDataSH().data(), GL_STATIC_DRAW);

		loc = glGetAttribLocation(m_SplatProgram.ID, "r_sh0");
		glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, sizeof(SGaussianShaderDataSH), (void*)offsetof(SGaussianShaderDataSH, r_sh0Vec));
		glEnableVertexAttribArray(loc);
		loc = glGetAttribLocation(m_SplatProgram.ID, "r_sh1");
		glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, sizeof(SGaussianShaderDataSH), (void*)offsetof(SGaussianShaderDataSH, r_sh1Vec));
		glEnableVertexAttribArray(loc);

		loc = glGetAttribLocation(m_SplatProgram.ID, "r_sh2");
		glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, sizeof(SGaussianShaderDataSH), (void*)offsetof(SGaussianShaderDataSH, r_sh2Vec));
		glEnableVertexAttribArray(loc);

		loc = glGetAttribLocation(m_SplatProgram.ID, "r_sh3");
		glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, sizeof(SGaussianShaderDataSH), (void*)offsetof(SGaussianShaderDataSH, r_sh3Vec));
		glEnableVertexAttribArray(loc);

		loc = glGetAttribLocation(m_SplatProgram.ID, "g_sh0");
		glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, sizeof(SGaussianShaderDataSH), (void*)offsetof(SGaussianShaderDataSH, g_sh0Vec));
		glEnableVertexAttribArray(loc);
		loc = glGetAttribLocation(m_SplatProgram.ID, "g_sh1");
		glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, sizeof(SGaussianShaderDataSH), (void*)offsetof(SGaussianShaderDataSH, g_sh1Vec));
		glEnableVertexAttribArray(loc);

		loc = glGetAttribLocation(m_SplatProgram.ID, "g_sh2");
		glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, sizeof(SGaussianShaderDataSH), (void*)offsetof(SGaussianShaderDataSH, g_sh2Vec));
		glEnableVertexAttribArray(loc);

		loc = glGetAttribLocation(m_SplatProgram.ID, "g_sh3");
		glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, sizeof(SGaussianShaderDataSH), (void*)offsetof(SGaussianShaderDataSH, g_sh3Vec));
		glEnableVertexAttribArray(loc);


		loc = glGetAttribLocation(m_SplatProgram.ID, "b_sh0");
		glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, sizeof(SGaussianShaderDataSH), (void*)offsetof(SGaussianShaderDataSH, b_sh0Vec));
		glEnableVertexAttribArray(loc);
		loc = glGetAttribLocation(m_SplatProgram.ID, "b_sh1");
		glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, sizeof(SGaussianShaderDataSH), (void*)offsetof(SGaussianShaderDataSH, b_sh1Vec));
		glEnableVertexAttribArray(loc);

		loc = glGetAttribLocation(m_SplatProgram.ID, "b_sh2");
		glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, sizeof(SGaussianShaderDataSH), (void*)offsetof(SGaussianShaderDataSH, b_sh2Vec));
		glEnableVertexAttribArray(loc);

		loc = glGetAttribLocation(m_SplatProgram.ID, "b_sh3");
		glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, sizeof(SGaussianShaderDataSH), (void*)offsetof(SGaussianShaderDataSH, b_sh3Vec));
		glEnableVertexAttribArray(loc);


		glGenBuffers(1, &m_VBOCov);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOCov);
		glBufferData(GL_ARRAY_BUFFER, sizeof(SGaussianShaderDataCov) * m_Data.getNum(), m_Data.getDataCov().data(), GL_STATIC_DRAW);
		loc = glGetAttribLocation(m_SplatProgram.ID, "cov3_col0");
		glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(SGaussianShaderDataCov), (void*)offsetof(SGaussianShaderDataCov, cov3_col0Vec));
		glEnableVertexAttribArray(loc);


		loc = glGetAttribLocation(m_SplatProgram.ID, "cov3_col1");
		glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(SGaussianShaderDataCov), (void*)offsetof(SGaussianShaderDataCov, cov3_col1Vec));
		glEnableVertexAttribArray(loc);


		loc = glGetAttribLocation(m_SplatProgram.ID, "cov3_col2");
		glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(SGaussianShaderDataCov), (void*)offsetof(SGaussianShaderDataCov, cov3_col2Vec));
		glEnableVertexAttribArray(loc);

	}

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_Data.getNum(), nullptr, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	glGenBuffers(1, &m_PositionSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_PositionSSBO);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * m_Data.getNum(), m_Data.getDataPos().data(), GL_DYNAMIC_STORAGE_BIT);

	glGenBuffers(1, &m_DepthSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_DepthSSBO);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t) * m_Data.getNum(), nullptr, GL_DYNAMIC_STORAGE_BIT);

	glGenBuffers(1, &m_DepthSSBO2);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_DepthSSBO2);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t) * m_Data.getNum(), nullptr, GL_DYNAMIC_STORAGE_BIT);

	glGenBuffers(1, &m_IndexSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_IndexSSBO);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t) * m_Data.getNum(), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT);

	glGenBuffers(1, &m_IndexSSBO2);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_IndexSSBO2);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t) * m_Data.getNum(), nullptr, GL_DYNAMIC_STORAGE_BIT);

	glGenBuffers(1, &m_HistogramSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_HistogramSSBO);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t) * NUM_WORKGROUPS * RADIX_SORT_BINS, nullptr, GL_DYNAMIC_STORAGE_BIT);

	glGenBuffers(1, &m_AtomicCounter);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, m_AtomicCounter);
	glBufferStorage(GL_ATOMIC_COUNTER_BUFFER, sizeof(uint32_t) * m_CounterVec.size(), m_CounterVec.data(), GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT);

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glBindBuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &m_ColorTexture);
	glBindTexture(GL_TEXTURE_2D, m_ColorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1200, 670, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture, 0);

	// 创建深度纹理附件
	glGenTextures(1, &m_DepthTexture);
	glBindTexture(GL_TEXTURE_2D, m_DepthTexture);

	// 设置纹理参数，这里使用 GL_DEPTH_COMPONENT24 确保深度值存储为 24 位
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 1200, 670, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	// 设置纹理采样参数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// 将深度纹理附件作为深度附件附加到帧缓冲对象
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		// 处理帧缓冲对象错误
		std::cerr << "Framebuffer is not complete!" << std::endl;
	}

	// 解绑帧缓冲对象
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void CGaussianSplatRenderable::changeMode(bool vShouldrawPointCloud) {
	if (m_IsPointCloud != vShouldrawPointCloud) {
		m_IsPointCloud = vShouldrawPointCloud;
		m_Data.updateCov(m_Scale, m_IsPointCloud);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOCov);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(SGaussianShaderDataCov) * m_Data.getNum(), m_Data.getDataCov().data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
void CGaussianSplatRenderable::render(float vDeltaTime, const glm::mat4& vProjectionMat, const glm::mat4& vViewMat, const glm::vec4& vViewport, const glm::vec2& vNearFar, float vScale) {
	if (m_Scale != vScale) {
		m_Scale = vScale;
		m_Data.updateCov(vScale, m_IsPointCloud);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOCov);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(SGaussianShaderDataCov) * m_Data.getNum(), m_Data.getDataCov().data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE);

	glm::vec4 clearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClearDepth(0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	const uint32_t MAX_DEPTH = std::numeric_limits<uint32_t>::max();

	m_PresortProgram.use();
	m_PresortProgram.setMat4("modelViewProj", vProjectionMat * vViewMat * m_ModelMat);
	m_PresortProgram.setVec2("nearFar", vNearFar);
	m_PresortProgram.setUint("keyMax", MAX_DEPTH);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_PositionSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_DepthSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_IndexSSBO);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 4, m_AtomicCounter);
	const int LOCAL_SIZE = 256;

	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, m_AtomicCounter);
	m_CounterVec[0] = 0;
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(uint32_t) * m_CounterVec.size(), (void*)m_CounterVec.data());
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
	glDispatchCompute(((GLuint)m_Data.getNum() + (LOCAL_SIZE - 1)) / LOCAL_SIZE, 1, 1);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, m_AtomicCounter);
	void* rawBuffer = glMapBufferRange(GL_ATOMIC_COUNTER_BUFFER, 0, m_CounterVec.size() * sizeof(float), GL_MAP_READ_BIT);
	if (rawBuffer)
	{
		memcpy((void*)m_CounterVec.data(), rawBuffer, m_CounterVec.size() * sizeof(float));
	}
	uint32_t sortCount = m_CounterVec[0];
	glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
	{

		const uint32_t NUM_ELEMENTS = static_cast<uint32_t>(sortCount);

		const uint32_t NUM_BYTES = 4;
		m_SortProgram.use();
		m_SortProgram.setUint("g_num_elements", NUM_ELEMENTS);
		m_SortProgram.setUint("g_num_workgroups", NUM_WORKGROUPS);
		m_SortProgram.setUint("g_num_blocks_per_workgroup", m_NumBlocksPerWorkgroup);

		m_HistogramProgram.use();
		m_HistogramProgram.setUint("g_num_elements", NUM_ELEMENTS);
		m_HistogramProgram.setUint("g_num_blocks_per_workgroup", m_NumBlocksPerWorkgroup);

		for (uint32_t i = 0; i < NUM_BYTES; i++)
		{
			m_HistogramProgram.use();
			m_HistogramProgram.setUint("g_shift", 8 * i);

			if (i == 0 || i == 2)
			{
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_DepthSSBO);
			}
			else
			{
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_DepthSSBO2);
			}
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_HistogramSSBO);

			glDispatchCompute(NUM_WORKGROUPS, 1, 1);

			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

			m_SortProgram.use();
			m_SortProgram.setUint("g_shift", 8 * i);

			if ((i % 2) == 0)  // even
			{
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_DepthSSBO);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_DepthSSBO2);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_IndexSSBO);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_IndexSSBO2);
			}
			else  // odd
			{
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_DepthSSBO2);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_DepthSSBO);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_IndexSSBO2);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_IndexSSBO);
			}
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_HistogramSSBO);

			glDispatchCompute(NUM_WORKGROUPS, 1, 1);

			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		}


		glBindBuffer(GL_COPY_READ_BUFFER, m_IndexSSBO);
		glBindBuffer(GL_COPY_WRITE_BUFFER, m_EBO);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sortCount * sizeof(uint32_t));
	}
	glm::vec3 eye = glm::vec3(glm::inverse(vViewMat * m_ModelMat)[3]);
	m_SplatProgram.use();
	m_SplatProgram.setInt("screenTexture", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ColorTexture);
	m_SplatProgram.setMat4("viewMat", vViewMat * m_ModelMat);
	m_SplatProgram.setMat4("projMat", vProjectionMat);
	m_SplatProgram.setVec4("viewport", vViewport);
	m_SplatProgram.setVec4("projParams", glm::vec4(0.0f, vNearFar.x, vNearFar.y, 0.0f));
	m_SplatProgram.setVec3("eye", eye);
	m_SplatProgram.setBool("is_point_cloud", m_IsPointCloud || !m_AttenuationMode);
	glBindVertexArray(m_VAO);
	int t1 = sortCount / 2, t2 = sortCount - sortCount / 2;

	glDisable(GL_DEPTH_TEST);
	glDrawElements(GL_POINTS, sortCount / 2, GL_UNSIGNED_INT, 0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GEQUAL);

	depthShader.use();
	depthShader.setInt("screenTexture", 0);

	glBindVertexArray(quadVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ColorTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);


	m_SplatProgram.use();
	glBindVertexArray(m_VAO);
	glEnable(GL_DEPTH_TEST);
	glDrawElements(GL_POINTS, t2, GL_UNSIGNED_INT, (void*)(t1 * sizeof(unsigned int)));

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

	glDisable(GL_BLEND);
	// clear all relevant buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);

	screenShader.use();
	screenShader.setInt("screenTexture", 0);

	glBindVertexArray(quadVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ColorTexture);	// use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

CGaussianSplatRenderable::CGaussianSplatRenderable() : m_SplatProgram("shader/splat_vert.glsl", "shader/splat_frag.glsl", "shader/splat_geom.glsl"),
m_PresortProgram("shader/presort_compute.glsl"),
m_SortProgram("shader/multi_radixsort.glsl"),
m_HistogramProgram("shader/multi_radixsort_histograms.glsl"),
screenShader("shader/screen_vert.glsl", "shader/screen_frag.glsl"),
depthShader("shader/write_depth_vert.glsl", "shader/write_depth_frag.glsl"){
	LOCAL_SIZE = 256;
	RADIX_SORT_BINS = 256;
	m_ModelMat = glm::mat4(1.0);
	m_ModelMat = glm::rotate(m_ModelMat, 3.14f, glm::vec3(0, 0, 1));
	m_ModelMat = glm::scale(m_ModelMat, glm::vec3(5, 5, 5));
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	// screen quad VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}
CGaussianSplatRenderable::~CGaussianSplatRenderable() {
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBOPos);
	glDeleteBuffers(1, &m_VBOSH);
	glDeleteBuffers(1, &m_VBOCov);
	glDeleteBuffers(1, &m_EBO);
	glDeleteBuffers(1, &m_PositionSSBO);
	glDeleteBuffers(1, &m_DepthSSBO);
	glDeleteBuffers(1, &m_DepthSSBO2);
	glDeleteBuffers(1, &m_IndexSSBO);
	glDeleteBuffers(1, &m_IndexSSBO2);
	glDeleteBuffers(1, &m_HistogramSSBO);
	glDeleteBuffers(1, &m_AtomicCounter);
}