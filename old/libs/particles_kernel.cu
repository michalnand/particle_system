#include "particles_kernel.h"

__global__
void particles_main_gpu_kernel(unsigned int count, sParticle *particle, float dt)
{
  unsigned int idx  = threadIdx.x + blockIdx.x*blockDim.x;

  if (idx < count)
  {
    unsigned int other = 0;
    float distance_min = 1000000000.0;

    for (unsigned int i = 0; i < count; i++)
      if (i != idx)
      {
        float distance = 0.0;

        distance+= (particle[idx].x - particle[i].x)*(particle[idx].x - particle[i].x);
        distance+= (particle[idx].y - particle[i].y)*(particle[idx].y - particle[i].y);
        distance+= (particle[idx].z - particle[i].z)*(particle[idx].z - particle[i].z);

        distance = sqrt(distance);

        if (distance < distance_min)
        {
          distance_min = distance;
          other = i;
        }
      }

    if (distance_min < particle[idx].r+particle[other].r)
    {
      /*
      float m = 2.0*particle[other].m/(particle[idx].m + particle[other].m);
      float d = distance_min*distance_min;

      particle[idx].ax = -m*(particle[idx].vx - particle[other].vx)*(particle[idx].x - particle[other].x)*(particle[idx].x - particle[other].x)/d;
      particle[idx].ay = -m*(particle[idx].vy - particle[other].vy)*(particle[idx].y - particle[other].y)*(particle[idx].y - particle[other].y)/d;
      particle[idx].az = -m*(particle[idx].vz - particle[other].vz)*(particle[idx].z - particle[other].z)*(particle[idx].z - particle[other].z)/d;
      */

      particle[idx].vx*= -1.0;
      particle[idx].vy*= -1.0;
      particle[idx].vz*= -1.0;

    }
    else
    {
      particle[idx].ax = 0.0;
      particle[idx].ay = 0.0;
      particle[idx].az = 0.0;
    }
  }
}

__global__
void particles_update_gpu_kernel(unsigned int count, sParticle *particle, float dt)
{
  unsigned int idx  = threadIdx.x + blockIdx.x*blockDim.x;

  if (idx < count)
  {
    particle[idx].vx+= particle[idx].ax;
    particle[idx].vy+= particle[idx].ay;
    particle[idx].vz+= particle[idx].az;

    particle[idx].x+= particle[idx].vx*dt;
    particle[idx].y+= particle[idx].vy*dt;
    particle[idx].z+= particle[idx].vz*dt;
  }
}

__global__
void particles_limit_ranges_gpu_kernel(unsigned int count, sParticle *particle, float dt)
{
  unsigned int idx  = threadIdx.x + blockIdx.x*blockDim.x;

  if (idx < count)
  {
    float k = 0.999;

    if (particle[idx].x > 1.0)
    {
      particle[idx].x = k;
      particle[idx].vx*= -1.0;
    }
    else
    if (particle[idx].x < -1.0)
    {
      particle[idx].x = -k;
      particle[idx].vx*= -1.0;
    }
    else
    if (particle[idx].y > 1.0)
    {
      particle[idx].y = k;
      particle[idx].vy*= -1.0;
    }
    else
    if (particle[idx].y < -1.0)
    {
      particle[idx].y = -k;
      particle[idx].vy*= -1.0;
    }
    if (particle[idx].z > 1.0)
    {
      particle[idx].z = k;
      particle[idx].vz*= -1.0;
    }
    else
    if (particle[idx].z < -1.0)
    {
      particle[idx].z = -k;
      particle[idx].vz*= -1.0;
    }
  }
}







void particles_main_cpu_kernel(unsigned int idx, unsigned int count, sParticle *particle, float dt)
{
  unsigned int other = 0;
  float distance_min = 1000000000.0;

  for (unsigned int i = 0; i < count; i++)
    if (i != idx)
    {
      float distance = 0.0;

      distance+= (particle[idx].x - particle[i].x)*(particle[idx].x - particle[i].x);
      distance+= (particle[idx].y - particle[i].y)*(particle[idx].y - particle[i].y);
      distance+= (particle[idx].z - particle[i].z)*(particle[idx].z - particle[i].z);

      distance = sqrt(distance);

      if (distance < distance_min)
      {
        distance_min = distance;
        other = i;
      }
    }

    if (distance_min < particle[idx].r+particle[other].r)
    {
      /*
      float m = 2.0*particle[other].m/(particle[idx].m + particle[other].m);
      float d = distance_min*distance_min;

      particle[idx].ax = -m*(particle[idx].vx - particle[other].vx)*(particle[idx].x - particle[other].x)*(particle[idx].x - particle[other].x)/d;
      particle[idx].ay = -m*(particle[idx].vy - particle[other].vy)*(particle[idx].y - particle[other].y)*(particle[idx].y - particle[other].y)/d;
      particle[idx].az = -m*(particle[idx].vz - particle[other].vz)*(particle[idx].z - particle[other].z)*(particle[idx].z - particle[other].z)/d;
      */

      particle[idx].vx*= -1.0;
      particle[idx].vy*= -1.0;
      particle[idx].vz*= -1.0;
      
    }
    else
    {
      particle[idx].ax = 0.0;
      particle[idx].ay = 0.0;
      particle[idx].az = 0.0;
    }
}


void particles_update_cpu_kernel(unsigned int idx, sParticle *particle, float dt)
{
    particle[idx].vx+= particle[idx].ax;
    particle[idx].vy+= particle[idx].ay;
    particle[idx].vz+= particle[idx].az;

    particle[idx].x+= particle[idx].vx*dt;
    particle[idx].y+= particle[idx].vy*dt;
    particle[idx].z+= particle[idx].vz*dt;
}


void particles_limit_ranges_cpu_kernel(unsigned int idx, sParticle *particle, float dt)
{
    float k = 0.999;

    if (particle[idx].x > 1.0)
    {
      particle[idx].x = k;
      particle[idx].vx*= -1.0;
    }
    else
    if (particle[idx].x < -1.0)
    {
      particle[idx].x = -k;
      particle[idx].vx*= -1.0;
    }
    else
    if (particle[idx].y > 1.0)
    {
      particle[idx].y = k;
      particle[idx].vy*= -1.0;
    }
    else
    if (particle[idx].y < -1.0)
    {
      particle[idx].y = -k;
      particle[idx].vy*= -1.0;
    }
    if (particle[idx].z > 1.0)
    {
      particle[idx].z = k;
      particle[idx].vz*= -1.0;
    }
    else
    if (particle[idx].z < -1.0)
    {
      particle[idx].z = -k;
      particle[idx].vz*= -1.0;
    }
}
