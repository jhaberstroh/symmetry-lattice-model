#include <stdlib.h>
#include <QDebug>

#ifndef SIMPLECLASS_H
#define SIMPLECLASS_H

class SimpleClass
{

private:
    bool paused;
    int m_count;

public:
    SimpleClass();
    double SmallPause();
    double PauseFunction();
};

#endif // SIMPLECLASS_H
