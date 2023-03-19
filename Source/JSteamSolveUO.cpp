/* JSTEAM MATLAB MEX INTERFACE
 * Unit Operation Interface
 * J.Currie November 2012 */

#include <mex.h>
#include "include\JSteamSolve.h"
#include "include\JSteamUtils.h"

using namespace JSteamDLL;

//Argument Enumeration (in expected order of arguments)
enum {eUNITOP, eIN1, eIN2, eIN3, eIN4, eIN5, eIN6, eIN7, eIN8, eIN9, eIN10, eIN11, eIN12, eIN13, eIN14};
enum {eOUT1=1, eOUT2, eOUT3, eOUT4, eOUT5, eOUT6, eOUT7, eOUT8, eOUT9};
//PRHS Defines    
#define pUNITOP prhs[eUNITOP]
#define pIN1    *mxGetPr(prhs[eIN1])
#define pIN2    *mxGetPr(prhs[eIN2])
#define pIN3    *mxGetPr(prhs[eIN3])
#define pIN4    *mxGetPr(prhs[eIN4])
#define pIN5    *mxGetPr(prhs[eIN5])
#define pIN6    *mxGetPr(prhs[eIN6])
#define pIN7    *mxGetPr(prhs[eIN7])
#define pIN8    *mxGetPr(prhs[eIN8])
#define pIN9    *mxGetPr(prhs[eIN9])
#define pIN10   *mxGetPr(prhs[eIN10])
#define pIN11   *mxGetPr(prhs[eIN11])
#define pIN12   *mxGetPr(prhs[eIN12])
#define pIN13   *mxGetPr(prhs[eIN13])
#define pIN14   *mxGetPr(prhs[eIN14])
//PLHS Defines
#define pOUT1   mxGetPr(plhs[eOUT1])
#define pOUT2   mxGetPr(plhs[eOUT2])
#define pOUT3   mxGetPr(plhs[eOUT3])
#define pOUT4   mxGetPr(plhs[eOUT4])
#define pOUT5   mxGetPr(plhs[eOUT5])
#define pOUT6   mxGetPr(plhs[eOUT6])
#define pOUT7   mxGetPr(plhs[eOUT7])
#define pOUT8   mxGetPr(plhs[eOUT8])

//Unit Operation Enumeration
enum {eBOILER,eCOMP1Q,eCOMP2Q,eCOMP1M,eCOMP2M,eCOOLMQ,eCOOLTM,eCOOLHM,eCOOLTQ,eDRTR,eDSPRMI,eDSPRMO,eFLASH,
      eHEATMQ,eHEATTM,eHEATHM,eHEATTQ,ePUMP,eTURB1Q,eTURB2Q,eTURB3Q,eTURB1M,eTURB2M,eTURB3M,eVALVE,
      eCOMP1Qc,eCOMP2Qc,eCOMP1Mc,eCOMP2Mc,eCOOLMQc,eCOOLTMc,eCOOLHMc,eCOOLTQc,eFLASHc,eHEATMQc,eHEATTMc,eHEATHMc,eHEATTQc,
      ePUMPc,eTURB1Qc,eTURB2Qc,eTURB3Qc,eTURB1Mc,eTURB2Mc,eTURB3Mc,eVALVEc,eFURNM,eFURNQ,eBLRFRD,eGTG,eGTGINJ,
      eHRSG,eHRSGSup};

namespace JSteamMEX
{   
    //List of Unit Operation Methods
    JSUFcn JSUFcns[] = {
        {"\n-------------------------------------------",NULL,NULL,NULL,NULL},
        {"WATER & STEAM UNIT OPERATIONS\n",NULL,NULL,NULL,NULL},
        {" - Boilers",NULL,NULL,NULL,NULL},
        {"UnitOp_Boiler",6,5,eBOILER,"[status,StmH,BFWM,BDM,BDH,Duty] = JSteamMEX('UnitOp_Boiler',BFWH,StmM,StmT,StmP,BDr,Eff)"},
        {" - Compressors",NULL,NULL,NULL,NULL},
        {"UnitOp_Compressor1Q",5,2,eCOMP1Q,"[status,outM,outH] = JSteamMEX('UnitOp_Compressor1Q',inH,inP,outP,Eff,Pwr)"},
        {"UnitOp_Compressor2Q",8,3,eCOMP2Q,"[status,outM2,outH1,outH2] = JSteamMEX('UnitOp_Compressor2Q',inH,inP,outP1,Eff1,outM1,outP2,Eff2,TPwr)"},
        {"UnitOp_Compressor1M",5,2,eCOMP1M,"[status,Pwr,outH] = JSteamMEX('UnitOp_Compressor1M',inH,inP,outP,Eff,outM)"},
        {"UnitOp_Compressor2M",8,3,eCOMP2M,"[status,TPwr,outH1,outH2] = JSteamMEX('UnitOp_Compressor2M',inH,inP,outP1,Eff1,outM1,outP2,Eff2,outM2)"},
        {" - Coolers & Heaters",NULL,NULL,NULL,NULL},
        {"UnitOp_CoolerMQ",4,2,eCOOLMQ,"[status,outH,outT] = JSteamMEX('UnitOp_CoolerMQ',inH,outP,outM,Duty)"},
        {"UnitOp_CoolerTM",4,2,eCOOLTM,"[status,outH,Duty] = JSteamMEX('UnitOp_CoolerTM',inH,outP,outT,outM)"},
        {"UnitOp_CoolerTM",4,2,eCOOLHM,"[status,outT,Duty] = JSteamMEX('UnitOp_CoolerHM',inH,outP,outH,outM)"},
        {"UnitOp_CoolerTQ",4,2,eCOOLTQ,"[status,outH,outM] = JSteamMEX('UnitOp_CoolerTQ',inH,outP,outT,Duty)"},
        {"UnitOp_HeaterMQ",4,2,eHEATMQ,"[status,outH,outT] = JSteamMEX('UnitOp_HeaterMQ',inH,outP,outM,Duty)"},
        {"UnitOp_HeaterTM",4,2,eHEATTM,"[status,outH,Duty] = JSteamMEX('UnitOp_HeaterTM',inH,outP,outT,outM)"},
        {"UnitOp_HeaterHM",4,2,eHEATHM,"[status,outT,Duty] = JSteamMEX('UnitOp_HeaterHM',inH,outP,outH,outM)"},
        {"UnitOp_HeaterTQ",4,2,eHEATTQ,"[status,outH,outM] = JSteamMEX('UnitOp_HeaterTQ',inH,outP,outT,Duty)"},
        {" - Plant Equipment",NULL,NULL,NULL,NULL},
        {"UnitOp_Deaerator",5,4,eDRTR,"[status,StmM,WatM,LiqH,VapM] = JSteamMEX('UnitOp_Deaerator',OpP,StmH,WatH,LiqM,Vtr)"},
        {"UnitOp_DesuperheaterMIN",5,3,eDSPRMI,"[status,WatInM,StmOutM,StmOutH] = JSteamMEX('UnitOp_DesuperheaterMIN',StmInH,WatInH,outP,outT,StmInM)"},
        {"UnitOp_DesuperheaterMOUT",5,3,eDSPRMO,"[status,WatInM,StmInM,StmOutH] = JSteamMEX('UnitOp_DesuperheaterMOUT',StmInH,WatInH,outP,outT,StmOutM)"},
        {"UnitOp_FlashDrum",3,4,eFLASH,"[status,VapM,VapH,LiqM,LiqH] = JSteamMEX('UnitOp_FlashDrum',OpP,inH,inM)"},
        {"UnitOp_Pump",5,2,ePUMP,"[status,outH,Pwr] = JSteamMEX('UnitOp_Pump',inH,inP,inM,outP,Eff)"},
        {"UnitOp_Valve",3,2,eVALVE,"[status,outT,outX] = JSteamMEX('UnitOp_Valve',inH,inP,outP)"},
        {" - Expanders/Turbines",NULL,NULL,NULL,NULL},
        {"UnitOp_Turbine1Q",5,2,eTURB1Q,"[status,outM,outH] = JSteamMEX('UnitOp_Turbine1Q',inH,inP,outP,Eff,Pwr)"},
        {"UnitOp_Turbine2Q",8,3,eTURB2Q,"[status,outM2,outH1,outH2] = JSteamMEX('UnitOp_Turbine2Q',inH,inP,outP1,Eff1,outM1,outP2,Eff2,TPwr)"},
        {"UnitOp_Turbine3Q",11,4,eTURB3Q,"[status,outM3,outH1,outH2,outH3] = JSteamMEX('UnitOp_Turbine3Q',inH,inP,outP1,Eff1,outM1,outP2,Eff2,outM2,outP3,Eff3,TPwr)"},
        {"UnitOp_Turbine1M",5,2,eTURB1M,"[status,Pwr,outH] = JSteamMEX('UnitOp_Turbine1M',inH,inP,outP,Eff,outM)"},
        {"UnitOp_Turbine2M",8,3,eTURB2M,"[status,TPwr,outH1,outH2] = JSteamMEX('UnitOp_Turbine2M',inH,inP,outP1,Eff1,outM1,outP2,Eff2,outM2)"},
        {"UnitOp_Turbine3M",11,4,eTURB3M,"[status,TPwr,outH1,outH2,outH3] = JSteamMEX('UnitOp_Turbine3M',inH,inP,outP1,Eff1,outM1,outP2,Eff2,outM2,outP3,Eff3,outM3)"},
        
        //Combustion Unit Operations
        {"\n-------------------------------------------",NULL,NULL,NULL,NULL}, 
        {"COMBUSTION UNIT OPERATIONS\n",NULL,NULL,NULL,NULL},
        {" - Boilers",NULL,NULL,NULL,NULL},
        {"UnitOp_BoilerFired",13,6,eBLRFRD,"[status,StmH,BFWM,airM,fuelM,FEff,Duty,Stack] = JSteamMEX('UnitOp_BoilerFired',BFWH,StmM,StmT,StmP,BDr,{Fuel_Mixture},fuelT,airT,airP,airRelHum,minStackT,O2,O2Mode)"},
        {" - Furnaces",NULL,NULL,NULL,NULL},
        {"UnitOp_FurnaceM",9,4,eFURNM,"[status,airM,Duty,FEff,acidDp,Stack] = JSteamMEX('UnitOp_FuranceM',{Fuel_Mixture},fuelT,airT,airP,airRelHum,minStackT,O2,O2Mode,fuelM)"},
        {"UnitOp_FurnaceQ",9,4,eFURNQ,"[status,airM,fuelM,FEff,acidDp,Stack] = JSteamMEX('UnitOp_FuranceQ',{Fuel_Mixture},fuelT,airT,airP,airRelHum,minStackT,O2,O2Mode,Duty)"},        
        {" - Gas Turbines & HRSGs",NULL,NULL,NULL,NULL},
        {"UnitOp_GasTurbine",10,3,eGTG,"[status,airM,fuelM,exhaustM,Exhaust] = JSteamMEX('UnitOp_GasTurbine',{Fuel_Mixture},fuelT,airT,airP,airRelHum,exhaustT,presRatio,compEff,GTPower,GTEff)"},
        {"UnitOp_GasTurbineInj",13,3,eGTGINJ,"[status,airM,fuelM,exhaustM,Exhaust] = JSteamMEX('UnitOp_GasTurbineInj',{Fuel_Mixture},fuelT,airT,airP,airRelHum,exhaustT,presRatio,compEff,StmT,StmP,StmM,GTPower,GTEff)"},
        {"UnitOp_HRSG",9,4,eHRSG,"[status,StmM,econDT,sprHDT,stackT,Stack] = JSteamMEX('UnitOp_HRSG',{GT_Exhaust},exhaustT,exhaustM,minAppT,minStackT,BFWH,StmT,StmP,BDr)"},
        {"UnitOp_HRSGSup",14,8,eHRSGSup,"[status,StmM,BaseStmM,MaxStmM,SecFuelM,econDT,sprHDT,stackT,stackM,Stack] = JSteamMEX('UnitOp_HRSGSup',{GT_Exhaust},{SecFuel_Mixture},OpMode,exhaustT,exhaustM,secFuelT,minAppT,minStackT,minStackO2,StmDemand,BFWH,StmT,StmP,BDr)"},
        
        //Customizable Fluid Operations
        {"\n-------------------------------------------",NULL,NULL,NULL,NULL},
        {"REFRIGERANT UNIT OPERATIONS\n",NULL,NULL,NULL,NULL},
        {" - Compressors",NULL,NULL,NULL,NULL},
        {"UnitOp_Compressor1QC",6,2,eCOMP1Qc,"[status,outM,outH] = JSteamMEX('UnitOp_Compressor1QC',{Refrig},inH,inP,outP,Eff,Pwr)"},
        {"UnitOp_Compressor2QC",9,3,eCOMP2Qc,"[status,outM2,outH1,outH2] = JSteamMEX('UnitOp_Compressor2QC',{Refrig},inH,inP,outP1,Eff1,outM1,outP2,Eff2,TPwr)"},
        {"UnitOp_Compressor1MC",6,2,eCOMP1Mc,"[status,Pwr,outH] = JSteamMEX('UnitOp_Compressor1MC',{Refrig},inH,inP,outP,Eff,outM)"},
        {"UnitOp_Compressor2MC",9,3,eCOMP2Mc,"[status,TPwr,outH1,outH2] = JSteamMEX('UnitOp_Compressor2MC',{Refrig},inH,inP,outP1,Eff1,outM1,outP2,Eff2,outM2)"},
        {" - Coolers & Heaters",NULL,NULL,NULL,NULL},
        {"UnitOp_CoolerMQC",5,2,eCOOLMQc,"[status,outH,outT] = JSteamMEX('UnitOp_CoolerMQC',{Refrig},inH,outP,outM,Duty)"},
        {"UnitOp_CoolerTMC",5,2,eCOOLTMc,"[status,outH,Duty] = JSteamMEX('UnitOp_CoolerTMC',{Refrig},inH,outP,outT,outM)"},
        {"UnitOp_CoolerHMC",5,2,eCOOLHMc,"[status,outT,Duty] = JSteamMEX('UnitOp_CoolerHMC',{Refrig},inH,outP,outH,outM)"},
        {"UnitOp_CoolerTQC",5,2,eCOOLTQc,"[status,outH,outM] = JSteamMEX('UnitOp_CoolerTQC',{Refrig},inH,outP,outT,Duty)"},
        {"UnitOp_HeaterMQC",5,2,eHEATMQc,"[status,outH,outT] = JSteamMEX('UnitOp_HeaterMQC',{Refrig},inH,outP,outM,Duty)"},
        {"UnitOp_HeaterTMC",5,2,eHEATTMc,"[status,outH,Duty] = JSteamMEX('UnitOp_HeaterTMC',{Refrig},inH,outP,outT,outM)"},
        {"UnitOp_HeaterHMC",5,2,eHEATHMc,"[status,outT,Duty] = JSteamMEX('UnitOp_HeaterHMC',{Refrig},inH,outP,outH,outM)"},
        {"UnitOp_HeaterTQC",5,2,eHEATTQc,"[status,outH,outM] = JSteamMEX('UnitOp_HeaterTQC',{Refrig},inH,outP,outT,Duty)"},
        {" - Plant Equipment",NULL,NULL,NULL,NULL},
        {"UnitOp_PumpC",6,2,ePUMPc,"[status,outH,Pwr] = JSteamMEX('UnitOp_PumpC',{Refrig},inH,inP,inM,outP,Eff)"},
        {"UnitOp_ValveC",4,2,eVALVEc,"[status,outT,outX] = JSteamMEX('UnitOp_ValveC',{Refrig},inH,inP,outP)"},
        {" - Expanders/Turbines",NULL,NULL,NULL,NULL},
        {"UnitOp_Turbine1QC",6,2,eTURB1Qc,"[status,outM,outH] = JSteamMEX('UnitOp_Turbine1QC',{Refrig},inH,inP,outP,Eff,Pwr)"},
        {"UnitOp_Turbine2QC",9,3,eTURB2Qc,"[status,outM2,outH1,outH2] = JSteamMEX('UnitOp_Turbine2QC',{Refrig},inH,inP,outP1,Eff1,outM1,outP2,Eff2,TPwr)"},
        {"UnitOp_Turbine3QC",12,4,eTURB3Qc,"[status,outM3,outH1,outH2,outH3] = JSteamMEX('UnitOp_Turbine3QC',{Refrig},inH,inP,outP1,Eff1,outM1,outP2,Eff2,outM2,outP3,Eff3,TPwr)"},
        {"UnitOp_Turbine1MC",6,2,eTURB1Mc,"[status,Pwr,outH] = JSteamMEX('UnitOp_Turbine1MC',{Refrig},inH,inP,outP,Eff,outM)"},
        {"UnitOp_Turbine2MC",9,3,eTURB2Mc,"[status,TPwr,outH1,outH2] = JSteamMEX('UnitOp_Turbine2MC',{Refrig},inH,inP,outP1,Eff1,outM1,outP2,Eff2,outM2)"},
        {"UnitOp_Turbine3MC",12,4,eTURB3Mc,"[status,TPwr,outH1,outH2,outH3] = JSteamMEX('UnitOp_Turbine3MC',{Refrig},inH,inP,outP1,Eff1,outM1,outP2,Eff2,outM2,outP3,Eff3,outM3)"},
        {NULL,NULL,NULL,NULL,NULL} //req for name finder
    };  
    
    void JSteamSolve::SolveUO(JSteam *JStm, const mxArray *prhs[], int nrhs, mxArray *plhs[])
    {
        char buf[1024];
        //Collect Unit Operation Name
        char *unit_name = mxArrayToString(pUNITOP);
        //Match it to known list
        JSUFcn *unitop;
        if(matchFcn(unit_name,unitop) == 0)
        {
            sprintf(buf,"Could not find unit operation [%s] in JSteam function list",unit_name);
            mexErrMsgTxt(buf);
        }
        //Check correct number of input args
        if((nrhs-1) < unitop->n_in)
        {
            sprintf(buf,"Unit operation [%s] requires %d input arguments but only %d were supplied.",unit_name,unitop->n_in,nrhs-1);
            mexErrMsgTxt(buf);
        }
        //Create Output arguments
        for(int i = 0; i <= unitop->n_out; i++)
            plhs[i] = mxCreateDoubleScalar(0.0);
        //Solve Unit Operation
        unitOpSolve(JStm,unitop->code,prhs,plhs);
        //Free Memory
        mxFree(unit_name);
    }
    
    //Print UnitOp Funcs Help
    void JSteamSolve::PrintUO()
    {
        //Main Steam Funcs
        JSUFcn *s = &JSUFcns[0];
        while(s && s->name) {
            if(s->sig != NULL)
                mexPrintf("%-24s - %s\n",s->name,s->sig);
            else
                mexPrintf("%s\n",s->name);
            s++;
        }
    }
    
    //Solve Unit Operation
    void JSteamSolve::unitOpSolve(JSteam *JStm, int model, const mxArray *prhs[], mxArray *plhs[])
    {
        double *status = mxGetPr(plhs[0]);   
        Mixture Refrig;
        double in1, in2, in3, in4,in5,in6,in7,in8,in9,in10,in11,in12,in13,in14;
        switch(model)
        {
            //Boilers
            case eBOILER:
                *status = (double)JStm->UnitOp_Boiler(pIN1,pIN2,pIN3,pIN4,pIN5,pIN6,pOUT1,pOUT2,pOUT3,pOUT4,pOUT5); break;
                
            //Compressors    
            case eCOMP1Q:
                *status = (double)JStm->UnitOp_Compressor1Q(pIN1,pIN2,pIN3,pIN4,pIN5,pOUT1,pOUT2); break;                
            case eCOMP2Q:
                *status = (double)JStm->UnitOp_Compressor2Q(pIN1,pIN2,pIN3,pIN4,pIN5,pIN6, pIN7,pIN8,pOUT1,pOUT2,pOUT3); break;                
            case eCOMP1M:
                *status = (double)JStm->UnitOp_Compressor1M(pIN1,pIN2,pIN3,pIN4,pIN5,pOUT1,pOUT2); break;            
            case eCOMP2M:
                *status = (double)JStm->UnitOp_Compressor2M(pIN1,pIN2,pIN3,pIN4,pIN5,pIN6,pIN7,pIN8,pOUT1,pOUT2,pOUT3); break;

            //Coolers & Heaters
            case eCOOLMQ:
                *status = (double)JStm->UnitOp_CoolerMQ(pIN1,pIN2,pIN3,pIN4,pOUT1,pOUT2); break;
            case eCOOLTM:
                *status = (double)JStm->UnitOp_CoolerTM(pIN1,pIN2,pIN3,pIN4,pOUT1,pOUT2); break; 
            case eCOOLHM:
                *status = (double)JStm->UnitOp_CoolerHM(pIN1,pIN2,pIN3,pIN4,pOUT1,pOUT2); break; 
            case eCOOLTQ:
                *status = (double)JStm->UnitOp_CoolerTQ(pIN1,pIN2,pIN3,pIN4,pOUT1,pOUT2); break;               
            case eHEATMQ:
                *status = (double)JStm->UnitOp_HeaterMQ(pIN1,pIN2,pIN3,pIN4,pOUT1,pOUT2); break;               
            case eHEATTM:
                *status = (double)JStm->UnitOp_HeaterTM(pIN1,pIN2,pIN3,pIN4,pOUT1,pOUT2); break;
            case eHEATHM:
                *status = (double)JStm->UnitOp_HeaterHM(pIN1,pIN2,pIN3,pIN4,pOUT1,pOUT2); break;
            case eHEATTQ:
                *status = (double)JStm->UnitOp_HeaterTQ(pIN1,pIN2,pIN3,pIN4,pOUT1,pOUT2); break;               

            //Plant Equipment
            case eDRTR:
                *status = (double)JStm->UnitOp_Deaerator(pIN1,pIN2,pIN3,pIN4,pIN5,pOUT1,pOUT2,pOUT3,pOUT4); break;                
            case eDSPRMI:
                *status = (double)JStm->UnitOp_DesuperheaterMin(pIN1,pIN2,pIN3,pIN4,pIN5,pOUT1,pOUT2,pOUT3); break;                
            case eDSPRMO:
                *status = (double)JStm->UnitOp_DesuperheaterMout(pIN1,pIN2,pIN3,pIN4,pIN5,pOUT1,pOUT2,pOUT3); break;                
            case eFLASH:
                *status = (double)JStm->UnitOp_FlashDrum(pIN1,pIN2,pIN3,pOUT1,pOUT2,pOUT3,pOUT4); break;                
            case ePUMP:
                *status = (double)JStm->UnitOp_Pump(pIN1,pIN2,pIN3,pIN4,pIN5,pOUT1,pOUT2); break;               
            case eVALVE:
                *status = (double)JStm->UnitOp_Valve(pIN1,pIN2,pIN3,pOUT1,pOUT2); break;
            
            //Turbines    
            case eTURB1Q:
                *status = (double)JStm->UnitOp_Turbine1Q(pIN1,pIN2,pIN3,pIN4,pIN5,pOUT1,pOUT2); break;                
            case eTURB2Q:
                *status = (double)JStm->UnitOp_Turbine2Q(pIN1,pIN2,pIN3,pIN4,pIN5,pIN6, pIN7,pIN8,pOUT1,pOUT2,pOUT3); break;  
            case eTURB3Q:
                *status = (double)JStm->UnitOp_Turbine3Q(pIN1,pIN2,pIN3,pIN4,pIN5,pIN6, pIN7,pIN8,pIN9,pIN10,pIN11,pOUT1,pOUT2,pOUT3,pOUT4); break; 
            case eTURB1M:
                *status = (double)JStm->UnitOp_Turbine1M(pIN1,pIN2,pIN3,pIN4,pIN5,pOUT1,pOUT2); break;            
            case eTURB2M:
                *status = (double)JStm->UnitOp_Turbine2M(pIN1,pIN2,pIN3,pIN4,pIN5,pIN6,pIN7,pIN8,pOUT1,pOUT2,pOUT3); break;
            case eTURB3M:
                *status = (double)JStm->UnitOp_Turbine3M(pIN1,pIN2,pIN3,pIN4,pIN5,pIN6, pIN7,pIN8,pIN9,pIN10,pIN11,pOUT1,pOUT2,pOUT3,pOUT4); break;
               
            //Customizable Fluid Operations
            case eCOMP1Qc:
            case eCOMP2Qc:
            case eCOMP1Mc:
            case eCOMP2Mc:
            case eTURB1Qc:  
            case eTURB2Qc:
            case eTURB3Qc:  
            case eTURB1Mc:  
            case eTURB2Mc:  
            case eTURB3Mc:  
            case eCOOLMQc:
            case eCOOLTMc:
            case eCOOLHMc:
            case eCOOLTQc:
            case eHEATMQc:
            case eHEATTMc:
            case eHEATHMc:
            case eHEATTQc:
            case eFLASHc:
            case ePUMPc:  
            case eVALVEc:
                //Create Mixture
                cellToMix(prhs[eIN1],Refrig);                
                in2 = pIN2, in3 = pIN3, in4 = pIN4; //note this local memory is required! something to do with REFPROP
                switch(model)
                {
                    case eCOMP1Qc:
                        in5 = pIN5; in6 = pIN6;
                        *status = (double)JStm->UnitOp_Compressor1Q(&Refrig,in2,in3,in4,in5,in6,pOUT1,pOUT2); break;                
                    case eCOMP2Qc:
                        in5 = pIN5; in6 = pIN6; in7 = pIN7; in8 = pIN8; in9 = pIN9;
                        *status = (double)JStm->UnitOp_Compressor2Q(&Refrig,in2,in3,in4,in5,in6,in7,in8,in9,pOUT1,pOUT2,pOUT3); break;                
                    case eCOMP1Mc:
                        in5 = pIN5; in6 = pIN6;
                        *status = (double)JStm->UnitOp_Compressor1M(&Refrig,in2,in3,in4,in5,in6,pOUT1,pOUT2); break;            
                    case eCOMP2Mc:
                        in5 = pIN5; in6 = pIN6; in7 = pIN7; in8 = pIN8; in9 = pIN9;
                        *status = (double)JStm->UnitOp_Compressor2M(&Refrig,in2,in3,in4,in5,in6,in7,in8,in9,pOUT1,pOUT2,pOUT3); break;
                    case eCOOLMQc:
                        in5 = pIN5; 
                        *status = (double)JStm->UnitOp_CoolerMQ(&Refrig,in2,in3,in4,in5,pOUT1,pOUT2); break;
                    case eCOOLTMc:
                        in5 = pIN5; 
                        *status = (double)JStm->UnitOp_CoolerTM(&Refrig,in2,in3,in4,in5,pOUT1,pOUT2); break;  
                    case eCOOLHMc:
                        in5 = pIN5; 
                        *status = (double)JStm->UnitOp_CoolerHM(&Refrig,in2,in3,in4,in5,pOUT1,pOUT2); break;
                    case eCOOLTQc:
                        in5 = pIN5; 
                        *status = (double)JStm->UnitOp_CoolerTQ(&Refrig,in2,in3,in4,in5,pOUT1,pOUT2); break;               
                    case eHEATMQc:
                        in5 = pIN5;
                        *status = (double)JStm->UnitOp_HeaterMQ(&Refrig,in2,in3,in4,in5,pOUT1,pOUT2); break;               
                    case eHEATTMc:
                        in5 = pIN5; 
                        *status = (double)JStm->UnitOp_HeaterTM(&Refrig,in2,in3,in4,in5,pOUT1,pOUT2); break;     
                    case eHEATHMc:
                        in5 = pIN5; 
                        *status = (double)JStm->UnitOp_HeaterHM(&Refrig,in2,in3,in4,in5,pOUT1,pOUT2); break;
                    case eHEATTQc:
                        in5 = pIN5; 
                        *status = (double)JStm->UnitOp_HeaterTQ(&Refrig,in2,in3,in4,in5,pOUT1,pOUT2); break;                     
                    //case eFLASHc: //NOT CURRENTLY WORKING
                    //    *status = (double)JStm->UnitOp_FlashDrum(&Refrig,pIN1,pIN2,pIN3,pOUT1,pOUT2,pOUT3,pOUT4); break;                
                    case ePUMPc:
                        in5 = pIN5; in6 = pIN6;
                        *status = (double)JStm->UnitOp_Pump(&Refrig,in2,in3,in4,in5,in6,pOUT1,pOUT2); break;               
                    case eVALVEc:
                        *status = (double)JStm->UnitOp_Valve(&Refrig,in2,in3,in4,pOUT1,pOUT2); break;                    
                    case eTURB1Qc:     
                        in5 = pIN5; in6 = pIN6;
                        *status = (double)JStm->UnitOp_Turbine1Q(&Refrig,in2,in3,in4,in5,in6,pOUT1,pOUT2); break;
                    case eTURB2Qc:
                        in5 = pIN5; in6 = pIN6; in7 = pIN7; in8 = pIN8; in9 = pIN9;
                        *status = (double)JStm->UnitOp_Turbine2Q(&Refrig,in2,in3,in4,in5,in6,in7,in8,in9,pOUT1,pOUT2,pOUT3); break;  
                    case eTURB3Qc:
                         in5 = pIN5; in6 = pIN6; in7 = pIN7; in8 = pIN8; in9 = pIN9; in10 = pIN10; in11 = pIN11; in12 = pIN12;
                        *status = (double)JStm->UnitOp_Turbine3Q(&Refrig,in2,in3,in4,in5,in6,in7,in8,in9,in10,in11,in12,pOUT1,pOUT2,pOUT3,pOUT4); break; 
                    case eTURB1Mc:
                        in5 = pIN5; in6 = pIN6;
                        *status = (double)JStm->UnitOp_Turbine1M(&Refrig,in2,in3,in4,in5,in6,pOUT1,pOUT2); break;            
                    case eTURB2Mc:
                        in5 = pIN5; in6 = pIN6; in7 = pIN7; in8 = pIN8; in9 = pIN9;
                        *status = (double)JStm->UnitOp_Turbine2M(&Refrig,in2,in3,in4,in5,in6,in7,in8,in9,pOUT1,pOUT2,pOUT3); break;
                    case eTURB3Mc:
                        in5 = pIN5; in6 = pIN6; in7 = pIN7; in8 = pIN8; in9 = pIN9; in10 = pIN10; in11 = pIN11; in12 = pIN12;
                        *status = (double)JStm->UnitOp_Turbine3M(&Refrig,in2,in3,in4,in5,in6,in7,in8,in9,in10,in11,in12,pOUT1,pOUT2,pOUT3,pOUT4); break;
                }
                break;
                
            //Combustion Unit Operations
            case eFURNM:
            case eFURNQ:
            case eBLRFRD:
            case eGTG:
            case eGTGINJ:
            case eHRSG:
            case eHRSGSup:
                               
                //Create Fuel and Air Mixtures
                Mixture Fuel, SecFuel, Air, Stack;                                                         
                //Solve
                switch(model)
                {
                    case eFURNM: //[status,airM,Duty,FEff,acidDp,Stack] = JSteamMEX('UnitOp_FuranceM',{Fuel_Mixture},fuelT,airT,airP,airRelHum,minStackT,O2,O2Mode,fuelM)
                        in2 = pIN2, in3 = pIN3, in4 = pIN4; in5 = pIN5; in6 = pIN6; in7 = pIN7; in8 = pIN8; in9 = pIN9;
                        cellToMix(prhs[eIN1],Fuel); Air.SetAsAir(in5,in4,in3);
                        *status = (double)JStm->UnitOp_FurnaceM(&Fuel,&Air,in2,in3,in6,in7,(int)in8,in9,&Stack,pOUT1,pOUT2,pOUT3);
                        *pOUT4 = Stack.AcidDewPoint();
                        mixToCell(Stack,&plhs[eOUT5]);
                        break;                        
                    case eFURNQ: //[status,airM,fuelM,FEff,acidDp,Stack] = JSteamMEX('UnitOp_FuranceQ',{Fuel_Mixture},fuelT,airT,airP,airRelHum,minStackT,O2,O2Mode,Duty)
                        in2 = pIN2, in3 = pIN3, in4 = pIN4; in5 = pIN5; in6 = pIN6; in7 = pIN7; in8 = pIN8; in9 = pIN9;
                        cellToMix(prhs[eIN1],Fuel); Air.SetAsAir(in5,in4,in3);
                        *status = (double)JStm->UnitOp_FurnaceQ(&Fuel,&Air,in2,in3,in6,in7,(int)in8,in9,&Stack,pOUT1,pOUT2,pOUT3);
                        *pOUT4 = Stack.AcidDewPoint();
                        mixToCell(Stack,&plhs[eOUT5]);
                        break;                        
                    case eBLRFRD: //[status,StmH,BFWM,airM,fuelM,FEff,Duty,Stack] = JSteamMEX('UnitOp_BoilerFired',BFWH,StmM,StmT,StmP,BDr,{Fuel_Mixture},fuelT,airT,airP,airRelHum,minStackT,O2,O2Mode)
                        in1 = pIN1; in2 = pIN2, in3 = pIN3, in4 = pIN4; in5 = pIN5; in7 = pIN7; in8 = pIN8; in9 = pIN9; in10 = pIN10; in11 = pIN11; in12 = pIN12; in13 = pIN13;
                        cellToMix(prhs[eIN6],Fuel); Air.SetAsAir(in10,in9,in8);
                        *status = (double)JStm->UnitOp_BoilerFired(in1,in2,in3,in4,in5,&Fuel,&Air,in7,in8,in11,in12,(int)in13,&Stack,pOUT1,pOUT2,pOUT3,pOUT4,pOUT5,pOUT6);
                        mixToCell(Stack,&plhs[eOUT7]);
                        break;                        
                    case eGTG: //[status,airM,fuelM,exhaustM,Exhaust] = JSteamMEX('UnitOp_GasTurbine',{Fuel_Mixture},fuelT,airT,airP,airRelHum,exhaustT,presRatio,compEff,GTPower,GTEff)
                        in2 = pIN2, in3 = pIN3, in4 = pIN4; in5 = pIN5; in6 = pIN6; in7 = pIN7; in8 = pIN8; in9 = pIN9; in10 = pIN10;
                        cellToMix(prhs[eIN1],Fuel); Air.SetAsAir(in5,in4,in3);
                        *status = (double)JStm->UnitOp_GasTurbine(&Fuel,&Air,in2,in3,in6,in4,in7,in8,in9,in10,&Stack,pOUT1,pOUT2,pOUT3);
                        mixToCell(Stack,&plhs[eOUT4]);
                        break;                        
                    case eGTGINJ: //[status,airM,fuelM,exhaustM,Exhaust] = JSteamMEX('UnitOp_GasTurbineInj',{Fuel_Mixture},fuelT,airT,airP,airRelHum,exhaustT,presRatio,compEff,StmT,StmP,StmM,GTPower,GTEff)
                        in2 = pIN2, in3 = pIN3, in4 = pIN4; in5 = pIN5; in6 = pIN6; in7 = pIN7; in8 = pIN8; in9 = pIN9; in10 = pIN10; in11 = pIN11; in12 = pIN12; in13 = pIN13;
                        cellToMix(prhs[eIN1],Fuel); Air.SetAsAir(in5,in4,in3);
                        *status = (double)JStm->UnitOp_GasTurbineInj(&Fuel,&Air,in2,in3,in6,in4,in7,in8,in9,in10,in11,in12,in13,&Stack,pOUT1,pOUT2,pOUT3);
                        mixToCell(Stack,&plhs[eOUT4]);
                        break;
                        
                    case eHRSG: //[status,StmM,econDT,sprHDT,stackT,Stack] = JSteamMEX('UnitOp_HRSG',{GT_Exhaust},exhaustT,exhaustM,minAppT,minStackT,BFWH,StmT,StmP,BDr)
                        in2 = pIN2, in3 = pIN3, in4 = pIN4; in5 = pIN5; in6 = pIN6; in7 = pIN7; in8 = pIN8; in9 = pIN9;
                        cellToMix(prhs[eIN1],Fuel); 
                        *status = (double)JStm->UnitOp_HRSG(&Fuel,in2,in3,in4,in5,in6,in7,in8,in9,&Stack,pOUT1,pOUT2,pOUT3,pOUT4);
                        mixToCell(Stack,&plhs[eOUT5]);
                        break;
  
                    case eHRSGSup: //[status,StmM,BaseStmM,MaxStmM,SecFuelM,econDT,sprHDT,stackT,stackM,Stack] = JSteamMEX('UnitOp_HRSGSup',{GT_Exhaust},{SecFuel_Mixture},OpMode,exhaustT,exhaustM,secFuelT,minAppT,minStackT,minStackO2,StmDemand,BFWH,StmT,StmP,BDr)
                        in3 = pIN3, in4 = pIN4; in5 = pIN5; in6 = pIN6; in7 = pIN7; in8 = pIN8; in9 = pIN9; in10 = pIN10; in11 = pIN11; in12 = pIN12; in13 = pIN13; in14 = pIN14;
                        cellToMix(prhs[eIN1],Fuel); cellToMix(prhs[eIN2],SecFuel);
                        *status = (double)JStm->UnitOp_HRSGSup(&Fuel,&SecFuel,(int)in3,in4,in5,in6,in7,in8,in9,in10,in11,in12,in13,in14,&Stack,pOUT1,pOUT2,pOUT3,pOUT4,pOUT5,pOUT6,pOUT7,pOUT8);
                        mixToCell(Stack,&plhs[eOUT9]);
                        break;
                }
                break;
        }
    }    
    
    //Match UnitOp Function
    int JSteamSolve::matchFcn(char *name, JSUFcn*& match)
    {
        //Get First Entry
        JSUFcn *s = &JSUFcns[0];
        //Match the name
        while(s && s->name) {
            if(strcmpi(name, s->name)) {
                match = s;
                return 1;
            }
            s++;
        }     
        match = NULL;
        return 0;
    }   
}