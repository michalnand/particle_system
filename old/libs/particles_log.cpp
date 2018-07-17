#include "particles_log.h"



ParticlesLog::ParticlesLog(std::string path, unsigned int particles_count)
{
  result_log.resize(particles_count);

  this->path = path;

  for (unsigned int i = 0; i < result_log.size(); i++)
  {
    std::string file_name;
    file_name = path + "/" + std::to_string(i) + ".dat";
    result_log[i].set_output_file_name(file_name);
  }
}

ParticlesLog::~ParticlesLog()
{

}

void ParticlesLog::add(Particles &particles)
{
  for (unsigned int i = 0; i < particles.count(); i++)
  {
    if (i > result_log.size())
    {
      result_log.push_back(Log());

      std::string file_name;
      file_name = path + "/" + std::to_string(i) + ".dat";

      result_log[result_log.size()-1].set_output_file_name(file_name);
    }

    sParticle particle = particles.get(i);

    result_log[i] << particles.iteration() << " ";

    result_log[i] << particle.x << " ";
    result_log[i] << particle.y << " ";
    result_log[i] << particle.z << " ";

    result_log[i] << particle.vx << " ";
    result_log[i] << particle.vy << " ";
    result_log[i] << particle.vz << "\n";

  }

}
