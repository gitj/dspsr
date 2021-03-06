//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2001 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/dspsr/dspsr/Kernel/Formats/cpsr/dsp/xfer_tape.h,v $
   $Revision: 1.2 $
   $Date: 2006/07/09 13:27:06 $
   $Author: wvanstra $ */

#ifndef __XFER_TAPE_H
#define __XFER_TAPE_H

#include <vector>

class SystemTime;
class rdisk;

int xfer_tape (const char* tapedev, vector<int>* filenos,
	       const vector<rdisk>& disks, char* ext = NULL, 
	       char* check_fptm = NULL, double leave_alone = 600e6, // 600MB
	       SystemTime* clock=NULL, int obstype=-1, bool keep=true);


#endif
