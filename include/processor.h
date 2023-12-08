#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
 public:
  float Utilization();

 private:
  long prevIdle_ = 0;
  long prevTotal_ = 0;
};

#endif