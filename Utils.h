//
// Created by rahul.kaplesh on 12/2/2019.
//

#ifndef POLYGONDIVIDE_BD_UTILS_H
#define POLYGONDIVIDE_BD_UTILS_H

#include <utility>
#include <map>

template<typename keyType, typename valueType>
std::pair<keyType, valueType> get_max(const std::multimap<keyType, valueType>& x) {
   using pairtype=std::pair<keyType, valueType>;
   return *std::max_element(x.begin(), x.end(), [](const pairtype& p1, const pairtype& p2) {
      return p1.first < p2.first;
   });
}

float roundDecimalTo(float var, int decimalPoints)
{
    int factor = std::pow(10, decimalPoints);
    float val = (int)(var * factor + 0.5);
    return (float)val / factor;
}

#endif //POLYGONDIVIDE_BD_UTILS_H
