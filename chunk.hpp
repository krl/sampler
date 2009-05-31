class Chunk {
 public:
  Chunk();

 private:
  int m_frames;
  jack_default_audio_sample_t *m_data;
  Chunk *m_next;
};
