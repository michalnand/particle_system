#include "basis.h"
#include <math.h>
#include <log.h>

Basis::Basis(   unsigned int input_size,
                unsigned int output_size,
                unsigned int basis_count,
                t_partnum learning_rate,
                t_partnum k
            )
{
  this->input_size = input_size;
  this->output_size = output_size;
  
  std::vector<t_partnum> input_tmp(input_size);
  std::vector<t_partnum> output_tmp(output_size);

  for (unsigned int i = 0; i < input_tmp.size(); i++)
    input_tmp[i] = 0.0;

  for (unsigned int i = 0; i < output_tmp.size(); i++)
    output_tmp[i] = 0.0;


  this->learning_rate = learning_rate;
  this->k = k;

  used_ptr = 0;

  for (unsigned int i = 0; i < basis_count; i++)
  {
    input.push_back(input_tmp);
    output.push_back(output_tmp);
  }

  for (unsigned int i = 0; i < basis_count; i++)
    similarity.push_back(0.0);
}


Basis::~Basis()
{

}


void Basis::learn(std::vector<t_partnum> &required_output, std::vector<t_partnum> &input_)
{
  if (used_ptr < input.size())
  {
    input[used_ptr]   = input_;
    output[used_ptr]  = required_output;
    used_ptr++;
  }
  else
  {
    compute_similarity(input_);

    for (unsigned int j = 0; j < input.size(); j++)
    {
      t_partnum v = similarity[j];
      for (unsigned int i = 0; i < input[j].size(); i++)
        input[j][i]+= learning_rate*(input_[i] - input[j][i])*v;
    }

    for (unsigned int j = 0; j < output.size(); j++)
    {
      t_partnum v = similarity[j];
      for (unsigned int i = 0; i < output[j].size(); i++)
        output[j][i]+= learning_rate*(required_output[i] - output[j][i])*v;
    }

  }
}

std::vector<t_partnum> Basis::get(std::vector<t_partnum> &input_)
{
  std::vector<t_partnum> result(output[0].size());
  for (unsigned int i = 0; i < result.size(); i++)
    result[i] = 0.0;

  compute_similarity(input_);

  for (unsigned int j = 0; j < output.size(); j++)
  {
    t_partnum v = similarity[j];
    for (unsigned int i = 0; i < output[j].size(); i++)
      result[i]+= v*output[j][i];
  }

  return result;
}


void Basis::compute_similarity(std::vector<t_partnum> &input_)
{
  for (unsigned int i = 0; i < similarity.size(); i++)
    similarity[i] = 0.0;

  t_partnum sum = 0.0;
  for (unsigned int i = 0; i < used_ptr; i++)
  {
    similarity[i] = similarity_function(input[i], input_);
    sum+= similarity[i];
  }

  for (unsigned int i = 0; i < used_ptr; i++)
    similarity[i]/= sum;
}

t_partnum Basis::similarity_function(std::vector<t_partnum> &va, std::vector<t_partnum> &vb)
{
  t_partnum result = 0.0;

  for (unsigned int i = 0; i < va.size(); i++)
  {
    t_partnum tmp = va[i] - vb[i];
    result+= tmp*tmp;
  }

  result = exp(-k*result);

  return result;
}


void Basis::save(std::string file_name)
{
  Log result_log(file_name);


  for (unsigned int j = 0; j < input.size(); j++)
  {
    for (unsigned int i = 0; i < input[j].size(); i++)
      result_log << input[j][i] << " ";

    for (unsigned int i = 0; i < output[j].size(); i++)
      result_log << output[j][i] << " ";

    result_log << "\n";
  }
}
