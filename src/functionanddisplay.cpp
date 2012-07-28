#include "functionanddisplay.h"

FunctionAndDisplay::FunctionAndDisplay(QWidget* parent)
    : QGraphicsView(parent)
{
    s = SimpleClass();
    ispaused = 0;
}
void FunctionAndDisplay::system_pause_toggle(bool pause){
    ispaused = pause;
}

void FunctionAndDisplay::function(){
    if (!ispaused){
        double retn = s.SmallPause();
        emit returned_value(retn);
    }
}
