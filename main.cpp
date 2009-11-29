using namespace std;

#include <string.h>
#include <sstream>
#include <iostream>
#include <memory>
#include <signal.h>

#include <jack/jack.h>
#include <jack/midiport.h>

#include "sound.hpp"
#include "voicepool.hpp"
#include "chunkpool.hpp"
#include "scheduler.hpp"

int test = 0;

bool once = true;

jack_port_t *port_control;
jack_port_t *port_play;

jack_port_t *port_output[OUTPUT_PORTS];
jack_port_t *port_input[INPUT_PORTS];
jack_client_t *client;

//Voice *test_voice_mono;

VoicePool *voicepool;
Scheduler *scheduler;
SoundBank *soundbank;
ChunkPool *chunkpool;

int wat = 0;

static int process(jack_nframes_t nframes, void *arg) {
  int i;
  sample *out[2];

  jack_nframes_t frame_start;

  frame_start = jack_last_frame_time(client);

  if (once) {
    scheduler->set_time_offset(frame_start);
    once = false;
  }

  for (int i = 0 ; i < OUTPUT_PORTS ; i++ )
    out[i] = (jack_default_audio_sample_t *) jack_port_get_buffer (port_output[i], nframes);

  for (int c = 0 ; c < OUTPUT_PORTS ; c++ )
    for (jack_nframes_t f = 0 ; f < nframes ; f++) {
      out[c][f] = 0.0;
  }

  voicepool->write(out, frame_start);

  return 0;
}

static void jack_shutdown(void *arg) {
  exit(1);
}

int main(int narg, char **args) {
  if ((client = jack_client_open("sampler", JackNullOption, NULL)) == 0) {
    fprintf(stderr, "jack server not running?\n");
    return 1;
  }

  // setup test sound and voice
  // test_sound = new Sound(args[1]);
  voicepool = new VoicePool(100,(int)jack_get_buffer_size(client),(int)jack_get_sample_rate(client));

  chunkpool = new ChunkPool();
  soundbank = new SoundBank(chunkpool);
  scheduler = new Scheduler(soundbank, voicepool, jack_get_sample_rate(client));

  // setup jack connections
  jack_set_process_callback (client, process, 0);
  jack_on_shutdown (client, jack_shutdown, 0);

  for (int i = 0; i < INPUT_PORTS; i++) {    
    ostringstream ss;
    ss << "in_" << i;
    port_input[i] = jack_port_register (client, ss.str().c_str(), JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
  }

  for (int i = 0; i < OUTPUT_PORTS; i++) {    
    ostringstream ss;
    ss << "out_" << i;
    port_output[i] = jack_port_register (client, ss.str().c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
  }

  // activate
  if (jack_activate (client)) {
    fprintf(stderr, "cannot activate client");
    return 1;
  }

  for (int i = 0; i < OUTPUT_PORTS; i++) {
    ostringstream a,b;
    a << "sampler:out_" << i;
    b << "system:playback_" << (i+1);
    jack_connect(client, a.str().c_str(), b.str().c_str());
  }

  // osc listener
  scheduler->run();

  delete voicepool;
  delete scheduler;
  delete soundbank;

  jack_client_close(client);
  exit (0);
}
