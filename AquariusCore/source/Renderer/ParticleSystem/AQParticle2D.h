#pragma once
#include "core/AquariusCore.h"
#include "core/AQObject.h"
#include "core/TimeSystem.h"
#include "MathSystem/AQMathEx.h"
#include "Renderer/OrthgraphicCamera.h"


namespace Aquarius
{

	class AQUARIUS_API AQParticcle2D
	{
	public:
		struct ParticleProperties
		{
			Eigen::Vector2f Position;
			Eigen::Vector2f Velosity;
			Eigen::Vector2f VelosityVariation;
			Eigen::Vector4f BeginColor;
			Eigen::Vector4f EndColor;

			AQFLOAT BeginSize;
			AQFLOAT EndSize;
			AQFLOAT SizeVariation;
			AQFLOAT Lifetime = 1.0f;
		};
	public:
		AQParticcle2D(const AQUINT maxparticles);

		void OnUpdate(DeltaTime& dt);
		void OnRender(OrthgraphicCamera& camera);

		void Emit(const ParticleProperties& properties);



		AQUINT GetParticleIndex() { return m_ParticleIndex; }
		AQUINT GetAliveParticleCount() { return m_AliveParticleCount; }
	private:
		struct Particle
		{
			Eigen::Vector2f Position;
			Eigen::Vector2f Velosity;
			Eigen::Vector4f BeginColor;
			Eigen::Vector4f EndColor;

			AQFLOAT Roatation = 0.0f;
			AQFLOAT BeginSize;
			AQFLOAT EndSize;
			AQFLOAT Lifetime = 1.0f;
			AQFLOAT LifeRemaining = 0.0f;

			bool Active = false;
		};

	private:
		std::vector<Particle> m_Particles;
		AQUINT m_ParticleIndex ;
		int m_AliveParticleCount = 0;
	};





}