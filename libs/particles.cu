#include "particles.h"
#include "particles_kernel.h"

#define PARTICLES_USE_CUDA  1


Particles::Particles()
{
  particles = nullptr;
  m_count = 0;
  m_dt = 1;
  m_iteration = 0;
  m_size = 0.0;
}

Particles::Particles(unsigned int count, float size, float dt)
{
  particles = nullptr;
  m_count = 0;
  m_dt = 1;
  m_iteration = 0;
  m_size = 0.0;

  init(count, size, dt);
}

Particles::~Particles()
{
  uninint();
}


void Particles::init(unsigned int count, float size, float dt)
{
  uninint();
  this->m_count = count;
  this->m_dt = dt;
  this->m_size = size;

  m_iteration = 0;

  #ifdef PARTICLES_USE_CUDA
    cudaMalloc(&particles, m_count*sizeof(sParticle));
  #else
    particles = new sParticle[m_count];
  #endif

  set_random();
}

void Particles::uninint()
{
  if (particles != nullptr)
  {
    #ifdef PARTICLES_USE_CUDA
      cudaFree(particles);
    #else
      delete particles;
    #endif

    particles = nullptr;
    m_count = 0;
  }
}

sParticle Particles::get(unsigned int idx)
{
  sParticle result;

  #ifdef PARTICLES_USE_CUDA
    cudaMemcpy(&result, &particles[idx], sizeof(sParticle), cudaMemcpyDeviceToHost);
  #else
    result = particles[idx];
  #endif

  return result;
}

std::vector<sParticle> Particles::get_all()
{
  std::vector<sParticle> result;
  for (unsigned int i = 0; i < m_count; i++)
    result.push_back(get(i));

  return result;
}

std::string Particles::get_string(unsigned int idx)
{
  sParticle tmp = get(idx);
  std::string result;

  result+= std::to_string(tmp.x) + " ";
  result+= std::to_string(tmp.y) + " ";
  result+= std::to_string(tmp.z) + " ";

  result+= std::to_string(tmp.vx) + " ";
  result+= std::to_string(tmp.vy) + " ";
  result+= std::to_string(tmp.vz) + " ";

  result+= std::to_string(tmp.ax) + " ";
  result+= std::to_string(tmp.ay) + " ";
  result+= std::to_string(tmp.az) + " ";

  result+= std::to_string(tmp.m) + " ";
  result+= std::to_string(tmp.r) + " ";

  return result;
}

void Particles::print(unsigned int idx)
{
  std::string tmp = get_string(idx);
  printf("%s\n", tmp.c_str());
}

void Particles::process()
{
  #ifdef PARTICLES_USE_CUDA

    dim3 block(256);
    dim3 grid((m_count  + block.x - 1)/block.x);

    particles_main_gpu_kernel<<<grid, block>>>(m_count, particles, m_dt);
    cudaDeviceSynchronize();

    particles_update_gpu_kernel<<<grid, block>>>(m_count, particles, m_dt);
    cudaDeviceSynchronize();

    particles_limit_ranges_gpu_kernel<<<grid, block>>>(m_count, particles, m_dt);
    cudaDeviceSynchronize();

  #else
    for (unsigned int i = 0; i < m_count; i++)
      particles_main_cpu_kernel(i, m_count, particles, m_dt);

    for (unsigned int i = 0; i < m_count; i++)
      particles_update_cpu_kernel(i, particles, m_dt);

    for (unsigned int i = 0; i < m_count; i++)
      particles_limit_ranges_cpu_kernel(i, particles, m_dt);
  #endif

  m_iteration++;
}

void Particles::set_random()
{
  for (unsigned int i = 0; i < m_count; i++)
  {
    sParticle tmp = random_particle(i);

    #ifdef PARTICLES_USE_CUDA
    cudaMemcpy(&particles[i], &tmp, sizeof(sParticle), cudaMemcpyHostToDevice);
    #else
    particles[i] = tmp;
    #endif
  }
}

sParticle Particles::random_particle(unsigned int idx)
{
  sParticle result;

  bool plane_mode = true;

  unsigned int side_size;
  float x, y, z;

  if (plane_mode)
  {
    side_size = sqrt(m_count);

    x = idx%side_size;
    y = idx/side_size;
    z = 0.0;

    x = (x/(1.0*side_size) - 0.5)*2.0;
    y = (y/(1.0*side_size) - 0.5)*2.0;
  }
  else
  {
    side_size = cbrt(m_count);

    x = idx%side_size;
    y = (idx/side_size)%side_size;
    z = idx/(side_size*side_size);

    x = (x/(1.0*side_size) - 0.5)*2.0;
    y = (y/(1.0*side_size) - 0.5)*2.0;
    z = (z/(1.0*side_size) - 0.5)*2.0;
  }


  result.r = m_size;
  result.m = 1.0;

  float k = (0.5/side_size);

  result.x = 0.98*(x + k*rnd());
  result.y = 0.98*(y + k*rnd());

  if (plane_mode)
    result.z = 0.0;
  else
    result.z = 0.98*(z + k*rnd());

  result.vx = rnd();
  result.vy = rnd();

  if (plane_mode)
    result.vz = 0.0;
  else
    result.vz = rnd();

  result.ax = 0.0;
  result.ay = 0.0;
  result.az = 0.0;



  return result;
}

float Particles::rnd()
{
  return ((rand()%2000000) - 1000000)/1000000.0;
}
