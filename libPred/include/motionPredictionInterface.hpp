#ifndef MOTIONPREDICTIONINTERFACE_HPP
#define MOTIONPREDICTIONINTERFACE_HPP

#include "quaternion.hpp"

class IMotionPredictor
{
public:
    virtual bool getPose(double time, Pose &pose) = 0;
    virtual bool addPoseSample(const Pose &pose) = 0;
};

#endif /* MOTIONPREDICTIONINTERFACE_HPP */