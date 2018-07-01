#include <particles.h>
#include <particles_log.h>
#include <particles_visualisation.h>
#include <timer.h>


void create_dataset(unsigned int iterations, float size, std::string path)
{
  Particles particles(50, size, 0.01);
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
  Particles particles_a(50, 0.08, 0.01);
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

  unsigned int count = 10000;
/*
  float size = 0.04;
  create_dataset(count, size, "/home/michal/dataset/cells/particles_0/training/");
  create_dataset(count, size, "/home/michal/dataset/cells/particles_0/testing/");

  size = 0.06;
  create_dataset(count, size, "/home/michal/dataset/cells/particles_1/training/");
  create_dataset(count, size, "/home/michal/dataset/cells/particles_1/testing/");

  size = 0.08;
  create_dataset(count, size, "/home/michal/dataset/cells/particles_2/training/");
  create_dataset(count, size, "/home/michal/dataset/cells/particles_2/testing/");
*/

  demo();

  printf("program done\n");
  return 0;
}
