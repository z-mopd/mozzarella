#ifndef INIT_HPP
#define INIT_HPP

namespace mzr {

bool IsInitialized();
bool IsClosed();

bool Init();
void Close();

double GetElapsedTime();
double GetCurrentTime();

void SleepTime(double seconds);

}

#endif
