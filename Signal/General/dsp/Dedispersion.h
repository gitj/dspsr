//-*-C++-*-

/* $Source: /cvsroot/dspsr/dspsr/Signal/General/dsp/Dedispersion.h,v $
   $Revision: 1.22 $
   $Date: 2005/10/03 02:57:27 $
   $Author: wvanstra $ */

#ifndef __Dedispersion_h
#define __Dedispersion_h

#include "dsp/Response.h"
#include "dsp/SampleDelayFunction.h"

namespace dsp {
  
  //! Phase-coherent dispersion removal frequency response function
  /* This class implements the phase-coherent dedispersion kernel, as
     determined by the frequency response of the interstellar
     medium. */

  class Dedispersion: public Response {

  public:

    //! Conversion factor between dispersion measure, DM, and dispersion, D
    static const double dm_dispersion;

    //! Fractional smearing added to reduce cyclical convolution effects
    static float smearing_buffer;

    //! Null constructor
    Dedispersion ();

    //! Set up and calculate the impulse_pos and impulse_neg attributes
    void prepare (const Observation* input, unsigned channels);

    //! Calculate the impulse_pos and impulse_neg attributes
    void prepare ();

    //! Match the dedispersion kernel to the input Observation
    virtual void match (const Observation* input, unsigned channels=0);

    //! Set the dispersion measure attribute in the output Observation
    virtual void mark (Observation* output);

    //! Set the dimensions of the data
    virtual void resize (unsigned npol, unsigned nchan,
			 unsigned ndat, unsigned ndim);

    //! Set the flag for a bin-centred spectrum
    virtual void set_dc_centred (bool dc_centred);

    //! Set the number of channels into which the band will be divided
    virtual void set_nchan (unsigned nchan);

    //! Set the centre frequency of the band-limited signal in MHz
    void set_centre_frequency (double centre_frequency);

    //! Return the centre frequency of the band-limited signal in MHz
    double get_centre_frequency () const { return centre_frequency; }

    //! Returns the centre frequency of the specified channel in MHz
    double get_centre_frequency (int ichan) const;

    //! Set the bandwidth of signal in MHz
    void set_bandwidth (double bandwidth);

    //! Return the bandwidth of signal in MHz
    double get_bandwidth () const { return bandwidth; }

    //! Set the dispersion measure in \f${\rm pc\,cm}^{-3}\f$
    void set_dispersion_measure (double dm);

    //! Get the dispersion measure in \f${\rm pc\,cm}^{-3}\f$
    double get_dispersion_measure () const { return dispersion_measure; }

    //! Set the Doppler shift due to the Earth's motion
    void set_Doppler_shift (double Doppler_shift);

    //! Return the doppler shift due to the Earth's motion
    double get_Doppler_shift () const { return Doppler_shift; }

    //! Set the flag to add fractional inter-channel delay
    void set_fractional_delay (bool fractional_delay);

    //! Get the flag to add fractional inter-channel delay
    bool get_fractional_delay () const { return fractional_delay; }

    //! Set the frequency resolution in each channel of the kernel
    void set_frequency_resolution (unsigned nfft);

    //! Get the frequency resolution in each channel of the kernel
    unsigned get_frequency_resolution () const { return ndat; }

    //! Return the smearing across the entire band time in seconds
    double get_smearing_time () const {
      return smearing_time (centre_frequency, bandwidth);
    }

    //! Return the smearing time across the worst sub-band in seconds
    double get_effective_smearing_time () const;

    //! Return the effective number of smearing samples
    unsigned get_effective_smearing_samples () const;

    //! Return the dispersion delay between freq1 and freq2
    /*! If freq2 is higher than freq1, delay_time is positive */
    double delay_time (double freq1, double freq2) const;

    //! Return the dispersion delay for the given frequency
    double delay_time (double freq) const;

    //! Return the dispersion delay for the given frequency
    double delay_time () const { return delay_time (centre_frequency); }

    //! Return the smearing time, given the centre frequency and bandwidth
    double smearing_time (double centre_frequency, double bandwidth) const;

    //! Compute the phases for a dedispersion kernel
    void build (std::vector<float>& phases, unsigned npts, unsigned nchan);

    //! Build the dedispersion frequency response kernel
    virtual void build ();

    class SampleDelay : public SampleDelayFunction {

    public:

      //! Default constructor
      SampleDelay ();

      //! Set up the dispersion delays
      bool match (const Observation* obs);

      //! Return the dispersion delay for the given frequency channel
      int64 get_delay (unsigned ichan, unsigned ipol);

    protected:

      //! Centre frequency of the band-limited signal in MHz
      double centre_frequency;

      //! Bandwidth of signal in MHz
      double bandwidth;

      //! Dispersion measure (in \f${\rm pc cm}^{-3}\f$)
      double dispersion_measure;

      //! The sampling rate (in Hz)
      double sampling_rate;

      //! The dispersive delays
      std::vector<int64> delays;

    };

  protected:

    //! Centre frequency of the band-limited signal in MHz
    double centre_frequency;

    //! Bandwidth of signal in MHz
    double bandwidth;

    //! Dispersion measure (in \f${\rm pc cm}^{-3}\f$)
    double dispersion_measure;

    //! Doppler shift due to the Earths' motion
    double Doppler_shift;

    //! Flag to add fractional inter-channel delay
    bool fractional_delay;

    //! Flag set when set_frequency_resolution() method is called
    bool frequency_resolution_set;

    //! Flag that the response and bandpass attributes reflect the state
    bool built;

    //! Return the effective smearing time in seconds (worker function)
    double smearing_time (int half) const;

    //! Return the number of complex samples of smearing (worker function)
    unsigned smearing_samples (int half) const;

  };
  
}

#endif
