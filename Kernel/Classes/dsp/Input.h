//-*-C++-*-

/* $Source: /cvsroot/dspsr/dspsr/Kernel/Classes/dsp/Input.h,v $
   $Revision: 1.26 $
   $Date: 2003/09/11 08:45:07 $
   $Author: hknight $ */

#ifndef __Input_h
#define __Input_h

#include "MJD.h"

#include "dsp/Operation.h"
#include "dsp/Observation.h"
#include "environ.h"

namespace dsp {

  class BitSeries;

  //! Pure virtual base class of all objects that can load BitSeries data
  /*! 
    This class defines the common interface as well as some basic
    functionality relating to sources of BitSeries data.
  */

  class Input : public Operation {

  public:
    
    //! Constructor
    Input (const char* name);
    
    //! Destructor
    virtual ~Input ();
    
    //! Copies the behaviour and information attributes (not data)
    virtual void copy (const Input* input);

    //! End of data
    virtual bool eod() = 0;
    
    //! Load BitSeries data
    virtual void load (BitSeries* data);

    //! Set the BitSeries to which data will be loaded
    virtual void set_output (BitSeries* data);

    //! Retrieve a pointer to the output.
    virtual BitSeries* get_output();

    //! Return true if output is set
    virtual bool has_output () const;

    //! Seek to the specified time sample
    virtual void seek (int64 offset, int whence = 0);

    //! Seek to a sample close to the specified MJD
    virtual void seek (MJD mjd);
    
    //! Return the number of time samples to load on each load_block
    virtual uint64 get_block_size () const { return block_size; }
    //! Set the number of time samples to load on each load_block
    virtual void set_block_size (uint64 _size);
    
    //! Return the number of time samples by which consecutive blocks overlap
    virtual uint64 get_overlap () const { return overlap; }
    //! Set the number of time samples by which consecutive blocks overlap
    virtual void set_overlap (uint64 _overlap) { overlap = _overlap; }

    //! Convenience function for returning block_size-overlap
    virtual uint64 get_stride () const 
    { return get_block_size()-get_overlap(); }

    //! Return the total number of time samples available
    virtual uint64 get_total_samples () const { return info.get_ndat(); }

    //! Set the total number of time samples available
    /*! Generally useful for debugging */
    void set_total_samples (uint64 s) { info.set_ndat(s); }

    //! Get the information about the data source
    virtual operator const Observation* () const { return &info; }

    //! Get the information about the data source
    virtual Observation* get_info () { return &info; }

    //! Get the information about the data source
    virtual const Observation* get_info () const { return &info; }

    //! Get the next time sample to be loaded
    uint64 get_load_sample () const { return load_sample; }

    //! Get the number of samples to be loaded
    uint64 get_load_size () const { return load_size; }

    //! Get the time sample resolution of the data source
    unsigned get_resolution () const { return resolution; }

    //! Convenience method used to seek in units of seconds
    void seek_seconds (double seconds, int whence = 0);

    //! Convenience method used to set the number of seconds
    void set_total_seconds (double seconds);

  protected:

    //! The BitSeries to which data will be loaded on next call to operate
    Reference::To<BitSeries> output;

    //! Load the next block of time samples into BitSeries
    /*! Implementations of this method must read get_load_size time
      samples, beginning with get_load_sample */
    virtual void load_data (BitSeries* data) = 0;

    //! Load data into the BitSeries specified with set_output
    virtual void operation ();

    //! Information about the data source (passed on to BitSeries in load)
    Observation info;

    //! Time sample resolution of the data source
    unsigned resolution;

    virtual void set_eod(bool _eod)
    { throw Error(FailedCall,"dsp::Input::set_eod()",
		  "This method is overloaded in Seekable, but not anything else, and you've called it from something else"); }

  private:

    //! Requested number of time samples to be read during load
    /*! The number of times samples actually read during the call to
      load_data is given by get_load_size.  This may be greater than
      block_size, owing to the resolution of the data source.
      Attributes of both the BitSeries and TimeSeries classes keep
      these details transparent at the top level. */
    uint64 block_size;
        
    //! Requested number of time samples by which data blocks overlap
    /*! The number of times samples by which data blocks overlap
      actually depends on the resolution of the data source.
      Attributes of both the BitSeries and TimeSeries classes keep
      these details transparent at the top level. */
    uint64 overlap;
    
    //! Offset from load_sample to the time sample requested by the user
    /*! Owing to the resolution of the data source, the first sample
      read during load_data may precede the time sample requested by
      the user.  Attributes of both the BitSeries and TimeSeries
      classes keep these details transparent at the top level. */
    unsigned resolution_offset;

    //! First time sample to be read on the next call to load_data
    uint64 load_sample;

    //! Number of time samples to be read on the next call to load_data
    uint64 load_size;

    //! Ensures that load size is properly set
    void set_load_size ();

  };

}

#endif // !defined(__Input_h)
