#include "particles_trajectory.h"
#include <experimental/filesystem>


ParticlesTrajectory::ParticlesTrajectory(std::string path, int mode)
{
  load(path, mode);
  find_extremal_positions();
}

ParticlesTrajectory::~ParticlesTrajectory()
{

}



void ParticlesTrajectory::load(std::string path, int mode)
{
    particles.clear();

    for (auto & p : std::experimental::filesystem::directory_iterator(path))
   {
     std::string particle_file;
     particle_file = p.path();

     if (std::experimental::filesystem::path(particle_file).extension() == ".dat")
     {
       particles.push_back(ParticleTrajectory(particle_file, mode));
     }
   }
}

unsigned int ParticlesTrajectory::size()
{
  return particles.size();
}

ParticleTrajectory& ParticlesTrajectory::get(unsigned int idx)
{
  return particles[idx];
}

ParticleTrajectory& ParticlesTrajectory::get_random()
{
  return get(rand()%particles.size());
}


void ParticlesTrajectory::find_extremal_positions()
{
  t_partnum k = -1000000000.0;

  m_max_x = k;
  m_min_x = -k;
  m_max_y = k;
  m_min_y = -k;
  m_max_z = k;
  m_min_z = -k;

  for (unsigned int i = 0; i < particles.size(); i++)
  {
    if (particles[i].max_x() > m_max_x)
      m_max_x = particles[i].max_x();
    if (particles[i].min_x() < m_min_x)
      m_min_x = particles[i].min_x();

    if (particles[i].max_y() > m_max_y)
      m_max_y = particles[i].max_y();
    if (particles[i].min_y() < m_min_y)
      m_min_y = particles[i].min_y();
 
    if (particles[i].max_z() > m_max_z)
      m_max_z = particles[i].max_z();
    if (particles[i].min_z() < m_min_z)
      m_min_z = particles[i].min_z();
  }

}
