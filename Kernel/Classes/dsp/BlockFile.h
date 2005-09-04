//-*-C++-*-

/* $Source: /cvsroot/dspsr/dspsr/Kernel/Classes/dsp/BlockFile.h,v $
   $Revision: 1.1 $
   $Date: 2005/09/04 19:27:22 $
   $Author: wvanstra $ */


#ifndef __dsp_BlockFile_h
#define __dsp_BlockFile_h

#include "dsp/File.h"

namespace dsp {

  //! Loads BitSeries data from a file organized in blocks
  class BlockFile : public File
  {
    
  public:
    
    //! Constructor
    BlockFile (const char* name);
    
    //! Destructor
    ~BlockFile ();

  protected:

    //! Number of bytes in each block
    uint64 block_bytes;

    //! Number of bytes in header of each block
    uint64 block_header_bytes;

    //! Number of bytes in tailer of each block
    uint64 block_tailer_bytes;

    //! Return ndat given the file and header sizes, nchan, npol, and ndim
    /*! Called by open_file for some file types, to determine that the
    header ndat matches the file size.  Requires 'info' parameters
    nchan, npol, and ndim as well as header_bytes to be correctly set */
    virtual int64 fstat_file_ndat(uint64 tailer_bytes=0);

    //! Load nbyte bytes of sampled data from the device into buffer
    /*! If the data stored on the device contains information other
      than the sampled data, this method should be overloaded and the
      additional information should be filtered out. */
    virtual int64 load_bytes (unsigned char* buffer, uint64 nbytes);
    
    //! Set the file pointer to the absolute number of sampled data bytes
    /*! If the header_bytes attribute is set, this number of bytes
      will be subtracted by File::seek_bytes before seeking.  If the
      data stored on the device after the header contains information
      other than the sampled data, this method should be overloaded
      and the additional information should be skipped. */
    virtual int64 seek_bytes (uint64 bytes);
    
  private:

    //! Initialize variables to sensible null values
    void init();

    //! The current byte within a block
    uint64 current_block_byte;

  };

}

#endif // !defined(__File_h)
  
