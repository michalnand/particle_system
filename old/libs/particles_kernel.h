#ifndef _PARTICLES_KERNEL_H_
#define _PARTICLES_KERNEL_H_

#include "particles_def.h"


__global__ void particles_main_gpu_kernel(unsigned int count, sParticle *particle, float dt);
__global__ void particles_update_gpu_kernel(unsigned int count, sParticle *particle, float dt);
__global__ void particles_limit_ranges_gpu_kernel(unsigned int count, sParticle *particle, float dt);


void particles_main_cpu_kernel(unsigned int idx, unsigned int count, sParticle *particle, float dt);
void particles_update_cpu_kernel(unsigned int idx, sParticle *particle, float dt);
void particles_limit_ranges_cpu_kernel(unsigned int idx, sParticle *particle, float dt);


#endif
