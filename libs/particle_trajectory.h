#ifndef _PARTICLE_TRAJECTORY_H_
#define _PARTICLE_TRAJECTORY_H_

#include "particle.h"
#include <vector>
#include <log.h>

#define ParticleTrajectory_READ_MODE_ALL        ((int)0)
#define ParticleTrajectory_READ_MODE_FIRST      ((int)1)

class ParticleTrajectory
{
  private:
    Log load_log;

    std::vector<Particle> trajectory;

  private:
    t_partnum m_max_x, m_min_x;
    t_partnum m_max_y, m_min_y;
    t_partnum m_max_z, m_min_z;
    t_partnum m_g_min, m_g_max;

  public:
    ParticleTrajectory();
    ParticleTrajectory(std::string dat_file_name, int read_mode = ParticleTrajectory_READ_MODE_ALL);

    ParticleTrajectory(ParticleTrajectory &other);
    ParticleTrajectory(const ParticleTrajectory &other);

    virtual ~ParticleTrajectory();

    ParticleTrajectory& operator =(ParticleTrajectory &other);

    void load_from_file(std::string dat_file_name, int read_mode = ParticleTrajectory_READ_MODE_ALL);

  public:
    unsigned int size();
    void save(std::string file_name);


    Particle& get(unsigned int idx);
    Particle& get_random();


    void add(Particle &particle);
    void clear();
    void compute_velocity();

    void print();

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

    t_partnum g_max()
    {
      return m_g_max;
    }

    t_partnum g_min()
    {
      return m_g_min;
    }

    void set_g_max(t_partnum value)
    {
      m_g_max = value;
    }

    void set_g_min(t_partnum value)
    {
      m_g_min = value;
    }



  private:
    void remove_comma(std::string &str);
    Particle parse_line(std::string &line);

    void find_extremal_positions();
    void normalise();
    void normalise(t_partnum min, t_partnum max);
};


#endif
