#include "jackdriver.hpp"

// =====================================================================
// JACK callbacks

int sc_jack_process_cb(jack_nframes_t nframes, void *arg)
{
  ((JackDriver*)arg)->process();
  return 0;
}

int sc_jack_graph_order_cb(void *arg)
{
  ((JackDriver*)arg)->graph_order_changed();
  return 0;
}

// =====================================================================
// Class methods

int JackDriver::process() {
  sample *out[2];
  sample *in[2];

  int numbufs = 2; // TODO: figure this out...

  m_DLL.Update();
  // debug
  // static int tick = 0;
  // if (++tick >= 1000) {
  //   tick = 0;
  //   printf("DLL: t %.6f p %.9f sr %.6f e %.9f avg(e) %.9f inc %.9f\n",
  // 	   m_DLL.PeriodTime(), m_DLL.Period(), m_DLL.SampleRate(),
  // 	   m_DLL.Error(), m_DLL.AvgError(), m_OSC_increment * kOSCtoSecs);
  //   cout << "osctime:  " << m_OSC_buftime << endl;
  //   cout << "nexttime: " << m_scheduler->next_time() << endl;
  // }
  // end debug

  m_OSC_buftime   = (uint64)((m_DLL.PeriodTime() - m_max_output_latency) * kSecondsToOSCunits + .5);
  m_OSC_increment = (uint64)((m_DLL.Period() / numbufs) * kSecondsToOSCunits + .5);
  m_smooth_samplerate = m_DLL.SampleRate();

  m_OSC_to_samples = m_smooth_samplerate * kOSCtoSecs;

  for (int i = 0 ; i < OUTPUT_PORTS ; i++ )
    out[i] = (jack_default_audio_sample_t *) jack_port_get_buffer (m_output[i], m_buffer_size);
  for (int i = 0 ; i < INPUT_PORTS ; i++ )
    in[i] = (jack_default_audio_sample_t *) jack_port_get_buffer (m_input[i], m_buffer_size);

  for (int c = 0 ; c < OUTPUT_PORTS ; c++ )
    for (jack_nframes_t f = 0 ; f < m_buffer_size ; f++) {
      out[c][f] = 0.0;
  }

  m_voicepool->read(in, m_buffer_size);

  uint64 sched_time;
  uint64 next_time = m_OSC_buftime + m_OSC_increment;
  Event *event;

  while ((sched_time = m_scheduler->next_time()) <= next_time) {
    event = m_scheduler->pop_event();
    uint64 diff;

    if (sched_time > m_OSC_buftime)
      diff = sched_time - m_OSC_buftime;
    else
      diff = m_OSC_buftime - sched_time;

    int sample_offset = (int)(((float)diff / (float)m_OSC_increment) * m_buffer_size);

    // cout << "next: " << next_time - m_OSC_buftime << endl;
    // cout << "diff: " << diff << endl;
    // cout << "part: " << (float)diff / (float)m_OSC_increment << endl;
    // cout << "ofst: " << sample_offset << endl;

    m_voicepool->play(event->m_sound, sample_offset);

    delete event;
  }

  m_voicepool->write(out, m_buffer_size);

  // advance OSC time
  m_OSC_buftime = next_time;

  return 0;
}

int JackDriver::init(char *name, Scheduler *scheduler, VoicePool *voicepool) {
  if ((m_client = jack_client_open("sampler", JackNullOption, NULL)) == 0) {
    fprintf(stderr, "jack server not running?\n");
    return 1;
  }

  m_samplerate  = jack_get_sample_rate(m_client);
  m_buffer_size = jack_get_buffer_size(m_client);
  m_scheduler   = scheduler;
  m_voicepool   = voicepool;

  // setup jack connections
  jack_set_process_callback     (m_client, sc_jack_process_cb,     this);
  jack_set_graph_order_callback (m_client, sc_jack_graph_order_cb, this);

  // register ports

  for (int i = 0; i < INPUT_PORTS; i++) {    
    ostringstream ss;
    ss << "in_" << i;
    m_input[i] = jack_port_register (m_client, ss.str().c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
  }

  for (int i = 0; i < OUTPUT_PORTS; i++) {    
    ostringstream ss;
    ss << "out_" << i;
    m_output[i] = jack_port_register (m_client, ss.str().c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
  }

  // activate
  if (jack_activate (m_client)) {
    fprintf(stderr, "cannot activate client");
    return 1;
  }

  for (int i = 0; i < OUTPUT_PORTS; i++) {
    ostringstream a,b;
    a << "sampler:out_" << i;
    b << "system:playback_" << (i+1);
    jack_connect(m_client, a.str().c_str(), b.str().c_str());
  }

  reset(jack_get_sample_rate(m_client), jack_get_buffer_size(m_client));
}


int JackDriver::reset(double samplerate, int buffer_size) {

  m_samplerate  = m_smooth_samplerate = samplerate;
  m_buffer_size = buffer_size;

  // TODO set samplerate of children
  
  m_DLL.Reset(m_samplerate,
	      m_buffer_size,
	      SC_JACK_DLL_BW);
}

int JackDriver::shutdown () {
  return jack_client_close(m_client);
}

int JackDriver::graph_order_changed()
{
  jack_nframes_t lat = 0;

  for (int i=0; i < OUTPUT_PORTS; ++i) {
    jack_nframes_t portLat = jack_port_get_total_latency(m_client, m_output[i]);
    if (portLat > lat) lat = portLat;
  }

  double maxLat = (double)lat / m_samplerate;

  if (maxLat != m_max_output_latency) {
    m_max_output_latency = maxLat;
    printf("max output latency %.1f ms\n", maxLat * 1e3);
  }

  return 1;
}
