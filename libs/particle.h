#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include <string>

typedef double t_partnum;

class Particle
{
  private:
    t_partnum m_x, m_y, m_z;
    t_partnum m_vx, m_vy, m_vz;

  public:
    Particle();

    Particle(t_partnum x_, t_partnum y_, t_partnum z_,
             t_partnum vx_ = 0.0, t_partnum vy_ = 0.0, t_partnum vz_ = 0.0);

    Particle(Particle &other);
    Particle(const Particle &other);

    virtual ~Particle();

    Particle& operator =(Particle &other);
    Particle& operator =(const Particle &other);


    void init(t_partnum x_, t_partnum y_, t_partnum z_,
              t_partnum vx_ = 0.0, t_partnum vy_ = 0.0, t_partnum vz_ = 0.0);

    t_partnum x()
    {
      return m_x;
    }

    t_partnum y()
    {
      return m_y;
    }

    t_partnum z()
    {
      return m_z;
    }

    t_partnum vx()
    {
      return m_vx;
    }

    t_partnum vy()
    {
      return m_vy;
    }

    t_partnum vz()
    {
      return m_vz;
    }

  public:
    void compute_velocity(Particle &prev_particle, t_partnum dt = 1.0);
    void update_position(t_partnum vx_, t_partnum vy_, t_partnum vz_, t_partnum dt = 1.0);

    std::string to_string();
    void print();

    t_partnum position_difference(Particle &other);
    t_partnum velocity_difference(Particle &other);

    void normalise(t_partnum k, t_partnum q);
};

#endif
