#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() {
  long idle = LinuxParser::IdleJiffies();
  long nonIdle = LinuxParser::ActiveJiffies();
  long total = idle + nonIdle;
  long totalDelta = total - prevTotal_;
  long idleDelta = idle - prevIdle_;

  prevIdle_ = idle;
  prevTotal_ = total;

  if (totalDelta == 0) {
    return 0.0; // Prevent division by zero.
  }

  return static_cast<float>(totalDelta - idleDelta) / totalDelta;
}
