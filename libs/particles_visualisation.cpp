#include "particles_visualisation.h"


ParticlesVisualisation::ParticlesVisualisation()
{

}

ParticlesVisualisation::~ParticlesVisualisation()
{

}

void ParticlesVisualisation::process(Particles &particles)
{
  visualisation.start();
  visualisation.translate(0, 0, -3.2);


  for (unsigned int i = 0; i < particles.count(); i++)
  {
    sParticle particle = particles.get(i);

    visualisation.push();


    visualisation.translate(particle.x, particle.y, particle.z);
    visualisation.set_color(1.0, 1.0, 1.0);
    visualisation.paint_circle(particle.r);

    visualisation.pop();
  }


  visualisation.finish();
}
