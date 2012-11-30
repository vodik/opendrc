//---------------------------------------------------------------------------
// $Header: D:\\Repository\\D\\Dev\\Psych499\\DRC_HdrStop.h,v 1.4 2011-03-14 16:54:05-04 alan Exp alan $
//
//    OpenDRC is an open-source implementation of the DRC Dual Route Cascaded Model 
//        of Visual Word Recognition and Reading Aloud.
//    Copyright (C) 2011  Alan Angold
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//	The author can be contacted at:
//		Alan.Angold@gmail.com
//	or
//		Alan Angold
//		549 Sandbrooke Court
//		Waterloo, Ontario 
//		Canada N2T2H4
//
//
//
//
// $Log: DRC_HdrStop.h,v $
// Revision 1.4  2011-03-14 16:54:05-04  alan
// Modified to allow compilation with GNUC.
// Fixed comment on #endif line
// Changed include files to <xxx> from <xxx.h> and converted standard include files from
// <xxx> to <cxxx>..
//
// Revision 1.0  2011-03-13 23:17:59-04  alan
// Initial revision
//
// Revision 1.3  2011-02-18 15:52:56-05  alan
// Updated files with license information.
//
//---------------------------------------------------------------------------


#ifndef _H_DRC_HdrStop
#define _H_DRC_HdrStop
/* #pragma message in HdrStop.h */


#ifdef __BORLANDC__
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <values.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <dir.h>
#include <io.h>
#include <exception>
#include <vector.h>
#elif __GNUC__
#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
/* #include <io.h> */
#include <exception>
#include <vector>
#endif

#include "DRC.h"
#include "DRC_CmdLine.h"
#include "DRC_Core.h"
#include "DRC_FileIO.h"
#include "Batch_y.h"


#include "DRC_CmdLine.p"
#include "DRC_FileIO.p"
#include "DRC_Core.p"
#include "DRC_Main.p"

// This "keyword" just flags subroutines so that my prototyping program can
// auto generate the *.p files (ie. proto xxx.cpp --> xxx.p)
#define ProtoType

/* #pragma hdrstop */

using namespace std;

//---------------------------------------------------------------------------
/* #pragma message out HdrStop.h */
#endif // _H_DRC_HdrStop


//---------------------------------------------------------------------------
// End of $RCSfile: DRC_HdrStop.h,v $
//---------------------------------------------------------------------------


