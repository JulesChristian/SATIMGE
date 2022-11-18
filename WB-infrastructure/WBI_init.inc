* Initalization of sets and parameters used for WB Indicators
* Bruno Merven November 2022

SETS
   WBI_Ind WBI indicators /
         C_GVA, GDP_RUN, GOVSHRX, INVSHRX, C_QFSlab, PalmaRatio, 20-20Ratio, C_PubDef,
         YIX, C_YIX, TINSX, MPSX, C_SavingsINS,
         YGX, EGX, GADJX, GOVGR, C_GovCons, GSAVX, GOVTAB,
         EXR, FSAVX, C_TSav,
         QINVX, PQX, C_InvVal, IADJX, C_QINV_IADJ,
         TRNADJX, trnsfrx_gov, trnsfrx_row, C_NetTrnsGov2Ins,
         QFSX, P_ActivePop, P_WAgePop, C_BroadUnEmpRate, C_LabForcePart, QVAX, PVAX     /
;

Parameters
* intermediate calc parameters
   WBI(WBI_Ind,AC,AY)  Indicators
;

WBI('P_ActivePop','total','2020') = 24292.9;
WBI('P_ActivePop','total','2021') = 24642.6;
WBI('P_ActivePop','total','2022') = 24992.2;
WBI('P_ActivePop','total','2023') = 25341.9;
WBI('P_ActivePop','total','2024') = 25691.6;
WBI('P_ActivePop','total','2025') = 26041.2;
WBI('P_ActivePop','total','2026') = 26390.6;
WBI('P_ActivePop','total','2027') = 26740;
WBI('P_ActivePop','total','2028') = 27089.3;
WBI('P_ActivePop','total','2029') = 27438.7;
WBI('P_ActivePop','total','2030') = 27788.1;
WBI('P_ActivePop','total','2031') = 28086.2;
WBI('P_ActivePop','total','2032') = 28384.4;
WBI('P_ActivePop','total','2033') = 28682.5;
WBI('P_ActivePop','total','2034') = 28980.7;
WBI('P_ActivePop','total','2035') = 29278.8;
WBI('P_ActivePop','total','2036') = 29514.3;
WBI('P_ActivePop','total','2037') = 29749.7;
WBI('P_ActivePop','total','2038') = 29985.2;
WBI('P_ActivePop','total','2039') = 30220.7;
WBI('P_ActivePop','total','2040') = 30456.1;
WBI('P_ActivePop','total','2041') = 30638.2;
WBI('P_ActivePop','total','2042') = 30820.3;
WBI('P_ActivePop','total','2043') = 31002.3;
WBI('P_ActivePop','total','2044') = 31184.4;
WBI('P_ActivePop','total','2045') = 31366.5;
WBI('P_ActivePop','total','2046') = 31469.2;
WBI('P_ActivePop','total','2047') = 31571.9;
WBI('P_ActivePop','total','2048') = 31674.6;
WBI('P_ActivePop','total','2049') = 31777.4;
WBI('P_ActivePop','total','2050') = 31880.1;


WBI('P_WAgePop','total','2020') = 38959.5;
WBI('P_WAgePop','total','2021') = 39520.3;
WBI('P_WAgePop','total','2022') = 40081.1;
WBI('P_WAgePop','total','2023') = 40641.9;
WBI('P_WAgePop','total','2024') = 41202.7;
WBI('P_WAgePop','total','2025') = 41763.4;
WBI('P_WAgePop','total','2026') = 42323.7;
WBI('P_WAgePop','total','2027') = 42884;
WBI('P_WAgePop','total','2028') = 43444.3;
WBI('P_WAgePop','total','2029') = 44004.6;
WBI('P_WAgePop','total','2030') = 44564.9;
WBI('P_WAgePop','total','2031') = 45043.1;
WBI('P_WAgePop','total','2032') = 45521.2;
WBI('P_WAgePop','total','2033') = 45999.4;
WBI('P_WAgePop','total','2034') = 46477.5;
WBI('P_WAgePop','total','2035') = 46955.7;
WBI('P_WAgePop','total','2036') = 47333.3;
WBI('P_WAgePop','total','2037') = 47710.9;
WBI('P_WAgePop','total','2038') = 48088.6;
WBI('P_WAgePop','total','2039') = 48466.2;
WBI('P_WAgePop','total','2040') = 48843.8;
WBI('P_WAgePop','total','2041') = 49135.8;
WBI('P_WAgePop','total','2042') = 49427.8;
WBI('P_WAgePop','total','2043') = 49719.8;
WBI('P_WAgePop','total','2044') = 50011.8;
WBI('P_WAgePop','total','2045') = 50303.8;
WBI('P_WAgePop','total','2046') = 50468.5;
WBI('P_WAgePop','total','2047') = 50633.3;
WBI('P_WAgePop','total','2048') = 50798;
WBI('P_WAgePop','total','2049') = 50962.7;
WBI('P_WAgePop','total','2050') = 51127.5;
