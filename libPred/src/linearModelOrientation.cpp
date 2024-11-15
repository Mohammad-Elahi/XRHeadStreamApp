#include "linearModelOrientation.hpp"

linearModelOrientation::linearModelOrientation(int pred_length, int lookback_length, double dt)
{
  this->pred_length = pred_length;
  this->lookback_length = lookback_length;
  this->dt = dt;
}

bool linearModelOrientation::readWeights()
{
  Eigen::MatrixXd weights(4 * this->pred_length, 4 * this->lookback_length);
//  std::ifstream weightsFile("/storage/emulated/0/Download/weightsOrientation.txt");
  std::ifstream weightsFile("apk:///assets/weightsOrientation.txt");

//    AAsset* asset = AAssetManager_open(mgr, (const char *) js, AASSET_MODE_UNKNOWN);
//    if (NULL == asset) {
//        __android_log_print(ANDROID_LOG_ERROR, NF_LOG_TAG, "_ASSET_NOT_FOUND_");
//        return JNI_FALSE;
//    }
//    long size = AAsset_getLength(asset);
//    char* buffer = (char*) malloc (sizeof(char)*size);
//    AAsset_read (asset,buffer,size);
//    __android_log_print(ANDROID_LOG_ERROR, NF_LOG_TAG, buffer);
//    AAsset_close(asset);

  double tmp;

  int r = 0;
  int c = 0;
  while (weightsFile >> tmp)
  {
    weights(r, c) = tmp;
    c += 1;
    if (c >= (this->lookback_length * 4))
    {
      c = 0;
      r += 1;
    }
  }

  this->weights = weights;
  return true;
}

std::vector<quaternion> linearModelOrientation::predict(std::vector<quaternion> past)
{
  Eigen::VectorXd quats_flat(4 * this->lookback_length);
  for (int i = 0; i < this->lookback_length; ++i)
  {
    // quats_flat(i * 4) = past.at(past.size() - 1 - i).w;
    // quats_flat(i * 4 + 1) = past.at(past.size() - 1 - i).x;
    // quats_flat(i * 4 + 2) = past.at(past.size() - 1 - i).y;
    // quats_flat(i * 4 + 3) = past.at(past.size() - 1 - i).z;
    quats_flat(i * 4) = past.at(i).w;
    quats_flat(i * 4 + 1) = past.at(i).x;
    quats_flat(i * 4 + 2) = past.at(i).y;
    quats_flat(i * 4 + 3) = past.at(i).z;
  }
  // std::cout << quats_flat << std::endl;

  Eigen::VectorXd pred_flat = this->weights * quats_flat;

  // std::cout << this->weights(0, 0) << std::endl;
  // std::cout << this->weights(0, 1) << std::endl;
  // std::cout << this->weights(0, 2) << std::endl;

  // std::cout << pred_flat << std::endl;
  // std::cout << pred_flat(0) << pred_flat(1) << pred_flat(2) << pred_flat(3) << pred_flat(4) << std::endl;
  std::vector<quaternion> ret;
  for (int i = 0; i < this->pred_length; ++i)
  {
    quaternion q;
    // q.timestamp = past.back().timestamp + time_t(this->dt * (double)(i + 1) * (double)CLOCKS_PER_SEC);
    q.timestamp = past.back().timestamp + (this->dt * (double)(i + 1));
    q.w = pred_flat(i * 4 + 0);
    q.x = pred_flat(i * 4 + 1);
    q.y = pred_flat(i * 4 + 2);
    q.z = pred_flat(i * 4 + 3);
    q.normalize();
    ret.push_back(q);
  }
  return ret;
}

void linearModelOrientation::provideWeightsFileRaw(std::vector<uint8_t> buffer) {
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
    Eigen::MatrixXd weights(4 * this->pred_length, 4 * this->lookback_length);
    int r = 0;
    int c = 0;
    for (auto& tmp : doubles)
    {
        weights(r, c) = tmp;
        c += 1;
        if (c >= (this->lookback_length * 4))
        {
            c = 0;
            r += 1;
        }
    }
    this->weights = weights;
}