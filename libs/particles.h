#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "particles_def.h"



class Particles
{
  private:
    unsigned int m_count;
    sParticle *particles;

    float m_dt;
    unsigned int m_iteration;

  public:
    Particles();
    Particles(unsigned int count, float dt = 0.001);
    virtual ~Particles();

    void init(unsigned int count, float dt = 0.001);
    void uninint();

    sParticle get(unsigned int idx);

    std::vector<sParticle> get_all();

    std::string get_string(unsigned int idx);
    void print(unsigned int idx);

    unsigned int count()
    {
      return m_count;
    }

    unsigned int iteration()
    {
      return m_iteration;
    }

    void process();

  private:
    void set_random();
    sParticle random_particle(unsigned int idx);

    float rnd();


};



#endif
