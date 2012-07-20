#include "simpleclass.h"

SimpleClass::SimpleClass()
    : paused(true), m_count(0)
{}

double SimpleClass::SmallPause(){
    qDebug() << m_count++ << endl;
    sleep(1);
    return rand();
}

double SimpleClass::PauseFunction(){
    for (int i = 0 ; i < 1000000 ; i++){

    }
    return rand();
}
