* Parameters Calculation for WB Infrastructure Indicators
* Bruno Merven November 2022

WBI('C_GVA',A,TC) = QVAX(A,'nat',XC,TC,'2050')*PVAX(A,'nat',XC,TC,'2050');
WBI('C_GVA','total',TC) = sum(ACNT,WBI('C_GVA',ACNT,TC));

WBI('MPSX',INS,TC) = MPSX(INS,XC,TC,'2050');

WBI('TINSX',INS,TC) = TINSX(INS,XC,TC,'2050');

WBI('YIX',INS,TC) = YIX(INS,XC,TC,'2050');

WBI('C_SavingsINS',INS,TC) = YIX(INS,XC,TC,'2050')*(1-TINSX(INS,XC,TC,'2050'))*MPSX(INS,XC,TC,'2050');
WBI('C_SavingsINS','total',TC) = sum(ACNT,WBI('C_SavingsINS',ACNT,TC));

WBI('FSAVX','total',TC) = FSAVX(XC,TC,'2050');

WBI('GSAVX','total',TC) = GSAVX(XC,TC,'2050');

WBI('C_PubDef','total',TC) = GSAVX(XC,TC,'2050')/WBI('C_GVA','total',TC);

WBI('C_TSav','total',TC) = WBI('C_SavingsINS','total',TC)+WBI('FSAVX','total',TC)+WBI('GSAVX','total',TC);

* not this is very slightly different to spreadsheet, in that QINV('s-e') is also included. QINV('s-e') stays constant as we're not full utilising this functionality at the moment
WBI('QINVX',C,TC) = SUM(IT,QINVX(C,IT,XC,TC,'2050'));

WBI('PQX',C,TC) = PQX(C,XC,TC,'2050');

WBI('C_InvVal',C,TC) = PQX(C,XC,TC,'2050')*WBI('QINVX',C,TC);
WBI('C_InvVal','total',TC) = SUM(ACNT,WBI('C_InvVal',ACNT,TC));

WBI('IADJX','total',TC) = IADJX('s-i',XC,TC,'2050');

WBI('C_QINV_IADJ','total',TC) = WBI('C_InvVal','total',TC)/WBI('IADJX','total',TC);

WBI('INVSHRX','total',TC) = INVSHRX(XC,TC,'2050');

WBI('GADJX','total',TC) = GADJX(XC,TC,'2050');

WBI('GOVSHRX','total',TC) = GOVSHRX(XC,TC,'2050');

WBI('GOVGR','total',TC) = GOVGR(XC,TC);

WBI('TRNADJX','total',TC) = TRNADJX(XC,TC,'2050');

WBI('trnsfrx_gov',INS,TC) = trnsfrx(INS,'gov',XC,TC,'2050');
WBI('trnsfrx_row',INS,TC) = trnsfrx(INS,'row',XC,TC,'2050');

WBI('C_NetTrnsGov2Ins','total',TC) = sum(ACNT,WBI('trnsfrx_gov',ACNT,TC))-WBI('trnsfrx_row','gov',TC);

WBI('EGX','total',TC) = EGX(XC,TC,'2050');

WBI('QFSX',F,TC) = QFSX(F,XC,TC,'2050');

WBI('C_QFSlab','total',TC) = sum(FLAB,QFSX(FLAB,XC,TC,'2050'));

WBI('C_BroadUnEmpRate','total',TC)$WBI('P_ActivePop','total',TC) = 1-(WBI('C_QFSlab','total',TC)/WBI('P_ActivePop','total',TC));

WBI('C_LabForcePart','total',TC)$WBI('P_WAgePop','total',TC) = WBI('C_QFSlab','total',TC)/WBI('P_WAgePop','total',TC);

WBI('PalmaRatio','total',TC) = REPORT('OtherEconIndicators','ACTGRP',TC,RUN,'PalmaRatio');

WBI('20-20Ratio','total',TC) = REPORT('OtherEconIndicators','ACTGRP',TC,RUN,'20-20Ratio');

WBI('QVAX',A,TC) = QVAX(A,'nat',XC,TC,'2050');

WBI('PVAX',A,TC) = PVAX(A,'nat',XC,TC,'2050');

WBI('C_GovCons',C,TC) = QGX(C,XC,TC,'2050')*PQX(C,XC,TC,'2050');
WBI('C_GovCons','total',TC) = sum(ACNT,WBI('C_GovCons',ACNT,TC));
