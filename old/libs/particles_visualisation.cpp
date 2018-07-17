#include "particles_visualisation.h"


ParticlesVisualisation::ParticlesVisualisation()
{

}

ParticlesVisualisation::~ParticlesVisualisation()
{

}

void ParticlesVisualisation::process(Particles &particles_a, Particles &particles_b)
{
  visualisation.start();
  visualisation.translate(0, 0, -3.2);

  visualisation.push();

  std::string str;
  str = "iteration " + std::to_string(particles_a.iteration());

  visualisation.set_color(1.0, 1.0, 1.0);
  visualisation.print(-0.2, -1.2, 0, str);

  visualisation.pop();

  for (unsigned int i = 0; i < particles_a.count(); i++)
  {
    sParticle particle = particles_a.get(i);

    visualisation.push();


    visualisation.translate(particle.x, particle.y, particle.z);
    visualisation.set_color(1.0, 0.0, 0.0);
    visualisation.paint_circle(particle.r);

    visualisation.pop();
  }

  for (unsigned int i = 0; i < particles_b.count(); i++)
  {
    sParticle particle = particles_b.get(i);

    visualisation.push();


    visualisation.translate(particle.x, particle.y, particle.z);
    visualisation.set_color(0.0, 0.0, 1.0);
    visualisation.paint_circle(particle.r);

    visualisation.pop();
  }


  visualisation.finish();
}


void ParticlesVisualisation::process(Particles &particles)
{
  visualisation.start();
  visualisation.translate(0, 0, -3.2);

  visualisation.push();

  std::string str;
  str = "iteration " + std::to_string(particles.iteration());

  visualisation.set_color(1.0, 0.0, 0.0);
  visualisation.print(-0.2, -1.2, 0, str);

  visualisation.pop();

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
