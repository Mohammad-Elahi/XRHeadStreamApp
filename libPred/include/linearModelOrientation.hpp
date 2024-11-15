#ifndef LINEARMODELORIENTATION_HPP
#define LINEARMODELORIENTATION_HPP

#define ON_ANDROID


#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

#include <vector>
#include <fstream>

#include <iostream>

#include "quaternion.hpp"

class linearModelOrientation
{
public:
  linearModelOrientation(int pred_length, int lookback_length, double dt);
  bool readWeights();
  void provideWeightsFileRaw(std::vector<uint8_t> buffer);
  std::vector<quaternion> predict(std::vector<quaternion> past);

private:
  int pred_length;
  int lookback_length;
  double dt;
  Eigen::MatrixXd weights;
};

#endif /* LINEARMODELORIENTATION_HPP */