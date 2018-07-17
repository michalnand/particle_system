#ifndef _PARTICLES_H_
#define _PARTICLES_H_

#include "particles_def.h"
#include "particles.h"
#include <log.h>
#include <string>
#include <vector>

class ParticlesLog
{
  private:
    std::string path;
    
    std::vector<Log> result_log;

  public:
    ParticlesLog(std::string path, unsigned int particles_count);
    virtual ~ParticlesLog();

    void add(Particles &particles);
};


#endif
