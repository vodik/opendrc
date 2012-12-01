//------------------------------------------------------------------------------
// $Header: D:\\Repository\\D\\Dev\\Psych499\\DRCTest_hdrstop.h,v 1.4 2011-03-14 16:52:37-04 alan Exp alan $
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
// $Log: DRCTest_hdrstop.h,v $
// Revision 1.4  2011-03-14 16:52:37-04  alan
// Modified to allow compilation with GNUC.
// Fixed comment on #endif line
// Changed include files.
// Added define for MAXPATH.
// Added namespace.
//
// Revision 1.3  2011-02-18 15:52:57-05  alan
// Updated files with license information.
//
//------------------------------------------------------------------------------
#ifndef _H_DRCTest_HdrStop
#define _H_DRCTest_HdrStop
/* #pragma message in DRCTest_HdrStop.h */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#ifdef __BORLANDC__
#include <dir.h>		// For findfirst()/findnext()
#elif __GNUC__
#include <dirent.h>		// For scanline()
#endif
#include <math.h>			// For fabs()
#include <vector>
#include <iostream>
#include <algorithm>

#ifdef __GNUC__
#define MAXPATH	260
#endif



using namespace std;

typedef struct {
    int start;
    int r_end;
    int c_end;
    int NumRow;
    int NumCol;
    int* Table;
}t_LCS;

typedef struct {
    unsigned int CRC;  // The CRC is to make string comparisons quicker.
    float        Act;  // The activation value (or 0.0 if none exists on line.
    char*        Str;
}t_CRCStr;

#include "DRCTest.p"

// This "keyword" just flags subroutines so that my prototyping program can
// auto generate the *.p files (ie. proto xxx.cpp --> xxx.p)
#define ProtoType

#pragma hdrstop


/* #pragma message out DRCTest_HdrStop.h */
#endif // _H_DRCTest_HdrStop

//------------------------------------------------------------------------------
// End of $RCSfile: DRCTest_hdrstop.h,v $
//------------------------------------------------------------------------------



