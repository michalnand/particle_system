#ifndef _PARTICLES_TRAJECTORY_H_
#define _PARTICLES_TRAJECTORY_H_

#include "particle_trajectory.h"

class ParticlesTrajectory
{
  private:
    std::vector<ParticleTrajectory> particles;

    t_partnum m_max_x, m_min_x;
    t_partnum m_max_y, m_min_y;
    t_partnum m_max_z, m_min_z;

  public:
    ParticlesTrajectory(std::string path, int mode = ParticleTrajectory_READ_MODE_ALL);
    virtual ~ParticlesTrajectory();

    void load(std::string path, int mode = ParticleTrajectory_READ_MODE_ALL);

    unsigned int size();

    ParticleTrajectory& get(unsigned int idx);
    ParticleTrajectory& get_random();

    void find_extremal_positions();

  public:
    t_partnum max_x()
    {
      return m_max_x;
    }

    t_partnum min_x()
    {
      return m_min_x;
    }

    t_partnum max_y()
    {
      return m_max_y;
    }

    t_partnum min_y()
    {
      return m_min_y;
    }

    t_partnum max_z()
    {
      return m_max_z;
    }

    t_partnum min_z()
    {
      return m_min_z;
    }


};


#endif
