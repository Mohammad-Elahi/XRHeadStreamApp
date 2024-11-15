#include "linearModelTranslation.hpp"

linearModelTranslation::linearModelTranslation(int pred_length, int lookback_length, double dt)
{
  this->pred_length = pred_length;
  this->lookback_length = lookback_length;
  this->dt = dt;
}

bool linearModelTranslation::readWeights()
{
  Eigen::MatrixXd weights(3 * this->pred_length, 3 * this->lookback_length);
  std::ifstream weightsFile("/storage/emulated/0/Download/weightsTranslation.txt");
  double tmp;

  int r = 0;
  int c = 0;
  while (weightsFile >> tmp)
  {
    weights(r, c) = tmp;
    c += 1;
    if (c >= (this->lookback_length * 3))
    {
      c = 0;
      r += 1;
    }
  }

  this->weights = weights;
  return true;
}

std::vector<position> linearModelTranslation::predict(std::vector<position> past)
{
  Eigen::VectorXd positions_flat(3 * this->lookback_length);
  for (int i = 0; i < this->lookback_length; ++i)
  {
    positions_flat(i * 3 + 0) = past.at(i).x;
    positions_flat(i * 3 + 1) = past.at(i).y;
    positions_flat(i * 3 + 2) = past.at(i).z;
  }
  // std::cout << positions_flat << std::endl;

  Eigen::VectorXd pred_flat = this->weights * positions_flat;

  // std::cout << this->weights(0, 0) << std::endl;
  // std::cout << this->weights(0, 1) << std::endl;
  // std::cout << this->weights(0, 2) << std::endl;

  // std::cout << pred_flat << std::endl;
  // std::cout << pred_flat(0) << pred_flat(1) << pred_flat(2) << pred_flat(3) << pred_flat(4) << std::endl;
  std::vector<position> ret;
  for (int i = 0; i < this->pred_length; ++i)
  {
    position q;
    // q.timestamp = past.back().timestamp + time_t(this->dt * (double)(i + 1) * (double)CLOCKS_PER_SEC);
    q.timestamp = past.back().timestamp + (this->dt * (double)(i + 1));
    q.x = pred_flat(i * 3 + 0);
    q.y = pred_flat(i * 3 + 1);
    q.z = pred_flat(i * 3 + 2);
    ret.push_back(q);
  }
  return ret;
}

void linearModelTranslation::provideWeightsFileRaw(std::vector<uint8_t> buffer) {
    std::vector<char> chars;
    std::vector<double> doubles;
    for(size_t i = 0; i < buffer.size(); ++i) {
        if(buffer[i] == '\n' || buffer[i] == '\r') {
            if(chars.size() > 0) {
                std::string s(chars.begin(), chars.end());
                doubles.push_back(std::stod(s));
                chars.resize(0);
            }
        }
        else{
            chars.push_back(static_cast<char>(buffer[i]));
        }
    }
    Eigen::MatrixXd weights(3 * this->pred_length, 3 * this->lookback_length);
    int r = 0;
    int c = 0;
    for (auto& tmp : doubles)
    {
        weights(r, c) = tmp;
        c += 1;
        if (c >= (this->lookback_length * 3))
        {
            c = 0;
            r += 1;
        }
    }
    this->weights = weights;
}