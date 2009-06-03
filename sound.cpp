using namespace std;

#include <string>
#include <iostream>

#include <sndfile.h>
#include <jack/jack.h>
#include "sound.hpp"

Sound::Sound(char *path) {
  cout << "opening file " << path << endl;

  SNDFILE *file;
  SF_INFO sfinfo;
  jack_default_audio_sample_t *interlace_buffer;
  jack_default_audio_sample_t *channel_buffer;
  int i, f;

  if (!(file = sf_open (path, SFM_READ, &sfinfo))) {
    printf ("Not able to open input file %s.\n", path) ;
    sf_perror (NULL);
  };

  interlace_buffer = new jack_default_audio_sample_t[sfinfo.frames * sfinfo.channels];
  sf_read_float(file, interlace_buffer, sfinfo.frames);

  for (i = 0; i < sfinfo.channels ; i++) {
    // deinterlace sound into channels
    channel_buffer = new jack_default_audio_sample_t[sfinfo.frames];
    for (f = 0; f < sfinfo.frames ; f++) {
      channel_buffer[f] = interlace_buffer[sfinfo.channels * f + i];
    }
    chunk[i] = new Chunk(sfinfo.frames, channel_buffer, NULL);
  }
 
  delete interlace_buffer;
  printf("file loaded: %s\nframes: %i\nchannels: %i\n", path, sfinfo.frames, sfinfo.channels);
}
