#include "particle.h"

#include <iostream>
#include <math.h>

Particle::Particle()
{
  m_x = 0.0;
  m_y = 0.0;
  m_z = 0.0;

  m_vx = 0.0;
  m_vy = 0.0;
  m_vz = 0.0;
}

Particle::Particle( t_partnum x_, t_partnum y_, t_partnum z_,
                    t_partnum vx_, t_partnum vy_, t_partnum vz_)
{
  init(x_, y_, z_, vx_, vy_, vz_);
}

Particle::Particle(Particle &other)
{
  init(other.x(), other.y(), other.z(), other.vx(), other.vy(), other.vz());
}

Particle::Particle(const Particle &other)
{
  m_x = other.m_x;
  m_y = other.m_y;
  m_z = other.m_z;

  m_vx = other.m_vx;
  m_vy = other.m_vy;
  m_vz = other.m_vz;
}


Particle::~Particle()
{

}


Particle& Particle::Particle::operator =(Particle &other)
{
  init(other.x(), other.y(), other.z(), other.vx(), other.vy(), other.vz());
  return *this;
}

Particle& Particle::Particle::operator =(const Particle &other)
{
  m_x = other.m_x;
  m_y = other.m_y;
  m_z = other.m_z;

  m_vx = other.m_vx;
  m_vy = other.m_vy;
  m_vz = other.m_vz;

  return *this;
}


void Particle::init(  t_partnum x_, t_partnum y_, t_partnum z_,
                      t_partnum vx_, t_partnum vy_, t_partnum vz_)
{
  m_x = x_;
  m_y = y_;
  m_z = z_;

  m_vx = vx_;
  m_vy = vy_;
  m_vz = vz_;
}

void Particle::compute_velocity(Particle &prev_particle, t_partnum dt)
{
  m_vx = (x() - prev_particle.x())*dt;
  m_vy = (y() - prev_particle.y())*dt;
  m_vz = (z() - prev_particle.z())*dt;
}

void Particle::update_position(t_partnum vx_, t_partnum vy_, t_partnum vz_, t_partnum dt)
{
  m_vx = vx_;
  m_vy = vy_;
  m_vz = vz_;

  m_x+= m_vx*dt;
  m_y+= m_vy*dt;
  m_z+= m_vz*dt;
}


std::string Particle::to_string()
{
  std::string result;

  result+= std::to_string(x())  + " ";
  result+= std::to_string(y())  + " ";
  result+= std::to_string(z())  + " ";

  result+= std::to_string(vx()) + " ";
  result+= std::to_string(vy()) + " ";
  result+= std::to_string(vz()) + " ";

  return result;
}

void Particle::print()
{
  std::cout << to_string() << "\n";
}


t_partnum Particle::position_difference(Particle &other)
{
  t_partnum result = 0.0;

  result+= pow(x() - other.x(), 2.0);
  result+= pow(y() - other.y(), 2.0);
  result+= pow(z() - other.z(), 2.0);

  return sqrt(result);
}

t_partnum Particle::velocity_difference(Particle &other)
{
  t_partnum result = 0.0;

  result+= pow(vx() - other.vx(), 2.0);
  result+= pow(vy() - other.vy(), 2.0);
  result+= pow(vz() - other.vz(), 2.0);

  return sqrt(result);
}

void Particle::normalise(t_partnum k, t_partnum q)
{
  m_x = k*m_x + q;
  m_y = k*m_y + q;
  m_z = k*m_z + q;
}
