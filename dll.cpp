// snatched from supercollider (GPL v.2 or later)
// Copyright (c) 2003-2006 James McCartney, stefan kersten
// modified by ksamp author.

#include "def.hpp"
#include <sys/time.h>
#include <oscpack/osc/OscTypes.h>
#include <jack/jack.h>
#include <math.h>

using namespace osc;

#define SC_JACK_DLL_BW 0.012

const uint32 kSECONDS_FROM_1900_to_1970 = (uint32)2208988800UL; /* 17 leap years */
const double kOSCtoSecs = 2.328306436538696e-10;
const double kMicrosToOSCunits = 4294.967296; // pow(2,32)/1e6
const double kSecondsToOSCunits = 4294967296.; // pow(2,32)

static inline int64 sc_JackOSCTime(const struct timeval& tv)
{
  return ((int64)(tv.tv_sec + kSECONDS_FROM_1900_to_1970) << 32)
    + (int64)(tv.tv_usec * kMicrosToOSCunits);
}

static inline int64 sc_JackOSCTime()
{
  struct timeval tv;
  gettimeofday(&tv, 0);
  return sc_JackOSCTime(tv);
}

static inline double sc_JackOSCTimeSeconds()
{
  return (uint64)sc_JackOSCTime() * kOSCtoSecs;
}

class SC_JackDLL
{
public:
  SC_JackDLL()
    : m_b(0.), m_c(0.),
      m_t0(0.), m_t1(0.), m_e2(0.),
      m_np(0), m_ei(0.), m_ec(0)
  { }

  void Reset(double sampleRate, jack_nframes_t periodFrames, double bandWidth)
  {
    double t = sc_JackOSCTimeSeconds();
    // compute coefficients
    m_np = periodFrames;
    m_b = 2*M_PI*bandWidth*m_np/sampleRate;
    m_c = m_b*m_b/2.;
    // initialize filter
    double tp = m_np/sampleRate;
    m_e2 = tp;
    m_t0 = t;
    m_t1 = t + tp;
    // initialize statistics
    m_ei = 0.;
    m_ec = 0;
  }

  void Update()
  {
    double t = sc_JackOSCTimeSeconds();
    // compute error
    double e = m_e = t - m_t1;
    // update filter
    m_t0 = m_t1;
    m_t1 += m_b * e + m_e2;
    m_e2 += m_c * e;
    // collect statistics
    m_ei += e;
    m_ec++;
  }

  double PeriodTime() const { return m_t0; }
  double NextPeriodTime() const { return m_t1; }
  double Period() const { return m_t1 - m_t0; }
  double SampleRate() const { return m_np/Period(); }
  double Error() const { return m_e; }
  double AvgError() const { return m_ec > 0 ? m_ei/m_ec : 0; }

private:
  double m_b, m_c;
  double m_t0, m_t1, m_e, m_e2;
  jack_nframes_t m_np;
  double m_ei;
  int m_ec;
};
