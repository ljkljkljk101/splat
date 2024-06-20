#pragma once
#include"glm/glm.hpp"
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
struct SGaussian
{

	float _Position[3];  // in world space
	float _Normal[3];  // unused
	float _F_dc[3];  // first order spherical harmonics coeff (sRGB color space)
	float _F_rest[45];  // more spherical harminics coeff
	float _Opacity;  // alpha = 1 / (1 + exp(-opacity));
	float _Scale[3];
	float _Rot[4];  // local rotation of guassian (real, i, j, k)
	glm::mat3 ComputeCovMat(float vScale = 1.0) const
	{
		glm::quat q(_Rot[0], _Rot[1], _Rot[2], _Rot[3]);
		glm::mat3 R(glm::normalize(q));
		glm::mat3 S(glm::vec3(expf(_Scale[0] * vScale), 0.0f, 0.0f),
			glm::vec3(0.0f, expf(_Scale[1] * vScale), 0.0f),
			glm::vec3(0.0f, 0.0f, expf(_Scale[2] * vScale)));
		return R * S * glm::transpose(S) * glm::transpose(R);
	}
	glm::mat3 ComputeCovMatPoint(float vScale = 1.0) const
	{
		glm::quat q(_Rot[0], _Rot[1], _Rot[2], _Rot[3]);
		glm::mat3 R(glm::normalize(q));
		glm::mat3 S(glm::vec3(expf(-5 * vScale), 0.0f, 0.0f),
			glm::vec3(0.0f, expf(-5 * vScale), 0.0f),
			glm::vec3(0.0f, 0.0f, expf(-5 * vScale)));
		return R * S * glm::transpose(S) * glm::transpose(R);
	}
};
struct  SGaussianShaderDataPos {
	glm::vec4 posVec;
};
struct  SGaussianShaderDataSH {
	glm::vec4 r_sh0Vec;
	glm::vec4 r_sh1Vec;
	glm::vec4 r_sh2Vec;
	glm::vec4 r_sh3Vec;

	glm::vec4 g_sh0Vec;
	glm::vec4 g_sh1Vec;
	glm::vec4 g_sh2Vec;
	glm::vec4 g_sh3Vec;

	glm::vec4 b_sh0Vec;
	glm::vec4 b_sh1Vec;
	glm::vec4 b_sh2Vec;
	glm::vec4 b_sh3Vec;
};
struct  SGaussianShaderDataCov {
	glm::vec3 cov3_col0Vec;
	glm::vec3 cov3_col1Vec;
	glm::vec3 cov3_col2Vec;
};
class CGaussianData {
public:
	bool readFromFile(const std::string& plyFilename) {
		std::ifstream plyFile(plyFilename, std::ios::binary);
		if (!plyFile.is_open())
		{
			printf("failed to open %s\n", plyFilename.c_str());
			return false;
		}
		std::string line;
		std::string s1, s2, s3;
		int count;
		std::getline(plyFile, line);
		if (line != "ply") {
			printf("Wrong Format File\n");
			return false;
		}
		std::getline(plyFile, line);
		if (line != "format binary_little_endian 1.0") {
			printf("Wrong Format File\n");
			return false;
		}
		std::getline(plyFile, line);
		std::istringstream input(line);
		input >> s1 >> s2 >> count;
		if (s1 != "element" && s2 != "vertex" && count < 0) {
			printf("Wrong Format File\n");
			return false;
		}
		while (std::getline(plyFile, line))
		{
			if (line == "end_header") {
				break;
			}
		}
		m_PointCloudGaussian.resize(count);
		plyFile.read((char*)m_PointCloudGaussian.data(), sizeof(SGaussian) * count);
		SGaussianShaderDataPos shaderDataPos;
		SGaussianShaderDataSH shaderDataSH;
		SGaussianShaderDataCov shaderDataCov;
		for (int i = 0; i < m_PointCloudGaussian.size(); i++) {
			auto& g = m_PointCloudGaussian[i];
			float alpha = 1.0f / (1.0f + expf(-g._Opacity));
			shaderDataPos.posVec = glm::vec4(g._Position[0], g._Position[1], g._Position[2], alpha);
			shaderDataSH.r_sh0Vec = glm::vec4(g._F_dc[0], g._F_rest[0], g._F_rest[1], g._F_rest[2]);
			shaderDataSH.g_sh0Vec = glm::vec4(g._F_dc[1], g._F_rest[15], g._F_rest[16], g._F_rest[17]);
			shaderDataSH.b_sh0Vec = glm::vec4(g._F_dc[2], g._F_rest[30], g._F_rest[31], g._F_rest[32]);

			shaderDataSH.r_sh1Vec = glm::vec4(g._F_rest[3], g._F_rest[4], g._F_rest[5], g._F_rest[6]);
			shaderDataSH.r_sh2Vec = glm::vec4(g._F_rest[7], g._F_rest[8], g._F_rest[9], g._F_rest[10]);
			shaderDataSH.r_sh3Vec = glm::vec4(g._F_rest[11], g._F_rest[12], g._F_rest[13], g._F_rest[14]);
			shaderDataSH.g_sh1Vec = glm::vec4(g._F_rest[18], g._F_rest[19], g._F_rest[20], g._F_rest[21]);
			shaderDataSH.g_sh2Vec = glm::vec4(g._F_rest[22], g._F_rest[23], g._F_rest[24], g._F_rest[25]);
			shaderDataSH.g_sh3Vec = glm::vec4(g._F_rest[26], g._F_rest[27], g._F_rest[28], g._F_rest[29]);
			shaderDataSH.b_sh1Vec = glm::vec4(g._F_rest[33], g._F_rest[34], g._F_rest[35], g._F_rest[36]);
			shaderDataSH.b_sh2Vec = glm::vec4(g._F_rest[37], g._F_rest[38], g._F_rest[39], g._F_rest[40]);
			shaderDataSH.b_sh3Vec = glm::vec4(g._F_rest[41], g._F_rest[42], g._F_rest[43], g._F_rest[44]);
			glm::mat3 V = g.ComputeCovMat();
			shaderDataCov.cov3_col0Vec = V[0];
			shaderDataCov.cov3_col1Vec = V[1];
			shaderDataCov.cov3_col2Vec = V[2];
			m_ShaderDataGaussianPos.push_back(shaderDataPos);
			m_ShaderDataGaussianSH.push_back(shaderDataSH);
			m_ShaderDataGaussianCov.push_back(shaderDataCov);
		}
	}
	void updateCov(float vScale, bool vShouldDrawPointCloud = false) {
		for (int i = 0; i < m_PointCloudGaussian.size(); i++) {
			auto& g = m_PointCloudGaussian[i];
			SGaussianShaderDataCov& shaderData = m_ShaderDataGaussianCov[i];
			glm::mat3 V;
			if (vShouldDrawPointCloud) {
				V = g.ComputeCovMatPoint(vScale);
			}
			else {
				V = g.ComputeCovMat(vScale);
			}
			shaderData.cov3_col0Vec = V[0];
			shaderData.cov3_col1Vec = V[1];
			shaderData.cov3_col2Vec = V[2];
		}
	}
	const std::vector<SGaussianShaderDataPos>& getDataPos() {
		return m_ShaderDataGaussianPos;
	}
	const std::vector<SGaussianShaderDataSH>& getDataSH() {
		return m_ShaderDataGaussianSH;
	}
	const std::vector<SGaussianShaderDataCov>& getDataCov() {
		return m_ShaderDataGaussianCov;
	}
	int getNum() {
		return m_PointCloudGaussian.size();
	}
private:
	std::vector<SGaussian> m_PointCloudGaussian;
	std::vector<SGaussianShaderDataPos> m_ShaderDataGaussianPos;
	std::vector<SGaussianShaderDataSH> m_ShaderDataGaussianSH;
	std::vector<SGaussianShaderDataCov> m_ShaderDataGaussianCov;
};