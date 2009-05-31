using namespace std;

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <math.h>

#include <jack/jack.h>
#include <jack/midiport.h>

#include "sound.hpp"

jack_port_t *port_control;
jack_port_t *port_play;

jack_port_t *output_l;
jack_port_t *output_r;
jack_client_t *client;

Sound *test_sound;
int wat = 0;

static void signal_handler(int sig){
  jack_client_close(client);
  fprintf(stderr, "signal received, exiting ...\n");
  exit(0);
}

static int process(jack_nframes_t nframes, void *arg) {
  int i;
  /* void* port_buf = jack_port_get_buffer(input_port, nframes); */
  jack_default_audio_sample_t *out_l = (jack_default_audio_sample_t *) jack_port_get_buffer (output_l, nframes);
  jack_default_audio_sample_t *out_r = (jack_default_audio_sample_t *) jack_port_get_buffer (output_r, nframes);

  // for(i=0; i<nframes; i++) {
  //   out_l[i] = test_chunk->data[0][wat];
  //   if (test_chunk->channels > 1)
  //     out_r[i] = test_chunk->data[1][wat];
  //   else
  //     out_r[i] = out_l[i];

  //   wat++;

  //   if (wat > test_chunk->frames)
  //     wat = 0;
  // }
  return 0;
}

static void jack_shutdown(void *arg) {
  exit(1);
}

int main(int narg, char **args)
{
  test_sound = new Sound("/home/krille/projekt/samp/24738_Corsica_S_A02_C1_raw.aif");

  if ((client = jack_client_open("sampler", JackNullOption, NULL)) == 0) {
    fprintf(stderr, "jack server not running?\n");
    return 1;
  }

  jack_set_process_callback (client, process, 0);
  jack_on_shutdown (client, jack_shutdown, 0);

  port_control = jack_port_register (client, "control", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
  port_play    = jack_port_register (client, "play", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);

  output_l = jack_port_register (client, "l", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
  output_r = jack_port_register (client, "r", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

  if (jack_activate (client)) {
    fprintf(stderr, "cannot activate client");
    return 1;
  }

  jack_connect(client, "sampler:l" , "system:playback_1");
  jack_connect(client, "sampler:r" , "system:playback_2");

  /* install a signal handler to properly quits jack client */
  signal(SIGQUIT, signal_handler);
  signal(SIGTERM, signal_handler);
  signal(SIGHUP, signal_handler);
  signal(SIGINT, signal_handler);

  /* run until interrupted */
  while(1) {
    sleep(1);
  }
  jack_client_close(client);
  exit (0);
}
