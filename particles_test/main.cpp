#include <trajectory_experiment.h>

#include <iostream>
#include <thread>
#include <timer.h>

std::string channel_name;

void experiment_0()
{
  TrajectoryExperiment experiment(channel_name+"/256_basis/parameters.json");

  experiment.train();
  experiment.predict_trajectory();
  experiment.local_prediction();
}

void experiment_1()
{
  TrajectoryExperiment experiment(channel_name+"/1024_basis/parameters.json");

  experiment.train();
  experiment.predict_trajectory();
  experiment.local_prediction();
}

void experiment_2()
{
  TrajectoryExperiment experiment(channel_name+"/4096_basis/parameters.json");

  experiment.train();
  experiment.predict_trajectory();
  experiment.local_prediction();
}

void experiment_3()
{
  TrajectoryExperiment experiment(channel_name+"/4096_basis_verification/parameters.json");

  experiment.train();
  experiment.predict_trajectory();
  experiment.local_prediction();
}

void experiment_4()
{
  TrajectoryExperiment experiment(channel_name+"/4096_basis_swap/parameters.json");

  experiment.train();
  experiment.predict_trajectory();
  experiment.local_prediction();
}

void experiment_5()
{
  TrajectoryExperiment experiment(channel_name+"/4096_basis_swap_verification/parameters.json");

  experiment.train();
  experiment.predict_trajectory();
  experiment.local_prediction();
}



void experiment_6()
{
  TrajectoryExperiment experiment(channel_name+"/8192_basis/parameters.json");

  experiment.train();
  experiment.predict_trajectory();
  experiment.local_prediction();
}


int main()
{
  std::vector<std::string> channels;

  channels.push_back("kanal_mato_sim50");
  channels.push_back("kanal_kika_sim6");
  channels.push_back("kanal_monika_sim3044");
  channels.push_back("kanal_kika_sim6_modulo");
  channels.push_back("kanal_kika_sim101");


  for (unsigned int i = 0; i < channels.size(); i++)
  {
    channel_name = channels[i];
    std::thread t_experiment_0(experiment_0);
    std::thread t_experiment_1(experiment_1);
    std::thread t_experiment_2(experiment_2);
    std::thread t_experiment_3(experiment_3);
    std::thread t_experiment_4(experiment_4);
    std::thread t_experiment_5(experiment_5);
    std::thread t_experiment_6(experiment_6);

    t_experiment_0.join();
    t_experiment_1.join();
    t_experiment_2.join();
    t_experiment_3.join();
    t_experiment_4.join();
    t_experiment_5.join();
    t_experiment_6.join();
  }

  printf("program done\n");
  return 0;
}
