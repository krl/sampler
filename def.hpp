#define GETTIMEOFDAY_TO_NTP_OFFSET 2208988800UL

#define OUTPUT_PORTS 2
#define INPUT_PORTS 2

#define MAX_VOICES 1024

#define OSC_PORT 7000

#define CHUNK_SIZE 11025     // quarter second in 44.5
#define MAX_CHUNKS 1024 * 64 // 4.5 channel-hours total

#define CHUNK_POOL_SIZE 128

#define sample jack_default_audio_sample_t
