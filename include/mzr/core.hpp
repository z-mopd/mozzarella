#ifndef INIT_HPP
#define INIT_HPP

namespace mzr {

bool Init();
void Close();

bool IsInitialized();
bool IsClosed();

double GetElapsedTime();
double GetCurrentTime();

void SleepTime(double seconds);

}

#endif
