#include <particles.h>
#include <particles_visualisation.h>
#include <timer.h>

int main()
{
  srand(time(NULL));


  Particles particles(1024, 0.003);
  ParticlesVisualisation visualisation;

  float k = 0.99;
  float computing_time = 0.0;

  while (1)
  {
      timer.start();
      particles.process();
      timer.stop();

      computing_time = k*computing_time + (1.0 - k)*timer.get_duration();
      printf("computing time %f [ms]\n", computing_time);

      if ((particles.iteration()%2) == 0)
      {
        visualisation.process(particles);
        timer.sleep_ms(5);
      }
  }



  printf("program done\n");
  return 0;
}
