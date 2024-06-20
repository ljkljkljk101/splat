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

	glEnable(GL_BLEND);

	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glm::vec4 clearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
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
	m_SplatProgram.setMat4("viewMat", vViewMat * m_ModelMat);
	m_SplatProgram.setMat4("projMat", vProjectionMat);
	m_SplatProgram.setVec4("viewport", vViewport);
	m_SplatProgram.setVec4("projParams", glm::vec4(0.0f, vNearFar.x, vNearFar.y, 0.0f));
	m_SplatProgram.setVec3("eye", eye);
	m_SplatProgram.setBool("is_point_cloud", m_IsPointCloud || !m_AttenuationMode);
	glBindVertexArray(m_VAO);
	glDrawElements(GL_POINTS, sortCount, GL_UNSIGNED_INT, 0);
}

CGaussianSplatRenderable::CGaussianSplatRenderable() : m_SplatProgram("shader/splat_vert.glsl", "shader/splat_frag.glsl", "shader/splat_geom.glsl"),
m_PresortProgram("shader/presort_compute.glsl"),
m_SortProgram("shader/multi_radixsort.glsl"),
m_HistogramProgram("shader/multi_radixsort_histograms.glsl") {
	LOCAL_SIZE = 256;
	RADIX_SORT_BINS = 256;
	m_ModelMat = glm::mat4(1.0);
	m_ModelMat = glm::rotate(m_ModelMat, 3.14f, glm::vec3(0, 0, 1));
	m_ModelMat = glm::scale(m_ModelMat, glm::vec3(5, 5, 5));
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