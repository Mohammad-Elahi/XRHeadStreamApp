#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include <math.h>
#include <time.h>
#include <vector>
#include <iostream>

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

struct quaternion
{
  // clock_t timestamp;
  double timestamp;

  double w = 1.0;
  double x = 0.0;
  double y = 0.0;
  double z = 0.0;

  double norm();

  void normalize();

  // friend std::ostream &operator<<(std::ostream &out, const quaternion &data);
};

std::ostream &operator<<(std::ostream &out, const quaternion &data);

quaternion operator+(const quaternion &a, const quaternion &b);

quaternion operator*(const quaternion &a, double b);

quaternion operator*(double b, const quaternion &a);

quaternion quatMult(quaternion a, quaternion b);

quaternion quatInv(quaternion x);

std::vector<quaternion> normlizeQuatSeries(std::vector<quaternion> quats, quaternion normalizer);

std::vector<quaternion> denormlizeQuatSeries(std::vector<quaternion> quats, quaternion normalizer);

void quatToRotVector(quaternion q);

struct position
{
  double timestamp;

  double x = 0.0;
  double y = 0.0;
  double z = 0.0;

  double norm();
};

std::ostream &operator<<(std::ostream &out, const position &data);

position operator+(const position &a, const position &b);

position operator-(const position &a, const position &b);

position operator*(const position &a, double b);

position operator*(double b, const position &a);

std::vector<position> normalizePositionSeries(std::vector<position> positions, position normalizer);

std::vector<position> denormalizePositionSeries(std::vector<position> positions, position normalizer);

struct Pose
{
  double timestamp;
  position position;
  quaternion orientation;
};

#endif /* QUATERNION_HPP */