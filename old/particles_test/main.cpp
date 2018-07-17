#include <particles.h>
#include <particles_log.h>
#include <particles_visualisation.h>
#include <timer.h>


void create_dataset(unsigned int iterations, std::string path)
{
  Particles particles(50, 0.01);
  ParticlesVisualisation visualisation;
  ParticlesLog result_log(path, particles.count());

  for (unsigned int i = 0; i < iterations; i++)
  {
    result_log.add(particles);
    particles.process();
    visualisation.process(particles);
  }
}


void demo()
{
  Particles particles_a(50, 0.01);
//  Particles particles_b(50, 0.01);
  ParticlesVisualisation visualisation;

  while(1)
  {
    particles_a.process();
//    particles_b.process();
    visualisation.process(particles_a);
    timer.sleep_ms(10);
  }
}

int main()
{
  srand(time(NULL));


  create_dataset(5000, "/home/michal/dataset/collisions/training/");
  create_dataset(5000, "/home/michal/dataset/collisions/testing/");

//  demo();

  printf("program done\n");
  return 0;
}
