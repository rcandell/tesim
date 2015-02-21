/* .\teprob.f -- translated by f2c (version 20100827).
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

union {
    struct {
	doublereal xmeas[41], xmv[12];
	integer isd;
    } _1;
    struct {
	doublereal xmeas[41], xmv[12];
    } _2;
} pv_;

#define pv_1 (pv_._1)
#define pv_2 (pv_._2)

struct {
    integer idv[20];
} dvec_;

#define dvec_1 dvec_

struct {
    doublereal uclr[8], ucvr[8], utlr, utvr, xlr[8], xvr[8], etr, esr, tcr, 
	    tkr, dlr, vlr, vvr, vtr, ptr, ppr[8], crxr[8], rr[4], rh, fwr, 
	    twr, qur, hwr, uar, ucls[8], ucvs[8], utls, utvs, xls[8], xvs[8], 
	    ets, ess, tcs, tks, dls, vls, vvs, vts, pts, pps[8], fws, tws, 
	    qus, hws, uclc[8], utlc, xlc[8], etc, esc, tcc, dlc, vlc, vtc, 
	    quc, ucvv[8], utvv, xvv[8], etv, esv, tcv, tkv, vtv, ptv, vcv[12],
	     vrng[12], vtau[12], ftm[13], fcm[104]	/* was [8][13] */, 
	    xst[104]	/* was [8][13] */, xmws[13], hst[13], tst[13], sfr[8],
	     cpflmx, cpprmx, cpdh, tcwr, tcws, htr[3], agsp, xdel[41], xns[41]
	    , tgas, tprod, vst[12];
    integer ivst[12];
} teproc_;

#define teproc_1 teproc_

struct {
    doublereal adist[12], bdist[12], cdist[12], ddist[12], tlast[12], tnext[
	    12], hspan[12], hzero[12], sspan[12], szero[12], spspan[12];
    integer idvwlk[12];
} wlk_;

#define wlk_1 wlk_

struct {
    doublereal avp[8], bvp[8], cvp[8], ah[8], bh[8], ch[8], ag[8], bg[8], cg[
	    8], av[8], ad[8], bd[8], cd[8], xmw[8];
} const_;

#define const_1 const_

struct {
    doublereal g;
} randsd_;

#define randsd_1 randsd_

/* Table of constant values */

static integer c__1 = 1;
static integer c__2 = 2;
static integer c__3 = 3;
static integer c__4 = 4;
static integer c__5 = 5;
static integer c__6 = 6;
static integer c__7 = 7;
static integer c__8 = 8;
static integer c__0 = 0;
static doublereal c_b28 = 1.1544;
static doublereal c_b29 = .3735;
static integer c__9 = 9;
static integer c__12 = 12;
static integer c__10 = 10;
static integer c__11 = 11;
static doublereal c_b78 = 4294967296.;


/*               Tennessee Eastman Process Control Test Problem */

/*                    James J. Downs and Ernest F. Vogel */

/*                  Process and Control Systems Engineering */
/*                        Tennessee Eastman Company */
/*                              P.O. Box 511 */
/*                          Kingsport,TN  37662 */

/*  Reference: */
/*    "A Plant-Wide Industrial Process Control Problem" */
/*    Presented at the AIChE 1990 Annual Meeting */
/*    Industrial Challenge Problems in Process Control,Paper #24a */
/*    Chicago,Illinois,November 14,1990 */

/*  Subroutines: */

/*    TEFUNC - Function evaluator to be called by integrator */
/*    TEINIT - Initialization */
/*    TESUBi - Utility subroutines, i=1,2,..,8 */


/*  The process simulation has 50 states (NN=50).  If the user wishes to */
/*  integrate additional states, NN must be increased accordingly in the */
/*  calling program.  The additional states should be appended to the end */
/*  of the YY vector, e.g. YY(51),...  The additional derivatives should */
/*  be appended to the end of the YP vector, e.g. YP(51),...  To initialize */
/*  the new states and to calculate derivatives for them, we suggest */
/*  creating new function evaluator and initialization routines as follows. */

/*          C----------------------------------------------- */
/*          C */
/*                SUBROUTINE FUNC(NN,TIME,YY,YP) */
/*          C */
/*                INTEGER NN */
/*                DOUBLE PRECISION TIME, YY(NN), YP(NN) */
/*          C */
/*          C  Call the function evaluator for the process */
/*          C */
/*                CALL TEFUNC(NN,TIME,YY,YP) */
/*          C */
/*          C  Calculate derivatives for additional states */
/*          C */
/*                YP(51) = .... */
/*                YP(52) = .... */
/*                   . */
/*                   . */
/*                   . */
/*                YP(NN) = .... */
/*          C */
/*                RETURN */
/*                END */
/*          C */
/*          C----------------------------------------------- */
/*          C */
/*                SUBROUTINE INIT(NN,TIME,YY,YP) */
/*          C */
/*                INTEGER NN */
/*                DOUBLE PRECISION TIME, YY(NN), YP(NN) */
/*          C */
/*          C  Call the initialization for the process */
/*          C */
/*                CALL TEINIT(NN,TIME,YY,YP) */
/*          C */
/*          C  Initialize additional states */
/*          C */
/*                YY(51) = .... */
/*                YY(52) = .... */
/*                   . */
/*                   . */
/*                   . */
/*                YY(NN) = .... */
/*          C */
/*                RETURN */
/*                END */
/*          C */
/*          C----------------------------------------------- */

/*  Differences between the code and its description in the paper: */

/*  1.  Subroutine TEINIT has TIME in the argument list.  TEINIT sets TIME */
/*      to zero. */

/*  2.  There are 8 utility subroutines (TESUBi) rather than 5. */

/*  3.  Process disturbances 14 through 20 do NOT need to be used in */
/*      conjunction with another disturbance as stated in the paper.  All */
/*      disturbances can be used alone or in any combination. */


/*  Manipulated Variables */

/*    XMV(1)     A Feed Flow (stream 1) */
/*    XMV(2)     D Feed Flow (stream 2) */
/*    XMV(3)     E Feed Flow (stream 3) */
/*    XMV(4)     A and C Feed Flow (stream 4) */
/*    XMV(5)     Compressor Recycle Valve */
/*    XMV(6)     Purge Valve (stream 9) */
/*    XMV(7)     Separator Pot Liquid Flow (stream 10) */
/*    XMV(8)     Stripper Liquid Product Flow (stream 11) */
/*    XMV(9)     Stripper Steam Valve */
/*    XMV(10)    Reactor Cooling Water Flow */
/*    XMV(11)    Condenser Cooling Water Flow */
/*    XMV(12)    Agitator Speed */

/*  Continuous Process Measurements */

/*    XMEAS(1)   A Feed  (stream 1)                    kscmh */
/*    XMEAS(2)   D Feed  (stream 2)                    kg/hr */
/*    XMEAS(3)   E Feed  (stream 3)                    kg/hr */
/*    XMEAS(4)   A and C Feed  (stream 4)              kscmh */
/*    XMEAS(5)   Recycle Flow  (stream 8)              kscmh */
/*    XMEAS(6)   Reactor Feed Rate  (stream 6)         kscmh */
/*    XMEAS(7)   Reactor Pressure                      kPa gauge */
/*    XMEAS(8)   Reactor Level                         % */
/*    XMEAS(9)   Reactor Temperature                   Deg C */
/*    XMEAS(10)  Purge Rate (stream 9)                 kscmh */
/*    XMEAS(11)  Product Sep Temp                      Deg C */
/*    XMEAS(12)  Product Sep Level                     % */
/*    XMEAS(13)  Prod Sep Pressure                     kPa gauge */
/*    XMEAS(14)  Prod Sep Underflow (stream 10)        m3/hr */
/*    XMEAS(15)  Stripper Level                        % */
/*    XMEAS(16)  Stripper Pressure                     kPa gauge */
/*    XMEAS(17)  Stripper Underflow (stream 11)        m3/hr */
/*    XMEAS(18)  Stripper Temperature                  Deg C */
/*    XMEAS(19)  Stripper Steam Flow                   kg/hr */
/*    XMEAS(20)  Compressor Work                       kW */
/*    XMEAS(21)  Reactor Cooling Water Outlet Temp     Deg C */
/*    XMEAS(22)  Separator Cooling Water Outlet Temp   Deg C */

/*  Sampled Process Measurements */

/*    Reactor Feed Analysis (Stream 6) */
/*        Sampling Frequency = 0.1 hr */
/*        Dead Time = 0.1 hr */
/*        Mole % */
/*    XMEAS(23)   Component A */
/*    XMEAS(24)   Component B */
/*    XMEAS(25)   Component C */
/*    XMEAS(26)   Component D */
/*    XMEAS(27)   Component E */
/*    XMEAS(28)   Component F */

/*    Purge Gas Analysis (Stream 9) */
/*        Sampling Frequency = 0.1 hr */
/*        Dead Time = 0.1 hr */
/*        Mole % */
/*    XMEAS(29)   Component A */
/*    XMEAS(30)   Component B */
/*    XMEAS(31)   Component C */
/*    XMEAS(32)   Component D */
/*    XMEAS(33)   Component E */
/*    XMEAS(34)   Component F */
/*    XMEAS(35)   Component G */
/*    XMEAS(36)   Component H */

/*    Product Analysis (Stream 11) */
/*        Sampling Frequency = 0.25 hr */
/*        Dead Time = 0.25 hr */
/*        Mole % */
/*    XMEAS(37)   Component D */
/*    XMEAS(38)   Component E */
/*    XMEAS(39)   Component F */
/*    XMEAS(40)   Component G */
/*    XMEAS(41)   Component H */

/*  Process Disturbances */

/*    IDV(1)   A/C Feed Ratio, B Composition Constant (Stream 4)          Step */
/*    IDV(2)   B Composition, A/C Ratio Constant (Stream 4)               Step */
/*    IDV(3)   D Feed Temperature (Stream 2)                              Step */
/*    IDV(4)   Reactor Cooling Water Inlet Temperature                    Step */
/*    IDV(5)   Condenser Cooling Water Inlet Temperature                  Step */
/*    IDV(6)   A Feed Loss (Stream 1)                                     Step */
/*    IDV(7)   C Header Pressure Loss - Reduced Availability (Stream 4)   Step */
/*    IDV(8)   A, B, C Feed Composition (Stream 4)            Random Variation */
/*    IDV(9)   D Feed Temperature (Stream 2)                  Random Variation */
/*    IDV(10)  C Feed Temperature (Stream 4)                  Random Variation */
/*    IDV(11)  Reactor Cooling Water Inlet Temperature        Random Variation */
/*    IDV(12)  Condenser Cooling Water Inlet Temperature      Random Variation */
/*    IDV(13)  Reaction Kinetics                                    Slow Drift */
/*    IDV(14)  Reactor Cooling Water Valve                            Sticking */
/*    IDV(15)  Condenser Cooling Water Valve                          Sticking */
/*    IDV(16)  Unknown */
/*    IDV(17)  Unknown */
/*    IDV(18)  Unknown */
/*    IDV(19)  Unknown */
/*    IDV(20)  Unknown */


/* ============================================================================= */

/* Subroutine */ int tefunc_(integer *nn, doublereal *time, doublereal *yy, 
	doublereal *yp)
{
    /* System generated locals */
    integer i__1;
    doublereal d__1;

    /* Builtin functions */
    double exp(doublereal), pow_dd(doublereal *, doublereal *), sqrt(
	    doublereal);

    /* Local variables */
    static integer i__;
    static doublereal rg, pr, r1f, r2f, uac, fin[8], dlp, vpr, uas, flms, 
	    xcmp[41], hwlk, vpos[12], xmns, swlk, spwlk, vovrl;
    extern /* Subroutine */ int tesub1_(doublereal *, doublereal *, 
	    doublereal *, integer *), tesub2_(doublereal *, doublereal *, 
	    doublereal *, integer *), tesub4_(doublereal *, doublereal *, 
	    doublereal *), tesub5_(doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *), tesub6_(doublereal *, 
	    doublereal *);
    extern doublereal tesub7_(integer *), tesub8_(integer *, doublereal *);
    static doublereal flcoef, tmpfac, uarlev;


/*       Function Evaluator */

/*         Inputs: */

/*           NN   = Number of differential equations */
/*           Time = Current time(hrs) */
/*           YY   = Current state values */

/*         Outputs: */

/*           YP   = Current derivative values */

/*  MEASUREMENT AND VALVE COMMON BLOCK */


/*   DISTURBANCE VECTOR COMMON BLOCK */

/* 	NOTE: I have included isd in the /PV/ common.  This is set */
/* 		non-zero when the process is shutting down. */
    /* Parameter adjustments */
    --yp;
    --yy;

    /* Function Body */
    for (i__ = 1; i__ <= 20; ++i__) {
	if (dvec_1.idv[i__ - 1] > 0) {
	    dvec_1.idv[i__ - 1] = 1;
	} else {
	    dvec_1.idv[i__ - 1] = 0;
	}
/* L500: */
    }
    wlk_1.idvwlk[0] = dvec_1.idv[7];
    wlk_1.idvwlk[1] = dvec_1.idv[7];
    wlk_1.idvwlk[2] = dvec_1.idv[8];
    wlk_1.idvwlk[3] = dvec_1.idv[9];
    wlk_1.idvwlk[4] = dvec_1.idv[10];
    wlk_1.idvwlk[5] = dvec_1.idv[11];
    wlk_1.idvwlk[6] = dvec_1.idv[12];
    wlk_1.idvwlk[7] = dvec_1.idv[12];
    wlk_1.idvwlk[8] = dvec_1.idv[15];
    wlk_1.idvwlk[9] = dvec_1.idv[16];
    wlk_1.idvwlk[10] = dvec_1.idv[17];
    wlk_1.idvwlk[11] = dvec_1.idv[19];
    for (i__ = 1; i__ <= 9; ++i__) {
	if (*time >= wlk_1.tnext[i__ - 1]) {
	    hwlk = wlk_1.tnext[i__ - 1] - wlk_1.tlast[i__ - 1];
	    swlk = wlk_1.adist[i__ - 1] + hwlk * (wlk_1.bdist[i__ - 1] + hwlk 
		    * (wlk_1.cdist[i__ - 1] + hwlk * wlk_1.ddist[i__ - 1]));
	    spwlk = wlk_1.bdist[i__ - 1] + hwlk * (wlk_1.cdist[i__ - 1] * 2. 
		    + hwlk * 3. * wlk_1.ddist[i__ - 1]);
	    wlk_1.tlast[i__ - 1] = wlk_1.tnext[i__ - 1];
	    tesub5_(&swlk, &spwlk, &wlk_1.adist[i__ - 1], &wlk_1.bdist[i__ - 
		    1], &wlk_1.cdist[i__ - 1], &wlk_1.ddist[i__ - 1], &
		    wlk_1.tlast[i__ - 1], &wlk_1.tnext[i__ - 1], &wlk_1.hspan[
		    i__ - 1], &wlk_1.hzero[i__ - 1], &wlk_1.sspan[i__ - 1], &
		    wlk_1.szero[i__ - 1], &wlk_1.spspan[i__ - 1], &
		    wlk_1.idvwlk[i__ - 1]);
	}
/* L900: */
    }
    for (i__ = 10; i__ <= 12; ++i__) {
	if (*time >= wlk_1.tnext[i__ - 1]) {
	    hwlk = wlk_1.tnext[i__ - 1] - wlk_1.tlast[i__ - 1];
	    swlk = wlk_1.adist[i__ - 1] + hwlk * (wlk_1.bdist[i__ - 1] + hwlk 
		    * (wlk_1.cdist[i__ - 1] + hwlk * wlk_1.ddist[i__ - 1]));
	    spwlk = wlk_1.bdist[i__ - 1] + hwlk * (wlk_1.cdist[i__ - 1] * 2. 
		    + hwlk * 3. * wlk_1.ddist[i__ - 1]);
	    wlk_1.tlast[i__ - 1] = wlk_1.tnext[i__ - 1];
	    if (swlk > .1) {
		wlk_1.adist[i__ - 1] = swlk;
		wlk_1.bdist[i__ - 1] = spwlk;
		wlk_1.cdist[i__ - 1] = -(swlk * 3. + spwlk * .2) / .01;
		wlk_1.ddist[i__ - 1] = (swlk * 2. + spwlk * .1) / .001;
		wlk_1.tnext[i__ - 1] = wlk_1.tlast[i__ - 1] + .1;
	    } else {
		pv_1.isd = -1;
		hwlk = wlk_1.hspan[i__ - 1] * tesub7_(&pv_1.isd) + 
			wlk_1.hzero[i__ - 1];
		wlk_1.adist[i__ - 1] = 0.;
		wlk_1.bdist[i__ - 1] = 0.;
/* Computing 2nd power */
		d__1 = hwlk;
		wlk_1.cdist[i__ - 1] = (doublereal) wlk_1.idvwlk[i__ - 1] / (
			d__1 * d__1);
		wlk_1.ddist[i__ - 1] = 0.;
		wlk_1.tnext[i__ - 1] = wlk_1.tlast[i__ - 1] + hwlk;
	    }
	}
/* L910: */
    }
    if (*time == 0.) {
	for (i__ = 1; i__ <= 12; ++i__) {
	    wlk_1.adist[i__ - 1] = wlk_1.szero[i__ - 1];
	    wlk_1.bdist[i__ - 1] = 0.;
	    wlk_1.cdist[i__ - 1] = 0.;
	    wlk_1.ddist[i__ - 1] = 0.;
	    wlk_1.tlast[i__ - 1] = 0.;
	    wlk_1.tnext[i__ - 1] = .1;
/* L950: */
	}
    }
    teproc_1.xst[24] = tesub8_(&c__1, time) - dvec_1.idv[0] * .03 - 
	    dvec_1.idv[1] * .00243719;
    teproc_1.xst[25] = tesub8_(&c__2, time) + dvec_1.idv[1] * .005;
    teproc_1.xst[26] = 1. - teproc_1.xst[24] - teproc_1.xst[25];
    teproc_1.tst[0] = tesub8_(&c__3, time) + dvec_1.idv[2] * 5.;
    teproc_1.tst[3] = tesub8_(&c__4, time);
    teproc_1.tcwr = tesub8_(&c__5, time) + dvec_1.idv[3] * 5.;
    teproc_1.tcws = tesub8_(&c__6, time) + dvec_1.idv[4] * 5.;
    r1f = tesub8_(&c__7, time);
    r2f = tesub8_(&c__8, time);
    for (i__ = 1; i__ <= 3; ++i__) {
	teproc_1.ucvr[i__ - 1] = yy[i__];
	teproc_1.ucvs[i__ - 1] = yy[i__ + 9];
	teproc_1.uclr[i__ - 1] = 0.f;
	teproc_1.ucls[i__ - 1] = 0.f;
/* L1010: */
    }
    for (i__ = 4; i__ <= 8; ++i__) {
	teproc_1.uclr[i__ - 1] = yy[i__];
	teproc_1.ucls[i__ - 1] = yy[i__ + 9];
/* L1020: */
    }
    for (i__ = 1; i__ <= 8; ++i__) {
	teproc_1.uclc[i__ - 1] = yy[i__ + 18];
	teproc_1.ucvv[i__ - 1] = yy[i__ + 27];
/* L1030: */
    }
    teproc_1.etr = yy[9];
    teproc_1.ets = yy[18];
    teproc_1.etc = yy[27];
    teproc_1.etv = yy[36];
    teproc_1.twr = yy[37];
    teproc_1.tws = yy[38];
    for (i__ = 1; i__ <= 12; ++i__) {
	vpos[i__ - 1] = yy[i__ + 38];
/* L1035: */
    }
    teproc_1.utlr = 0.f;
    teproc_1.utls = 0.f;
    teproc_1.utlc = 0.f;
    teproc_1.utvv = 0.f;
    for (i__ = 1; i__ <= 8; ++i__) {
	teproc_1.utlr += teproc_1.uclr[i__ - 1];
	teproc_1.utls += teproc_1.ucls[i__ - 1];
	teproc_1.utlc += teproc_1.uclc[i__ - 1];
	teproc_1.utvv += teproc_1.ucvv[i__ - 1];
/* L1040: */
    }
    for (i__ = 1; i__ <= 8; ++i__) {
	teproc_1.xlr[i__ - 1] = teproc_1.uclr[i__ - 1] / teproc_1.utlr;
	teproc_1.xls[i__ - 1] = teproc_1.ucls[i__ - 1] / teproc_1.utls;
	teproc_1.xlc[i__ - 1] = teproc_1.uclc[i__ - 1] / teproc_1.utlc;
	teproc_1.xvv[i__ - 1] = teproc_1.ucvv[i__ - 1] / teproc_1.utvv;
/* L1050: */
    }
    teproc_1.esr = teproc_1.etr / teproc_1.utlr;
    teproc_1.ess = teproc_1.ets / teproc_1.utls;
    teproc_1.esc = teproc_1.etc / teproc_1.utlc;
    teproc_1.esv = teproc_1.etv / teproc_1.utvv;
    tesub2_(teproc_1.xlr, &teproc_1.tcr, &teproc_1.esr, &c__0);
    teproc_1.tkr = teproc_1.tcr + 273.15f;
    tesub2_(teproc_1.xls, &teproc_1.tcs, &teproc_1.ess, &c__0);
    teproc_1.tks = teproc_1.tcs + 273.15f;
    tesub2_(teproc_1.xlc, &teproc_1.tcc, &teproc_1.esc, &c__0);
    tesub2_(teproc_1.xvv, &teproc_1.tcv, &teproc_1.esv, &c__2);
    teproc_1.tkv = teproc_1.tcv + 273.15f;
    tesub4_(teproc_1.xlr, &teproc_1.tcr, &teproc_1.dlr);
    tesub4_(teproc_1.xls, &teproc_1.tcs, &teproc_1.dls);
    tesub4_(teproc_1.xlc, &teproc_1.tcc, &teproc_1.dlc);
    teproc_1.vlr = teproc_1.utlr / teproc_1.dlr;
    teproc_1.vls = teproc_1.utls / teproc_1.dls;
    teproc_1.vlc = teproc_1.utlc / teproc_1.dlc;
    teproc_1.vvr = teproc_1.vtr - teproc_1.vlr;
    teproc_1.vvs = teproc_1.vts - teproc_1.vls;
    rg = 998.9f;
    teproc_1.ptr = 0.f;
    teproc_1.pts = 0.f;
    for (i__ = 1; i__ <= 3; ++i__) {
	teproc_1.ppr[i__ - 1] = teproc_1.ucvr[i__ - 1] * rg * teproc_1.tkr / 
		teproc_1.vvr;
	teproc_1.ptr += teproc_1.ppr[i__ - 1];
	teproc_1.pps[i__ - 1] = teproc_1.ucvs[i__ - 1] * rg * teproc_1.tks / 
		teproc_1.vvs;
	teproc_1.pts += teproc_1.pps[i__ - 1];
/* L1110: */
    }
    for (i__ = 4; i__ <= 8; ++i__) {
	vpr = exp(const_1.avp[i__ - 1] + const_1.bvp[i__ - 1] / (teproc_1.tcr 
		+ const_1.cvp[i__ - 1]));
	teproc_1.ppr[i__ - 1] = vpr * teproc_1.xlr[i__ - 1];
	teproc_1.ptr += teproc_1.ppr[i__ - 1];
	vpr = exp(const_1.avp[i__ - 1] + const_1.bvp[i__ - 1] / (teproc_1.tcs 
		+ const_1.cvp[i__ - 1]));
	teproc_1.pps[i__ - 1] = vpr * teproc_1.xls[i__ - 1];
	teproc_1.pts += teproc_1.pps[i__ - 1];
/* L1120: */
    }
    teproc_1.ptv = teproc_1.utvv * rg * teproc_1.tkv / teproc_1.vtv;
    for (i__ = 1; i__ <= 8; ++i__) {
	teproc_1.xvr[i__ - 1] = teproc_1.ppr[i__ - 1] / teproc_1.ptr;
	teproc_1.xvs[i__ - 1] = teproc_1.pps[i__ - 1] / teproc_1.pts;
/* L1130: */
    }
    teproc_1.utvr = teproc_1.ptr * teproc_1.vvr / rg / teproc_1.tkr;
    teproc_1.utvs = teproc_1.pts * teproc_1.vvs / rg / teproc_1.tks;
    for (i__ = 4; i__ <= 8; ++i__) {
	teproc_1.ucvr[i__ - 1] = teproc_1.utvr * teproc_1.xvr[i__ - 1];
	teproc_1.ucvs[i__ - 1] = teproc_1.utvs * teproc_1.xvs[i__ - 1];
/* L1140: */
    }
    teproc_1.rr[0] = exp(31.5859536f - 20130.850528434825f / teproc_1.tkr) * 
	    r1f;
    teproc_1.rr[1] = exp(3.00094014f - 10065.425264217412f / teproc_1.tkr) * 
	    r2f;
    teproc_1.rr[2] = exp(53.4060443f - 30196.275792652239f / teproc_1.tkr);
    teproc_1.rr[3] = teproc_1.rr[2] * .767488334;
    if (teproc_1.ppr[0] > 0.f && teproc_1.ppr[2] > 0.f) {
	r1f = pow_dd(teproc_1.ppr, &c_b28);
	r2f = pow_dd(&teproc_1.ppr[2], &c_b29);
	teproc_1.rr[0] = teproc_1.rr[0] * r1f * r2f * teproc_1.ppr[3];
	teproc_1.rr[1] = teproc_1.rr[1] * r1f * r2f * teproc_1.ppr[4];
    } else {
	teproc_1.rr[0] = 0.f;
	teproc_1.rr[1] = 0.f;
    }
    teproc_1.rr[2] = teproc_1.rr[2] * teproc_1.ppr[0] * teproc_1.ppr[4];
    teproc_1.rr[3] = teproc_1.rr[3] * teproc_1.ppr[0] * teproc_1.ppr[3];
    for (i__ = 1; i__ <= 4; ++i__) {
	teproc_1.rr[i__ - 1] *= teproc_1.vvr;
/* L1200: */
    }
    teproc_1.crxr[0] = -teproc_1.rr[0] - teproc_1.rr[1] - teproc_1.rr[2];
    teproc_1.crxr[2] = -teproc_1.rr[0] - teproc_1.rr[1];
    teproc_1.crxr[3] = -teproc_1.rr[0] - teproc_1.rr[3] * 1.5;
    teproc_1.crxr[4] = -teproc_1.rr[1] - teproc_1.rr[2];
    teproc_1.crxr[5] = teproc_1.rr[2] + teproc_1.rr[3];
    teproc_1.crxr[6] = teproc_1.rr[0];
    teproc_1.crxr[7] = teproc_1.rr[1];
    teproc_1.rh = teproc_1.rr[0] * teproc_1.htr[0] + teproc_1.rr[1] * 
	    teproc_1.htr[1];
    teproc_1.xmws[0] = 0.f;
    teproc_1.xmws[1] = 0.f;
    teproc_1.xmws[5] = 0.f;
    teproc_1.xmws[7] = 0.f;
    teproc_1.xmws[8] = 0.f;
    teproc_1.xmws[9] = 0.f;
    for (i__ = 1; i__ <= 8; ++i__) {
	teproc_1.xst[i__ + 39] = teproc_1.xvv[i__ - 1];
	teproc_1.xst[i__ + 55] = teproc_1.xvr[i__ - 1];
	teproc_1.xst[i__ + 63] = teproc_1.xvs[i__ - 1];
	teproc_1.xst[i__ + 71] = teproc_1.xvs[i__ - 1];
	teproc_1.xst[i__ + 79] = teproc_1.xls[i__ - 1];
	teproc_1.xst[i__ + 95] = teproc_1.xlc[i__ - 1];
	teproc_1.xmws[0] += teproc_1.xst[i__ - 1] * const_1.xmw[i__ - 1];
	teproc_1.xmws[1] += teproc_1.xst[i__ + 7] * const_1.xmw[i__ - 1];
	teproc_1.xmws[5] += teproc_1.xst[i__ + 39] * const_1.xmw[i__ - 1];
	teproc_1.xmws[7] += teproc_1.xst[i__ + 55] * const_1.xmw[i__ - 1];
	teproc_1.xmws[8] += teproc_1.xst[i__ + 63] * const_1.xmw[i__ - 1];
	teproc_1.xmws[9] += teproc_1.xst[i__ + 71] * const_1.xmw[i__ - 1];
/* L2010: */
    }
    teproc_1.tst[5] = teproc_1.tcv;
    teproc_1.tst[7] = teproc_1.tcr;
    teproc_1.tst[8] = teproc_1.tcs;
    teproc_1.tst[9] = teproc_1.tcs;
    teproc_1.tst[10] = teproc_1.tcs;
    teproc_1.tst[12] = teproc_1.tcc;
    tesub1_(teproc_1.xst, teproc_1.tst, teproc_1.hst, &c__1);
    tesub1_(&teproc_1.xst[8], &teproc_1.tst[1], &teproc_1.hst[1], &c__1);
    tesub1_(&teproc_1.xst[16], &teproc_1.tst[2], &teproc_1.hst[2], &c__1);
    tesub1_(&teproc_1.xst[24], &teproc_1.tst[3], &teproc_1.hst[3], &c__1);
    tesub1_(&teproc_1.xst[40], &teproc_1.tst[5], &teproc_1.hst[5], &c__1);
    tesub1_(&teproc_1.xst[56], &teproc_1.tst[7], &teproc_1.hst[7], &c__1);
    tesub1_(&teproc_1.xst[64], &teproc_1.tst[8], &teproc_1.hst[8], &c__1);
    teproc_1.hst[9] = teproc_1.hst[8];
    tesub1_(&teproc_1.xst[80], &teproc_1.tst[10], &teproc_1.hst[10], &c__0);
    tesub1_(&teproc_1.xst[96], &teproc_1.tst[12], &teproc_1.hst[12], &c__0);
    teproc_1.ftm[0] = vpos[0] * teproc_1.vrng[0] / 100.f;
    teproc_1.ftm[1] = vpos[1] * teproc_1.vrng[1] / 100.f;
    teproc_1.ftm[2] = vpos[2] * (1. - dvec_1.idv[5]) * teproc_1.vrng[2] / 
	    100.f;
    teproc_1.ftm[3] = vpos[3] * (1. - dvec_1.idv[6] * .2) * teproc_1.vrng[3] /
	     100.f + 1e-10;
    teproc_1.ftm[10] = vpos[6] * teproc_1.vrng[6] / 100.f;
    teproc_1.ftm[12] = vpos[7] * teproc_1.vrng[7] / 100.f;
    uac = vpos[8] * teproc_1.vrng[8] * (tesub8_(&c__9, time) + 1.) / 100.f;
    teproc_1.fwr = vpos[9] * teproc_1.vrng[9] / 100.f;
    teproc_1.fws = vpos[10] * teproc_1.vrng[10] / 100.f;
    teproc_1.agsp = (vpos[11] + 150.f) / 100.f;
    dlp = teproc_1.ptv - teproc_1.ptr;
    if (dlp < 0.f) {
	dlp = 0.f;
    }
    flms = sqrt(dlp) * 1937.6;
    teproc_1.ftm[5] = flms / teproc_1.xmws[5];
    dlp = teproc_1.ptr - teproc_1.pts;
    if (dlp < 0.f) {
	dlp = 0.f;
    }
    flms = sqrt(dlp) * 4574.21 * (1. - tesub8_(&c__12, time) * .25);
    teproc_1.ftm[7] = flms / teproc_1.xmws[7];
    dlp = teproc_1.pts - 760.f;
    if (dlp < 0.f) {
	dlp = 0.f;
    }
    flms = vpos[5] * .151169 * sqrt(dlp);
    teproc_1.ftm[9] = flms / teproc_1.xmws[9];
    pr = teproc_1.ptv / teproc_1.pts;
    if (pr < 1.f) {
	pr = 1.f;
    }
    if (pr > teproc_1.cpprmx) {
	pr = teproc_1.cpprmx;
    }
    flcoef = teproc_1.cpflmx / 1.197;
/* Computing 3rd power */
    d__1 = pr;
    flms = teproc_1.cpflmx + flcoef * (1.f - d__1 * (d__1 * d__1));
    teproc_1.cpdh = flms * (teproc_1.tcs + 273.15) * 1.8e-6 * 1.9872 * (
	    teproc_1.ptv - teproc_1.pts) / (teproc_1.xmws[8] * teproc_1.pts);
    dlp = teproc_1.ptv - teproc_1.pts;
    if (dlp < 0.f) {
	dlp = 0.f;
    }
    flms -= vpos[4] * 53.349 * sqrt(dlp);
    if (flms < .001) {
	flms = .001;
    }
    teproc_1.ftm[8] = flms / teproc_1.xmws[8];
    teproc_1.hst[8] += teproc_1.cpdh / teproc_1.ftm[8];
    for (i__ = 1; i__ <= 8; ++i__) {
	teproc_1.fcm[i__ - 1] = teproc_1.xst[i__ - 1] * teproc_1.ftm[0];
	teproc_1.fcm[i__ + 7] = teproc_1.xst[i__ + 7] * teproc_1.ftm[1];
	teproc_1.fcm[i__ + 15] = teproc_1.xst[i__ + 15] * teproc_1.ftm[2];
	teproc_1.fcm[i__ + 23] = teproc_1.xst[i__ + 23] * teproc_1.ftm[3];
	teproc_1.fcm[i__ + 39] = teproc_1.xst[i__ + 39] * teproc_1.ftm[5];
	teproc_1.fcm[i__ + 55] = teproc_1.xst[i__ + 55] * teproc_1.ftm[7];
	teproc_1.fcm[i__ + 63] = teproc_1.xst[i__ + 63] * teproc_1.ftm[8];
	teproc_1.fcm[i__ + 71] = teproc_1.xst[i__ + 71] * teproc_1.ftm[9];
	teproc_1.fcm[i__ + 79] = teproc_1.xst[i__ + 79] * teproc_1.ftm[10];
	teproc_1.fcm[i__ + 95] = teproc_1.xst[i__ + 95] * teproc_1.ftm[12];
/* L5020: */
    }
    if (teproc_1.ftm[10] > .1f) {
	if (teproc_1.tcc > 170.f) {
	    tmpfac = teproc_1.tcc - 120.262f;
	} else if (teproc_1.tcc < 5.292f) {
	    tmpfac = .1f;
	} else {
	    tmpfac = 363.744f / (177.f - teproc_1.tcc) - 2.22579488f;
	}
	vovrl = teproc_1.ftm[3] / teproc_1.ftm[10] * tmpfac;
	teproc_1.sfr[3] = vovrl * 8.501f / (vovrl * 8.501f + 1.f);
	teproc_1.sfr[4] = vovrl * 11.402f / (vovrl * 11.402f + 1.f);
	teproc_1.sfr[5] = vovrl * 11.795f / (vovrl * 11.795f + 1.f);
	teproc_1.sfr[6] = vovrl * .048f / (vovrl * .048f + 1.f);
	teproc_1.sfr[7] = vovrl * .0242f / (vovrl * .0242f + 1.f);
    } else {
	teproc_1.sfr[3] = .9999f;
	teproc_1.sfr[4] = .999f;
	teproc_1.sfr[5] = .999f;
	teproc_1.sfr[6] = .99f;
	teproc_1.sfr[7] = .98f;
    }
    for (i__ = 1; i__ <= 8; ++i__) {
	fin[i__ - 1] = 0.f;
	fin[i__ - 1] += teproc_1.fcm[i__ + 23];
	fin[i__ - 1] += teproc_1.fcm[i__ + 79];
/* L6010: */
    }
    teproc_1.ftm[4] = 0.f;
    teproc_1.ftm[11] = 0.f;
    for (i__ = 1; i__ <= 8; ++i__) {
	teproc_1.fcm[i__ + 31] = teproc_1.sfr[i__ - 1] * fin[i__ - 1];
	teproc_1.fcm[i__ + 87] = fin[i__ - 1] - teproc_1.fcm[i__ + 31];
	teproc_1.ftm[4] += teproc_1.fcm[i__ + 31];
	teproc_1.ftm[11] += teproc_1.fcm[i__ + 87];
/* L6020: */
    }
    for (i__ = 1; i__ <= 8; ++i__) {
	teproc_1.xst[i__ + 31] = teproc_1.fcm[i__ + 31] / teproc_1.ftm[4];
	teproc_1.xst[i__ + 87] = teproc_1.fcm[i__ + 87] / teproc_1.ftm[11];
/* L6030: */
    }
    teproc_1.tst[4] = teproc_1.tcc;
    teproc_1.tst[11] = teproc_1.tcc;
    tesub1_(&teproc_1.xst[32], &teproc_1.tst[4], &teproc_1.hst[4], &c__1);
    tesub1_(&teproc_1.xst[88], &teproc_1.tst[11], &teproc_1.hst[11], &c__0);
    teproc_1.ftm[6] = teproc_1.ftm[5];
    teproc_1.hst[6] = teproc_1.hst[5];
    teproc_1.tst[6] = teproc_1.tst[5];
    for (i__ = 1; i__ <= 8; ++i__) {
	teproc_1.xst[i__ + 47] = teproc_1.xst[i__ + 39];
	teproc_1.fcm[i__ + 47] = teproc_1.fcm[i__ + 39];
/* L6130: */
    }
    if (teproc_1.vlr / 7.8f > 50.f) {
	uarlev = 1.f;
    } else if (teproc_1.vlr / 7.8f < 10.f) {
	uarlev = 0.f;
    } else {
	uarlev = teproc_1.vlr * .025f / 7.8f - .25f;
    }
/* Computing 2nd power */
    d__1 = teproc_1.agsp;
    teproc_1.uar = uarlev * (d__1 * d__1 * -.5f + teproc_1.agsp * 2.75f - 
	    2.5f) * .85549;
    teproc_1.qur = teproc_1.uar * (teproc_1.twr - teproc_1.tcr) * (1. - 
	    tesub8_(&c__10, time) * .35);
/* Computing 4th power */
    d__1 = teproc_1.ftm[7] / 3528.73f, d__1 *= d__1;
    uas = (1.f - 1.f / (d__1 * d__1 + 1.f)) * .404655f;
    teproc_1.qus = uas * (teproc_1.tws - teproc_1.tst[7]) * (1. - tesub8_(&
	    c__11, time) * .25);
    teproc_1.quc = 0.;
    if (teproc_1.tcc < 100.f) {
	teproc_1.quc = uac * (100.f - teproc_1.tcc);
    }
    pv_1.xmeas[0] = teproc_1.ftm[2] * .359f / 35.3145f;
    pv_1.xmeas[1] = teproc_1.ftm[0] * teproc_1.xmws[0] * .454f;
    pv_1.xmeas[2] = teproc_1.ftm[1] * teproc_1.xmws[1] * .454f;
    pv_1.xmeas[3] = teproc_1.ftm[3] * .359f / 35.3145f;
    pv_1.xmeas[4] = teproc_1.ftm[8] * .359f / 35.3145f;
    pv_1.xmeas[5] = teproc_1.ftm[5] * .359f / 35.3145f;
    pv_1.xmeas[6] = (teproc_1.ptr - 760.f) / 760.f * 101.325f;
    pv_1.xmeas[7] = (teproc_1.vlr - 84.6f) / 666.7f * 100.f;
    pv_1.xmeas[8] = teproc_1.tcr;
    pv_1.xmeas[9] = teproc_1.ftm[9] * .359f / 35.3145f;
    pv_1.xmeas[10] = teproc_1.tcs;
    pv_1.xmeas[11] = (teproc_1.vls - 27.5f) / 290.f * 100.f;
    pv_1.xmeas[12] = (teproc_1.pts - 760.f) / 760.f * 101.325f;
    pv_1.xmeas[13] = teproc_1.ftm[10] / teproc_1.dls / 35.3145f;
    pv_1.xmeas[14] = (teproc_1.vlc - 78.25f) / teproc_1.vtc * 100.f;
    pv_1.xmeas[15] = (teproc_1.ptv - 760.f) / 760.f * 101.325f;
    pv_1.xmeas[16] = teproc_1.ftm[12] / teproc_1.dlc / 35.3145f;
    pv_1.xmeas[17] = teproc_1.tcc;
    pv_1.xmeas[18] = teproc_1.quc * 1040. * .454f;
    pv_1.xmeas[19] = teproc_1.cpdh * 392.7;
    pv_1.xmeas[19] = teproc_1.cpdh * 293.07;
    pv_1.xmeas[20] = teproc_1.twr;
    pv_1.xmeas[21] = teproc_1.tws;
    pv_1.isd = 0;
    if (pv_1.xmeas[6] > 3e3f) {
	pv_1.isd = 1;
    }
    if (teproc_1.vlr / 35.3145f > 24.f) {
	pv_1.isd = 1;
    }
    if (teproc_1.vlr / 35.3145f < 2.f) {
	pv_1.isd = 1;
    }
    if (pv_1.xmeas[8] > 175.f) {
	pv_1.isd = 1;
    }
    if (teproc_1.vls / 35.3145f > 12.f) {
	pv_1.isd = 1;
    }
    if (teproc_1.vls / 35.3145f < 1.f) {
	pv_1.isd = 1;
    }
    if (teproc_1.vlc / 35.3145f > 8.f) {
	pv_1.isd = 1;
    }
    if (teproc_1.vlc / 35.3145f < 1.f) {
	pv_1.isd = 1;
    }
    if (*time > 0.f && pv_1.isd == 0) {
	for (i__ = 1; i__ <= 22; ++i__) {
	    tesub6_(&teproc_1.xns[i__ - 1], &xmns);
	    pv_1.xmeas[i__ - 1] += xmns;
/* L6500: */
	}
    }
    xcmp[22] = teproc_1.xst[48] * 100.f;
    xcmp[23] = teproc_1.xst[49] * 100.f;
    xcmp[24] = teproc_1.xst[50] * 100.f;
    xcmp[25] = teproc_1.xst[51] * 100.f;
    xcmp[26] = teproc_1.xst[52] * 100.f;
    xcmp[27] = teproc_1.xst[53] * 100.f;
    xcmp[28] = teproc_1.xst[72] * 100.f;
    xcmp[29] = teproc_1.xst[73] * 100.f;
    xcmp[30] = teproc_1.xst[74] * 100.f;
    xcmp[31] = teproc_1.xst[75] * 100.f;
    xcmp[32] = teproc_1.xst[76] * 100.f;
    xcmp[33] = teproc_1.xst[77] * 100.f;
    xcmp[34] = teproc_1.xst[78] * 100.f;
    xcmp[35] = teproc_1.xst[79] * 100.f;
    xcmp[36] = teproc_1.xst[99] * 100.f;
    xcmp[37] = teproc_1.xst[100] * 100.f;
    xcmp[38] = teproc_1.xst[101] * 100.f;
    xcmp[39] = teproc_1.xst[102] * 100.f;
    xcmp[40] = teproc_1.xst[103] * 100.f;
    if (*time == 0.) {
	for (i__ = 23; i__ <= 41; ++i__) {
	    teproc_1.xdel[i__ - 1] = xcmp[i__ - 1];
	    pv_1.xmeas[i__ - 1] = xcmp[i__ - 1];
/* L7010: */
	}
	teproc_1.tgas = .1f;
	teproc_1.tprod = .25f;
    }
    if (*time >= teproc_1.tgas) {
	for (i__ = 23; i__ <= 36; ++i__) {
	    pv_1.xmeas[i__ - 1] = teproc_1.xdel[i__ - 1];
	    tesub6_(&teproc_1.xns[i__ - 1], &xmns);
	    pv_1.xmeas[i__ - 1] += xmns;
	    teproc_1.xdel[i__ - 1] = xcmp[i__ - 1];
/* L7020: */
	}
	teproc_1.tgas += .1f;
    }
    if (*time >= teproc_1.tprod) {
	for (i__ = 37; i__ <= 41; ++i__) {
	    pv_1.xmeas[i__ - 1] = teproc_1.xdel[i__ - 1];
	    tesub6_(&teproc_1.xns[i__ - 1], &xmns);
	    pv_1.xmeas[i__ - 1] += xmns;
	    teproc_1.xdel[i__ - 1] = xcmp[i__ - 1];
/* L7030: */
	}
	teproc_1.tprod += .25f;
    }
    for (i__ = 1; i__ <= 8; ++i__) {
	yp[i__] = teproc_1.fcm[i__ + 47] - teproc_1.fcm[i__ + 55] + 
		teproc_1.crxr[i__ - 1];
	yp[i__ + 9] = teproc_1.fcm[i__ + 55] - teproc_1.fcm[i__ + 63] - 
		teproc_1.fcm[i__ + 71] - teproc_1.fcm[i__ + 79];
	yp[i__ + 18] = teproc_1.fcm[i__ + 87] - teproc_1.fcm[i__ + 95];
	yp[i__ + 27] = teproc_1.fcm[i__ - 1] + teproc_1.fcm[i__ + 7] + 
		teproc_1.fcm[i__ + 15] + teproc_1.fcm[i__ + 31] + 
		teproc_1.fcm[i__ + 63] - teproc_1.fcm[i__ + 39];
/* L9010: */
    }
    yp[9] = teproc_1.hst[6] * teproc_1.ftm[6] - teproc_1.hst[7] * 
	    teproc_1.ftm[7] + teproc_1.rh + teproc_1.qur;
/* 		Here is the "correct" version of the separator energy balance: */
/* 	YP(18)=HST(8)*FTM(8)- */
/*    .(HST(9)*FTM(9)-cpdh)- */
/*    .HST(10)*FTM(10)- */
/*    .HST(11)*FTM(11)+ */
/*    .QUS */
/* 		Here is the original version */
    yp[18] = teproc_1.hst[7] * teproc_1.ftm[7] - teproc_1.hst[8] * 
	    teproc_1.ftm[8] - teproc_1.hst[9] * teproc_1.ftm[9] - 
	    teproc_1.hst[10] * teproc_1.ftm[10] + teproc_1.qus;
    yp[27] = teproc_1.hst[3] * teproc_1.ftm[3] + teproc_1.hst[10] * 
	    teproc_1.ftm[10] - teproc_1.hst[4] * teproc_1.ftm[4] - 
	    teproc_1.hst[12] * teproc_1.ftm[12] + teproc_1.quc;
    yp[36] = teproc_1.hst[0] * teproc_1.ftm[0] + teproc_1.hst[1] * 
	    teproc_1.ftm[1] + teproc_1.hst[2] * teproc_1.ftm[2] + 
	    teproc_1.hst[4] * teproc_1.ftm[4] + teproc_1.hst[8] * 
	    teproc_1.ftm[8] - teproc_1.hst[5] * teproc_1.ftm[5];
    yp[37] = (teproc_1.fwr * 500.53f * (teproc_1.tcwr - teproc_1.twr) - 
	    teproc_1.qur * 1e6 / 1.8f) / teproc_1.hwr;
    yp[38] = (teproc_1.fws * 500.53f * (teproc_1.tcws - teproc_1.tws) - 
	    teproc_1.qus * 1e6 / 1.8f) / teproc_1.hws;
    teproc_1.ivst[9] = dvec_1.idv[13];
    teproc_1.ivst[10] = dvec_1.idv[14];
    teproc_1.ivst[4] = dvec_1.idv[18];
    teproc_1.ivst[6] = dvec_1.idv[18];
    teproc_1.ivst[7] = dvec_1.idv[18];
    teproc_1.ivst[8] = dvec_1.idv[18];
    for (i__ = 1; i__ <= 12; ++i__) {
	if (*time == 0. || (d__1 = teproc_1.vcv[i__ - 1] - pv_1.xmv[i__ - 1], 
		abs(d__1)) > teproc_1.vst[i__ - 1] * teproc_1.ivst[i__ - 1]) {
	    teproc_1.vcv[i__ - 1] = pv_1.xmv[i__ - 1];
	}
	if (teproc_1.vcv[i__ - 1] < 0.f) {
	    teproc_1.vcv[i__ - 1] = 0.f;
	}
	if (teproc_1.vcv[i__ - 1] > 100.f) {
	    teproc_1.vcv[i__ - 1] = 100.f;
	}
	yp[i__ + 38] = (teproc_1.vcv[i__ - 1] - vpos[i__ - 1]) / 
		teproc_1.vtau[i__ - 1];
/* L9020: */
    }
    if (*time > 0.f && pv_1.isd != 0) {
	i__1 = *nn;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    yp[i__] = 0.f;
/* L9030: */
	}
    }
    return 0;
} /* tefunc_ */


/* ============================================================================= */

/* Subroutine */ int teinit_(integer *nn, doublereal *time, doublereal *yy, 
	doublereal *yp)
{
    static integer i__;
    extern /* Subroutine */ int tefunc_(integer *, doublereal *, doublereal *,
	     doublereal *);


/*       Initialization */

/*         Inputs: */

/*           NN   = Number of differential equations */

/*         Outputs: */

/*           Time = Current time(hrs) */
/*           YY   = Current state values */
/*           YP   = Current derivative values */

    /* Parameter adjustments */
    --yp;
    --yy;

    /* Function Body */
    const_1.xmw[0] = 2.f;
    const_1.xmw[1] = 25.4f;
    const_1.xmw[2] = 28.f;
    const_1.xmw[3] = 32.f;
    const_1.xmw[4] = 46.f;
    const_1.xmw[5] = 48.f;
    const_1.xmw[6] = 62.f;
    const_1.xmw[7] = 76.f;
    const_1.avp[0] = 0.f;
    const_1.avp[1] = 0.f;
    const_1.avp[2] = 0.f;
    const_1.avp[3] = 15.92f;
    const_1.avp[4] = 16.35f;
    const_1.avp[5] = 16.35f;
    const_1.avp[6] = 16.43f;
    const_1.avp[7] = 17.21f;
    const_1.bvp[0] = 0.f;
    const_1.bvp[1] = 0.f;
    const_1.bvp[2] = 0.f;
    const_1.bvp[3] = -1444.f;
    const_1.bvp[4] = -2114.f;
    const_1.bvp[5] = -2114.f;
    const_1.bvp[6] = -2748.f;
    const_1.bvp[7] = -3318.f;
    const_1.cvp[0] = 0.f;
    const_1.cvp[1] = 0.f;
    const_1.cvp[2] = 0.f;
    const_1.cvp[3] = 259.f;
    const_1.cvp[4] = 265.5f;
    const_1.cvp[5] = 265.5f;
    const_1.cvp[6] = 232.9f;
    const_1.cvp[7] = 249.6f;
    const_1.ad[0] = 1.f;
    const_1.ad[1] = 1.f;
    const_1.ad[2] = 1.f;
    const_1.ad[3] = 23.3f;
    const_1.ad[4] = 33.9f;
    const_1.ad[5] = 32.8f;
    const_1.ad[6] = 49.9f;
    const_1.ad[7] = 50.5f;
    const_1.bd[0] = 0.f;
    const_1.bd[1] = 0.f;
    const_1.bd[2] = 0.f;
    const_1.bd[3] = -.07f;
    const_1.bd[4] = -.0957f;
    const_1.bd[5] = -.0995f;
    const_1.bd[6] = -.0191f;
    const_1.bd[7] = -.0541f;
    const_1.cd[0] = 0.f;
    const_1.cd[1] = 0.f;
    const_1.cd[2] = 0.f;
    const_1.cd[3] = -2e-4f;
    const_1.cd[4] = -1.52e-4f;
    const_1.cd[5] = -2.33e-4f;
    const_1.cd[6] = -4.25e-4f;
    const_1.cd[7] = -1.5e-4f;
    const_1.ah[0] = 1e-6;
    const_1.ah[1] = 1e-6;
    const_1.ah[2] = 1e-6;
    const_1.ah[3] = 9.6e-7;
    const_1.ah[4] = 5.73e-7;
    const_1.ah[5] = 6.52e-7;
    const_1.ah[6] = 5.15e-7;
    const_1.ah[7] = 4.71e-7;
    const_1.bh[0] = 0.f;
    const_1.bh[1] = 0.f;
    const_1.bh[2] = 0.f;
    const_1.bh[3] = 8.7e-9;
    const_1.bh[4] = 2.41e-9;
    const_1.bh[5] = 2.18e-9;
    const_1.bh[6] = 5.65e-10;
    const_1.bh[7] = 8.7e-10;
    const_1.ch[0] = 0.f;
    const_1.ch[1] = 0.f;
    const_1.ch[2] = 0.f;
    const_1.ch[3] = 4.81e-11;
    const_1.ch[4] = 1.82e-11;
    const_1.ch[5] = 1.94e-11;
    const_1.ch[6] = 3.82e-12;
    const_1.ch[7] = 2.62e-12;
    const_1.av[0] = 1e-6;
    const_1.av[1] = 1e-6;
    const_1.av[2] = 1e-6;
    const_1.av[3] = 8.67e-5;
    const_1.av[4] = 1.6e-4;
    const_1.av[5] = 1.6e-4;
    const_1.av[6] = 2.25e-4;
    const_1.av[7] = 2.09e-4;
    const_1.ag[0] = 3.411e-6;
    const_1.ag[1] = 3.799e-7;
    const_1.ag[2] = 2.491e-7;
    const_1.ag[3] = 3.567e-7;
    const_1.ag[4] = 3.463e-7;
    const_1.ag[5] = 3.93e-7;
    const_1.ag[6] = 1.7e-7;
    const_1.ag[7] = 1.5e-7;
    const_1.bg[0] = 7.18e-10;
    const_1.bg[1] = 1.08e-9;
    const_1.bg[2] = 1.36e-11;
    const_1.bg[3] = 8.51e-10;
    const_1.bg[4] = 8.96e-10;
    const_1.bg[5] = 1.02e-9;
    const_1.bg[6] = 0.;
    const_1.bg[7] = 0.;
    const_1.cg[0] = 6e-13;
    const_1.cg[1] = -3.98e-13;
    const_1.cg[2] = -3.93e-14;
    const_1.cg[3] = -3.12e-13;
    const_1.cg[4] = -3.27e-13;
    const_1.cg[5] = -3.12e-13;
    const_1.cg[6] = 0.;
    const_1.cg[7] = 0.;
    yy[1] = 10.40491389f;
    yy[2] = 4.363996017f;
    yy[3] = 7.570059737f;
    yy[4] = .4230042431f;
    yy[5] = 24.15513437f;
    yy[6] = 2.942597645f;
    yy[7] = 154.3770655f;
    yy[8] = 159.186596f;
    yy[9] = 2.808522723f;
    yy[10] = 63.75581199f;
    yy[11] = 26.74026066f;
    yy[12] = 46.38532432f;
    yy[13] = .2464521543f;
    yy[14] = 15.20484404f;
    yy[15] = 1.852266172f;
    yy[16] = 52.44639459f;
    yy[17] = 41.20394008f;
    yy[18] = .569931776f;
    yy[19] = .4306056376f;
    yy[20] = .0079906200783;
    yy[21] = .9056036089f;
    yy[22] = .016054258216;
    yy[23] = .7509759687f;
    yy[24] = .088582855955;
    yy[25] = 48.27726193f;
    yy[26] = 39.38459028f;
    yy[27] = .3755297257f;
    yy[28] = 107.7562698f;
    yy[29] = 29.77250546f;
    yy[30] = 88.32481135f;
    yy[31] = 23.03929507f;
    yy[32] = 62.85848794f;
    yy[33] = 5.546318688f;
    yy[34] = 11.92244772f;
    yy[35] = 5.555448243f;
    yy[36] = .9218489762f;
    yy[37] = 94.59927549f;
    yy[38] = 77.29698353f;
    yy[39] = 63.05263039f;
    yy[40] = 53.97970677f;
    yy[41] = 24.64355755f;
    yy[42] = 61.30192144f;
    yy[43] = 22.21f;
    yy[44] = 40.06374673f;
    yy[45] = 38.1003437f;
    yy[46] = 46.53415582f;
    yy[47] = 47.44573456f;
    yy[48] = 41.10581288f;
    yy[49] = 18.11349055f;
    yy[50] = 50.f;
    for (i__ = 1; i__ <= 12; ++i__) {
	pv_2.xmv[i__ - 1] = yy[i__ + 38];
	teproc_1.vcv[i__ - 1] = pv_2.xmv[i__ - 1];
	teproc_1.vst[i__ - 1] = 2.;
	teproc_1.ivst[i__ - 1] = 0;
/* L200: */
    }
    teproc_1.vrng[0] = 400.f;
    teproc_1.vrng[1] = 400.f;
    teproc_1.vrng[2] = 100.f;
    teproc_1.vrng[3] = 1500.f;
    teproc_1.vrng[6] = 1500.f;
    teproc_1.vrng[7] = 1e3f;
    teproc_1.vrng[8] = .03f;
    teproc_1.vrng[9] = 1e3f;
    teproc_1.vrng[10] = 1200.f;
    teproc_1.vtr = 1300.f;
    teproc_1.vts = 3500.f;
    teproc_1.vtc = 156.5f;
    teproc_1.vtv = 5e3f;
    teproc_1.htr[0] = .06899381054;
    teproc_1.htr[1] = .05;
    teproc_1.hwr = 7060.f;
    teproc_1.hws = 11138.f;
    teproc_1.sfr[0] = .995f;
    teproc_1.sfr[1] = .991f;
    teproc_1.sfr[2] = .99f;
    teproc_1.sfr[3] = .916f;
    teproc_1.sfr[4] = .936f;
    teproc_1.sfr[5] = .938f;
    teproc_1.sfr[6] = .058;
    teproc_1.sfr[7] = .0301;
    teproc_1.xst[0] = 0.f;
    teproc_1.xst[1] = 1e-4f;
    teproc_1.xst[2] = 0.f;
    teproc_1.xst[3] = .9999f;
    teproc_1.xst[4] = 0.f;
    teproc_1.xst[5] = 0.f;
    teproc_1.xst[6] = 0.f;
    teproc_1.xst[7] = 0.f;
    teproc_1.tst[0] = 45.f;
    teproc_1.xst[8] = 0.f;
    teproc_1.xst[9] = 0.f;
    teproc_1.xst[10] = 0.f;
    teproc_1.xst[11] = 0.f;
    teproc_1.xst[12] = .9999f;
    teproc_1.xst[13] = 1e-4f;
    teproc_1.xst[14] = 0.f;
    teproc_1.xst[15] = 0.f;
    teproc_1.tst[1] = 45.f;
    teproc_1.xst[16] = .9999f;
    teproc_1.xst[17] = 1e-4f;
    teproc_1.xst[18] = 0.f;
    teproc_1.xst[19] = 0.f;
    teproc_1.xst[20] = 0.f;
    teproc_1.xst[21] = 0.f;
    teproc_1.xst[22] = 0.f;
    teproc_1.xst[23] = 0.f;
    teproc_1.tst[2] = 45.f;
    teproc_1.xst[24] = .485f;
    teproc_1.xst[25] = .005f;
    teproc_1.xst[26] = .51f;
    teproc_1.xst[27] = 0.f;
    teproc_1.xst[28] = 0.f;
    teproc_1.xst[29] = 0.f;
    teproc_1.xst[30] = 0.f;
    teproc_1.xst[31] = 0.f;
    teproc_1.tst[3] = 45.f;
    teproc_1.cpflmx = 280275.f;
    teproc_1.cpprmx = 1.3f;
    teproc_1.vtau[0] = 8.f;
    teproc_1.vtau[1] = 8.f;
    teproc_1.vtau[2] = 6.f;
    teproc_1.vtau[3] = 9.f;
    teproc_1.vtau[4] = 7.f;
    teproc_1.vtau[5] = 5.f;
    teproc_1.vtau[6] = 5.f;
    teproc_1.vtau[7] = 5.f;
    teproc_1.vtau[8] = 120.f;
    teproc_1.vtau[9] = 5.f;
    teproc_1.vtau[10] = 5.f;
    teproc_1.vtau[11] = 5.f;
    for (i__ = 1; i__ <= 12; ++i__) {
	teproc_1.vtau[i__ - 1] /= 3600.f;
/* L300: */
    }
    randsd_1.g = 1431655765.;
    teproc_1.xns[0] = .0012;
    teproc_1.xns[1] = 18.;
    teproc_1.xns[2] = 22.;
    teproc_1.xns[3] = .05;
    teproc_1.xns[4] = .2;
    teproc_1.xns[5] = .21;
    teproc_1.xns[6] = .3;
    teproc_1.xns[7] = .5;
    teproc_1.xns[8] = .01;
    teproc_1.xns[9] = .0017;
    teproc_1.xns[10] = .01;
    teproc_1.xns[11] = 1.;
    teproc_1.xns[12] = .3;
    teproc_1.xns[13] = .125;
    teproc_1.xns[14] = 1.;
    teproc_1.xns[15] = .3;
    teproc_1.xns[16] = .115;
    teproc_1.xns[17] = .01;
    teproc_1.xns[18] = 1.15;
    teproc_1.xns[19] = .2;
    teproc_1.xns[20] = .01;
    teproc_1.xns[21] = .01;
    teproc_1.xns[22] = .25;
    teproc_1.xns[23] = .1;
    teproc_1.xns[24] = .25;
    teproc_1.xns[25] = .1;
    teproc_1.xns[26] = .25;
    teproc_1.xns[27] = .025;
    teproc_1.xns[28] = .25;
    teproc_1.xns[29] = .1;
    teproc_1.xns[30] = .25;
    teproc_1.xns[31] = .1;
    teproc_1.xns[32] = .25;
    teproc_1.xns[33] = .025;
    teproc_1.xns[34] = .05;
    teproc_1.xns[35] = .05;
    teproc_1.xns[36] = .01;
    teproc_1.xns[37] = .01;
    teproc_1.xns[38] = .01;
    teproc_1.xns[39] = .5;
    teproc_1.xns[40] = .5;
    for (i__ = 1; i__ <= 20; ++i__) {
	dvec_1.idv[i__ - 1] = 0;
/* L500: */
    }
    wlk_1.hspan[0] = .2;
    wlk_1.hzero[0] = .5;
    wlk_1.sspan[0] = .03;
    wlk_1.szero[0] = .485;
    wlk_1.spspan[0] = 0.;
    wlk_1.hspan[1] = .7;
    wlk_1.hzero[1] = 1.;
    wlk_1.sspan[1] = .003;
    wlk_1.szero[1] = .005;
    wlk_1.spspan[1] = 0.;
    wlk_1.hspan[2] = .25;
    wlk_1.hzero[2] = .5;
    wlk_1.sspan[2] = 10.;
    wlk_1.szero[2] = 45.;
    wlk_1.spspan[2] = 0.;
    wlk_1.hspan[3] = .7;
    wlk_1.hzero[3] = 1.;
    wlk_1.sspan[3] = 10.;
    wlk_1.szero[3] = 45.;
    wlk_1.spspan[3] = 0.;
    wlk_1.hspan[4] = .15;
    wlk_1.hzero[4] = .25;
    wlk_1.sspan[4] = 10.;
    wlk_1.szero[4] = 35.;
    wlk_1.spspan[4] = 0.;
    wlk_1.hspan[5] = .15;
    wlk_1.hzero[5] = .25;
    wlk_1.sspan[5] = 10.;
    wlk_1.szero[5] = 40.;
    wlk_1.spspan[5] = 0.;
    wlk_1.hspan[6] = 1.;
    wlk_1.hzero[6] = 2.;
    wlk_1.sspan[6] = .25;
    wlk_1.szero[6] = 1.;
    wlk_1.spspan[6] = 0.;
    wlk_1.hspan[7] = 1.;
    wlk_1.hzero[7] = 2.;
    wlk_1.sspan[7] = .25;
    wlk_1.szero[7] = 1.;
    wlk_1.spspan[7] = 0.;
    wlk_1.hspan[8] = .4;
    wlk_1.hzero[8] = .5;
    wlk_1.sspan[8] = .25;
    wlk_1.szero[8] = 0.;
    wlk_1.spspan[8] = 0.;
    wlk_1.hspan[9] = 1.5;
    wlk_1.hzero[9] = 2.;
    wlk_1.sspan[9] = 0.;
    wlk_1.szero[9] = 0.;
    wlk_1.spspan[9] = 0.;
    wlk_1.hspan[10] = 2.;
    wlk_1.hzero[10] = 3.;
    wlk_1.sspan[10] = 0.;
    wlk_1.szero[10] = 0.;
    wlk_1.spspan[10] = 0.;
    wlk_1.hspan[11] = 1.5;
    wlk_1.hzero[11] = 2.;
    wlk_1.sspan[11] = 0.;
    wlk_1.szero[11] = 0.;
    wlk_1.spspan[11] = 0.;
    for (i__ = 1; i__ <= 12; ++i__) {
	wlk_1.tlast[i__ - 1] = 0.;
	wlk_1.tnext[i__ - 1] = .1;
	wlk_1.adist[i__ - 1] = wlk_1.szero[i__ - 1];
	wlk_1.bdist[i__ - 1] = 0.;
	wlk_1.cdist[i__ - 1] = 0.;
	wlk_1.ddist[i__ - 1] = 0.;
/* L550: */
    }
    *time = 0.f;
    tefunc_(nn, time, &yy[1], &yp[1]);
    return 0;
} /* teinit_ */


/* ============================================================================= */

/* Subroutine */ int tesub1_(doublereal *z__, doublereal *t, doublereal *h__, 
	integer *ity)
{
    /* System generated locals */
    doublereal d__1;

    /* Local variables */
    static integer i__;
    static doublereal r__, hi;

    /* Parameter adjustments */
    --z__;

    /* Function Body */
    if (*ity == 0) {
	*h__ = 0.;
	for (i__ = 1; i__ <= 8; ++i__) {
/* Computing 2nd power */
	    d__1 = *t;
	    hi = *t * (const_1.ah[i__ - 1] + const_1.bh[i__ - 1] * *t / 2. + 
		    const_1.ch[i__ - 1] * (d__1 * d__1) / 3.);
	    hi *= 1.8;
	    *h__ += z__[i__] * const_1.xmw[i__ - 1] * hi;
/* L100: */
	}
    } else {
	*h__ = 0.;
	for (i__ = 1; i__ <= 8; ++i__) {
/* Computing 2nd power */
	    d__1 = *t;
	    hi = *t * (const_1.ag[i__ - 1] + const_1.bg[i__ - 1] * *t / 2. + 
		    const_1.cg[i__ - 1] * (d__1 * d__1) / 3.);
	    hi *= 1.8;
	    hi += const_1.av[i__ - 1];
	    *h__ += z__[i__] * const_1.xmw[i__ - 1] * hi;
/* L200: */
	}
    }
    if (*ity == 2) {
	r__ = 3.57696e-6;
	*h__ -= r__ * (*t + 273.15f);
    }
    return 0;
} /* tesub1_ */

/* Subroutine */ int tesub2_(doublereal *z__, doublereal *t, doublereal *h__, 
	integer *ity)
{
    static integer j;
    static doublereal dh, dt, err, tin, htest;
    extern /* Subroutine */ int tesub1_(doublereal *, doublereal *, 
	    doublereal *, integer *), tesub3_(doublereal *, doublereal *, 
	    doublereal *, integer *);

    /* Parameter adjustments */
    --z__;

    /* Function Body */
    tin = *t;
    for (j = 1; j <= 100; ++j) {
	tesub1_(&z__[1], t, &htest, ity);
	err = htest - *h__;
	tesub3_(&z__[1], t, &dh, ity);
	dt = -err / dh;
	*t += dt;
/* L250: */
	if (abs(dt) < 1e-12) {
	    goto L300;
	}
    }
    *t = tin;
L300:
    return 0;
} /* tesub2_ */

/* Subroutine */ int tesub3_(doublereal *z__, doublereal *t, doublereal *dh, 
	integer *ity)
{
    /* System generated locals */
    doublereal d__1;

    /* Local variables */
    static integer i__;
    static doublereal r__, dhi;

    /* Parameter adjustments */
    --z__;

    /* Function Body */
    if (*ity == 0) {
	*dh = 0.;
	for (i__ = 1; i__ <= 8; ++i__) {
/* Computing 2nd power */
	    d__1 = *t;
	    dhi = const_1.ah[i__ - 1] + const_1.bh[i__ - 1] * *t + const_1.ch[
		    i__ - 1] * (d__1 * d__1);
	    dhi *= 1.8;
	    *dh += z__[i__] * const_1.xmw[i__ - 1] * dhi;
/* L100: */
	}
    } else {
	*dh = 0.;
	for (i__ = 1; i__ <= 8; ++i__) {
/* Computing 2nd power */
	    d__1 = *t;
	    dhi = const_1.ag[i__ - 1] + const_1.bg[i__ - 1] * *t + const_1.cg[
		    i__ - 1] * (d__1 * d__1);
	    dhi *= 1.8;
	    *dh += z__[i__] * const_1.xmw[i__ - 1] * dhi;
/* L200: */
	}
    }
    if (*ity == 2) {
	r__ = 3.57696e-6;
	*dh -= r__;
    }
    return 0;
} /* tesub3_ */

/* Subroutine */ int tesub4_(doublereal *x, doublereal *t, doublereal *r__)
{
    static integer i__;
    static doublereal v;

    /* Parameter adjustments */
    --x;

    /* Function Body */
    v = 0.f;
    for (i__ = 1; i__ <= 8; ++i__) {
	v += x[i__] * const_1.xmw[i__ - 1] / (const_1.ad[i__ - 1] + (
		const_1.bd[i__ - 1] + const_1.cd[i__ - 1] * *t) * *t);
/* L10: */
    }
    *r__ = 1.f / v;
    return 0;
} /* tesub4_ */

/* Subroutine */ int tesub5_(doublereal *s, doublereal *sp, doublereal *adist,
	 doublereal *bdist, doublereal *cdist, doublereal *ddist, doublereal *
	tlast, doublereal *tnext, doublereal *hspan, doublereal *hzero, 
	doublereal *sspan, doublereal *szero, doublereal *spspan, integer *
	idvflag)
{
    /* System generated locals */
    doublereal d__1;

    /* Local variables */
    static doublereal h__;
    static integer i__;
    static doublereal s1, s1p;
    extern doublereal tesub7_(integer *);

    i__ = -1;
    h__ = *hspan * tesub7_(&i__) + *hzero;
    s1 = *sspan * tesub7_(&i__) * *idvflag + *szero;
    s1p = *spspan * tesub7_(&i__) * *idvflag;
    *adist = *s;
    *bdist = *sp;
/* Computing 2nd power */
    d__1 = h__;
    *cdist = ((s1 - *s) * 3. - h__ * (s1p + *sp * 2.)) / (d__1 * d__1);
/* Computing 3rd power */
    d__1 = h__;
    *ddist = ((*s - s1) * 2. + h__ * (s1p + *sp)) / (d__1 * (d__1 * d__1));
    *tnext = *tlast + h__;
    return 0;
} /* tesub5_ */

/* Subroutine */ int tesub6_(doublereal *std, doublereal *x)
{
    static integer i__;
    extern doublereal tesub7_(integer *);

    *x = 0.;
    for (i__ = 1; i__ <= 12; ++i__) {
	*x += tesub7_(&i__);
    }
    *x = (*x - 6.) * *std;
    return 0;
} /* tesub6_ */

doublereal tesub7_(integer *i__)
{
    /* System generated locals */
    doublereal ret_val, d__1;

    /* Builtin functions */
    double d_mod(doublereal *, doublereal *);

    d__1 = randsd_1.g * 9228907.;
    randsd_1.g = d_mod(&d__1, &c_b78);
    if (*i__ >= 0) {
	ret_val = randsd_1.g / 4294967296.;
    }
    if (*i__ < 0) {
	ret_val = randsd_1.g * 2. / 4294967296. - 1.;
    }
    return ret_val;
} /* tesub7_ */

doublereal tesub8_(integer *i__, doublereal *t)
{
    /* System generated locals */
    doublereal ret_val;

    /* Local variables */
    static doublereal h__;

    h__ = *t - wlk_1.tlast[*i__ - 1];
    ret_val = wlk_1.adist[*i__ - 1] + h__ * (wlk_1.bdist[*i__ - 1] + h__ * (
	    wlk_1.cdist[*i__ - 1] + h__ * wlk_1.ddist[*i__ - 1]));
    return ret_val;
} /* tesub8_ */

