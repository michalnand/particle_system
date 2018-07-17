#ifndef _TRAJECTORY_EXPERIMENT_H_
#define _TRAJECTORY_EXPERIMENT_H_

#include <particles_trajectory.h>
#include <basis.h>
#include <log.h>
#include <json_config.h>

class TrajectoryExperiment
{
  private:
    ParticlesTrajectory *training;
    ParticlesTrajectory *testing;
    ParticlesTrajectory *predicted;

    Basis *basis;

    JsonConfig json;
    std::string result_dir;
    unsigned int training_iterations;

  public:
    TrajectoryExperiment(std::string json_config_file_name);
    virtual ~TrajectoryExperiment();

    void train();

    t_partnum monte_carlo_error(unsigned int points_count);

    void predict_trajectory();
    void local_prediction();

  private:
    void save_basis_image(unsigned int height, unsigned int width);

    t_partnum map(  t_partnum source_min, t_partnum source_max,
                    t_partnum destination_min, t_partnum destination_max,
                    t_partnum value);

};


#endif
