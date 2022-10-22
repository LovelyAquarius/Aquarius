#pragma once
#include "core/AquariusCore.h"
#include "core/AQObject.h"
#include "core/TimeSystem.h"

#include "Renderer/OrthgraphicCamera.h"


#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
namespace Aquarius
{

	class AQUARIUS_API AQParticcle2D
	{
	public:
		struct ParticleProperties
		{
			glm::vec2 Position;
			glm::vec2 Velosity;
			glm::vec2 VelosityVariation;
			glm::vec4 BeginColor;
			glm::vec4 EndColor;

			float BeginSize;
			float EndSize;
			float SizeVariation;
			float Lifetime = 1.0f;
		};
	public:
		AQParticcle2D(const uint32_t maxparticles);

		void OnUpdate(DeltaTime& dt);
		void OnRender(OrthgraphicCamera& camera);

		void Emit(const ParticleProperties& properties);



		uint32_t GetParticleIndex() { return m_ParticleIndex; }
		uint32_t GetAliveParticleCount() { return m_AliveParticleCount; }
	private:
		struct Particle
		{
			glm::vec2 Position;
			glm::vec2 Velosity;
			glm::vec4 BeginColor;
			glm::vec4 EndColor;

			float Roatation = 0.0f;
			float BeginSize;
			float EndSize;
			float Lifetime = 1.0f;
			float LifeRemaining = 0.0f;

			bool Active = false;
		};

	private:
		std::vector<Particle> m_Particles;
		uint32_t m_ParticleIndex ;
		int m_AliveParticleCount = 0;
	};





}