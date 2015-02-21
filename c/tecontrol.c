/* .\tecontrol.f -- translated by f2c (version 20100827).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"

/* Common Block Declarations */

struct {
    doublereal xmeas[41], xmv[12];
} pv_;

#define pv_1 pv_

struct {
    integer idv[20];
} dvec_;

#define dvec_1 dvec_

struct {
    doublereal setpt[20], deltat;
} ctrlall_;

#define ctrlall_1 ctrlall_

struct {
    integer flag__;
} flag6_;

#define flag6_1 flag6_

struct {
    doublereal gain1, errold1;
} ctrl1_;

#define ctrl1_1 ctrl1_

struct {
    doublereal gain2, errold2;
} ctrl2_;

#define ctrl2_1 ctrl2_

struct {
    doublereal gain3, errold3;
} ctrl3_;

#define ctrl3_1 ctrl3_

struct {
    doublereal gain4, errold4;
} ctrl4_;

#define ctrl4_1 ctrl4_

struct {
    doublereal gain5, taui5, errold5;
} ctrl5_;

#define ctrl5_1 ctrl5_

struct {
    doublereal gain6, errold6;
} ctrl6_;

#define ctrl6_1 ctrl6_

struct {
    doublereal gain7, errold7;
} ctrl7_;

#define ctrl7_1 ctrl7_

struct {
    doublereal gain8, errold8;
} ctrl8_;

#define ctrl8_1 ctrl8_

struct {
    doublereal gain9, errold9;
} ctrl9_;

#define ctrl9_1 ctrl9_

struct {
    doublereal gain10, taui10, errold10;
} ctrl10_;

#define ctrl10_1 ctrl10_

struct {
    doublereal gain11, taui11, errold11;
} ctrl11_;

#define ctrl11_1 ctrl11_

struct {
    doublereal gain13, taui13, errold13;
} ctrl13_;

#define ctrl13_1 ctrl13_

struct {
    doublereal gain14, taui14, errold14;
} ctrl14_;

#define ctrl14_1 ctrl14_

struct {
    doublereal gain15, taui15, errold15;
} ctrl15_;

#define ctrl15_1 ctrl15_

struct {
    doublereal gain16, taui16, errold16;
} ctrl16_;

#define ctrl16_1 ctrl16_

struct {
    doublereal gain17, taui17, errold17;
} ctrl17_;

#define ctrl17_1 ctrl17_

struct {
    doublereal gain18, taui18, errold18;
} ctrl18_;

#define ctrl18_1 ctrl18_

struct {
    doublereal gain19, taui19, errold19;
} ctrl19_;

#define ctrl19_1 ctrl19_

struct {
    doublereal gain20, taui20, errold20;
} ctrl20_;

#define ctrl20_1 ctrl20_

struct {
    doublereal gain22, taui22, errold22;
} ctrl22_;

#define ctrl22_1 ctrl22_

/* Table of constant values */

static integer c__1 = 1;
static integer c__5 = 5;


/*               Tennessee Eastman Process Control Test Problem */

/* 			Original codes written by */

/*                    James J. Downs and Ernest F. Vogel */

/*                  Process and Control Systems Engineering */
/*                        Tennessee Eastman Company */
/*                              P.O. Box 511 */
/*                          Kingsport, Tennessee 37662 */

/* -------------------------------------------------------------------- */

/*  New version is a closed-loop plant-wide control scheme for */
/*  the Tennessee Eastman Process Control Test Problem */

/*  The modifications are by: */

/* 	      Evan L. Russell, Leo H. Chiang and Richard D. Braatz */

/* 		     Large Scale Systems Research Laboratory */
/* 	                Department of Chemical Engineering */
/* 		    University of Illinois at Urbana-Champaign */
/* 	                 600 South Mathews Avenue, Box C-3 */
/* 			      Urbana, Illinois 61801 */
/* 		 	    http://brahms.scs.uiuc.edu */

/* The modified text is Copyright 1998-2002 by The Board of Trustees */
/* of the University of Illinois.  All rights reserved. */

/* Permission hereby granted, free of charge, to any person obtaining a copy */
/* of this software and associated documentation files (the "Software"), to */
/* deal with the Software without restriction, including without limitation */
/* the rights to use, copy, modify, merge, publish, distribute, sublicense, */
/* and/or sell copies of the Software, and to permit persons to whom the */
/* Software is furnished to do so, subject to the following conditions: */
/* 		1. Redistributions of source code must retain the above copyright */
/* 		   notice, this list of conditions and the following disclaimers. */
/* 		2. Redistributions in binary form must reproduce the above */
/* 		   copyright notice, this list of conditions and the following */
/* 		   disclaimers in the documentation and/or other materials */
/* 		   provided with the distribution. */
/* 		3. Neither the names of Large Scale Research Systems Laboratory, */
/* 		   University of Illinois, nor the names of its contributors may */
/* 		   be used to endorse or promote products derived from this */
/* 		   Software without specific prior written permission. */

/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS */
/* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL */
/* THE CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR */
/* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, */
/* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER */
/* DEALINGS IN THE SOFTWARE. */
/* ---------------------------------------------------------------------- */

/*  Users should cite the original code using the following references: */

/*    J.J. Downs and E.F. Vogel, "A plant-wide industrial process control */
/*    problem." Presented at the AIChE 1990 Annual Meeting, Session on */
/*    Industrial Challenge Problems in Process Control, Paper #24a */
/*    Chicago, Illinois, November 14, 1990. */

/*    J.J. Downs and E.F. Vogel, "A plant-wide industrial process control */
/*    problem," Computers and Chemical Engineering, 17:245-255 (1993). */

/*  Users should cite the modified code using the following references: */

/*    E.L. Russell, L.H. Chiang, and R.D. Braatz. Data-driven Techniques */
/*    for Fault Detection and Diagnosis in Chemical Processes, Springer-Verlag, */
/*    London, 2000. */

/*    L.H. Chiang, E.L. Russell, and R.D. Braatz. Fault Detection and */
/*    Diagnosis in Industrial Systems, Springer-Verlag, London, 2001. */

/*    L.H. Chiang, E.L. Russell, and R.D. Braatz. "Fault diagnosis in */
/*    chemical processes using Fisher discriminant analysis, discriminant */
/*    partial least squares, and principal component analysis," Chemometrics */
/*    and Intelligent Laboratory Systems, 50:243-252, 2000. */

/*    E.L. Russell, L.H. Chiang, and R.D. Braatz. "Fault detection in */
/*    industrial processes using canonical variate analysis and dynamic */
/*    principal component analysis," Chemometrics and Intelligent Laboratory */
/*    Systems, 51:81-93, 2000. */


/*  Main program for demonstrating application of the modified Tennessee Eastman */
/*  Process Control Test Problem */


/*  Instructions for running the program */
/*  ==================================== */

/*  1) Go to line 220, change NPTS to the number of data points to simulate. For each */
/*     minute of operation, 60 points are generated. */

/*  2) Go to line 226, change SSPTS to the number of data points to simulate in steady */
/*     state operation before implementing the disturbance. */

/*  3) Go to line 367, implement any of the 21 programmed disturbances. For example, to */
/*     implement disturbance 2, type IDV(2)=1 . */

/*  4) The program will generate 15 output files and all data are recorded every */
/*     180 seconds, see Table 1 for details.  The default path is the home directory. */
/*     To change the file name and path, modify lines 346-360 accordingly. */
/*     To overwrite the files that already existed, change STATUS='new' to */
/*     STATUS='old' from lines 346-360. */


/*    		 Table 1: Content of the output files */

/*      File Name		        	Content */
/*      ---------			        ------- */
/*    TE_data_inc.dat	   		  Time (in seconds) */
/*    TE_data_mv1.dat	   Measurements for manipulated variables 1 to 4 */
/*    TE_data_mv2.dat	   Measurements for manipulated variables 5 to 8 */
/*    TE_data_mv3.dat	   Measurements for manipulated variables 9 to 12 */
/*    TE_data_me01.dat	   Measurements for measurement variables 1 to 4 */
/*    TE_data_me02.dat	   Measurements for measurement variables 5 to 8 */
/*    TE_data_me03.dat	   Measurements for measurement variables 9 to 12 */
/*    TE_data_me04.dat	   Measurements for measurement variables 13 to 16 */
/*    TE_data_me05.dat	   Measurements for measurement variables 17 to 20 */
/*    TE_data_me06.dat	   Measurements for measurement variables 21 to 24 */
/*    TE_data_me07.dat	   Measurements for measurement variables 25 to 28 */
/*    TE_data_me08.dat	   Measurements for measurement variables 29 to 32 */
/*    TE_data_me09.dat	   Measurements for measurement variables 33 to 36 */
/*    TE_data_me10.dat	   Measurements for measurement variables 37 to 40 */
/*    TE_data_me11.dat	   Measurements for measurement variable 41 */

/*  5) To ensure the randomness of the measurement noises, the random number */
/*     G in the sub program (teprob.f, line 1187) has to be changed each time before */
/*     running 'temain_mod.f'. */

/*  6) Save the changes in 'temain_mod.f' and 'teprob.f' and compile the program in */
/*      unix by typing */
/*      f77 temain_mod.f teprob.f */

/*  7) Run the program by typing */
/*      a.out */


/* ============================================================================= */


/*  MEASUREMENT AND VALVE COMMON BLOCK */

/* Main program */ int MAIN__(void)
{
    /* Format strings */
    static char fmt_111[] = "(1x,i6)";

    /* System generated locals */
    integer i__1;
    olist o__1;
    cllist cl__1;

    /* Builtin functions */
    integer f_open(olist *), s_wsfe(cilist *), do_fio(integer *, char *, 
	    ftnlen), e_wsfe(void), f_clos(cllist *);
    /* Subroutine */ int s_stop(char *, ftnlen);

    /* Local variables */
    extern /* Subroutine */ int conshand_(void);
    static integer i__, nn;
    static doublereal yp[50], yy[50], time;
    static integer test, npts, test1, test4;
    static real sspts;
    extern /* Subroutine */ int teinit_(integer *, doublereal *, doublereal *,
	     doublereal *), intgtr_(integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *), output_(void), contrl1_(void), 
	    contrl2_(void), contrl3_(void), contrl4_(void), contrl5_(void), 
	    contrl6_(void), contrl7_(void), contrl8_(void), contrl9_(void), 
	    contrl10_(void), contrl11_(void), contrl13_(void), contrl14_(void)
	    , contrl15_(void), contrl16_(void), contrl17_(void), contrl18_(
	    void), contrl19_(void), contrl20_(void);

    /* Fortran I/O blocks */
    static cilist io___11 = { 0, 111, 0, fmt_111, 0 };



/*   DISTURBANCE VECTOR COMMON BLOCK */


/*   CONTROLLER COMMON BLOCK */

/*      DOUBLE PRECISION SETPT, GAIN, TAUI, ERROLD, DELTAT */
/*      COMMON/CTRL/ SETPT, GAIN, TAUI, ERROLD, DELTAT */


/*  Local Variables */



/*  Set the number of differential equations (states).  The process has 50 */
/*  states.  If the user wishes to integrate additional states, NN must be */
/*  increased by the number of additional differential equations. */

    nn = 50;

/*  Set the number of points to simulate */

    npts = 172800;

/*  Set the number of pints to simulate in steady state operation */

    sspts = 28800.f;

/*  Integrator Step Size:  1 Second Converted to Hours */

    ctrlall_1.deltat = 2.7777777777777778e-4f;

/*  Initialize Process */
/*  (Sets TIME to zero) */

    teinit_(&nn, &time, yy, yp);

/*  Set Controller Parameters */
/*  Make a Stripper Level Set Point Change of +15% */

/* C      SETPT = XMEAS(15) + 15.0 */
/* C      GAIN = 2.0 */
/* C      TAUI = 5.0 */
/* C      ERROLD = 0.0 */
    ctrlall_1.setpt[0] = 3664.f;
    ctrl1_1.gain1 = 1.f;
    ctrl1_1.errold1 = 0.f;
    ctrlall_1.setpt[1] = 4509.3f;
    ctrl2_1.gain2 = 1.f;
    ctrl2_1.errold2 = 0.f;
    ctrlall_1.setpt[2] = .25052f;
    ctrl3_1.gain3 = 1.f;
    ctrl3_1.errold3 = 0.f;
    ctrlall_1.setpt[3] = 9.3477f;
    ctrl4_1.gain4 = 1.f;
    ctrl4_1.errold4 = 0.f;
    ctrlall_1.setpt[4] = 26.902f;
    ctrl5_1.gain5 = -.083f;
    ctrl5_1.taui5 = 2.7777777777777778e-4f;
    ctrl5_1.errold5 = 0.f;
    ctrlall_1.setpt[5] = .33712f;
    ctrl6_1.gain6 = 1.22f;
    ctrl6_1.errold6 = 0.f;
    ctrlall_1.setpt[6] = 50.f;
    ctrl7_1.gain7 = -2.2660000000000005f;
/* CS: Was -2.06, added 20% */
    ctrl7_1.errold7 = 0.f;
    ctrlall_1.setpt[7] = 50.f;
    ctrl8_1.gain8 = -1.944f;
/* CS: Was -1.62, added 20% */
    ctrl8_1.errold8 = 0.f;
    ctrlall_1.setpt[8] = 230.31f;
    ctrl9_1.gain9 = .41f;
    ctrl9_1.errold9 = 0.f;
    ctrlall_1.setpt[9] = 94.599f;
    ctrl10_1.gain10 = -1.5600000000000001f;
    ctrl10_1.taui10 = .40333333333333332f;
    ctrl10_1.errold10 = 0.f;
    ctrlall_1.setpt[10] = 22.949f;
    ctrl11_1.gain11 = 1.09f;
    ctrl11_1.taui11 = .72222222222222221f;
    ctrl11_1.errold11 = 0.f;
    ctrlall_1.setpt[12] = 32.188f;
    ctrl13_1.gain13 = 18.f;
    ctrl13_1.taui13 = .88f;
    ctrl13_1.errold13 = 0.f;
    ctrlall_1.setpt[13] = 6.882f;
    ctrl14_1.gain14 = 8.3f;
    ctrl14_1.taui14 = .88f;
    ctrl14_1.errold14 = 0.f;
    ctrlall_1.setpt[14] = 18.776f;
    ctrl15_1.gain15 = 2.37f;
    ctrl15_1.taui15 = 1.4080555555555556f;
    ctrl15_1.errold15 = 0.f;
    ctrlall_1.setpt[15] = 65.731f;
    ctrl16_1.gain16 = .16899999999999998f;
    ctrl16_1.taui16 = .065555555555555561f;
    ctrl16_1.errold16 = 0.f;
    ctrlall_1.setpt[16] = 75.f;
    ctrl17_1.gain17 = 1.1099999999999999f;
    ctrl17_1.taui17 = .88f;
    ctrl17_1.errold17 = 0.f;
    ctrlall_1.setpt[17] = 120.4f;
    ctrl18_1.gain18 = 14.15f;
/* CS: Was *10 */
    ctrl18_1.taui18 = .27277777777777779f;
    ctrl18_1.errold18 = 0.f;
    ctrlall_1.setpt[18] = 13.823f;
    ctrl19_1.gain19 = -5.5466666666666669f;
    ctrl19_1.taui19 = 1.76f;
    ctrl19_1.errold19 = 0.f;
    ctrlall_1.setpt[19] = .8357f;
    ctrl20_1.gain20 = -3.2600000000000002f;
    ctrl20_1.taui20 = 3.4466666666666668f;
    ctrl20_1.errold20 = 0.f;
    ctrlall_1.setpt[11] = 2633.7f;
    ctrl22_1.gain22 = -5.f;
    ctrl22_1.taui22 = .27777777777777779f;
    ctrl22_1.errold22 = 0.f;

/*    Example Disturbance: */
/*    Change Reactor Cooling */

    pv_1.xmv[0] = 63.052999999999997f;
    pv_1.xmv[1] = 53.979999999999997f;
    pv_1.xmv[2] = 24.643999999999998f;
    pv_1.xmv[3] = 61.302f;
    pv_1.xmv[4] = 22.210000000000001f;
    pv_1.xmv[5] = 40.064f;
    pv_1.xmv[6] = 38.100000000000001f;
    pv_1.xmv[7] = 46.533999999999999f;
    pv_1.xmv[8] = 47.445999999999998f;
    pv_1.xmv[9] = 41.106000000000002f;
    pv_1.xmv[10] = 18.114000000000001f;

/* 	SETPT(6)=SETPT(6) + 0.2 */

/*  Set all Disturbance Flags to OFF */

    for (i__ = 1; i__ <= 20; ++i__) {
	dvec_1.idv[i__ - 1] = 0;
/* L100: */
    }
/*      IDV(20)=1 */




    o__1.oerr = 0;
    o__1.ounit = 111;
    o__1.ofnmlen = 22;
    o__1.ofnm = "output/TE_data_inc.dat";
    o__1.orl = 0;
    o__1.osta = "new";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    o__1.oerr = 0;
    o__1.ounit = 1111;
    o__1.ofnmlen = 22;
    o__1.ofnm = "output/TE_data_mv1.dat";
    o__1.orl = 0;
    o__1.osta = "new";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    o__1.oerr = 0;
    o__1.ounit = 1112;
    o__1.ofnmlen = 22;
    o__1.ofnm = "output/TE_data_mv2.dat";
    o__1.orl = 0;
    o__1.osta = "new";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    o__1.oerr = 0;
    o__1.ounit = 1113;
    o__1.ofnmlen = 22;
    o__1.ofnm = "output/TE_data_mv3.dat";
    o__1.orl = 0;
    o__1.osta = "new";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    o__1.oerr = 0;
    o__1.ounit = 2111;
    o__1.ofnmlen = 23;
    o__1.ofnm = "output/TE_data_me01.dat";
    o__1.orl = 0;
    o__1.osta = "new";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    o__1.oerr = 0;
    o__1.ounit = 2112;
    o__1.ofnmlen = 23;
    o__1.ofnm = "output/TE_data_me02.dat";
    o__1.orl = 0;
    o__1.osta = "new";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    o__1.oerr = 0;
    o__1.ounit = 2113;
    o__1.ofnmlen = 23;
    o__1.ofnm = "output/TE_data_me03.dat";
    o__1.orl = 0;
    o__1.osta = "new";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    o__1.oerr = 0;
    o__1.ounit = 2114;
    o__1.ofnmlen = 23;
    o__1.ofnm = "output/TE_data_me04.dat";
    o__1.orl = 0;
    o__1.osta = "new";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    o__1.oerr = 0;
    o__1.ounit = 2115;
    o__1.ofnmlen = 23;
    o__1.ofnm = "output/TE_data_me05.dat";
    o__1.orl = 0;
    o__1.osta = "new";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    o__1.oerr = 0;
    o__1.ounit = 2116;
    o__1.ofnmlen = 23;
    o__1.ofnm = "output/TE_data_me06.dat";
    o__1.orl = 0;
    o__1.osta = "new";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    o__1.oerr = 0;
    o__1.ounit = 2117;
    o__1.ofnmlen = 23;
    o__1.ofnm = "output/TE_data_me07.dat";
    o__1.orl = 0;
    o__1.osta = "new";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    o__1.oerr = 0;
    o__1.ounit = 2118;
    o__1.ofnmlen = 23;
    o__1.ofnm = "output/TE_data_me08.dat";
    o__1.orl = 0;
    o__1.osta = "new";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    o__1.oerr = 0;
    o__1.ounit = 2119;
    o__1.ofnmlen = 23;
    o__1.ofnm = "output/TE_data_me09.dat";
    o__1.orl = 0;
    o__1.osta = "new";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    o__1.oerr = 0;
    o__1.ounit = 2120;
    o__1.ofnmlen = 23;
    o__1.ofnm = "output/TE_data_me10.dat";
    o__1.orl = 0;
    o__1.osta = "new";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    o__1.oerr = 0;
    o__1.ounit = 2121;
    o__1.ofnmlen = 23;
    o__1.ofnm = "output/TE_data_me11.dat";
    o__1.orl = 0;
    o__1.osta = "new";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);


/*  Simulation Loop */

    i__1 = npts;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if ((real) i__ >= sspts) {
	    dvec_1.idv[11] = 1;
	}
	test = i__ % 3;
	if (test == 0) {
	    contrl1_();
	    contrl2_();
	    contrl3_();
	    contrl4_();
	    contrl5_();
	    contrl6_();
	    contrl7_();
	    contrl8_();
	    contrl9_();
	    contrl10_();
	    contrl11_();
	    contrl16_();
	    contrl17_();
	    contrl18_();
	}
	test1 = i__ % 360;
	if (test1 == 0) {
	    contrl13_();
	    contrl14_();
	    contrl15_();
	    contrl19_();
	}
	test1 = i__ % 900;
	if (test1 == 0) {
	    contrl20_();
	}
/* 	  TEST3=MOD(I,5000) */
/* 	  IF (TEST3.EQ.0) THEN */
/* 		PRINT *, 'Simulation time (in seconds) = ', I */
/* 	ENDIF */

	test4 = i__ % 180;
	if (test4 == 0) {
	    output_();
	    s_wsfe(&io___11);
	    do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
	    e_wsfe();
	}

	intgtr_(&nn, &time, &ctrlall_1.deltat, yy, yp);

	conshand_();

/* L1000: */
    }
/* 		PRINT *, 'Simulation is done. ' */

    cl__1.cerr = 0;
    cl__1.cunit = 111;
    cl__1.csta = 0;
    f_clos(&cl__1);
    cl__1.cerr = 0;
    cl__1.cunit = 1111;
    cl__1.csta = 0;
    f_clos(&cl__1);
    cl__1.cerr = 0;
    cl__1.cunit = 1112;
    cl__1.csta = 0;
    f_clos(&cl__1);
    cl__1.cerr = 0;
    cl__1.cunit = 1113;
    cl__1.csta = 0;
    f_clos(&cl__1);
    cl__1.cerr = 0;
    cl__1.cunit = 2111;
    cl__1.csta = 0;
    f_clos(&cl__1);
    cl__1.cerr = 0;
    cl__1.cunit = 2112;
    cl__1.csta = 0;
    f_clos(&cl__1);
    cl__1.cerr = 0;
    cl__1.cunit = 2113;
    cl__1.csta = 0;
    f_clos(&cl__1);
    cl__1.cerr = 0;
    cl__1.cunit = 2114;
    cl__1.csta = 0;
    f_clos(&cl__1);
    cl__1.cerr = 0;
    cl__1.cunit = 2115;
    cl__1.csta = 0;
    f_clos(&cl__1);
    cl__1.cerr = 0;
    cl__1.cunit = 2116;
    cl__1.csta = 0;
    f_clos(&cl__1);
    cl__1.cerr = 0;
    cl__1.cunit = 2117;
    cl__1.csta = 0;
    f_clos(&cl__1);
    cl__1.cerr = 0;
    cl__1.cunit = 2118;
    cl__1.csta = 0;
    f_clos(&cl__1);
    cl__1.cerr = 0;
    cl__1.cunit = 2119;
    cl__1.csta = 0;
    f_clos(&cl__1);
    cl__1.cerr = 0;
    cl__1.cunit = 2120;
    cl__1.csta = 0;
    f_clos(&cl__1);
    cl__1.cerr = 0;
    cl__1.cunit = 2121;
    cl__1.csta = 0;
    f_clos(&cl__1);
    s_stop("", (ftnlen)0);
    return 0;
} /* MAIN__ */


/* ============================================================================= */

/* C      SUBROUTINE CONTRL */

/*  Discrete control algorithms */


/*   MEASUREMENT AND VALVE COMMON BLOCK */

/* C      DOUBLE PRECISION XMEAS, XMV */
/* C      COMMON/PV/ XMEAS(41), XMV(12) */

/*   CONTROLLER COMMON BLOCK */

/* C      DOUBLE PRECISION SETPT, GAIN, TAUI, ERROLD, DELTAT */
/* C      COMMON/CTRL/ SETPT, GAIN, TAUI, ERROLD, DELTAT */

/* C      DOUBLE PRECISION ERR, DXMV */

/*  Example PI Controller: */
/*    Stripper Level Controller */

/*    Calculate Error */

/* C      ERR = SETPT - XMEAS(15) */

/*    Proportional-Integral Controller (Velocity Form) */
/*         GAIN = Controller Gain */
/*         TAUI = Reset Time (min) */

/* C      DXMV = GAIN * ( ( ERR - ERROLD ) + ERR * DELTAT * 60. / TAUI ) */

/* C      XMV(8) = XMV(8) - DXMV */

/* C      ERROLD = ERR */

/* C      RETURN */
/* C      END */

/* ============================================================================= */

/* Subroutine */ int contrl1_(void)
{
    static doublereal err1, dxmv;


/*  Discrete control algorithms */


/*   MEASUREMENT AND VALVE COMMON BLOCK */


/*   CONTROLLER COMMON BLOCK */



/*  Example PI Controller: */
/*    Stripper Level Controller */

/*    Calculate Error */

    err1 = (ctrlall_1.setpt[0] - pv_1.xmeas[1]) * 100.f / 5811.f;

/*    Proportional-Integral Controller (Velocity Form) */
/*         GAIN = Controller Gain */
/*         TAUI = Reset Time (min) */

    dxmv = ctrl1_1.gain1 * (err1 - ctrl1_1.errold1);

    pv_1.xmv[0] += dxmv;

    ctrl1_1.errold1 = err1;

    return 0;
} /* contrl1_ */


/* ============================================================================= */

/* Subroutine */ int contrl2_(void)
{
    static doublereal err2, dxmv;


/*  Discrete control algorithms */


/*   MEASUREMENT AND VALVE COMMON BLOCK */


/*   CONTROLLER COMMON BLOCK */



/*  Example PI Controller: */
/*    Stripper Level Controller */

/*    Calculate Error */

    err2 = (ctrlall_1.setpt[1] - pv_1.xmeas[2]) * 100.f / 8354.f;

/*    Proportional-Integral Controller (Velocity Form) */
/*         GAIN = Controller Gain */
/*         TAUI = Reset Time (min) */

    dxmv = ctrl2_1.gain2 * (err2 - ctrl2_1.errold2);

    pv_1.xmv[1] += dxmv;

    ctrl2_1.errold2 = err2;

    return 0;
} /* contrl2_ */


/* ============================================================================= */

/* Subroutine */ int contrl3_(void)
{
    static doublereal err3, dxmv;


/*  Discrete control algorithms */


/*   MEASUREMENT AND VALVE COMMON BLOCK */


/*   CONTROLLER COMMON BLOCK */



/*  Example PI Controller: */
/*    Stripper Level Controller */

/*    Calculate Error */

    err3 = (ctrlall_1.setpt[2] - pv_1.xmeas[0]) * 100.f / 1.017f;

/*    Proportional-Integral Controller (Velocity Form) */
/*         GAIN = Controller Gain */
/*         TAUI = Reset Time (min) */

    dxmv = ctrl3_1.gain3 * (err3 - ctrl3_1.errold3);

    pv_1.xmv[2] += dxmv;

    ctrl3_1.errold3 = err3;

    return 0;
} /* contrl3_ */


/* ============================================================================= */

/* Subroutine */ int contrl4_(void)
{
    static doublereal err4, dxmv;


/*  Discrete control algorithms */


/*   MEASUREMENT AND VALVE COMMON BLOCK */


/*   CONTROLLER COMMON BLOCK */



/*  Example PI Controller: */
/*    Stripper Level Controller */

/*    Calculate Error */

    err4 = (ctrlall_1.setpt[3] - pv_1.xmeas[3]) * 100.f / 15.25f;

/*    Proportional-Integral Controller (Velocity Form) */
/*         GAIN = Controller Gain */
/*         TAUI = Reset Time (min) */

    dxmv = ctrl4_1.gain4 * (err4 - ctrl4_1.errold4);

    pv_1.xmv[3] += dxmv;

    ctrl4_1.errold4 = err4;

    return 0;
} /* contrl4_ */


/* ============================================================================= */

/* Subroutine */ int contrl5_(void)
{
    static doublereal err5, dxmv;


/*  Discrete control algorithms */


/*   MEASUREMENT AND VALVE COMMON BLOCK */


/*   CONTROLLER COMMON BLOCK */



/*  Example PI Controller: */
/*    Stripper Level Controller */

/*    Calculate Error */

    err5 = (ctrlall_1.setpt[4] - pv_1.xmeas[4]) * 100.f / 53.f;

/*    Proportional-Integral Controller (Velocity Form) */
/*         GAIN = Controller Gain */
/*         TAUI = Reset Time (min) */

/* 	PRINT *, 'GAIN5= ', GAIN5 */
/* 	PRINT *, 'TAUI5= ', TAUI5 */
/* 	PRINT *, 'ERR5= ', ERR5 */
/* 	PRINT *, 'ERROLD5= ', ERROLD5 */

    dxmv = ctrl5_1.gain5 * (err5 - ctrl5_1.errold5 + err5 * ctrlall_1.deltat *
	     3.f / ctrl5_1.taui5);

    pv_1.xmv[4] += dxmv;

    ctrl5_1.errold5 = err5;

    return 0;
} /* contrl5_ */


/* ============================================================================= */

/* Subroutine */ int contrl6_(void)
{
    static doublereal err6, dxmv;


/*  Discrete control algorithms */


/*   MEASUREMENT AND VALVE COMMON BLOCK */


/*   CONTROLLER COMMON BLOCK */



/*  Example PI Controller: */
/*     Stripper Level Controller */
    if (pv_1.xmeas[12] >= 2950.f) {
	pv_1.xmv[5] = 100.f;
	flag6_1.flag__ = 1;
    } else if (flag6_1.flag__ == 1 && pv_1.xmeas[12] >= 2633.7f) {
	pv_1.xmv[5] = 100.f;
    } else if (flag6_1.flag__ == 1 && pv_1.xmeas[12] <= 2633.7f) {
	pv_1.xmv[5] = 40.06f;
	ctrlall_1.setpt[5] = .33712f;
	ctrl6_1.errold6 = 0.f;
	flag6_1.flag__ = 0;
    } else if (pv_1.xmeas[12] <= 2300.f) {
	pv_1.xmv[5] = 0.f;
	flag6_1.flag__ = 2;
    } else if (flag6_1.flag__ == 2 && pv_1.xmeas[12] <= 2633.7f) {
	pv_1.xmv[5] = 0.f;
    } else if (flag6_1.flag__ == 2 && pv_1.xmeas[12] >= 2633.7f) {
	pv_1.xmv[5] = 40.06f;
	ctrlall_1.setpt[5] = .33712f;
	ctrl6_1.errold6 = 0.f;
	flag6_1.flag__ = 0;
    } else {
	flag6_1.flag__ = 0;

/*    Calculate Error */

	err6 = (ctrlall_1.setpt[5] - pv_1.xmeas[9]) * 100.f / 1.f;

/*    Proportional-Integral Controller (Velocity Form) */
/*         GAIN = Controller Gain */
/*         TAUI = Reset Time (min) */

/* 	PRINT *, 'XMV(6)= ', XMV(6) */
	dxmv = ctrl6_1.gain6 * (err6 - ctrl6_1.errold6);

/* 	PRINT *, 'GAIN6= ', GAIN6 */
/* 	PRINT *, 'SETPT(6)= ', SETPT(6) */
/* 	PRINT *, 'XMEAS(10)= ', XMEAS(10) */
	pv_1.xmv[5] += dxmv;

/* 	PRINT *, 'ERROLD6= ', ERROLD6 */
/* 	PRINT *, 'ERR6= ', ERR6 */
/* 	PRINT *, 'XMV(6)== ', XMV(6) */
	ctrl6_1.errold6 = err6;
    }

    return 0;
} /* contrl6_ */


/* ============================================================================= */

/* Subroutine */ int contrl7_(void)
{
    static doublereal err7, dxmv;


/*  Discrete control algorithms */


/*   MEASUREMENT AND VALVE COMMON BLOCK */


/*   CONTROLLER COMMON BLOCK */



/*  Example PI Controller: */
/*    Stripper Level Controller */

/*    Calculate Error */

    err7 = (ctrlall_1.setpt[6] - pv_1.xmeas[11]) * 100.f / 70.f;

/*    Proportional-Integral Controller (Velocity Form) */
/*         GAIN = Controller Gain */
/*         TAUI = Reset Time (min) */

    dxmv = ctrl7_1.gain7 * (err7 - ctrl7_1.errold7);

    pv_1.xmv[6] += dxmv;

    ctrl7_1.errold7 = err7;

    return 0;
} /* contrl7_ */


/* ============================================================================= */

/* Subroutine */ int contrl8_(void)
{
    static doublereal err8, dxmv;


/*  Discrete control algorithms */


/*   MEASUREMENT AND VALVE COMMON BLOCK */


/*   CONTROLLER COMMON BLOCK */



/*  Example PI Controller: */
/*    Stripper Level Controller */

/*    Calculate Error */

    err8 = (ctrlall_1.setpt[7] - pv_1.xmeas[14]) * 100.f / 70.f;

/*    Proportional-Integral Controller (Velocity Form) */
/*         GAIN = Controller Gain */
/*         TAUI = Reset Time (min) */

    dxmv = ctrl8_1.gain8 * (err8 - ctrl8_1.errold8);

    pv_1.xmv[7] += dxmv;

    ctrl8_1.errold8 = err8;

    return 0;
} /* contrl8_ */


/* ============================================================================= */

/* Subroutine */ int contrl9_(void)
{
    static doublereal err9, dxmv;


/*  Discrete control algorithms */


/*   MEASUREMENT AND VALVE COMMON BLOCK */


/*   CONTROLLER COMMON BLOCK */



/*  Example PI Controller: */
/*    Stripper Level Controller */

/*    Calculate Error */

    err9 = (ctrlall_1.setpt[8] - pv_1.xmeas[18]) * 100.f / 460.f;

/*    Proportional-Integral Controller (Velocity Form) */
/*         GAIN = Controller Gain */
/*         TAUI = Reset Time (min) */

    dxmv = ctrl9_1.gain9 * (err9 - ctrl9_1.errold9);

    pv_1.xmv[8] += dxmv;

    ctrl9_1.errold9 = err9;

    return 0;
} /* contrl9_ */


/* ============================================================================= */

/* Subroutine */ int contrl10_(void)
{
    static doublereal err10, dxmv;


/*  Discrete control algorithms */


/*   MEASUREMENT AND VALVE COMMON BLOCK */


/*   CONTROLLER COMMON BLOCK */



/*  Example PI Controller: */
/*    Stripper Level Controller */

/*    Calculate Error */

    err10 = (ctrlall_1.setpt[9] - pv_1.xmeas[20]) * 100.f / 150.f;

/*    Proportional-Integral Controller (Velocity Form) */
/*         GAIN = Controller Gain */
/*         TAUI = Reset Time (min) */

    dxmv = ctrl10_1.gain10 * (err10 - ctrl10_1.errold10 + err10 * 
	    ctrlall_1.deltat * 3.f / ctrl10_1.taui10);

    pv_1.xmv[9] += dxmv;

    ctrl10_1.errold10 = err10;

    return 0;
} /* contrl10_ */


/* ============================================================================= */

/* Subroutine */ int contrl11_(void)
{
    static doublereal err11, dxmv;


/*  Discrete control algorithms */


/*   MEASUREMENT AND VALVE COMMON BLOCK */


/*   CONTROLLER COMMON BLOCK */



/*  Example PI Controller: */
/*    Stripper Level Controller */

/*    Calculate Error */

    err11 = (ctrlall_1.setpt[10] - pv_1.xmeas[16]) * 100.f / 46.f;

/*    Proportional-Integral Controller (Velocity Form) */
/*         GAIN = Controller Gain */
/*         TAUI = Reset Time (min) */

    dxmv = ctrl11_1.gain11 * (err11 - ctrl11_1.errold11 + err11 * 
	    ctrlall_1.deltat * 3.f / ctrl11_1.taui11);

    pv_1.xmv[10] += dxmv;

    ctrl11_1.errold11 = err11;

    return 0;
} /* contrl11_ */


/* ============================================================================= */

/* Subroutine */ int contrl13_(void)
{
    static doublereal err13, dxmv;


/*  Discrete control algorithms */


/*   MEASUREMENT AND VALVE COMMON BLOCK */


/*   CONTROLLER COMMON BLOCK */



/*  Example PI Controller: */
/*    Stripper Level Controller */

/*    Calculate Error */

    err13 = (ctrlall_1.setpt[12] - pv_1.xmeas[22]) * 100.f / 100.f;

/*    Proportional-Integral Controller (Velocity Form) */
/*         GAIN = Controller Gain */
/*         TAUI = Reset Time (min) */

    dxmv = ctrl13_1.gain13 * (err13 - ctrl13_1.errold13 + err13 * 
	    ctrlall_1.deltat * 360.f / ctrl13_1.taui13);

    ctrlall_1.setpt[2] += dxmv * 1.017f / 100.f;

    ctrl13_1.errold13 = err13;

    return 0;
} /* contrl13_ */


/* ============================================================================= */

/* Subroutine */ int contrl14_(void)
{
    static doublereal err14, dxmv;


/*  Discrete control algorithms */


/*   MEASUREMENT AND VALVE COMMON BLOCK */


/*   CONTROLLER COMMON BLOCK */



/*  Example PI Controller: */
/*    Stripper Level Controller */

/*    Calculate Error */

    err14 = (ctrlall_1.setpt[13] - pv_1.xmeas[25]) * 100.f / 100.f;

/*    Proportional-Integral Controller (Velocity Form) */
/*         GAIN = Controller Gain */
/*         TAUI = Reset Time (min) */

    dxmv = ctrl14_1.gain14 * (err14 - ctrl14_1.errold14 + err14 * 
	    ctrlall_1.deltat * 360.f / ctrl14_1.taui14);

    ctrlall_1.setpt[0] += dxmv * 5811.f / 100.f;

    ctrl14_1.errold14 = err14;

    return 0;
} /* contrl14_ */


/* ============================================================================= */

/* Subroutine */ int contrl15_(void)
{
    static doublereal err15, dxmv;


/*  Discrete control algorithms */


/*   MEASUREMENT AND VALVE COMMON BLOCK */


/*   CONTROLLER COMMON BLOCK */



/*  Example PI Controller: */
/*    Stripper Level Controller */

/*    Calculate Error */

    err15 = (ctrlall_1.setpt[14] - pv_1.xmeas[26]) * 100.f / 100.f;

/*    Proportional-Integral Controller (Velocity Form) */
/*         GAIN = Controller Gain */
/*         TAUI = Reset Time (min) */

    dxmv = ctrl15_1.gain15 * (err15 - ctrl15_1.errold15 + err15 * 
	    ctrlall_1.deltat * 360.f / ctrl15_1.taui15);

    ctrlall_1.setpt[1] += dxmv * 8354.f / 100.f;

    ctrl15_1.errold15 = err15;

    return 0;
} /* contrl15_ */


/* ============================================================================= */

/* Subroutine */ int contrl16_(void)
{
    static doublereal err16, dxmv;


/*  Discrete control algorithms */


/*   MEASUREMENT AND VALVE COMMON BLOCK */


/*   CONTROLLER COMMON BLOCK */



/*  Example PI Controller: */
/*    Stripper Level Controller */

/*    Calculate Error */

    err16 = (ctrlall_1.setpt[15] - pv_1.xmeas[17]) * 100.f / 130.f;

/*    Proportional-Integral Controller (Velocity Form) */
/*         GAIN = Controller Gain */
/*         TAUI = Reset Time (min) */

    dxmv = ctrl16_1.gain16 * (err16 - ctrl16_1.errold16 + err16 * 
	    ctrlall_1.deltat * 3.f / ctrl16_1.taui16);

    ctrlall_1.setpt[8] += dxmv * 460.f / 100.f;

    ctrl16_1.errold16 = err16;

    return 0;
} /* contrl16_ */


/* ============================================================================= */

/* Subroutine */ int contrl17_(void)
{
    static doublereal err17, dxmv;


/*  Discrete control algorithms */


/*   MEASUREMENT AND VALVE COMMON BLOCK */


/*   CONTROLLER COMMON BLOCK */



/*  Example PI Controller: */
/*    Stripper Level Controller */

/*    Calculate Error */

    err17 = (ctrlall_1.setpt[16] - pv_1.xmeas[7]) * 100.f / 50.f;

/*    Proportional-Integral Controller (Velocity Form) */
/*         GAIN = Controller Gain */
/*         TAUI = Reset Time (min) */

    dxmv = ctrl17_1.gain17 * (err17 - ctrl17_1.errold17 + err17 * 
	    ctrlall_1.deltat * 3.f / ctrl17_1.taui17);

    ctrlall_1.setpt[3] += dxmv * 15.25f / 100.f;

    ctrl17_1.errold17 = err17;

    return 0;
} /* contrl17_ */


/* ============================================================================= */

/* Subroutine */ int contrl18_(void)
{
    static doublereal err18, dxmv;


/*  Discrete control algorithms */


/*   MEASUREMENT AND VALVE COMMON BLOCK */


/*   CONTROLLER COMMON BLOCK */



/*  Example PI Controller: */
/*    Stripper Level Controller */

/*    Calculate Error */

    err18 = (ctrlall_1.setpt[17] - pv_1.xmeas[8]) * 100.f / 150.f;

/*    Proportional-Integral Controller (Velocity Form) */
/*         GAIN = Controller Gain */
/*         TAUI = Reset Time (min) */

    dxmv = ctrl18_1.gain18 * (err18 - ctrl18_1.errold18 + err18 * 
	    ctrlall_1.deltat * 3.f / ctrl18_1.taui18);

    ctrlall_1.setpt[9] += dxmv * 150.f / 100.f;

    ctrl18_1.errold18 = err18;

    return 0;
} /* contrl18_ */


/* ============================================================================= */

/* Subroutine */ int contrl19_(void)
{
    static doublereal err19, dxmv;


/*  Discrete control algorithms */


/*   MEASUREMENT AND VALVE COMMON BLOCK */


/*   CONTROLLER COMMON BLOCK */



/*  Example PI Controller: */
/*    Stripper Level Controller */

/*    Calculate Error */

    err19 = (ctrlall_1.setpt[18] - pv_1.xmeas[29]) * 100.f / 26.f;
/*      PRINT *, 'ERROLD19= ', ERROLD19 */

/*    Proportional-Integral Controller (Velocity Form) */
/*         GAIN = Controller Gain */
/*         TAUI = Reset Time (min) */

    dxmv = ctrl19_1.gain19 * (err19 - ctrl19_1.errold19 + err19 * 
	    ctrlall_1.deltat * 360.f / ctrl19_1.taui19);

    ctrlall_1.setpt[5] += dxmv * 1.f / 100.f;
/*      PRINT *, 'SETPT(6)= ', SETPT(6) */

    ctrl19_1.errold19 = err19;

    return 0;
} /* contrl19_ */


/* ============================================================================= */

/* Subroutine */ int contrl20_(void)
{
    static doublereal err20, dxmv;


/*  Discrete control algorithms */


/*   MEASUREMENT AND VALVE COMMON BLOCK */


/*   CONTROLLER COMMON BLOCK */



/*  Example PI Controller: */
/*    Stripper Level Controller */

/*    Calculate Error */

    err20 = (ctrlall_1.setpt[19] - pv_1.xmeas[37]) * 100.f / 1.6f;

/*    Proportional-Integral Controller (Velocity Form) */
/*         GAIN = Controller Gain */
/*         TAUI = Reset Time (min) */

    dxmv = ctrl20_1.gain20 * (err20 - ctrl20_1.errold20 + err20 * 
	    ctrlall_1.deltat * 900.f / ctrl20_1.taui20);

    ctrlall_1.setpt[15] += dxmv * 130.f / 100.f;

    ctrl20_1.errold20 = err20;

    return 0;
} /* contrl20_ */


/* ============================================================================= */

/* Subroutine */ int contrl22_(void)
{
    static doublereal err22, dxmv;


/*  Discrete control algorithms */


/*   MEASUREMENT AND VALVE COMMON BLOCK */


/*   CONTROLLER COMMON BLOCK */



/*  Example PI Controller: */
/*    Stripper Level Controller */

/*    Calculate Error */

    err22 = ctrlall_1.setpt[11] - pv_1.xmeas[12];

/*    Proportional-Integral Controller (Velocity Form) */
/*         GAIN = Controller Gain */
/*         TAUI = Reset Time (min) */

    dxmv = ctrl22_1.gain22 * (err22 - ctrl22_1.errold22 + err22 * 
	    ctrlall_1.deltat * 3.f / ctrl22_1.taui22);

    pv_1.xmv[5] += dxmv;

    ctrl22_1.errold22 = err22;

    return 0;
} /* contrl22_ */


/* ============================================================================= */

/* Subroutine */ int output_(void)
{
    /* Builtin functions */
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle(void);

    /* Fortran I/O blocks */
    static cilist io___52 = { 0, 1111, 0, 0, 0 };
    static cilist io___53 = { 0, 1112, 0, 0, 0 };
    static cilist io___54 = { 0, 1113, 0, 0, 0 };
    static cilist io___55 = { 0, 2111, 0, 0, 0 };
    static cilist io___56 = { 0, 2112, 0, 0, 0 };
    static cilist io___57 = { 0, 2113, 0, 0, 0 };
    static cilist io___58 = { 0, 2114, 0, 0, 0 };
    static cilist io___59 = { 0, 2115, 0, 0, 0 };
    static cilist io___60 = { 0, 2116, 0, 0, 0 };
    static cilist io___61 = { 0, 2117, 0, 0, 0 };
    static cilist io___62 = { 0, 2118, 0, 0, 0 };
    static cilist io___63 = { 0, 2119, 0, 0, 0 };
    static cilist io___64 = { 0, 2120, 0, 0, 0 };
    static cilist io___65 = { 0, 2121, 0, 0, 0 };




/*   MEASUREMENT AND VALVE COMMON BLOCK */


    s_wsle(&io___52);
    do_lio(&c__5, &c__1, (char *)&pv_1.xmv[0], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmv[1], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmv[2], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmv[3], (ftnlen)sizeof(doublereal));
    e_wsle();
    s_wsle(&io___53);
    do_lio(&c__5, &c__1, (char *)&pv_1.xmv[4], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmv[5], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmv[6], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmv[7], (ftnlen)sizeof(doublereal));
    e_wsle();
    s_wsle(&io___54);
    do_lio(&c__5, &c__1, (char *)&pv_1.xmv[8], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmv[9], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmv[10], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmv[11], (ftnlen)sizeof(doublereal));
    e_wsle();
    s_wsle(&io___55);
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[0], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[1], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[2], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[3], (ftnlen)sizeof(doublereal));
    e_wsle();
    s_wsle(&io___56);
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[4], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[5], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[6], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[7], (ftnlen)sizeof(doublereal));
    e_wsle();
    s_wsle(&io___57);
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[8], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[9], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[10], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[11], (ftnlen)sizeof(doublereal));
    e_wsle();
    s_wsle(&io___58);
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[12], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[13], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[14], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[15], (ftnlen)sizeof(doublereal));
    e_wsle();
    s_wsle(&io___59);
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[16], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[17], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[18], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[19], (ftnlen)sizeof(doublereal));
    e_wsle();
    s_wsle(&io___60);
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[20], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[21], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[22], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[23], (ftnlen)sizeof(doublereal));
    e_wsle();
    s_wsle(&io___61);
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[24], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[25], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[26], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[27], (ftnlen)sizeof(doublereal));
    e_wsle();
    s_wsle(&io___62);
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[28], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[29], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[30], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[31], (ftnlen)sizeof(doublereal));
    e_wsle();
    s_wsle(&io___63);
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[32], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[33], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[34], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[35], (ftnlen)sizeof(doublereal));
    e_wsle();
    s_wsle(&io___64);
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[36], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[37], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[38], (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[39], (ftnlen)sizeof(doublereal));
    e_wsle();
    s_wsle(&io___65);
    do_lio(&c__5, &c__1, (char *)&pv_1.xmeas[40], (ftnlen)sizeof(doublereal));
    e_wsle();
/* L100: */
/* L200: */
/* L300: */

    return 0;
} /* output_ */


/* ============================================================================= */

/* Subroutine */ int intgtr_(integer *nn, doublereal *time, doublereal *
	deltat, doublereal *yy, doublereal *yp)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__;
    extern /* Subroutine */ int tefunc_(integer *, doublereal *, doublereal *,
	     doublereal *);


/*  Euler Integration Algorithm */




    /* Parameter adjustments */
    --yp;
    --yy;

    /* Function Body */
    tefunc_(nn, time, &yy[1], &yp[1]);

    *time += *deltat;

    i__1 = *nn;
    for (i__ = 1; i__ <= i__1; ++i__) {

	yy[i__] += yp[i__] * *deltat;

/* L100: */
    }

    return 0;
} /* intgtr_ */


/* ============================================================================= */

/* Subroutine */ int conshand_(void)
{
    static integer i__;


/*  Euler Integration Algorithm */




    for (i__ = 1; i__ <= 11; ++i__) {
	if (pv_1.xmv[i__ - 1] <= 0.f) {
	    pv_1.xmv[i__ - 1] = 0.f;
	}
	if (pv_1.xmv[i__ - 1] >= 100.f) {
	    pv_1.xmv[i__ - 1] = 100.f;
	}
/* L100: */
    }

    return 0;
} /* conshand_ */

