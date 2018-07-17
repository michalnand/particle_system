#include "trajectory_experiment.h"

#include <iostream>
#include <image.h>

TrajectoryExperiment::TrajectoryExperiment(std::string json_config_file_name)
{
  json.load(json_config_file_name);


  training  = new ParticlesTrajectory(json.result["training"].asString());
  testing   = new ParticlesTrajectory(json.result["testing"].asString());
  predicted = new ParticlesTrajectory(json.result["testing"].asString(), ParticleTrajectory_READ_MODE_FIRST);


  unsigned int basis_count = json.result["basis_count"].asInt();
  float learning_rate = json.result["learning_rate"].asFloat();
  float k = json.result["k"].asFloat();

  basis = new Basis(3, 3, basis_count, learning_rate, k);

  training_iterations = 50*basis_count;

  result_dir = json.result["result_dir"].asString();
}


TrajectoryExperiment::~TrajectoryExperiment()
{
  delete training;
  delete testing;
  delete predicted;

  delete basis;
}


void TrajectoryExperiment::train()
{
  std::vector<t_partnum> input(basis->get_input_size());
  std::vector<t_partnum> required_output(basis->get_output_size());

//  Log error_progress_log(result_dir+"/error_progress.log");

  for (unsigned int i = 0; i < training_iterations; i++)
  {
    Particle particle = training->get_random().get_random();

    input[0] = particle.x();
    input[1] = particle.y();
    input[2] = particle.z();

    required_output[0] = particle.vx();
    required_output[1] = particle.vy();
    required_output[2] = particle.vz();

    basis->learn(required_output, input);

    if (i > 100)
    if ((i%100) == 0)
    {
      std::cout << "learning done " << i*100.0/training_iterations << "%\n";

      /*
      t_partnum error = monte_carlo_error(1000);
      error_progress_log << i << " " << error << "\n";
      */
    }
  }

  std::cout << "saving basis\n";

  basis->save(result_dir+"/basis.dat");

  std::cout << "saving basis images\n";
  save_basis_image(256, 700);

  std::cout << "training done\n";
}


t_partnum TrajectoryExperiment::monte_carlo_error(unsigned int points_count)
{
  t_partnum result = 0.0;

  std::vector<t_partnum> input(basis->get_input_size());

  for (unsigned int i = 0; i < points_count; i++)
  {
    Particle particle_reference = testing->get_random().get_random();

    input[0] = particle_reference.x();
    input[1] = particle_reference.y();
    input[2] = particle_reference.z();

    auto basis_output = basis->get(input);

    Particle particle_testing(  particle_reference.x(), particle_reference.y(), particle_reference.z(),
                                basis_output[0], basis_output[1], basis_output[2]);

    result+= particle_reference.velocity_difference(particle_testing);
  }

  result/= points_count;

  return result;
}


void TrajectoryExperiment::predict_trajectory()
{
  std::vector<t_partnum> input(basis->get_input_size());

  unsigned int iterations_count = 0;

  for (unsigned int particle_id = 0; particle_id < testing->size(); particle_id++)
    if (testing->get(particle_id).size() > iterations_count)
      iterations_count = testing->get(particle_id).size();


  Log error_log(result_dir+"/summary_error_during_trajectory_reconstruction.log");

  for (unsigned int iteration = 0; iteration < iterations_count; iteration++)
  {
    std::cout << "processing trajectory " << iteration*100.0/iterations_count << "%\n";

    t_partnum error_average = 0.0;
    t_partnum error_min     = 1000000000.0;
    t_partnum error_max     = -error_min;

    unsigned int real_cells_count = 0;

    for (unsigned int particle_id = 0; particle_id < testing->size(); particle_id++)
    {
      if (iteration < testing->get(particle_id).size())
      {
        Particle particle = predicted->get(particle_id).get(iteration);

        input[0] = particle.x();
        input[1] = particle.y();
        input[2] = particle.z();

        auto basis_output = basis->get(input);

        particle.update_position(basis_output[0], basis_output[1], basis_output[2]);

        predicted->get(particle_id).add(particle);

        Particle required_particle = testing->get(particle_id).get(iteration);

        t_partnum error = required_particle.position_difference(particle);

        error_average+= error;
        if (error > error_max)
          error_max = error;
        if (error < error_min)
          error_min = error;
        real_cells_count++;
      }
    }

    error_average/= real_cells_count;

    error_log << iteration << " ";
    error_log << error_average << " ";
    error_log << error_min << " ";
    error_log << error_max << "\n";

  }

  for (unsigned int particle = 0; particle < testing->size(); particle++)
  {
    testing->get(particle).save(result_dir+"/required_"+std::to_string(particle)+".dat");
  }

  for (unsigned int particle = 0; particle < predicted->size(); particle++)
  {
    predicted->get(particle).save(result_dir+"/predicted_"+std::to_string(particle)+".dat");
  }

  JsonConfig trajectory_required_json_log;

  for (unsigned int particle_id = 0; particle_id < testing->size(); particle_id++)
  for (unsigned int iteration = 0; iteration < testing->get(particle_id).size(); iteration++)
  {
    Particle particle = testing->get(particle_id).get(iteration);

    trajectory_required_json_log.result[particle_id][iteration]["position"][0] = particle.x();
    trajectory_required_json_log.result[particle_id][iteration]["position"][1] = particle.y();
    trajectory_required_json_log.result[particle_id][iteration]["position"][2] = particle.z();

    trajectory_required_json_log.result[particle_id][iteration]["velocity"][0] = particle.vx();
    trajectory_required_json_log.result[particle_id][iteration]["velocity"][1] = particle.vy();
    trajectory_required_json_log.result[particle_id][iteration]["velocity"][2] = particle.vz();
  }

  trajectory_required_json_log.save(result_dir+"/trajectory_required.json");

  JsonConfig trajectory_predicted_json_log;


  for (unsigned int particle_id = 0; particle_id < predicted->size(); particle_id++)
  for (unsigned int iteration = 0; iteration < predicted->get(particle_id).size(); iteration++)
  {
    Particle particle = predicted->get(particle_id).get(iteration);

    trajectory_predicted_json_log.result[particle_id][iteration]["position"][0] = particle.x();
    trajectory_predicted_json_log.result[particle_id][iteration]["position"][1] = particle.y();
    trajectory_predicted_json_log.result[particle_id][iteration]["position"][2] = particle.z();

    trajectory_predicted_json_log.result[particle_id][iteration]["velocity"][0] = particle.vx();
    trajectory_predicted_json_log.result[particle_id][iteration]["velocity"][1] = particle.vy();
    trajectory_predicted_json_log.result[particle_id][iteration]["velocity"][2] = particle.vz();
  }

  trajectory_predicted_json_log.save(result_dir+"/trajectory_predicted.json");
}


void TrajectoryExperiment::local_prediction()
{
  std::vector<t_partnum> input(basis->get_input_size());
  Log result_log_all(result_dir+"/local_prediction_all.log");


  JsonConfig local_prediction_json;


  for (unsigned int particle_id = 0; particle_id < testing->size(); particle_id++)
  {
    std::cout << "processing particle " << particle_id*100.0/testing->size() << "%\n";

    Log result_log(result_dir+"/local_prediction_"+std::to_string(particle_id)+".log");

    for (unsigned int iteration = 0; iteration < testing->get(particle_id).size(); iteration++)
    {
      Particle particle = testing->get(particle_id).get(iteration);

      input[0] = particle.x();
      input[1] = particle.y();
      input[2] = particle.z();

      auto basis_output = basis->get(input);

      result_log << particle.x() << " ";
      result_log << particle.y() << " ";
      result_log << particle.z() << " ";

      result_log << particle.vx() << " ";
      result_log << particle.vy() << " ";
      result_log << particle.vz() << " ";

      result_log << basis_output[0] << " ";
      result_log << basis_output[1] << " ";
      result_log << basis_output[2] << "\n";


      result_log_all << particle.x() << " ";
      result_log_all << particle.y() << " ";
      result_log_all << particle.z() << " ";

      result_log_all << particle.vx() << " ";
      result_log_all << particle.vy() << " ";
      result_log_all << particle.vz() << " ";

      result_log_all << basis_output[0] << " ";
      result_log_all << basis_output[1] << " ";
      result_log_all << basis_output[2] << "\n";

      local_prediction_json.result[particle_id][iteration]["position"][0] = particle.x();
      local_prediction_json.result[particle_id][iteration]["position"][1] = particle.y();
      local_prediction_json.result[particle_id][iteration]["position"][2] = particle.z();

      local_prediction_json.result[particle_id][iteration]["velocity"][0] = particle.vx();
      local_prediction_json.result[particle_id][iteration]["velocity"][1] = particle.vy();
      local_prediction_json.result[particle_id][iteration]["velocity"][2] = particle.vz();

      local_prediction_json.result[particle_id][iteration]["predicted_velocity"][0] = basis_output[0];
      local_prediction_json.result[particle_id][iteration]["predicted_velocity"][1] = basis_output[1];
      local_prediction_json.result[particle_id][iteration]["predicted_velocity"][2] = basis_output[2];
    }
  }

  local_prediction_json.save(result_dir+"/local_prediction.json");
}

void TrajectoryExperiment::save_basis_image(unsigned int height, unsigned int width)
{
  Image image_xyz(width, height);
  Image image_x(width, height);
  Image image_y(width, height);
  Image image_z(width, height);

  std::vector<t_partnum> input(basis->get_input_size());

  for (unsigned int y = 0; y < height; y++)
  {
    for (unsigned int x = 0; x < width; x++)
    {
      t_partnum x_ = map(0, width, training->min_x(), training->max_x(), x);
      t_partnum y_ = map(0, height, training->min_y(), training->max_y(), y);
      t_partnum z_ = (training->min_z() + training->max_z())/2.0;

      input[0] = x_;
      input[1] = y_;
      input[2] = z_;

      auto basis_output = basis->get(input);

      image_xyz.pixels[y][x].b[0] = basis_output[0];
      image_xyz.pixels[y][x].b[1] = basis_output[1];
      image_xyz.pixels[y][x].b[2] = basis_output[2];

      image_x.pixels[y][x].b[0] = basis_output[0];
      image_x.pixels[y][x].b[1] = basis_output[0];
      image_x.pixels[y][x].b[2] = basis_output[0];

      image_y.pixels[y][x].b[0] = basis_output[1];
      image_y.pixels[y][x].b[1] = basis_output[1];
      image_y.pixels[y][x].b[2] = basis_output[1];

      image_z.pixels[y][x].b[0] = basis_output[2];
      image_z.pixels[y][x].b[1] = basis_output[2];
      image_z.pixels[y][x].b[2] = basis_output[2];
    }
  }

  image_xyz.save(result_dir+"/basis_xyz.png");
  image_x.save(result_dir+"/basis_x.png");
  image_y.save(result_dir+"/basis_y.png");
  image_z.save(result_dir+"/basis_z.png");
}


t_partnum TrajectoryExperiment::map(  t_partnum source_min, t_partnum source_max,
                                      t_partnum destination_min, t_partnum destination_max,
                                      t_partnum value)
{
  t_partnum result = 0.0;

  if (source_min >= source_max)
    return result;

  if (destination_min >= destination_max)
    return result;

  t_partnum k = (destination_max - destination_min)/(source_max - source_min);
  t_partnum q = destination_max - k*source_max;

  result = k*value + q;

  return result;
}
