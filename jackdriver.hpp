#include "def.hpp"
#include <sstream>
#include <iostream>
#include <jack/jack.h>
#include <jack/midiport.h>
#include <oscpack/osc/OscTypes.h>

#include "scheduler.hpp"
#include "voicepool.hpp"
#include "dll.cpp"

using namespace osc;

class JackDriver {

private:
  jack_client_t	 *m_client;
  jack_port_t    *m_output[OUTPUT_PORTS];
  jack_port_t    *m_input[INPUT_PORTS];
  Scheduler      *m_scheduler;
  VoicePool      *m_voicepool;

  // timing related
  SC_JackDLL	 m_DLL;
  uint64         m_OSC_buftime;
  uint64         m_OSC_increment;
  uint64         m_OSC_to_samples;
  double         m_smooth_samplerate;
  double         m_max_output_latency;

public:
  int process             ();
  int reset               (double samplerate, int buffer_size);
  int init                (char *name, Scheduler *scheduler, VoicePool *voicepool);
  int shutdown            ();
  int graph_order_changed ();

  int            m_samplerate;
  jack_nframes_t m_buffer_size;  
};
