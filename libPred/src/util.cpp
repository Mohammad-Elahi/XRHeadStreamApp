#include "util.hpp"

template <typename T>
std::vector<T> createUniformTimeseries(std::vector<T> quatsRaw, double samplingInterval, double horizonLength)
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
