$OFFSYMLIST OFFSYMXREF
$ONEMPTY
OPTIONS ITERLIM=1000, LIMROW=0, LIMCOL=0, SOLPRINT=OFF, MCP=PATH, NLP=IPOPT;
STANDCGE.SOLVELINK = 2;
*STANDCGE.OPTFILE = 1;

SCALAR
   fail    track if a solve fails /0/
 ;


SETS
*Simulations
 X                       simulations
 XC(X)                   active simulations
 XNB(X)                  nonbase simulations
*Time periods
* T                       time periods
 TC(T)                   active time periods
 TCN1(T)                 TC except base year
 T1(T)                   first year of simulation
 T2(T)                   second year of simulation
 TN(T)                   final year
*Times model sets
 TT(T)                   TIMES model years
 TTC(T)                  current TIMES model year
 TT1(T)                  TIMES model base year
 TTN(T)                  TIMES model final years (beyond CGE years)
*FH: include times final year run
 TTNN(T)                 TIMES model final year


*Model management sets
 XBASINIT(X,T)           simulations with variables initialized at base and yr level
 MISCAC                  miscellaneous simulation data elements  / QG, GTRH, HREM, FSAV, GSAV, PWE, PWM, MPS, DTAX /
*Sector-specific closures
 FL(A)                   sector-specific (fixed) land demand
 AFX(A,RD)               sectors with calibrated capital stock growth
 ANFX(A,RD)              sectors without calibrated capital stock growth
 EFX(T,T)                CGE years (TP) when elec capital is fixed during TIMES years (T)

*TIMES instances
 XCT                     SATIM instance used for sim XC
 MXCT(X, XCT)            Mapping of CGE runs to SATIM runs
 TDIR                    SATIM directory-folder list
 TIMESDIR(T,TDIR)        SATIM directory-folder specified for run
 CTA(A)                  Sectors taxed with carbon tax
                         /acoal,        agold,        amore,        acoil,         angas,
                          ahydr,        amine,        afood,        abevt,        atext,
                          aclth,        aleat,        afoot,        awood,        apapr,
                          aprnt,        apetr,        abchm,        aochm,        arubb,
                          aplas,        aglas,        anmet,        airon,        anfrm,
                          ametp,        amach,        aemch,        asequ,        avehi,
                          atequ,        afurn,        aoman,        acons,        atrps,
                          altrp-p,      altrp-f,      awtrp,        aatrp      /

 TECHTYPE                SATIM->eSAGE Tech Types
                         / ctl, gtl, oil, hyd, spv, sth, wnd, cds, clw, gas, nuc, die /


;

PARAMETERS
 XNUM                    number of simulations
 TNUM                    number of active years (used for reporting)
 TTGAP(T)                TIMES time period gap
*Macroeconomic and factor market closures
 CLOSURES(AC,X)          closure selection from Excel
 NUMERAIRE(X)            numeraire
 SICLOS(X)               value for savings-investment closure
 ROWCLOS(X)              value for rest-of-world closure
 GOVCLOS(X)              value for government closure
 MPS01SIM(X,INS)         0-1 par for potential flexing of savings rates
 TINS01SIM(X,INS)        0-1 par for potential flexing of direct tax rates
 FMOBFE(X,F)             factor is fully employed and mobile
 FACTFE(X,F)             factor is fully employed and activity-specific
 FMOBUE(X,F)             factor is unemployed and mobile
*Capital composition and price of capital good
 BMAT(C,FCAP)            capital composition matrix by type of capital
 PKAP(FCAP)              price of capital good by type
 GFCF                    nominal gross fixed capital formation

*Detailed TFP growth projections
 SECGR(A,RD,X,T)         detailed-annual sectoral TFP growth projections

*New policy simulation parameters
 CARBONTAX(X,T)          RSA carbon tax (Rands per ton of CO2 equivalent from Paltsev)
 RETALTAX(X,T)           carbon tax by foreign countries on RSA exports (Rands per ton of CO2 equivalent)
 CLEANTECH(C,X,T)        carbon content adjustment factor
 EINVEST(X,T)            electricity sector build plan investment cost (billions of Rands)
 ECAPACITY(X,RD,T)       electricity sector capacity stream (relative to base year)
 PWGR(C,RUN,T)             world price ratios from Paltsev
*
 XIMPHYD(X)              import hydro on-off

*Stuff for SATIM-F (Full sector model link)
 ECEMLINK(X)            link level to energy model - 0 no link - 1 power - 2 incl.activities - 3 incl.hh
 TCARBONTAX(X,AY,TT)    carbon tax from TIMES-SATIM (R per ton)
*fh:020218
 CTA_EM(A)              emissions of carbon taxed sectors
 CTA_EMR(A)             estimated carbon tax by each sector
 CTA_EMRR(A)            estimated effective carbon tax rate paid by each sector
*FH: IASS Shock
 PWE_COAL(C,RW,X,T)     baseline sensitivity for global coal demand
 rhova_coal(A,RD,X,T)       increase capital intensity of coal mining

*fh 291118: SANEDI-EV shocks
 deltaqev(c,rw,run,t)
 deltatev(c,rw,run,t)
;

 CTA_EMRR(CTA)=0;

$call    "gdxxrw i=cge\%basedata% o=basedata index=index!a6 checkdate"
$gdxin   basedata.gdx
$load    X T TDIR TIMESDIR
$loaddc  XC TC TT TTN FL AFX CLOSURES PWGR CARBONTAX RETALTAX
*CLEANTECH EINVEST ECAPACITY
*fh 291118: SANEDI-EV shocks
$loaddc deltaqev deltatev

alias (TC,TCP), (TT, TTP);

*CGE model times periods
 T1(TC)$(ORD(TC) EQ SMIN(TCP, ORD(TCP))) = YES;
 T2(TC)$(ORD(TC) EQ 2)                   = YES;
 TN(TC)$(ORD(TC) EQ SMAX(TCP, ORD(TCP))) = YES;
 TNUM = SUM(T$TN(T), ORD(T)) - SUM(T$T1(T), ORD(T));

*Times model time periods
 TT1(TT)$(ORD(TT) EQ 1) = YES;
 TCN1(TC) = not T1(TC);
 TTNN(TT)$(ORD(TT) EQ SMAX(TTP, ORD(TTP))) = YES;

*Count gap between solve years
* TGAP(T)$(TC(T) AND NOT T1(T)) = ORD(T) - SMAX(TCP$TC(TCP), ORD(TCP)$(ORD(T) GT ORD(TCP)));
 TTGAP(T)$(TT(T) AND NOT T1(T)) = ORD(T) - SMAX(TCP$TT(TCP), ORD(TCP)$(ORD(T) GT ORD(TCP)));

*CGE years (TP) when elec capital is fixed during TIMES years (T)
 EFX(T,TCP)$(TT(T) AND (ORD(T) GE ORD(TCP))) = YES;

*Simulations
 XC('INIT') = NO;

 XNUM = MAX(CARD(XC)-1,1) ;
 XNB(XC) = YES;
 XNB('BASE') = NO;
 XBASINIT(XC,TC) = NO;

*Fixed capital growth sectors
 ANFX(A,RD)$(NOT AFX(A,RD)) = YES;

$ontext
 Set
 TD(TC)  /2021*2050/

 Parameter
 TDUM(TC);

 TDUM(TC)=1;
 TDUM(TD)=0;
$offtext
*------------------------------------------------------------------------------
*1. Excel inputted scenario calibration
*------------------------------------------------------------------------------

$include cge\includes\2projection.inc

*----------------------------------------------------------------------
*2. Non-excel inputted scenario calibration
*----------------------------------------------------------------------

*Notes on units in CGE model
* Coal: millions of short tons
* Crude oil: millions of short tons
* Petroleum: billions of liters
* Electricity: millions of MWh
* SAM: billions of Rands
* Coefficient:
*        Coal = tons CO2 per ton coal
*        Oil  = tons CO2 per kiloliter oil
*        Gas  = tons CO2 per kiloliter gas

SETS
 CEGY(C)                 energy commodities   / ccoal-low, ccoal-hgh, ccoil, cngas, celec, cpetr_p, cpetr_d, cpetr_o /
*ccoal, ccoal-dis,
;

PARAMETER
 TCO2DSIM(X,T)           carbon tax on domestically consumed goods (Rands per ton of CO2 equivalent)
 TCO2MSIM(X,T)           carbon tax on imported goods (Rands per ton of CO2 equivalent)
 TCO2ESIM(X,T)           carbon tax rebate on exported goods (Rands per ton of CO2 equivalent)
 RTCO2ESIM(X,T)          retaliatory carbon tax rebate on exported goods (Rands per ton of CO2 equivalent)
 CO2CSIM(C,X,T)          carbon content coefficient (CO2 equivalent per native unit)
 CO2ESIM(C,RW,X,T)       carbon content coefficient (CO2 equivalent per native unit)
 CO2MSIM(C,RW,X,T)       carbon content coefficient (CO2 equivalent per native unit)
*Energy-price efficiency
 RHOEGYINT(C,X,T)        elasticity of tech change for intermediates
 RHOEGYHHD(C,X,T)        elasticity of tech change for households
 ISHR(A,RD)              share of new capital in each activity
 ICAX(C,A,RD,X,T,TT)     tracking input output coefficients
*Used by fixed energy supply mixes based on TIMES results
 ECAPGR(X,T,TT)       growth in electricity sector capacity (ECAP) levels by energy source
 PCAPGR(X,T,TT)       electricity sector capacity levels by energy source
*Energy sector investment costs
 IADJSESIM(X,T)          total electricity investment cost growth
 EAFXCAP(X,T,TT)      electricity capital from TIMES in units inconsistent with CGE
*bm+fh 201118
 PAFXCAP(X,T,TT)      refinery capital from TIMES in units inconsistent with CGE
;

*Carbon tax ------
* Divide by 1000 to convert units (from billions of rands / million of tons to rands / ton)
* TCO2SIM(X,T) = 0;
 TCO2MSIM(XC,TC) = 0;

 TCO2DSIM(XC,TC)$(NOT T1(TC)) = 0;
 TCO2ESIM(XC,TC)$(NOT T1(TC)) = 0;
 TCO2MSIM(XC,TC)$(NOT T1(TC)) = 0;

 TCO2DSIM(XC,TC)$(NOT T1(TC)) = CARBONTAX(XC,TC)/1000;
 TCO2ESIM(XC,TC)$(NOT T1(TC)) = CARBONTAX(XC,TC)/1000;
 TCO2MSIM(XC,TC)$(NOT T1(TC)) = CARBONTAX(XC,TC)/1000;

*Retaliatory tax on RSA exports based on carbon content -----
 RTCO2ESIM(XC,TC)$(NOT T1(TC)) = RETALTAX(XC,TC)/1000/EXR0;

*Cleaner coal ------
* Technology introduced over time (needs to matched to build plan)
*fh 022019
* CLEANTECH(C,XC,TC)$(NOT CLEANTECH(C,XC,TC)) = 1;
* CO2CSIM(C,XC,TC)    = co2c0(C)    * CLEANTECH(C,XC,TC);
* CO2ESIM(C,RW,XC,TC) = co2e0(C,RW) * CLEANTECH(C,XC,TC);
* CO2MSIM(C,RW,XC,TC) = co2m0(C,RW) * CLEANTECH(C,XC,TC);

*Energy intensity elasticities ------
* Intermediates
 RHOEGYINT(CEGY,XC,TC) = 0;
*-0.08;
* Households
 RHOEGYHHD(CEGY,XC,TC) = -0.0;

*Electricity sector -----
* Exogenous capacity from IRP2
 ECAPGR(XC,TC,TT) = 0.00;
 PCAPGR(XC,TC,TT) = 0.00;
* ECAPGR(RD,XC,TC)$ECAPACITY(XC,RD,TC-1) = ECAPACITY(XC,RD,TC)/ECAPACITY(XC,RD,TC-1)-1;

* ECAPGR(RD,'REVISED',TC) = ECAPGR(RD,'BASE',TC);
* Remove energy investment costs from total investment funds
 IADJSESIM(XC,TC) = 1;
* IADJSESIM(XC,TC) = (SUM(C, PQ0(C)*QINV0(C,'S-E'))+EINVEST(XC,TC))/SUM(C, PQ0(C)*QINV0(C,'S-E'));
* IADJSESIM(XC,TC)$XNB(XC) = IADJSESIM('BASE',TC);

*Interest rate on foreign debt ----
* Cost of electricity sector investment
 EINVEST(X,T) = 0;
* Interest rate on additional spending incurred under the revised electricity build plan
 irate = 0.05;

 rhova_coal(A,RD,X,T)=rhova(A,RD);
* rhova_coal('acoal',RD,'capinten',T)=rhova('acoal',RD)*0.5;



*----------------------------------------------------------------------
*3. Closures
*----------------------------------------------------------------------

*Macroeconomic closures
 NUMERAIRE(XC) = CLOSURES('DUM',XC);
 ROWCLOS(XC)   = CLOSURES('ROW',XC);
 GOVCLOS(XC)   = CLOSURES('GOV',XC);
 SICLOS(XC)    = CLOSURES('S-I',XC);

*Institutional savings and tax rate adjustments
 MPS01SIM(XC,INSDNG)  = 1;
 TINS01SIM(XC,INSDNG) = 1;

*Remove electricity enterprises from exogenous MPS adjustment
 MPS01SIM(XC,'ENT-E')  = 0;
 MPS01SIM(XC,'ENT-N')  = 0;
 TINS01SIM(XC,'ENT-E') = 0;

*No value-added tax adjustment on electricity sectors
 tva01(A,RD)       = YES;
 tva01('AELEC',RD) = NO;
*fh 71218
 tva01('APETR',RD) = NO;

*No direct tax adjustment on electricity sectors
 trn01(INSDNG)  = YES;
 trn01('ENT-N') = NO;
 trn01('ENT-E') = NO;

*CA Hard code Baseline as fully employed labor
*JT: AE paper (removed this code)
* CLOSURES(FLAB,'BASE') = 1;

*Factor markets closures
 FMOBFE(XC,F)$(CLOSURES(F,XC) EQ 1) = 1;
 FACTFE(XC,F)$(CLOSURES(F,XC) EQ 2) = 1;
 FMOBUE(XC,F)$(CLOSURES(F,XC) EQ 3) = 1;
 FMOBFE(XC,F)$(CLOSURES(F,XC) EQ 4) = 1;

*If no value is specified for a factor, impose FMOBFE:
 FMOBFE(XC,F)$(FMOBFE(XC,F) + FACTFE(XC,F) + FMOBUE(XC,F) EQ 0) = 1;


*Initialize CGE model parameters and variables
$include cge\includes\2initialize.inc

*Initialise links to TIMES model
$include cge\includes\2energyinit.inc
$include cge\includes\2energychecksinit.inc




*----------------------------------------------------------------------
*4. Time period loop
*----------------------------------------------------------------------




*bm PWGR('ccoal-hgh',X,T) = -0.005;
