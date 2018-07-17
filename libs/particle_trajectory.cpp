#include "particle_trajectory.h"

#include <fstream>
#include <sstream>
#include <iostream>

ParticleTrajectory::ParticleTrajectory()
{

}

ParticleTrajectory::ParticleTrajectory(std::string dat_file_name, int read_mode)
{
  load_from_file(dat_file_name, read_mode);
  find_extremal_positions();
//  normalise();
  compute_velocity();
}

ParticleTrajectory::ParticleTrajectory(ParticleTrajectory &other)
{
  trajectory = other.trajectory;

  m_max_x = other.m_max_x;
  m_min_x = other.m_min_x;
  m_max_y = other.m_max_y;
  m_min_y = other.m_min_y;
  m_max_z = other.m_max_z;
  m_min_z = other.m_min_z;
  m_g_min = other.m_g_min;
  m_g_max = other.m_g_max;
}

ParticleTrajectory::ParticleTrajectory(const ParticleTrajectory &other)
{
  trajectory = other.trajectory;

  m_max_x = other.m_max_x;
  m_min_x = other.m_min_x;
  m_max_y = other.m_max_y;
  m_min_y = other.m_min_y;
  m_max_z = other.m_max_z;
  m_min_z = other.m_min_z;
  m_g_min = other.m_g_min;
  m_g_max = other.m_g_max;
}

ParticleTrajectory::~ParticleTrajectory()
{

}

ParticleTrajectory& ParticleTrajectory::operator =(ParticleTrajectory &other)
{
  trajectory = other.trajectory;

  m_max_x = other.m_max_x;
  m_min_x = other.m_min_x;
  m_max_y = other.m_max_y;
  m_min_y = other.m_min_y;
  m_max_z = other.m_max_z;
  m_min_z = other.m_min_z;
  m_g_min = other.m_g_min;
  m_g_max = other.m_g_max;

  return *this;
}

void ParticleTrajectory::load_from_file(std::string dat_file_name, int read_mode)
{
  load_log << "loading particle from " << dat_file_name;

  std::ifstream ifs(dat_file_name);

  if (ifs.is_open())
  {
    std::string line;

    std::getline(ifs, line);

    while(std::getline(ifs, line))
    {
      remove_comma(line);

      auto particle = parse_line(line);
      trajectory.push_back(particle);
      if (read_mode == ParticleTrajectory_READ_MODE_FIRST)
        break;
    }

    load_log << " done\n";
  }
  else
    load_log << " ERROR\n";
}

void ParticleTrajectory::save(std::string file_name)
{
  Log result_log(file_name);

  for (unsigned int i = 0; i < trajectory.size(); i++)
  {
    std::string line = trajectory[i].to_string();
    result_log << line << "\n";
  }
}


unsigned int ParticleTrajectory::size()
{
  return trajectory.size();
}

Particle& ParticleTrajectory::get(unsigned int idx)
{
  return trajectory[idx];
}

Particle& ParticleTrajectory::get_random()
{
  return get(rand()%size());
}

void ParticleTrajectory::add(Particle &particle)
{
  trajectory.push_back(particle);
}

void ParticleTrajectory::clear()
{
  trajectory.clear();
}

void ParticleTrajectory::compute_velocity()
{
  for (unsigned int i = 1; i < trajectory.size(); i++)
  {
    trajectory[i].compute_velocity(trajectory[i-1]);
  }
}

void ParticleTrajectory::print()
{
  for (unsigned int i = 0; i < trajectory.size(); i++)
    trajectory[i].print();

}

void ParticleTrajectory::remove_comma(std::string &str)
{
  for (unsigned int i = 0; i < str.size(); i++)
    if (str[i] == ',')
      str[i] = ' ';
}

Particle ParticleTrajectory::parse_line(std::string &line)
{
  std::stringstream ss(line);

  unsigned int iteration;
  t_partnum x, y, z;
  t_partnum vx, vy, vz;

  ss >> iteration;
  ss >> x;
  ss >> y;
  ss >> z;
  ss >> vx;
  ss >> vy;
  ss >> vz;

  Particle result(x, y, z, vx, vy, vz);

  return result;
}


void ParticleTrajectory::find_extremal_positions()
{
  t_partnum tmp = 100000000.0;

  m_max_x = -tmp;
  m_max_y = -tmp;
  m_max_z = -tmp;

  m_min_x =  tmp;
  m_min_y =  tmp;
  m_min_z =  tmp;

  for (unsigned int i = 0; i < trajectory.size(); i++)
  {
    if (trajectory[i].x() > m_max_x)
      m_max_x = trajectory[i].x();

    if (trajectory[i].x() < m_min_x)
      m_min_x = trajectory[i].x();

    if (trajectory[i].y() > m_max_y)
      m_max_y = trajectory[i].y();

    if (trajectory[i].y() < m_min_y)
      m_min_y = trajectory[i].y();

    if (trajectory[i].z() > m_max_z)
      m_max_z = trajectory[i].z();

    if (trajectory[i].z() < m_min_z)
      m_min_z = trajectory[i].z();
  }


  m_g_max = max_x();

  if (max_y() > m_g_max)
    m_g_max = max_y();

  if (max_z() > m_g_max)
    m_g_max = max_z();

  m_g_min = min_x();

  if (min_y() < m_g_min)
    m_g_min = min_y();

  if (min_z() < m_g_min)
    m_g_min = min_z();
}


void ParticleTrajectory::normalise()
{
  find_extremal_positions();
  normalise(g_min(), g_max());
}

void ParticleTrajectory::normalise(t_partnum min, t_partnum max)
{
  t_partnum k = 2.0/(max - min);
  t_partnum q = 1.0 - k*max;

  for (unsigned int i = 0; i < trajectory.size(); i++)
    trajectory[i].normalise(k, q);

  find_extremal_positions();
}
