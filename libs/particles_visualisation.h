#ifndef _PARTICLES_VISUALISATION_H_
#define _PARTICLES_VISUALISATION_H_

#include <particles.h>
#include <visualisation.h>


class ParticlesVisualisation
{
  private: 
    Visualisation visualisation;

  public:
    ParticlesVisualisation();
    virtual ~ParticlesVisualisation();

    void process(Particles &particles);
};


#endif
