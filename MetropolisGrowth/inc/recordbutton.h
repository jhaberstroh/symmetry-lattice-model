#ifndef __RECORDBUTTON_H_INCLUDED__
#define __RECORDBUTTON_H_INCLUDED__

#include "orderparamfile.h"
#include "latticefile.h"

class Interaction;

class RecordButton{
    private:
        unsigned int m_recording_interval_size;
        unsigned int m_recording_counter;
        unsigned int m_total_counter;
        bool m_paused;

    public:

        RecordButton(int recording_interval_size)
        : m_recording_interval_size(recording_interval_size),
        m_total_counter(0), m_recording_counter(0), m_paused(true){}

    //These functions should be appropriately extended in subclasses, but this is the
    // basic functionality.
        inline void Reset()     { m_total_counter = 0; m_recording_counter = 0; m_paused = true;}
        inline void StepFrame() { if(!m_paused){
                                    m_total_counter++; m_recording_counter++;
                                    if (m_recording_counter >= m_recording_interval_size){
                                        Record();
                                        m_recording_counter = 0; }}}
        inline void Pause()     { m_paused = true; }
        inline void Unpause()   { m_paused = false;}
        inline void Restart()   { Reset(); Unpause();}

        virtual void Record();
}

class MetropolisRecordButton : class RecordButton{
    private:
        LatticeFile& m_metropolis_latfile;
        OrderParamFile& m_metropolis_opfile;
        Interaction& m_metropolis_interaciton;
        string lat_image_base_filename

    public:
        MetropolisRecordButton(LatticeFile& lf, OrderParamFile& opf, Interaction& itract):
        RecordButton(), m_metropolis_latfile(lf), m_metropolis_opfile(opf), m_metropolis_interaction(itract) {}

        inline void Reset(){
            RecordButton::Reset();}
        inline void Record(){
            //TODO:Will have the op file's destination set in reset
            m_metropolis_opfile.Track()
            //TODO: Will use an indexed base filename
            m_metropolis_latfile.MakeSquareLatticeColorImage(m_metropolis_interaction)}

}


#endif // __RECORDBUTTON_H_INCLUDED__
