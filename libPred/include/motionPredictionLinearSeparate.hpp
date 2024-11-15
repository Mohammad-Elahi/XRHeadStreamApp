#ifndef MOTIONPREDICTIONLINSEP_HPP
#define MOTIONPREDICTIONLINSEP_HPP

#include "quaternion.hpp"
#include "motionPredictionInterface.hpp"
#include "linearModelOrientation.hpp"
#include "linearModelTranslation.hpp"

// #include "util.hpp"
// constexpr unsigned int PREDICTION_LENGTH = 500;
// constexpr unsigned int LOOKBACK_LENGTH = 100;

class motionPredctionLinearSeparate : public IMotionPredictor
{
public:
    motionPredctionLinearSeparate();
    bool getPose(double time, Pose &pose);
    bool addPoseSample(const Pose &pose);
    bool buffersFilled();
    void provideWeightsFileRaw(std::vector<uint8_t> bufferOrientation, std::vector<uint8_t> bufferTranslation);

private:
    template <typename T>
    void addToSampleBuffer(std::vector<T> &buf, T el);

    template <typename T>
    std::vector<T> createUniformTimeseries(std::vector<T> quatsRaw, double samplingInterval, double horizonLength);

    std::vector<quaternion> sampleBufferOrientation;
    std::vector<position> sampleBufferTranslation;

    linearModelOrientation linModelOrientation;
    linearModelTranslation linModelTranslation;

    std::vector<quaternion> predBufferQuat;
    std::vector<position> predBufferPos;
    double lastLatestValueTime;
};

#endif /* MOTIONPREDICTIONLINSEP_HPP */