#ifndef _BASIS_H_
#define _BASIS_H_

#include "particle.h"
#include <vector>
#include <string>


class Basis
{
  private:
    std::vector<std::vector<t_partnum>> input;
    std::vector<std::vector<t_partnum>> output;

    std::vector<t_partnum> similarity;


    unsigned int used_ptr;
    t_partnum learning_rate, k;
    unsigned int input_size, output_size;

  public:
    Basis(  unsigned int input_size,
            unsigned int output_size,
            unsigned int basis_count,
            t_partnum learning_rate,
            t_partnum k
          );

    virtual ~Basis();


    void learn(std::vector<t_partnum> &required_output, std::vector<t_partnum> &input_);
    std::vector<t_partnum> get(std::vector<t_partnum> &input_);

    void save(std::string file_name);

    unsigned int get_input_size()
    {
      return input_size;
    }

    unsigned int get_output_size()
    {
      return output_size;
    }

  private:
    void compute_similarity(std::vector<t_partnum> &input_);
    t_partnum similarity_function(std::vector<t_partnum> &va, std::vector<t_partnum> &vb);
};

#endif
