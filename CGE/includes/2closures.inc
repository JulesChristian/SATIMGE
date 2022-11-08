
*Macro closures ---------------------------------

   mps01(INSDNG)  = MPS01SIM(XC,INSDNG);
   tins01(INSDNG) = TINS01SIM(XC,INSDNG);

*Numeraire
  IF(NUMERAIRE(XC) EQ 1,
   CPI.FX      = CPI0;
   DPI.LO     = -INF;
   DPI.UP     = +INF;
   );
  IF(NUMERAIRE(XC) EQ 2,
   DPI.FX     = DPI0;
   CPI.LO     = -INF;
   CPI.UP     = +INF;
   );

*Government
  IF(GOVCLOS(XC) EQ 1,
   TRNADJ.FX  = TRNADJ0;
   TAADJ.FX   = TAADJ0;
   TAPS.FX    = TAPS0;
   TINSADJ.FX = TINSADJ.L + DTAXGR(XC,TC);
   TQADJ.FX   = TQADJ0;
   TQPS.FX    = TQPS0;
   TVAADJ.FX  = TVAADJ0;
   TVAPS.FX   = TVAPS0;
   DTINS.FX   = DTINS0;
   GSAV.LO    = -INF;
   GSAV.UP    = +INF;
   );
  IF(GOVCLOS(XC) EQ 2,
   TRNADJ.FX  = TRNADJ0;
   TAADJ.FX   = TAADJ0;
   TAPS.FX    = TAPS0;
*   TAPS.FX    = TAPSX('BASE',TC,TT);
   TINSADJ.FX = TINSADJ.L + DTAXGR(XC,TC);
   TQADJ.FX   = TQADJ0;
   TQPS.FX    = TQPS0;
   TVAADJ.FX  = TVAADJ0;
   TVAPS.FX   = TVAPS0;
   DTINS.LO   = -INF;
   DTINS.UP   = +INF;
   GSAV.FX    = GSAV.L * (1+GSAVGR(XC,TC));
*   GSAV.FX    = GSAVX('BASE',TC,TT);
   );
  IF(GOVCLOS(XC) EQ 3,
   TRNADJ.FX  = TRNADJ0;
   TAADJ.FX   = TAADJ0;
   TAPS.FX    = TAPS0;
   TINSADJ.LO = -INF;
   TINSADJ.UP = +INF;
   TQADJ.FX   = TQADJ0;
   TQPS.FX    = TQPS0;
   TVAADJ.FX  = TVAADJ0;
   TVAPS.FX   = TVAPS0;
   DTINS.FX   = DTINS.L + DTAXGR(XC,TC);
   GSAV.FX    = GSAV.L * (1+GSAVGR(XC,TC));
*   GSAV.FX    = GSAVX('BASE',TC,TT);
   );
  IF(GOVCLOS(XC) EQ 4,
   TRNADJ.FX  = TRNADJ0;
   TAADJ.FX   = TAADJ0;
   TAPS.LO = -INF;
   TAPS.UP = +INF;
   TINSADJ.FX = TINSADJ.L + DTAXGR(XC,TC);
   TQADJ.FX   = TQADJ0;
   TQPS.FX    = TQPS0;
   TVAADJ.FX  = TVAADJ0;
   TVAPS.FX   = TVAPS0;
   DTINS.FX   = DTINS0;
*   GSAV.FX    = GSAV.L * (1+GSAVGR(XC,TC));
   GSAV.FX    = GSAVX('BASE',TC,TT);
   );
  IF(GOVCLOS(XC) EQ 5,
   TRNADJ.FX  = TRNADJ0;
   TAADJ.LO = -INF;
   TAADJ.UP = +INF;
   TAPS.FX    = TAPS0;
   TINSADJ.FX = TINSADJ.L + DTAXGR(XC,TC);
   TQADJ.FX   = TQADJ0;
   TQPS.FX    = TQPS0;
   TVAADJ.FX  = TVAADJ0;
   TVAPS.FX   = TVAPS0;
   DTINS.FX   = DTINS0;
*   GSAV.FX    = GSAV.L * (1+GSAVGR(XC,TC));
   GSAV.FX    = GSAVX('BASE',TC,TT);
   );

 IF(GOVCLOS(XC) EQ 6,
   TRNADJ.FX  = TRNADJ0;
   TAPS.FX    = TAPS0;
   TAADJ.FX   = TAADJ0;
   TINSADJ.FX = TINSADJ.L + DTAXGR(XC,TC);
   TQADJ.FX   = TQADJ0;
   TQPS.LO    = -INF;
   TQPS.UP    = +INF;
   TVAADJ.FX  = TVAADJ0;
   TVAPS.FX   = TVAPS0;
   DTINS.FX   = DTINS0;
   GSAV.FX    = GSAV.L * (1+GSAVGR(XC,TC));
*   GSAV.FX    = GSAVX('BASE',TC,TT);
   );
  IF(GOVCLOS(XC) EQ 7,
   TRNADJ.FX  = TRNADJ0;
   TAPS.FX    = TAPS0;
   TAADJ.FX   = TAADJ0;
   TINSADJ.FX = TINSADJ.L + DTAXGR(XC,TC);
   TQADJ.LO   = -INF;
   TQADJ.UP   = +INF;
   TQPS.FX    = TQPS0;
   TVAADJ.FX  = TVAADJ0;
   TVAPS.FX   = TVAPS0;
   DTINS.FX   = DTINS0;
   GSAV.FX    = GSAV.L * (1+GSAVGR(XC,TC));
*   GSAV.FX    = GSAVX('BASE',TC,TT);
   );
 IF(GOVCLOS(XC) EQ 8,
   TRNADJ.FX  = TRNADJ0;
   TAPS.FX    = TAPS0;
   TAADJ.FX   = TAADJ0;
   TINSADJ.FX = TINSADJ.L + DTAXGR(XC,TC);
   TQADJ.FX   = TQADJ0;
   TQADJ.FX   = TQADJ0;
   TQPS.FX    = TQPS0;
   TVAPS.LO   = -INF;
   TVAPS.UP   = +INF;
   DTINS.FX   = DTINS0;
*   GSAV.FX    = GSAV.L * (1+GSAVGR(XC,TC));
   GSAV.FX    = GSAVX('BASE',TC,TT);
   );
  IF(GOVCLOS(XC) EQ 9,
   TRNADJ.FX  = TRNADJ0;
   TAPS.FX    = TAPS0;
   TAADJ.FX   = TAADJ0;
   TINSADJ.FX = TINSADJ.L + DTAXGR(XC,TC);
   TQADJ.FX  = TQADJ0;
   TQPS.FX    = TQPS0;
   TVAADJ.LO   = -INF;
   TVAADJ.UP   = +INF;
   TVAPS.FX   = TVAPS0;
   DTINS.FX   = DTINS0;
*   GSAV.FX    = GSAV.L * (1+GSAVGR(XC,TC));
   GSAV.FX    = GSAVX('BASE',TC,TT);
   );

  IF(GOVCLOS(XC) EQ 10,
   TRNADJ.LO  = -INF;
   TRNADJ.UP  = +INF;
   TAPS.FX    = TAPS0;
   TAADJ.FX   = TAADJ0;
   TINSADJ.FX = TINSADJ.L + DTAXGR(XC,TC);
   TQADJ.FX  = TQADJ0;
   TQPS.FX    = TQPS0;
   TVAADJ.FX  = TVAADJ0;
   TVAPS.FX   = TVAPS0;
   DTINS.FX   = DTINS0;
*   GSAV.FX    = GSAV.L * (1+GSAVGR(XC,TC));
   GSAV.FX    = GSAVX('BASE',TC,TT);
   );

* [JS 2022_11_03:] I created this new (11th GOV) closure rule for the purpose of analysing government budget impacts of SDG Beyond the Gap investments.
* Be ware, compared to for example GOVCLOS eq 1 there is one endogenous variable more here, while there is one less in the New (6th) S-I closure below:

IF(GOVCLOS(XC) EQ 11,
* Government transfers scaling factor set to 0 for TRNADJ0 = 0
   TRNADJ.FX  = TRNADJ0;
* Activity tax scaling factor
   TAADJ.FX   = TAADJ0;
* TAPS is point change in activity tax rate, set at 0 (TAPS0 = 0 in 1model.inc )
   TAPS.FX    = TAPS0;
* [JS, 2022_03_11: I replaced the following on direct tax rate adjustment by a new parameter DTAXDYN, declared in 2simulation.inc]
* TINSADJ.FX = TINSADJ.L + DTAXGR(XC,TC);
* TINSADJ.FX = DTAXDYN(XC,TC);
  TINSADJ.LO = -INF;
  TINSADJ.UP = +INF;
  TQADJ.FX   = TQADJ0;
  TQPS.FX    = TQPS0;
  TVAADJ.FX  = TVAADJ0;
  TVAPS.FX   = TVAPS0;
  DTINS.FX   = DTINS0;
  GSAV.LO    = -INF;
  GSAV.UP    = +INF;
  );

*Current account
  IF(ROWCLOS(XC) EQ 1,
   FBOR.FX   = FBOR.L;
   FSAV.FX   = FSAV.L * (1+FSAVGR(XC,TC)+FSAVGRSHK(XC,TC));
   EXR.LO    = -INF;
   EXR.UP    = +INF;
   );
  IF(ROWCLOS(XC) EQ 2,
   FBOR.FX   = FBOR.L;
   EXR.FX    = EXR0;
   FSAV.LO   = -INF;
   FSAV.UP   = +INF;
   );

*SAVINGS AND INVESTMENT and Govt expenditure closure
  IF(SICLOS(XC) EQ 1,
   MPSADJ.FX = MPSADJ.L * (1+MPSGR(XC,TC));
   DMPS.LO   = -INF;
   DMPS.UP   = +INF;
   IADJ.FX(IT) = IADJ.L(IT);
   INVSHR.LO = -INF;
   INVSHR.UP = +INF;
   GADJ.FX   = GADJ.L * (1+GOVGR(XC,TC));
   GOVSHR.LO = -INF;
   GOVSHR.UP = +INF;
   );
  IF(SICLOS(XC) EQ 2,
   MPSADJ.LO = -INF;
   MPSADJ.UP = +INF;
   DMPS.FX   = DMPS.L + MPSGR(XC,TC);
   IADJ.FX(IT) = IADJ.L(IT);
   INVSHR.LO = -INF;
   INVSHR.UP = +INF;
   GADJ.FX   = GADJ.L * (1+GOVGR(XC,TC));
   GOVSHR.LO = -INF;
   GOVSHR.UP = +INF;
   );
  IF(SICLOS(XC) EQ 3,
   MPSADJ.FX = (1+MPSADJ.L) * (1+MPSGR(XC,TC)) - 1;
   DMPS.FX   = DMPS0;
   IADJ.FX('S-E') = IADJSESIM(XC,TC);
   IADJ.LO('S-I') = -INF;
   IADJ.UP('S-I') = +INF;
   INVSHR.LO = -INF;
   INVSHR.UP = +INF;
   GADJ.FX   = GADJ.L * (1+GOVGR(XC,TC));
   GOVSHR.LO = -INF;
   GOVSHR.UP = +INF;
   );
  IF(SICLOS(XC) EQ 4,
   MPSADJ.FX = MPSADJ.L * (1+MPSGR(XC,TC));
   DMPS.LO   = -INF;
   DMPS.UP   = +INF;
   IADJ.FX('S-E') = IADJSESIM(XC,TC);
   IADJ.LO('S-I') = -INF;
   IADJ.UP('S-I') = +INF;
   INVSHR.FX = INVSHR0;
   GADJ.LO   = -INF;
   GADJ.UP   = +INF;
   GOVSHR.FX = GOVSHR0;
   );
  IF(SICLOS(XC) EQ 5,
   MPSADJ.LO = -INF;
   MPSADJ.UP = +INF;
   DMPS.FX   = DMPS.L + MPSGR(XC,TC);
   IADJ.FX('S-E') = IADJSESIM(XC,TC);
   IADJ.LO('S-I') = -INF;
   IADJ.UP('S-I') = +INF;
   INVSHR.FX = INVSHR0;
   GADJ.LO   = -INF;
   GADJ.UP   = +INF;
   GOVSHR.FX = GOVSHR0;
   );
* [JS 2022_11_03:] I created this new (6th S-I) closure rule for the purpose of analysing government budget impacts of SDG Beyond the Gap investments.
* Be ware, compared to for example SICLOS eq 3 there is one endogenous variable less here, while there is one more in the New (11th) Gov closure above:
 IF(SICLOS(XC) EQ 6,
*   MPSADJ.FX = (1+MPSADJ.L) * (1+MPSGR(XC,TC)) - 1;
   MPSADJ.FX = MPSADJ0;
   DMPS.FX   = DMPS0;
* IADJ.FX('S-E') = IADJSESIM(XC,TC);
* IADJ.LO('S-I') = -INF;
* IADJ.UP('S-I') = +INF;
* [JS 2022_11_03:] The following line (IADJ.FX) fixes investment to the IADJDYN trajectory, see 2closure.inc and 2closure_loop.inc .
  IADJ.FX(IT) = IADJ.L(IT);
  INVSHR.LO = -INF;
  INVSHR.UP = +INF;
* [JS 2022_11_03:] Just a reminder to check how GADJ and GOVGR work.
  GADJ.FX   = GADJ.L * (1+GOVGR(XC,TC));
  GOVSHR.LO = -INF;
  GOVSHR.UP = +INF;
  );

*Factor closures ------------------

  LOOP(F$FMOBFE(XC,F),
   WFDIST.FX(F,A,RD)$ANFX(A,RD) = WFDIST0(F,A,RD);
   QFS.FX(F)$(NOT FLS(F)) = QFS.L(F);
   WF.LO(F) = -INF;
   WF.UP(F) = +INF;
   QF.LO(F,A,RD)$(ANFX(A,RD) AND QF0(F,A,RD)) = -INF;
   QF.UP(F,A,RD)$(ANFX(A,RD) AND QF0(F,A,RD)) = +INF;
*  Upward-sloping curves (shift QFS0 and leave QFS flexible)
   QFS0(F)$FLS(F) = SUM((RD,A), QF.L(F,A,RD)*COVID(XC,TC)*LABGRSHK(F,XC,TC));
   QFS.LO(F)$FLS(F) = -INF;
   QFS.UP(F)$FLS(F) = +INF;
  );

  LOOP(F$FACTFE(XC,F),
   WF.FX(F)   = WF.L(F);
   QF.FX(F,A,RD) = QF.L(F,A,RD);
   WFDIST.LO(F,A,RD)$QF0(F,A,RD) = -INF;
   WFDIST.UP(F,A,RD)$QF0(F,A,RD) = +INF;
   QFS.L(F)  = SUM((A,RD),QF.L(F,A,RD));
   QFS.LO(F) = -INF;
   QFS.UP(F) = +INF;
  );

  LOOP(F$FMOBUE(XC,F),
   WFDIST.FX(F,A,RD) = WFDIST0(F,A,RD);
   WF.FX(F) = WF.L(F) * (1+WGR(F,XC,TC));
   QF.LO(F,A,RD)$QF0(F,A,RD) = -INF;
   QF.UP(F,A,RD)$QF0(F,A,RD) = +INF;
   QFS.LO(F) = -INF;
   QFS.UP(F) = +INF;
  );

 LOOP(A$FL(A),
   QF.FX(FLND,A,RD) = QF.L(FLND,A,RD);
   WFDIST.LO(FLND,A,RD)$QF0(FLND,A,RD) = -INF;
   WFDIST.UP(FLND,A,RD)$QF0(FLND,A,RD) = +INF;
 );

*For nested factors, no changes are to be made to the closure options of the
*aggregate factors essentially these factors do not exist and are entirely flexible
*in every variable thta typically appears in the factor closure (wf, wfdist, qf, qfs).
*these aggregate factors are also not allowed to have upward sloping labor supply
*curves - all such closures apply only to the lowest nesting (or actual factors)
  LOOP(F$FA(F),
   QFS.LO(F)  = -INF ;
   QFS.UP(F)  = +INF ;
   WF.LO(F)   = -INF;
   WF.UP(F)   = +INF;
   QF.LO(F,A,RD)$QF0(F,A,RD) = -INF;
   QF.UP(F,A,RD)$QF0(F,A,RD) = +INF;
   WFDIST.LO(F,A,RD) = -INF;
   WFDIST.UP(F,A,RD) = +INF;
  );




