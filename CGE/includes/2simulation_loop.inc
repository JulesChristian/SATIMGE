
*Loop over simulations
LOOP(XC$MRUNX(RUN,XC),

execute_load "dem_start.gdx" GVA_FS POP YHE TFHPOP MFHHT QA_FS QD_FS;
execute_unload "drivers.gdx" GVA_FS POP YHE TFHPOP MFHHT PAMS_RUN QA_FS QD_FS;

* Loop over TIMES model solve periods
  LOOP(TT$(NOT fail),

    TABAR(A,RD)=TA0(A,RD);

*Identify factors with upward-sloping supply curves
    FLS(F) = NO;
    FLS(F)$(CLOSURES(F,XC) EQ 4) = YES;

*Initialize CGE model parameters and variables
$include cge\includes\2varinit.inc


    if(SIM_SATIM(RUN) eq 1,

* Write Drivers to DMD_PROJ workbook
         execute 'gdxxrw.exe i=drivers.gdx o=.\SATIM\DataSpreadsheets\DMD_PRJ.xlsx index=index_G2E!a6';
* Read resulting Demand from DMD_PROJ workbook
         execute 'gdxxrw.exe i=.\SATIM\DataSpreadsheets\DMD_PRJ.xlsx o=EnergyDemand.gdx index=index_E2G!a6';
         execute_load "EnergyDemand.gdx" SIM_DEMX;
* Need to Add Occupancy and Freight loading per vehicle category to calculate pkm and tkm

* Adjust Transport Demand
*  LOOP(TC$(ORD(TC) gt 6),
*        SIM_DEMX(TPPR,TC)$(SUM(H,QHX('cprtr',H,XC,TC-1,TT-1))) = SIM_DEMX(TPPR,TC-1)*SUM(H,QHX('cprtr',H,XC,TC,TT-1))/SUM(H,QHX('cprtr',H,XC,TC-1,TT-1));
*);
* LOOP(TC,


* Write Demand DDS File
         PUT  SIM_DEM_FILE;
         SIM_DEM_FILE.pc = 2;
         SIM_DEM_FILE.nd = 13;
         SIM_DEM_FILE.ap = 0;

         PUT 'PARAMETER ACOM_PROJ /' /;

         LOOP((DEM1,TC),
                 EFVAL = SIM_DEMX(DEM1,TC);
                 if(EFVAL,
                         PUT "REGION1.", DEM1.TL, ".", TC.TL, EFVAL /;
                 else
                         PUT "REGION1.", DEM1.TL, ".", TC.TL, "eps" /;

                 );
         );
         PUT "/;"/;

*Write Cumulative CO2 CAP bound
         PUT 'PARAMETER ACOMCUMNET /' /;
         EFVAL = SIM_CO2CUMUL(RUN)*1000000;
         if(EFVAL,
                 PUT "REGION1.CO2EQSB.2021.2050.'UP'  ", EFVAL /;
         );
         PUTCLOSE "/;";

         PUT  ShowRunNumber;
         RUNTIMES2.pc = 2;
         RUNTIMES2.nd = 5;
         RUNTIMES2.ap = 0;
         PUTCLOSE "";


$include SATIM\includes\2runTIMES.inc
  );
* if(SIM_SATIM(RUN) eq 1,
*$exit


$include SATIM\includes\2TIMESReport.inc



 EPRICE(T1,TT)=PQ0('CELEC');

*b apply growth rate to calculate EPRICE
 loop(TC,
   if(not T1(TC),
     EPRICE(TC,TT) = EPRICE(TC-1,TT) * (1+ETPRICEGR(TC));
   );
 );

 HCAP(XC,TC,TT) = Flow_out_FSFuels('hydr','chydr',TC);


* make acoal output leontief - controlled by SATIM - see theta calcs below
 ACET2('acoal') = NO;
* can't get apetr leontief to work yet.
 ACET2('apetr') = YES;

* do reverse mapping for industry sectors (excludes power and petroleum)
 ICAGR(CXEP,A,TC) = SUM(FSXEP$MFSA(FSXEP,A),ICAFSGR(CXEP,FSXEP,TC));

* Improvements across all intermediate inputs to match learning in SATIM.
 ICAGR(C,'afcel',TC) = 0.971;
 ICAGR(C,'aelct',TC) = 0.957;

 IFAGR(F,A,TC) = SUM(FS$MFSA(FS,A),IFAFSGR(F,FS,TC));
*IFAGR(F,A,TC) = 1;

*BM correction on power sector from freight demand for moving coal
 ICAGR('cftrp-l','aelec',TC) = ICAGR('ccoal-low','aelec',TC);

*tmp fix until EB is improved
 ica('celec','apgm','NAT') = 0.5*ica0('celec','apgm','NAT');
 ica('celec','altrp-f','NAT') = 0.72*ica0('celec','altrp-f','NAT');
 ica('ccoal-hgh','amore','NAT') = 0.3*ica0('ccoal-hgh','amore','NAT');


*   Loop over CGE model solve periods
 LOOP(TC$(NOT fail),

*fh 71218
*bm+fh 201118 removed - already in 2runtimes.inc
  PAFXCAP(XC,TC,TT)$(EFX(TT,TC) AND ifa('FCAP','APETR','nat')) = ifa('FCAP','APETR','nat')*iva('APETR','nat')*sum(C,FLOW_OUT_FSFuels('petr',C,TC));

  IF(NOT T1(TC),

*       ENERGY: Carbon tax rate
*        tco2d = SIM_CO2PRICE(RUN,TC)/1000;
*        tco2e = SIM_CO2PRICE(RUN,TC)/1000;
*        tco2m = SIM_CO2PRICE(RUN,TC)/1000;

*       ENERGY: Retaliatory carbon tax on RSA exports
    rtco2e = RTCO2ESIM(XC,TC);

*       ENERGY: Carbon content cofficient
$include cge\includes\2carbon.inc
* 2carbon.inc needs to be updated to reflect non combustion emissions
*        co2c(C)    = CO2CSIM(C,XC,TC);
*        co2e(C,RW) = CO2ESIM(C,RW,XC,TC);
*        co2m(C,RW) = CO2MSIM(C,RW,XC,TC);
*        co2e(C,RW) = 0;
*        co2m(C,RW) = 0;

*FH
    tabar(CTA,'NAT')=tabar(CTA,'NAT')+CTA_EMRR(CTA);

*       Long term TFP growth
    alphava(A,RD) = alphava(A,RD) * (1+TFPGR(A,RD,XC,TC));

*FH  removed inclusion by SG causing elec employment to explode
*        leova(A,RD) = leova(A,RD) * (1+TFPGR(A,RD,XC,TC));

*fh CHANGED TO ABOVE
*        alphava(A,RD) = alphava(A,RD) * (1+SECGR(A,RD,XC,TC)/100);
*        leova(A,RD) = leova(A,RD) * (1+SECGR(A,RD,XC,TC)/100);

*       Long term factor-specific productivity growth
    fprd(F,A,RD)$(NOT AFX(A,RD)) = fprd(F,A,RD) * (1+FPRDGR(F,XC,TC));

*       World price changes - standard CGE input
* setting international prices for hydrogen, celct and cfcel to track projected learning - this should be linked to a central template
* we should be doing the same for coal, oil, gas, and other commodities such as ciron, cement, etc...
    PWGR('celct',RUN,TC)$(ord(TC) gt 9) = -0.0326;
    PWGR('cfcel',RUN,TC)$(ord(TC) gt 9) = -0.0191;
    PWGR('chydr',RUN,TC)$(ord(TC) gt 9) = -0.015;


    pwebar(C,RW) = pwebar(C,RW) * (1+PWGR(C,RUN,TC));
    pwmbar(C,RW) = pwmbar(C,RW) * (1+PWGR(C,RUN,TC));
*fh 211218
    pwmbar('CVEHI','REST') = pwmbar('CVEHI','REST');
*        pwebar(C,RW) = pwebar(C,RW) * (1+PWEGR(C,XC,TC));
*        pwmbar(C,RW) = pwmbar(C,RW) * (1+PWMGR(C,XC,TC));

    rhova(A,RD) = rhova_coal(A,RD,XC,TC);

*       Population growth
    hpop(H) = hpop(H) * (1+POPGR(H,XC,TC));
$ontext
*fh: link population growth to average annual satim growth by group
        hpop('hhd-0' ) = hpop('hhd-0' ) * (1+(((tfhpop_x('hhd-le','2050')+tfhpop_x('hhd-ln','2050'))/(tfhpop_x('hhd-le','2012')+tfhpop_x('hhd-ln','2012')))**(1/(2050-2012))-1));
        hpop('hhd-1' ) = hpop('hhd-1' ) * (1+(((tfhpop_x('hhd-le','2050')+tfhpop_x('hhd-ln','2050'))/(tfhpop_x('hhd-le','2012')+tfhpop_x('hhd-ln','2012')))**(1/(2050-2012))-1));
        hpop('hhd-2' ) = hpop('hhd-2' ) * (1+(((tfhpop_x('hhd-le','2050')+tfhpop_x('hhd-ln','2050'))/(tfhpop_x('hhd-le','2012')+tfhpop_x('hhd-ln','2012')))**(1/(2050-2012))-1));
        hpop('hhd-3' ) = hpop('hhd-3' ) * (1+(((tfhpop_x('hhd-le','2050')+tfhpop_x('hhd-ln','2050'))/(tfhpop_x('hhd-le','2012')+tfhpop_x('hhd-ln','2012')))**(1/(2050-2012))-1));
        hpop('hhd-4' ) = hpop('hhd-4' ) * (1+(((tfhpop_x('hhd-me','2050')+tfhpop_x('hhd-mn','2050'))/(tfhpop_x('hhd-me','2012')+tfhpop_x('hhd-mn','2012')))**(1/(2050-2012))-1));
        hpop('hhd-5' ) = hpop('hhd-5' ) * (1+(((tfhpop_x('hhd-me','2050')+tfhpop_x('hhd-mn','2050'))/(tfhpop_x('hhd-me','2012')+tfhpop_x('hhd-mn','2012')))**(1/(2050-2012))-1));
        hpop('hhd-6' ) = hpop('hhd-6' ) * (1+(((tfhpop_x('hhd-me','2050')+tfhpop_x('hhd-mn','2050'))/(tfhpop_x('hhd-me','2012')+tfhpop_x('hhd-mn','2012')))**(1/(2050-2012))-1));
        hpop('hhd-7' ) = hpop('hhd-7' ) * (1+(((tfhpop_x('hhd-me','2050')+tfhpop_x('hhd-mn','2050'))/(tfhpop_x('hhd-me','2012')+tfhpop_x('hhd-mn','2012')))**(1/(2050-2012))-1));
        hpop('hhd-8' ) = hpop('hhd-8' ) * (1+(((tfhpop_x('hhd-he','2050')+tfhpop_x('hhd-hn','2050'))/(tfhpop_x('hhd-he','2012')+tfhpop_x('hhd-hn','2012')))**(1/(2050-2012))-1));
        hpop(hhd9)     = hpop(hhd9)     * (1+(((tfhpop_x('hhd-he','2050')+tfhpop_x('hhd-hn','2050'))/(tfhpop_x('hhd-he','2012')+tfhpop_x('hhd-hn','2012')))**(1/(2050-2012))-1));
$offtext

* phase out elc price differential over time - this should only apply to industry!!! - need to fix !!!
    IF(ord(TC) lt 8,
         tui(C,A,RD) = tui(C,A,RD)*.5;
    );

*       CA driven by previous year gdp growth (see bottom of TC loop)
    trnsfr(INS,ACNT) = trnsfr(INS,ACNT) * (1+TRNSFRGR(INS,ACNT,XC,TC));

*bm make ratio of coal outputs exogenous and control from SATIM outputs
    theta('acoal','ccoal-low') = COAL_FLOW(RUN,TC,'ccoal-low')/sum(C,COAL_FLOW(RUN,TC,C))*(1-(theta('acoal','cmore')+theta('acoal','cmine')));
    theta('acoal','ccoal-hgh') = COAL_FLOW(RUN,TC,'ccoal-hgh')/sum(C,COAL_FLOW(RUN,TC,C))*(1-(theta('acoal','cmore')+theta('acoal','cmine')));

* shrinking production of cmore and cmine out of coal
    theta('acoal','cmore') = theta('acoal','cmore')*0.95;
    theta('acoal','cmine') = theta('acoal','cmine')*0.95;

* adjust ica and ifa for acoal based on previous pacoal and new theta
    pacoalx(TC,RUN) = sum(CCOAL,theta('acoal',CCOAL)*pq0(CCOAL))/sum(CCOAL,theta('acoal',CCOAL));
*  ICAGR(C,'acoal',TC)$pacoalx(TC-1,RUN) = pacoalx(TC,RUN)/pacoalx(TC-1,RUN);
*  IFAGR(F,'acoal',TC)$pacoalx(TC-1,RUN) = pacoalx(TC,RUN)/pacoalx(TC-1,RUN);



* make adjustments to ica

    ica('chydr','aelec','NAT')$(EIntensity_FS('elec','chydr',TC-1) LT 0.0001 and EIntensity_FS('elec','chydr',TC) GT 0.0001)
                     = EIntensity_FS('elec','chydr',TC);
    ica('chydr',A,'NAT')$(SUM(FSXEP$MFSA(FSXEP,A),EIntensity_FS(FSXEP,'chydr',TC-1)) LT 0.0001 and SUM(FSXEP$MFSA(FSXEP,A),EIntensity_FS(FSXEP,'chydr',TC)) GT 0.0001 AND ica('chydr',A,'NAT'))
                         = SUM(FSXEP$MFSA(FSXEP,A),EIntensity_FS(FSXEP,'chydr',TC));
* tmp fix until QA approach is done properly
    ica('chydr',A,'NAT')$(SUM(FSXEP$MFSA(FSXEP,A),EIntensity_FS(FSXEP,'chydr',TC-1)) LT 0.0001 and SUM(FSXEP$MFSA(FSXEP,A),EIntensity_FS(FSXEP,'chydr',TC)) GT 0.0001 AND ica('chydr',A,'NAT'))
                         = ica('chydr',A,'NAT') * QVA.L(A,'nat')/QA.L(A);

* calculate ica change for private passenger. This needs to be generalised to other household end-uses
    EHIntensity(C,'aprtr',TC) = FLOW_IN_FSFuels('tppr',C,TC)/sum(TPPR,COM_PROJ('REGION1',TC,TPPR));

*BM 190320: SETTING STARTING POINT FOR ICA private transport
    ica('celec','aprtr','NAT')$(EHIntensity('celec','aprtr',TC-1) LT 0.0001 and EHIntensity('celec','aprtr',TC) GT 0.00001)
                         = EHIntensity('celec','aprtr',TC)/EHIntensity('cpetr_p','aprtr',TC)*ica('cpetr_p','aprtr','NAT');

    icagr(C,'aprtr',TC)$(EHIntensity(C,'aprtr',TC-1) gt 0.0001) = EHIntensity(C,'aprtr',TC)/EHIntensity(C,'aprtr',TC-1);

*!!!!
*ICAGR(C,A,TC)=1;

    ica(C,A,'NAT')$ICAGR(C,A,TC) = ica(C,A,'NAT')*ICAGR(C,A,TC);

* Other fixes to ica

* acoal
* fade coal own use to match Energy model
    ica('ccoal-low','acoal','NAT') = ica('ccoal-low','acoal','NAT')*0.5;
* apetr
    ica('ccoil','apetr','NAT') = max(0.07,ica('ccoil','apetr','NAT'));



* tmp fix on pp until fixed on SATIM side
    ica('celec','apapr','NAT') = ica0('celec','apapr','NAT');

* Hydrogen was calibrated on cbchm but does not need those inputs so taking them out
    ica('cammo','ahydr','NAT') = 0.000001;
    ica('cbchm','ahydr','NAT')$(ORD(TC) gt 20) = 0.001;
    ica('cochm','ahydr','NAT')$(ORD(TC) gt 20) = 0.001;

    Ica('celec','AHYDR','nat')$(SUM(A,ICA('CHYDR',A,'nat')) GT 0.001) = EIntensity_FS('hydr','celec',TC);
    Ica('cngas','AHYDR','nat')$(SUM(A,ICA('CHYDR',A,'nat')) GT 0.001 and EIntensity_FS('hydr','cngas',TC)) = EIntensity_FS('hydr','cngas',TC);

* adjustments to hydrogen (set empirically)
    ica('chydr','airon','NAT')$(EIntensity_FS('is','chydr',TC) gt 0.001) = EIntensity_FS('is','chydr',TC)*.034;

    ica('chydr','apetr','NAT')$(EIntensity_FS('petr','chydr',TC) gt 0.001) = EIntensity_FS('petr','chydr',TC);

    ica('chydr','aammo','NAT')$(EIntensity_FS('am','chydr',TC) gt 0.001) = EIntensity_FS('am','chydr',TC)*0.92;

* setting electrolyser input (empirically) - this could be made more elegant later
    ica('celct','ahydr','NAT')$(ord(TC) gt 5) = max(0.04,ica('celct','ahydr','NAT') *0.1);
* 19 is the base year price of electrolysers
    ica('celct','ahydr','NAT')$(TVNCAP(TC,'UELEP1HGN') gt 0.15) = max(0.04,TVNCAP(TC,'UELEP1HGN')*19/FLOW_OUT_FSFuels('hydr','chydr',TC));

* setting fcell input - this could be made more elegant later
* 3.3 is the base year price of electrolysers
    ica('cfcel','avehi','NAT')$(TVCAP(TC,'HFCP-TRAHCV-N') gt 0.15) = TVCAP(TC,'HFCP-TRAHCV-N')*3.3/QA.L('avehi');

* Ammonia sector calibrated on cbchm
    ica('cammo','aammo','NAT') = 0.28;
    ica('cbchm','aammo','NAT') = 0.01;
    ica('cochm','aammo','NAT') = 0.01;
    ica('celec','aammo','NAT') = 0.005;

* tweaks to iron and steel sector
    ica('cpgm','airon','NAT') = (ica0('cpgm','airon','NAT') * COM_PROJ('REGION1',TC,'IISCRS') + ica0('cpgm','airon','NAT')* 0.9 * VARACT('REGION1',TC,'PEXGDRI'))
                              /(COM_PROJ('REGION1',TC,'IISCRS') + VARACT('REGION1',TC,'PEXGDRI'));

    ica('cnfrm','airon','NAT') = (ica0('cnfrm','airon','NAT') * COM_PROJ('REGION1',TC,'IISCRS') + 0.0001 * VARACT('REGION1',TC,'PEXGDRI'))
                              /(COM_PROJ('REGION1',TC,'IISCRS') + VARACT('REGION1',TC,'PEXGDRI'));

    ica('ciron','airon','NAT') = (ica0('ciron','airon','NAT') * COM_PROJ('REGION1',TC,'IISCRS') + 0.0001 * VARACT('REGION1',TC,'PEXGDRI'))
                              /(COM_PROJ('REGION1',TC,'IISCRS') + VARACT('REGION1',TC,'PEXGDRI'));


*bm/sr/fh added this to save model from crashing from demand for ccoal-low disapearing completely.
    ica('ccoal-low',A,'NAT')$ica0('ccoal-low',A,'NAT') = max(ica('ccoal-low',A,'NAT'),0.0004);
    ica('ccoal-low','ahydr','NAT') = 0.00000001;


    QA.L('ahydr')$(HCAP(XC,TC,TT) gt 0.00001) = FLOW_OUT_FSFuels('hydr','chydr',TC);

*fh 12/11/19: changing from LES to CD
$ontext
* for Households
  gammamgr(C,H,TC,TT) = SUM(FH$MFHHT(FH,H,TC),FLOW_IN_FHFuelsGR(FH,C,TC));

  gammam(C,H)$gammamgr(C,H,TC,TT) = gammam(C,H)*gammamgr(C,H,TC,TT);

*bm work in progress          gammam(C,H)$(gammagr(C,H,TC) and not(gammam(C,H)) =

*);
$offtext

*  change to IFA

    ifa(F,A,'nat') = ifa(F,A,'nat')*IFAGR(F,A,TC);

*       ENERGY: Set electricity sector capital growth rate equal to TIMES results
*       CA: 2capital.inc modified to use EINVCOST
*       CA: below is modified to determine energy capital necessary given capacity data from TIMES
*       CA; below we do not worry about units of capital as we calculate growth rates
*       CA: note that below accommodates evluation in ifa and iva if imposed

* Exogenous sectors
*CA  slow electricity capital growth when utilization rate is low outside of TIMES periods

* Electrolyser sector - supply of electrolysers is linked to electrolyser exports and PEM electrolyser capacity in SATIM (ELCTCAP)
    AFX('AELCT',RD) = YES;
    AFLEO('AELCT')$(ELCTCAP(TC,RUN) GT 0.015) = YES;
    AFXGR('AELCT','NAT',XC,TC)$(ELCTCAP(TC-1,RUN) GT 0.015) = ELCTCAP(TC,RUN)/ELCTCAP(TC-1,RUN)-1;

* this is done to catch sudden increase in electrolyser activity/demand from h2 sector
* 19 is base year price of electrolyser. Future cost decreases are captured by TFP growth and EE in intermediates (via ica decrease)
    QF.L(F,'AELCT','NAT')$(ELCTCAP(TC,RUN) GT 0.015 and ELCTCAP(TC-1,RUN) LT 0.015) = ifa(F,'AELCT','NAT')*iva('AELCT','NAT')*ELCTCAP(TC,RUN)*19;
* QE is not fixed this just helps the MCP by setting a starting point (hopefully) close to the solution
    QE.L('CELCT','REST')$(TVACT(TC,'PEXHETP') GT 0.015 and TVACT(TC-1,'PEXHETP') lt 0.015) = TVACT(TC,'PEXHETP')*19;

* fuelcell sector - supply of fuel cells is linked to fuel cell export and fuel cell capacity in trucks (ELCTCAP)
    AFLEO('AFCEL')$(FCELLCAP(TC,RUN) GT 0.015) = YES;
    AFX('AFCEL',RD) = YES;
    AFXGR('AFCEL','NAT',XC,TC)$(FCELLCAP(TC-1,RUN) GT 0.015) = FCELLCAP(TC,RUN)/FCELLCAP(TC-1,RUN)-1;

* this is done to catch sudden increase in fuelcell activity/demand from freight/vehicle sector
* 3.3 is base year price of fuel cell. Future cost decreases are captured by TFP growth and EE in intermediates
    QF.L(F,'AFCEL','NAT')$(FCELLCAP(TC,RUN) GT 0.015 and FCELLCAP(TC-1,RUN) LT 0.015) = ifa(F,'AFCEL','NAT')*iva('AFCEL','NAT')*FCELLCAP(TC,RUN)*3.3;
* QE is not fixed this just helps the MCP by setting a starting point (hopefully) close to the solution, 3.3 is the base year price
    QE.L('CFCEL','REST')$(TVACT(TC,'PEXHFCP') GT 0.015 and TVACT(TC-1,'PEXHFCP') lt 0.015) = TVACT(TC,'PEXHFCP')*3.3;

* electricity sector
    AFX('AELEC',RD) = NO;
    AFX('AELEC',RD)$EFX(TT,TC) = YES;
    AFXGR('aelec','NAT',XC,TC)$EFX(TT,TC) = QFEGY_GR(RUN,TC)- 1;

* coal sector
    AFX('acoal',RD)$EFX(TT,TC) = YES;
    CCAP(TC,TT) = sum(C,FLOW_OUT_FSFUELS('coal',C,TC));
    AFXGR('ACOAL','NAT',XC,TC)$(COALMINCAP(TC-1) and pacoalx(TC-1,RUN)) = (COALMINCAP(TC)/COALMINCAP(TC-1))-1;
*pacoalx(TC,RUN)/pacoalx(TC-1,RUN))-1;
    AFLEO('ACOAL')$(NOT T1(TC)) = YES;
* setting starting position to avoid -ve values on qe
    QE.L('ccoal-hgh','REST')$(NOT T1(TC)) = TVACT(TC,'PEXCOA');
    QE.L('ccoal-low','REST')$(NOT T1(TC)) = 0;

* petroleum sector
    AFX('APETR',RD) = NO;
    AFX('APETR',RD)$EFX(TT,TC) = YES;

    AFXGR('APETR','NAT',XC,TC)$EFX(TT,TC) = PAFXCAP(XC,TC,TT)/PAFXCAP(XC,TC-1,TT)- 1;
    theta('apetr','cpetr_o') = FLOW_OUT_FSFuels('petr','cpetr_o',TC)/sum(C,FLOW_OUT_FSFuels('petr',C,TC));
    theta('apetr','cpetr_d') = FLOW_OUT_FSFuels('petr','cpetr_d',TC)/sum(C,FLOW_OUT_FSFuels('petr',C,TC));
    theta('apetr','cpetr_p') = FLOW_OUT_FSFuels('petr','cpetr_p',TC)/sum(C,FLOW_OUT_FSFuels('petr',C,TC));
* helping the solver
    QXAC.L('apetr',c)$AFXGR('APETR','NAT',XC,TC) = QXAC.L('apetr',c)*(1+AFXGR('APETR','NAT',XC,TC));
    QA.L('apetr')$AFXGR('APETR','NAT',XC,TC) = QA.L('apetr')*(1+AFXGR('APETR','NAT',XC,TC));
    QINT.L(c,'apetr')$AFXGR('APETR','NAT',XC,TC) = QINT.L(c,'apetr')*(1+AFXGR('APETR','NAT',XC,TC));
    QVA.L('apetr','nat')$AFXGR('APETR','NAT',XC,TC) = QVA.L('apetr','nat')*(1+AFXGR('APETR','NAT',XC,TC));
    QF.L(FLAB,'APETR',RD)$AFXGR('APETR','NAT',XC,TC) = QF.L(FLAB,'APETR',RD)*(1+AFXGR('APETR','NAT',XC,TC));

* Oil Sector
    AFX('ACOIL',RD)$EFX(TT,TC) = YES;
    AFLEO('ACOIL') = YES;
    AFXGR('ACOIL','NAT',XC,TC)$(Flow_in_FSFuels('petr','ccoil',TC-1) > 0.5)  = max(-0.9,Flow_in_FSFuels('petr','ccoil',TC)/Flow_in_FSFuels('petr','ccoil',TC-1)-1);

* Hydrogen Sector
    AFX('AHYDR',RD) = YES;
    AFLEO('AHYDR') = YES;
    QF.L(F,'AHYDR','NAT')$(HCAP(XC,TC,TT) GT 0.1 and HCAP(XC,TC-1,TT) le 0.1) = ifa(F,'AHYDR','NAT')*iva('AHYDR','NAT')*FLOW_OUT_FSFuels('hydr','chydr',TC);
    AFXGR('AHYDR','NAT',XC,TC)$(HCAP(XC,TC-1,TT) GT 0.1) = FLOW_OUT_FSFuels('hydr','chydr',TC)/FLOW_OUT_FSFuels('hydr','chydr',TC-1)-1;
* set reasonable starting value for QVA otherwise wfdist goes -ve.
    QVA.L('AHYDR','NAT')$(HCAP(XC,TC,TT) GT 0.00001) = sum(F,QF.L(F,'AHYDR','NAT'));
    PVA.L('AHYDR','NAT')$(HCAP(XC,TC,TT) GT 0.00001) = 1;

*CA fix imports in accordance with TIMES for all periods
*!          QM.FX('CELEC','REST') =  FLOW_OUT_FSFuels('imp','celec',TC);

* other exogenous sectors
* gold - set to slow decline
    AFX('AGOLD',RD)=YES;
    AFLEO('AGOLD') = YES;
    AFXGR('AGOLD','NAT',XC,TC) = -0.001;

* ngas
    AFX('ANGAS',RD) = YES;
*    AFLEO('ANGAS') = YES;
    AFXGR('ANGAS','NAT',XC,TC) = -.05;

* chemicals set to partially track Secunda and jet fuel production
    CTL_Weight(TC,RUN) = 0.7 * VARACT('REGION1',TC-1,'UCTLCLEIN-E')/VARACT('REGION1','2012','UCTLCLEIN-E');

    AFX('ABCHM',RD) = YES;
    AFLEO('ABCHM') = YES;
    AFX('AOCHM',RD) = YES;
    AFLEO('AOCHM') = YES;

    AFXGR('ABCHM','NAT',XC,TC)$QINTCBCHM(TC-2,RUN) =
                         (1 - CTL_Weight(TC,RUN)) * (QINTCBCHM(TC-1,RUN)/QINTCBCHM(TC-2,RUN)) +
                         CTL_Weight(TC,RUN) * VARACT('REGION1',TC,'UCTLCLEIN-E')/(VARACT('REGION1',TC-1,'UCTLCLEIN-E')+0.00001) - 1;

    AFXGR('AOCHM','NAT',XC,TC)$QINTCOCHM(TC-2,RUN) =
                         (1 - CTL_Weight(TC,RUN)) * (QINTCOCHM(TC-1,RUN)/QINTCOCHM(TC-2,RUN)) +
                         CTL_Weight(TC,RUN) * VARACT('REGION1',TC,'UCTLCLEIN-E')/(VARACT('REGION1',TC-1,'UCTLCLEIN-E')+0.00001) - 1;

* Exogenous Exports can only be used in sectors that exist and active in the base year
* the export level specification assumes a 1mton export in 2012 and that PEXGDRI is the green iron export in mtons
    QE.FX('ciron','REST') = QE0('ciron','REST')*(1+VARACT('REGION1',TC,'PEXGDRI'));

* ammonia
    QE.FX('cammo','REST')$VARACT('REGION1',TC-1,'PEXNH3') = QE.L('cammo','REST')*VARACT('REGION1',TC,'PEXNH3')/VARACT('REGION1',TC-1,'PEXNH3');

* PGM
    QE.FX('cpgm','REST') = QE.L('cpgm','REST')*VARACT('REGION1',TC,'PEXPGM')/VARACT('REGION1',TC-1,'PEXPGM');



* set ANFX
    ANFX(A,RD)=NO;
    ANFX(A,RD)$(NOT AFX(A,RD))=YES;



*       Capital stock accumulation and allocation
$include cge\includes\2capital.inc

*       ENERGY: Tech change in energy use via input output matrix
*       Share of new capital in total
    ishr(A,RD)$SUM(FCAP, QF.L(FCAP,A,RD)) = SUM(FCAP, DKAP(FCAP,A,RD))/SUM(FCAP, QF.L(FCAP,A,RD));
    ishr(A,RD)$(ishr(A,RD) GT 1) = 1;
*       Evolve IO coefficient
*bm        ica(CEGY,A,RD)$ica(CEGY,A,RD) = (1-ishr(A,RD))*ica(CEGY,A,RD) + ishr(A,RD)*ica(CEGY,A,RD)*(PQ.L(CEGY)/PQ0(CEGY)/(DPI.L/DPI0))**rhoegyint(CEGY,XC,TC);
*       Tech change in energy use for households
*bm        betam(CEGY,H) = betam(CEGY,H)*((PQ.L(CEGY)/CPI.L)/(PQ0(CEGY)/CPI0))**rhoegyhhd(CEGY,XC,TC);
*bm        betam(C,H)$EH0(H) = betam(C,H)/SUM(CP, betam(CP,H));

*       Land expansion
    QF.L('FLND',A,RD) = QF.L('FLND',A,RD)*(1+FACGR('FLND',XC,TC));

*       Labor supply growth
    QF.L(FLAB,A,RD)$(NOT T1(TC)) = QF.L(FLAB,A,RD)*(1+FACGR(FLAB,XC,TC));

*       Total factor supply
    QFS.L(F) = SUM((RD,A), QF.L(F,A,RD));

      );
*     end NOT T1(TC) loop

$include cge\includes\2closures.inc

*FH: Removed code
$ontext
*CA     Trend wfdist factors for labor towards 1
        IF(ORD(TC) GE 2 AND ORD(TC) LE 11,
            WFDIST.FX(FLAB,A,RD)$WFDIST0(FLAB,A,RD)=WFDIST0(FLAB,A,RD)+(ORD(TC)-1)*(1-WFDIST0(FLAB,A,RD))/10;
        );
        IF(ORD(TC) GT 11,
            WFDIST.FX(FLAB,A,RD)$WFDIST0(FLAB,A,RD)=1;
        );
$offtext

*bm!    IF(ORD(TT) gt 1 AND NOT T1(TC),
    IF(NOT T1(TC),

*bm to recalc household budget shares
$include cge\includes\EnergyGoodsBudgetAllocation.inc


*bm to recalc household budget shares
$include cge\includes\BudgetShare_Recalc.inc


);
*     end of IF ord(tt) gt 1 condition


*CA set complementarity conditions on energy factor returns
* not sure that this is needed anymore
  QFS_FOR.L(FCAP)=0;
  QFS_FOR.UP(FCAP)=+INF;
  QFS_FOR.lo(FCAP)=-INF;
*FH 71218
  QFS_FOR.LO('FEGY')=0;
  ALPHAQF.LO= -INF;
  ALPHAQF.UP= +INF;
  QFS.FX('FCAP')=QFS.L('FCAP');
*FH 71218
  QFS.UP('FEGY')=QFS.L('FEGY');

*FH 71218
  QF.UP('FEGY','AELEC','NAT')= +INF;
  QF.LO('FEGY','AELEC','NAT')= -INF;

  QF.UP('FCAP',A,RD)$QF0('FCAP',A,RD)=+INF;
  QF.LO('FCAP',A,RD)$QF0('FCAP',A,RD)=-INF;


  SOLVE standcge USING MCP ;

  fail$(standcge.modelstat ne 1)=1;
  abort$(abs(walras.l) gt .001 AND NOT fail) "Walras' law lost", walras.l;

*CATemp make GAMS stop at or near error for debugging
*      fail$(ord(tt) eq 1 AND ord(tc) eq 1)=1;

  CTA_EM(A)=(QINT.L('CCOAL-LOW',A)*CO2C('CCOAL-LOW')
                  +QINT.L('CCOAL-HGH',A)*CO2C('CCOAL-HGH')
                  +QINT.L('CCOIL',A)*CO2C('CCOIL')
                  +QINT.L('CNGAS',A)*CO2C('CNGAS'));

  CTA_EMR(A)= CTA_EM(A);
*(SIM_CO2PRICE(RUN,TC)/1000);

  CTA_EMRR(A)$(PAR.L(A,'NAT')*QAR.L(A,'NAT'))=CTA_EMR(A)/(PAR.L(A,'NAT')*QAR.L(A,'NAT'));

$include cge\includes\2results.inc

$include cge\includes\2energychecks.inc

* bits needed for some of the exogenous sectors
  QINTCBCHM(TC,RUN) = SUM(a,QINT.L('CBCHM',a));
  QINTCOCHM(TC,RUN) = SUM(a,QINT.L('CBCHM',a));
  QINTCIRON(TC,RUN) = SUM(a,QINT.L('CIRON',a));


*       CA make growth in transfers from government to HH a function of nominal absorption growth plus a small increment
  IF(NOT T1(TC),
        TRNSFRGR(H,'GOV',XC,TC+1)=TABS.L/TABSX(XC,TC-1,TT)-1 + .005;
  );

  CTA_EMRR(CTA)=CTA_EMRRX(CTA,RUN,XC,TC,TT-1);
 );
*   end TC loop

* generate demand projection, fuel price projection and investment schedules for the next TIMES run
$include cge\includes\2energyfcast.inc
*!$include cge\includes\2energychecks.inc



);
*end TT loop

$batinclude cge\includes\eSAGE_Report.inc


);
*end XC loop
