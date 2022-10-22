#include "AQPCH.h"
#include "AQParticle2D.h"
#include "core/Application.h"
#include "core/RandomSystem.h"
#include "Renderer/Renderer2D.h"
#include <GLM/gtc/constants.hpp>
#include <GLM/gtx/compatibility.hpp>

namespace Aquarius
{


	AQParticcle2D::AQParticcle2D(const uint32_t maxparticles)
	{
		m_Particles.resize(maxparticles);
		m_ParticleIndex = m_Particles.size() - 1;
	}

	void AQParticcle2D::OnUpdate(DeltaTime& dt)
	{
		for (auto& particle :m_Particles)
		{
			if (!particle.Active)
			{
				continue;
			}
			if (particle.LifeRemaining <= 0.0f)
			{
				particle.Active = false;
				m_AliveParticleCount--;
				continue;
			}
			particle.LifeRemaining -= dt;
			particle.Position += particle.Velosity * (float)dt;
			particle.Roatation +=20.0f * dt;
		}
	}

	void AQParticcle2D::OnRender(OrthgraphicCamera& camera)
	{
		AQ_TIME_MONITOR("AQParticcle2D::OnRender");

		Renderer2D::BeginScene(camera);
		for (auto& particle : m_Particles)
		{
			if (!particle.Active)
				continue;
			float life = particle.LifeRemaining / particle.Lifetime;
			glm::vec4 color = glm::lerp(particle.EndColor, particle.BeginColor, life);
			float size = glm::lerp(particle.EndSize, particle.BeginSize, life);

			Renderer2D::DrawQuad(particle.Position, particle.Roatation, { size, size }, color);
		}
		Renderer2D::EndScene();
	}

	void AQParticcle2D::Emit(const ParticleProperties& properties)
	{
		Particle& particle = m_Particles[m_ParticleIndex];
		if (!particle.Active)
		{
			particle.Active = true;
			m_AliveParticleCount++;

			particle.Position = properties.Position;
			particle.Roatation = AQRandom::Float() * 2.0f * glm::pi<float>();

			particle.Velosity = properties.Velosity;
			particle.Velosity.x += properties.VelosityVariation.x * (AQRandom::Float() - 0.5f);
			particle.Velosity.y += properties.VelosityVariation.y * (AQRandom::Float() - 0.5f);

			particle.BeginColor = properties.BeginColor;
			particle.EndColor = properties.EndColor;

			particle.Lifetime = properties.Lifetime;
			particle.LifeRemaining = properties.Lifetime;
			particle.BeginSize = properties.BeginSize + properties.SizeVariation * (AQRandom::Float() - 0.5f);
			particle.EndSize = properties.EndSize;
		}
		
		if (m_ParticleIndex)
			m_ParticleIndex--;
		else
			m_ParticleIndex = m_Particles.size() - 1;

	/*	m_ParticleIndex = --m_ParticleIndex % m_Particles.size();*/

	}




}