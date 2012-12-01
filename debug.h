// -----------------------------------------------------------------------------
// $Header: D:\\Repository\\C\\Programs\\Cygwin\\home\\Alan\\dev\\psych499\\debug.h,v 1.0 2011-03-13 23:17:57-04 alan Exp alan $
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
// Debug file to allow for debugging projects.
//
// NOTES:
//    WARNING!!!!!!!!!
//      * If you leave out an Exit you will get an unexplainable crash.  Look for
//        ---> and corresponding <--- that don't have line numbers that match.  That
//        is the Exit should be just a few lines after the Enter statement.  The
//        ladder of routine invocations will grow infinitely to the right of the
//        <progname>.dbg file.  Check for this.
//    DEFINES:
//      DBGvars - define this in the module where the debug variables are to be
//                located (use in only one module).
//      NOASSERT - Define if you want to turn off the ShowMessage() call in the
//                 DbgAssert(M) function.
//      DEBUG - Define this for the modules you want to debug.
//
//    OUTPUT:
//      DebugOut(M)
//      DbgAssert(M) - Output will be to the Event Log (View/Debug Windows/Event Log).
//
// $Log: debug.h,v $
// Revision 1.0  2011-03-13 23:17:57-04  alan
// Initial revision
//
// Revision 1.4  2006-10-19 23:22:28-04  alan
// Made array sizes all defined by a meta-constant (so I could make them all bigger by just chaning one number).
// Added a IfDebug define to conditionalize blocks of code.
// Put a warning in to warn when the filename stack overflows.  This can easily happen if we forget to put a corresponding Exit; into a subroutine.
//
// Revision 1.3  2006-02-25 11:58:03-05  alan
// Fixed up a bug with DBGClose() not actually closing the file streams.
// Added a pseudonym debugraw() for debugBare() because this was used in some code.
//
// Revision 1.2  2006-02-24 23:07:36-05  alan
// <>
//
// -----------------------------------------------------------------------------


#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>

/* #pragma message ("in debug.h") */

#define DBGMAX 100

// Debug levels.
#define DBG_EE	0x01
#define DBG_MS	0x02
#define DBG_PF	0x04
#define DBG_VB  0x08
#define DBG_XX  0x00

// Define DBGVars in only one module.
#ifdef DBGvars
	int   DBGind=0;
	FILE *DBGfh=stdout;
	FILE *DBGsh=stdout;
	char *DBGnames[DBGMAX];
    char *DBGrtns[DBGMAX];
    int   DBGlines[DBGMAX];
    int   DBGlvl=DBG_EE | DBG_MS | DBG_PF;
    int   fi;
    int   DBGFlowThrough=0;
#else
    #ifdef __BCPLUSPLUS__
	    extern "C" int DBGind;
	    extern "C" FILE *DBGfh;
	    extern "C" FILE *DBGsh;
	    extern "C" char *DBGnames[DBGMAX];
	    extern "C" char *DBGrtns[DBGMAX];
	    extern "C" int   DBGlines[DBGMAX];
	    extern "C" int   DBGlvl;
		extern "C" int   fi;
        extern "C" int   DBGFlowThrough;
    #else
	    extern int DBGind;
	    extern FILE *DBGfh;
	    extern FILE *DBGsh;
	    extern char *DBGnames[DBGMAX];
	    extern char *DBGrtns[DBGMAX];
	    extern int   DBGlines[DBGMAX];
	    extern int   DBGlvl;
		extern int   fi;
        extern int   DBGFlowThrough;
    #endif
#endif

#ifdef DEBUG
// -----------------------------------------------------------------------------

#define DBGI(n) for(fi=0;fi<(n);fi++) fprintf(DBGfh,"    ");

// DBGNoFiles
#ifndef DBGNoFiles

#define DBGOpen(fname)  \
		if((DBGfh=fopen(fname,"w"))==NULL){ \
            perror("DBG Output File");\
            DBGfh=stdout;            \
        }else{ \
            fprintf(DBGfh,"#### DBGOpen Called %s[%d]\n",__FILE__,__LINE__); \
        } \
        fflush(DBGfh);

#define DBGshOpen(fname,pname)  \
		if((DBGsh=fopen(fname,"w"))==NULL){ \
			perror("DBG Subroutine Hierarchy File");\
			DBGfh=stdout;            \
		}else{ \
			fprintf(DBGfh,"#### DBGshOpen Called %s[%d]\n",__FILE__,__LINE__); \
			fprintf(DBGsh,"Debug SubHeirchy File Program(%s)\n",pname); \
		} \
		fflush(DBGfh);

#define DBGClose() \
		{   DBGI(DBGind);            \
			fprintf(DBGfh,"#### DBGClose Called %s[%d]\n",__FILE__,__LINE__); \
			if(DBGfh!=stdout) {fflush(DBGfh);fclose(DBGfh);}; \
			if(DBGsh!=stdout) {fflush(DBGsh);fclose(DBGsh);}; \
			DBGfh=stdout;  \
		}
#endif // DBGNoFiles


// -----------------------------------------------------------------------------
// DEBUG
// -----------------------------------------------------------------------------
#ifdef OutputDebugString
        #define DebugOut(M) AnsiOutputDebug(M,__FILE__,__LINE__)
        inline void __fastcall AnsiOutputDebug(String Msg,char *File,int Line){
            OutputDebugString((String("### ")+File+"("+Line+") ### "+Msg+" --- ").c_str());
        }
        #define DbgAssert(C) ((C)?(void)0:DebugAssert(#C,__FILE__,__LINE__))
        void DebugAssert(String Cond,char *File,int Line){
            OutputDebugString("### DEBUG ASSERTION FAILURE --- ");
            OutputDebugString((String("###       Assertion (")+Cond+") FAILED --- ").c_str());
            OutputDebugString((String("###       File=")+File+" --- ").c_str());
            OutputDebugString((String("###       Line=")+Line+" --- ").c_str());
            #ifndef NOASSERT
            ShowMessage(("Assertion failure "+Cond+" \n at "+File+"("+Line+")").c_str());
            #endif
        }
#else
    #pragma message ("No OutputDebugString() function")
#endif

// -----------------------------------------------------------------------------

    #define DBGAlloc(str) \
            str

    #define IfDebug \
            if(DBGlvl&DBG_MS)

    #define Milestone(str)  \
            if(DBGlvl&DBG_MS){           \
                DBGI(DBGind);            \
                fprintf(DBGfh,"==== Milestone: %s %s[%d]\n",str,__FILE__,__LINE__); \
                fflush(DBGfh);\
            }

    #define DBGNote(str)   \
            if(DBGlvl&DBG_MS){           \
                DBGI(DBGind);            \
                fprintf(DBGfh,"**** DBG Note: %s %s[%d] ****\n",str,__FILE__,__LINE__); \
                fflush(DBGfh);\
            }

    #define CheckPoint(num)  \
            if(DBGlvl&DBG_MS){           \
                DBGI(DBGind);            \
                fprintf(DBGfh,"==== CheckPoint: %d %s[%d]\n",num,__FILE__,__LINE__); \
                fflush(DBGfh);\
            }

    #define Enter(rtname) \
            if(DBGlvl&DBG_EE){           \
                DBGnames[DBGind]=rtname; \
                DBGrtns[DBGind]=__FILE__; \
                DBGlines[DBGind]=__LINE__; \
                if((DBGind>0)&&(DBGsh!=stdout)) { \
                    if(DBGFlowThrough){           \
                        fprintf(DBGsh,"%s#%s#%d=%s#%s(FlowThrough)#%d\n", \
                            DBGrtns[DBGind-2],DBGnames[DBGind-2],DBGlines[DBGind-2], \
                            __FILE__,rtname,__LINE__); \
                        fflush(DBGsh);\
                        DBGFlowThrough=0;        \
                    }else{                        \
                        fprintf(DBGsh,"%s#%s#%d=%s#%s#%d\n", \
                            DBGrtns[DBGind-1],DBGnames[DBGind-1],DBGlines[DBGind-1], \
                            __FILE__,rtname,__LINE__); \
                        fflush(DBGsh);\
                    } \
                }  \
                DBGI(DBGind);            \
                fprintf(DBGfh,"---> %s %s[%d]\n",rtname,__FILE__,__LINE__); \
                fflush(DBGfh);\
                DBGind++;                \
                if(DBGind+5>DBGMAX) {  \
                    fprintf(DBGfh,"ERROR: (Debug.h) DBGind(=%d) Approaching Maximum!!! Check for missing Exit; statement!\n",DBGind); \
                } \
            }

    #define FlowThrough(rtname)  \
            if(DBGlvl&DBG_EE){           \
                DBGnames[DBGind]=rtname; \
                DBGrtns[DBGind]=__FILE__; \
                DBGlines[DBGind]=__LINE__; \
                DBGI(DBGind);            \
                fprintf(DBGfh,"---> %s %s[%d]\n",rtname,__FILE__,__LINE__); \
                fflush(DBGfh);\
                DBGFlowThrough=1;        \
                DBGind++;                \
            }

    #define Exit   \
            if(DBGlvl&DBG_EE){           \
                DBGind--;                \
                DBGI(DBGind);            \
                fprintf(DBGfh,"<--- %s %s[%d]\n",DBGnames[DBGind],__FILE__,__LINE__); \
                DBGnames[DBGind]=NULL;   \
                fflush(DBGfh);\
            }

    #define VEnter(rtname)  \
            if(DBGlvl&DBG_EE&DBG_VB){    \
                DBGnames[DBGind]=rtname; \
                DBGI(DBGind);            \
                fprintf(DBGfh,"---> %s %s[%d]\n",rtname,__FILE__,__LINE__); \
                DBGind++;                \
            }

    #define VExit  \
            if(DBGlvl&DBG_EE&DBG_VB){    \
                DBGind--;                \
                DBGI(DBGind);            \
                fprintf(DBGfh,"<--- %s %s[%d]\n",DBGnames[DBGind],__FILE__,__LINE__); \
                DBGnames[DBGind]=NULL;   \
                fflush(DBGfh);			 \
            }

    #define debug0(fmt)  \
            if(DBGlvl&DBG_PF){ DBGI(DBGind);fprintf(DBGfh,fmt);fflush(DBGfh); }
    #define debug1(fmt,arg1)  \
            if(DBGlvl&DBG_PF){ DBGI(DBGind);fprintf(DBGfh,fmt,arg1);fflush(DBGfh); }
    #define debug2(fmt,arg1,arg2)  \
            if(DBGlvl&DBG_PF){ DBGI(DBGind);fprintf(DBGfh,fmt,arg1,arg2);fflush(DBGfh); }
    #define debug3(fmt,arg1,arg2,arg3)  \
            if(DBGlvl&DBG_PF){ DBGI(DBGind);fprintf(DBGfh,fmt,arg1,arg2,arg3);fflush(DBGfh); }
    #define debug4(fmt,arg1,arg2,arg3,arg4)  \
            if(DBGlvl&DBG_PF){ DBGI(DBGind);fprintf(DBGfh,fmt,arg1,arg2,arg3,arg4);fflush(DBGfh); };
    #define debug5(fmt,arg1,arg2,arg3,arg4,arg5)  \
            if(DBGlvl&DBG_PF){ DBGI(DBGind);fprintf(DBGfh,fmt,arg1,arg2,arg3,arg4,arg5);fflush(DBGfh); }
    #define debugBare(fmt,arg1)  \
            if(DBGlvl&DBG_PF){ fprintf(DBGfh,fmt,arg1);fflush(DBGfh); }
    #define debugraw(fmt,arg1)  \
            debugBare(fmt,arg1)
            
#else //DEBUG
// -----------------------------------------------------------------------------

    #define DBGOpen(fname)
    #define DBGshOpen(fname,pname)
    #define DBGClose()


#ifdef __cplusplus
    #define DebugOut(M)
    #define AnsiOutputDebug(Msg)
    #define DbgAssert(C)
    #define DebugAssert(C,F,L)
#endif // __cplusplus
    #define DBGAlloc(str)

    #define IfDebug                         if(DBGlvl&DBG_XX)

    #define Milestone(str)
    #define DBGNote(str)
    #define CheckPoint(num)
    #define Enter(rtname)
    #define FlowThrough(rtname)
    #define Exit
    #define VEnter(rtname)
    #define VExit
    #define debug0(fmt)
    #define debug1(fmt,arg1)
    #define debug2(fmt,arg1,art2)
    #define debug3(fmt,arg1,art2,arg3)
    #define debug4(fmt,arg1,art2,arg3,arg4)
    #define debug5(fmt,arg1,art2,arg3,arg4,arg5)
    #define debugBare(fmt,arg1)
    #define debugraw(fmt,arg1)
#endif  //DEBUG

/* #pragma message ("out debug.h") */

#endif  //_DEBUG_H

// -----------------------------------------------------------------------------
// End of $RCSfile: debug.h,v $
// -----------------------------------------------------------------------------




