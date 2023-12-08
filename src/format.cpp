#include <string>
#include <iomanip> // for std::setw and std::setfill

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) {
    // Calculate hours, minutes, and remaining seconds
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int remainingSeconds = seconds % 60;

    // Format the result as HH:MM:SS
    std::ostringstream formattedTime;
    formattedTime << std::setw(2) << std::setfill('0') << hours << ":" 
                 << std::setw(2) << std::setfill('0') << minutes << ":" 
                 << std::setw(2) << std::setfill('0') << remainingSeconds;

    return formattedTime.str();
}
