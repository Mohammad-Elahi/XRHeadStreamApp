#ifndef LINEARMODELTRANSLATION_HPP
#define LINEARMODELTRANSLATION_HPP

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

#include <vector>
#include <fstream>

#include <iostream>

#include "quaternion.hpp"

class linearModelTranslation
{
public:
  linearModelTranslation(int pred_length, int lookback_length, double dt);
  bool readWeights();
  std::vector<position> predict(std::vector<position> past);
  void provideWeightsFileRaw(std::vector<uint8_t> buffer);

private:
  int pred_length;
  int lookback_length;
  double dt;
  Eigen::MatrixXd weights;
};

#endif /* LINEARMODELTRANSLATION_HPP */