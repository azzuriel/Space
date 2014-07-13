#ifndef FPSCounter_h__
#define FPSCounter_h__

#include <list>
#include "GameTimer.h"

class FPSCounter
{
private:
    std::list<double> fpsStack;

    double fpsTime;

public:
    FPSCounter(void);
    ~FPSCounter(void);

    void Update(GameTimer gt);

    unsigned int GetCount() const;

};


#endif // FPSCounter_h__