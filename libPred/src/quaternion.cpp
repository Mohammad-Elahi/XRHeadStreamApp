#include "quaternion.hpp"

double quaternion::norm()
{
  double norm = sqrt(this->w * this->w + this->x * this->x + this->y * this->y + this->z * this->z);
  return norm;
}

void quaternion::normalize()
{
  double norm = this->norm();
  this->w = this->w / norm;
  this->x = this->x / norm;
  this->y = this->y / norm;
  this->z = this->z / norm;
}

std::ostream &operator<<(std::ostream &out, const quaternion &data)
{
  out << "(x: " << data.x << ", y: " << data.y << ", z: " << data.z << ", w: " << data.w << ")";
  return out;
}

quaternion operator+(const quaternion &a, const quaternion &b)
{
  quaternion ret;
  ret.timestamp = a.timestamp + b.timestamp;
  ret.w = a.w + b.w;
  ret.x = a.x + b.x;
  ret.y = a.y + b.y;
  ret.z = a.z + b.z;
  return ret;
}

quaternion operator*(const quaternion &a, double b)
{
  quaternion ret;
  // ret.timestamp = (time_t)(((double)a.timestamp) * b);
  ret.timestamp = a.timestamp * b;
  ret.w = a.w * b;
  ret.x = a.x * b;
  ret.y = a.y * b;
  ret.z = a.z * b;
  return ret;
}

quaternion operator*(double b, const quaternion &a)
{
  return a * b;
}

quaternion quatMult(quaternion a, quaternion b)
{
  quaternion ret;
  ret.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
  ret.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
  ret.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
  ret.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
  ret.normalize();
  return ret;
}

quaternion quatInv(quaternion x)
{
  quaternion ret = x;
  ret.w = x.w;
  ret.x = -x.x;
  ret.y = -x.y;
  ret.z = -x.z;
  ret.normalize();
  return ret;
}

std::vector<quaternion> normlizeQuatSeries(std::vector<quaternion> quats, quaternion normalizer)
{
  std::vector<quaternion> out;
  for (auto &q : quats)
  {
    q.normalize();
    out.push_back(quatMult(q, quatInv(normalizer)));
    out.back().normalize();
  }
  return out;
}

std::vector<quaternion> denormlizeQuatSeries(std::vector<quaternion> quats, quaternion normalizer)
{
  std::vector<quaternion> out;
  for (auto &q : quats)
  {
    out.push_back(quatMult(q, normalizer));
  }
  return out;
}

void quatToRotVector(quaternion q)
{

  double v = Eigen::Vector3d(q.x, q.y, q.z).norm();
  double alpha = 2 * std::atan2(v, q.w);
  double sa = std::sin(alpha / 2);
  double eps = 1e-6;
  Eigen::Vector3d ew(eps, 0.0, 0.0);
  if (sa != 0)
  {
    ew = Eigen::Vector3d(alpha * q.x / sa, alpha * q.y / sa, alpha * q.z / sa);
  }
  std::cout << ew.norm() * 180.0 / 3.141592 << std::endl;
}

double position::norm()
{
  double norm = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
  return norm;
}

std::ostream &operator<<(std::ostream &out, const position &data)
{
  out << "(x: " << data.x << ", y: " << data.y << ", z: " << data.z << ")";
  return out;
}

position operator+(const position &a, const position &b)
{
  position ret;
  ret.timestamp = a.timestamp + b.timestamp;
  ret.x = a.x + b.x;
  ret.y = a.y + b.y;
  ret.z = a.z + b.z;
  return ret;
}

position operator-(const position &a, const position &b)
{
  position ret;
  ret.timestamp = a.timestamp - b.timestamp;
  ret.x = a.x - b.x;
  ret.y = a.y - b.y;
  ret.z = a.z - b.z;
  return ret;
}

position operator*(const position &a, double b)
{
  position ret;
  ret.timestamp = a.timestamp * b;
  ret.x = a.x * b;
  ret.y = a.y * b;
  ret.z = a.z * b;
  return ret;
}

position operator*(double b, const position &a)
{
  return a * b;
}

std::vector<position> normalizePositionSeries(std::vector<position> positions, position normalizer)
{
  std::vector<position> out;
  for (auto &p : positions)
  {
    out.push_back(p + (normalizer * -1));
  }
  return out;
}

std::vector<position> denormalizePositionSeries(std::vector<position> positions, position normalizer)
{
  std::vector<position> out;
  for (auto &p : positions)
  {
    out.push_back(p + normalizer);
  }
  return out;
}
