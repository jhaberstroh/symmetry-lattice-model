#ifndef FUNCTIONANDDISPLAY_H
#define FUNCTIONANDDISPLAY_H

#include <QGraphicsView>
#include "simpleclass.h"

class FunctionAndDisplay : public QGraphicsView
{
    Q_OBJECT

public:
    explicit FunctionAndDisplay(QWidget* parent = 0);
    void function();
    
public slots:
     void system_pause_toggle(bool pause);

signals:
     double returned_value(double val);

private:
    bool ispaused;

};

#endif // FUNCTIONANDDISPLAY_H
