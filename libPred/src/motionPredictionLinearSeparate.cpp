#include "motionPredictionLinearSeparate.hpp"

motionPredctionLinearSeparate::motionPredctionLinearSeparate() : linModelOrientation(500, 100, 1.0 / 1000.0), linModelTranslation(500, 100, 1.0 / 1000.0)
{
    this->lastLatestValueTime = 0.0;
    int PREDICTION_LENGTH = 500;
    int LOOKBACK_LENGTH = 100;

    // linModelOrientation = linearModelOrientation(PREDICTION_LENGTH, LOOKBACK_LENGTH, 1.0 / 1000.0);
    linModelOrientation.readWeights();

    // linModelTranslation = linearModelTranslation(PREDICTION_LENGTH, LOOKBACK_LENGTH, 1.0 / 1000.0);
    linModelTranslation.readWeights();

    sampleBufferOrientation = std::vector<quaternion>(500);
    sampleBufferTranslation = std::vector<position>(500);
}

bool motionPredctionLinearSeparate::buffersFilled()
{
    bool ret;
    if (this->sampleBufferOrientation.back().timestamp == 0.0)
    {
        return false;
    }
    double smallestVal = this->sampleBufferOrientation.back().timestamp;
    for (int i = 1; i < this->sampleBufferOrientation.size(); ++i)
    {
        if (this->sampleBufferOrientation[i].timestamp != 0.0)
        {

            smallestVal = std::min(this->sampleBufferOrientation[i].timestamp, smallestVal);
        }
    }
    double diff = this->sampleBufferOrientation.back().timestamp - smallestVal;
    ret = diff >= 0.1;
    return ret;
}

bool motionPredctionLinearSeparate::getPose(double time, Pose &pose)
{
    int PREDICTION_LENGTH = 500;
    int LOOKBACK_LENGTH = 100;
    if (time > (sampleBufferOrientation.back().timestamp + (((double)PREDICTION_LENGTH) / 1000.0)))
    {
        // return false;
        time = (sampleBufferOrientation.back().timestamp + (((double)PREDICTION_LENGTH) / 1000.0));
    }

    // could also do interpolation into past...
    if (time < sampleBufferOrientation.back().timestamp)
    {
        return false;
    }

    if (!this->buffersFilled())
    {
        return false;
    }

    // double lastTime = sampleBufferOrientation[0].timestamp;
    // double smallestDiff = 999.9;
    // for (int i = 1; i < sampleBufferOrientation.size(); ++i)
    // {
    //     double diff = sampleBufferOrientation[i].timestamp - lastTime;
    //     lastTime = sampleBufferOrientation[i].timestamp;
    //     if (lastTime != 0.0)
    //     {
    //         smallestDiff = std::min(smallestDiff, diff);
    //     }
    // }
    // std::cout << " " << smallestDiff * 1000.0 << std::endl;

    // buffer when running multiple predictions on same data
    if ((lastLatestValueTime != sampleBufferOrientation.back().timestamp) || (lastLatestValueTime == 0.0))
    {

        std::vector<quaternion> pastSamplesUniform = createUniformTimeseries(sampleBufferOrientation, 1.0 / 1000.0, 100);
        quaternion normalizer = pastSamplesUniform[LOOKBACK_LENGTH - 1];
        auto pastSamples_norm = normlizeQuatSeries(pastSamplesUniform, normalizer);
        // std::cout << "Last normalized input sample: (" << pastSamples_norm[999].x << ", " << pastSamples_norm[999].y << ", " << pastSamples_norm[999].z << ", " << pastSamples_norm[999].w << ")\n";
        auto predSamples_norm = linModelOrientation.predict(pastSamples_norm);
        // std::cout << "First normalized output sample: (" << predSamples_norm[0].x << ", " << predSamples_norm[0].y << ", " << predSamples_norm[0].z << ", " << predSamples_norm[0].w << ")\n";
        auto predSamples = denormlizeQuatSeries(predSamples_norm, normalizer);
        this->predBufferQuat = predSamples;

        std::vector<position> pastPositionSamplesUniform = createUniformTimeseries(sampleBufferTranslation, 1.0 / 1000.0, 100);
        position normalizerPosition = pastPositionSamplesUniform[LOOKBACK_LENGTH - 1];
        auto pastPositionSamples_norm = normalizePositionSeries(pastPositionSamplesUniform, normalizerPosition);
        // std::cout << "Last normalized input sample: (" << pastSamples_norm[999].x << ", " << pastSamples_norm[999].y << ", " << pastSamples_norm[999].z << ", " << pastSamples_norm[999].w << ")\n";
        auto predSamplesPosition_norm = linModelTranslation.predict(pastPositionSamples_norm);
        // std::cout << "First normalized output sample: (" << predSamples_norm[0].x << ", " << predSamples_norm[0].y << ", " << predSamples_norm[0].z << ", " << predSamples_norm[0].w << ")\n";
        auto predSamplesPosition = denormalizePositionSeries(predSamplesPosition_norm, normalizerPosition);
        this->predBufferPos = predSamplesPosition;
        this->lastLatestValueTime = pastPositionSamplesUniform.back().timestamp;
    }

    pose.timestamp = time;
    pose.orientation.timestamp = time;
    pose.position.timestamp = time;
    double sampleTime = 1.0 / 1000.0;
    size_t index = ((time - lastLatestValueTime - sampleTime) / sampleTime);
    pose.orientation = predBufferQuat[index];
    pose.position = predBufferPos[index];
    pose.orientation.normalize();
    return true;
}

bool motionPredctionLinearSeparate::addPoseSample(const Pose &pose)
{
    int PREDICTION_LENGTH = 500;
    int LOOKBACK_LENGTH = 100;

    addToSampleBuffer(sampleBufferOrientation, pose.orientation);
    addToSampleBuffer(sampleBufferTranslation, pose.position);
    return true;
}

template <typename T>
void motionPredctionLinearSeparate::addToSampleBuffer(std::vector<T> &buf, T el)
{
    if (el.timestamp > buf.back().timestamp)
    {
        for (int i = 1; i < buf.size(); ++i)
        {
            buf[i - 1] = buf[i];
        }
        buf[buf.size() - 1] = el;
    }
}

template <typename T>
std::vector<T> motionPredctionLinearSeparate::createUniformTimeseries(std::vector<T> quatsRaw, double samplingInterval, double horizonLength)
{
    int ind = quatsRaw.size() - 1;
    // clock_t time = quatsRaw.at(ind).timestamp;
    double time = quatsRaw.at(ind).timestamp;
    std::vector<T> out{quatsRaw.at(ind)};
    while (out.size() < horizonLength)
    {
        while (time > quatsRaw.at(ind - 1).timestamp)
        {
            // time = time - samplingInterval * ((double)CLOCKS_PER_SEC);
            time = time - samplingInterval;
            double ratio = ((double)(quatsRaw.at(ind).timestamp - time)) / ((double)(quatsRaw.at(ind).timestamp - quatsRaw.at(ind - 1).timestamp));
            T q = (ratio * quatsRaw[ind - 1]) + ((1 - ratio) * quatsRaw[ind]);
            if (out.size() < horizonLength)
            {
                out.push_back(q);
            }
        }
        ind += -1;
    }
    std::reverse(out.begin(), out.end());
    return out;
}

void motionPredctionLinearSeparate::provideWeightsFileRaw(std::vector<uint8_t> bufferOrientation, std::vector<uint8_t> bufferTranslation) {
    this->linModelOrientation.provideWeightsFileRaw(bufferOrientation);
    this->linModelTranslation.provideWeightsFileRaw(bufferTranslation);
}
