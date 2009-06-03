using namespace std;

#include "sound.hpp"

#define MIDI_MAPPINGS = 100

class Midimapping {
public:
  Midimapping();

private:
  Sound *m_mappings[MIDI_MAPPINGS];
};
