#include "Particle.h"
#include"BasicParticle.h"

BasicParticle* Particle::CreateParticle()
{
	BasicParticle* basic = new BasicParticle;
	basic->Initialize();
	return basic;
}
