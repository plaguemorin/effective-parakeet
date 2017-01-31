

#ifndef RTPRTCP_BUFFERNETWORKHELPER_H
#define RTPRTCP_BUFFERNETWORKHELPER_H

#include <cstdint>

template<typename T, unsigned int SIZE = sizeof(T)>
T FromNetworkOrderRead(const uint8_t *p) {
  T retval(0);
  for (unsigned int i = 0; i < SIZE; ++i)
    retval |= static_cast<T>(p[i]) << ((SIZE - 1 - i) * 8);
  return retval;
};


#endif
