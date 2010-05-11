/***************************************************************************
 *
 *   Copyright (C) 2010 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "CommandLine.h"
#include "RealTimer.h"
#include "malloc16.h"

#include "dsp/Filterbank.h"
#include "dsp/Memory.h"

#if HAVE_CUFFT
#include "dsp/FilterbankCUDA.h"
#include "dsp/MemoryCUDA.h"
#endif

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>

using namespace std;

class Speed : public Reference::Able
{
public:

  Speed ();

  // parse command line options
  void parseOptions (int argc, char** argv);

  // run the test
  void runTest ();

protected:

  unsigned nloop;
  unsigned nfft;
  unsigned nchan;
  unsigned niter;
  bool real_to_complex;
};


Speed::Speed ()
{
  niter = 10;
  nloop = 0;
  nfft = 1024;
  nchan = 1;
  real_to_complex = false;
}

int main(int argc, char** argv) try
{
  Speed speed;
  speed.parseOptions (argc, argv);
  speed.runTest ();
  return 0;
}
 catch (Error& error)
   {
     cerr << error << endl;
     return -1;
   }

void Speed::parseOptions (int argc, char** argv)
{
  CommandLine::Menu menu;
  CommandLine::Argument* arg;

  menu.set_help_header ("filterbank_speed - measure Filterbank speed");
  menu.set_version ("filterbank_speed version 1.0");

  arg = menu.add (real_to_complex, 'r');
  arg->set_help ("real-to-complex FFT");

  arg = menu.add (nfft, 'n', "nfft");
  arg->set_help ("FFT length");

  arg = menu.add (nchan, 'c', "nchan");
  arg->set_help ("number of channels");

  arg = menu.add (niter, 'N', "niter");
  arg->set_help ("number of iterations");

  menu.parse (argc, argv);
}

double order (unsigned nfft)
{
  return nfft * log2 (nfft);
}

void Speed::runTest ()
{
  // pretend like 256 MB of memory will be used
  uint64_t data_length = 256*1024*1024;

  unsigned nfloat = nchan * nfft;
  if (!real_to_complex)
    nfloat *= 2;

  unsigned size = sizeof(float) * nfloat;

  if (!nloop)
  {
    nloop = data_length / size;
    if (nloop > 2000)
      nloop = 2000;
    cerr << "Speed::runTest nloop=" << nloop << endl;
  }

  dsp::Filterbank::Engine* engine = 0;
  dsp::Memory* memory = 0;

#if HAVE_CUFFT
  engine = new CUDA::Engine;
  memory = new CUDA::DeviceMemory;
#endif

  if (!memory)
    memory = new dsp::Memory;

  if (!engine)
    throw Error (InvalidState, "Speed::runTest",
		 "engine not set");

  float* in = (float*) memory->do_allocate (data_length);
  memory->do_zero (in, data_length);

  engine->scratch = (float*) memory->do_allocate (size + 4*sizeof(float));
  engine->setup (nchan, nfft);

  cerr << "entering loop" << endl;

  double total_time = 0;

  for (unsigned j=0; j<niter; j++)
  {
    RealTimer timer;
    timer.start ();

    for (unsigned i=0; i<nloop; i++)
      engine->perform (in);

    engine->finish ();

    timer.stop ();

    total_time += timer.get_elapsed();
  }

  double time_us = total_time * 1e6 / (nloop*niter);

  // cerr << "time=" << time << endl;

  memory->do_free (in);
  memory->do_free (engine->scratch);

  double log2_nfft = log2(nfft);

  double mflops = 5.0 * nfft * log2_nfft / time_us;

  cerr << "nchan=" << nchan << " nfft=" << nfft << " time=" << time_us << "us"
    " log2(nfft)=" << log2_nfft << " mflops=" << mflops << endl;

  cout << nchan << " " << nfft << " " << time_us << " "
       << log2_nfft << " " << mflops << endl;
}

