/* JSTEAM MATLAB MEX INTERFACE
 * Parallel Solver Interface
 * J.Currie 2012-2014 */

#include <mex.h>
#include <ppl.h>
#include "include\JSteamSolve.h"
#include "include\JSteamUtils.h"

using namespace JSteamDLL;
using namespace Concurrency;

namespace JSteamMEX
{   
    //Thermodynamic & Transport Functions
    JSFcn JSFcns[] = {
        {"-------------------------------------------",NULL,NULL,NULL,NULL,NULL},
        {"WATER & STEAM FUNCTIONS [IAPWS IF97]\ne.g. JSteamMEX('HPT',1,100)\n",NULL,NULL,NULL,NULL,NULL},
        {" - Isobaric Heat Capacity (default kJ/(kg.K))",NULL,NULL,NULL,NULL,NULL},
        {"CpPH",NULL,&JSteam::CpPH,NULL,NULL,NULL},
        {"CpPS",NULL,&JSteam::CpPS,NULL,NULL,NULL},
        {"CpPT",NULL,&JSteam::CpPT,NULL,NULL,NULL},
        {"CpPX",NULL,&JSteam::CpPX,NULL,NULL,NULL},
        {"CpTX",NULL,&JSteam::CpTX,NULL,NULL,NULL},
        {" - Isochoric Heat Capacity default (kJ/(kg.K))",NULL,NULL,NULL,NULL,NULL},
        {"CvPH",NULL,&JSteam::CvPH,NULL,NULL,NULL},
        {"CvPS",NULL,&JSteam::CvPS,NULL,NULL,NULL},
        {"CvPT",NULL,&JSteam::CvPT,NULL,NULL,NULL},
        {"CvPX",NULL,&JSteam::CvPX,NULL,NULL,NULL},
        {"CvTX",NULL,&JSteam::CvTX,NULL,NULL,NULL},
        {" - Enthalpy default (kJ/kg)",NULL,NULL,NULL,NULL,NULL},
        {"HPS",NULL,&JSteam::HPS,NULL,NULL,NULL},
        {"HPT",NULL,&JSteam::HPT,NULL,NULL,NULL},
        {"HTS",NULL,&JSteam::HTS,NULL,NULL,NULL},
        {"HPX",NULL,&JSteam::HPX,NULL,NULL,NULL},
        {"HTX",NULL,&JSteam::HTX,NULL,NULL,NULL},
        {" - Entropy (default kJ/(kg.K))",NULL,NULL,NULL,NULL,NULL},
        {"SPH",NULL,&JSteam::SPH,NULL,NULL,NULL},
        {"SPT",NULL,&JSteam::SPT,NULL,NULL,NULL},
        {"SPX",NULL,&JSteam::SPX,NULL,NULL,NULL},
        {"STX",NULL,&JSteam::STX,NULL,NULL,NULL}, 
        {" - Pressure (default bar)",NULL,NULL,NULL,NULL,NULL},
        {"PHS",NULL,&JSteam::PHS,NULL,NULL,NULL},
        {"PTS",NULL,&JSteam::PTS,NULL,NULL,NULL},
        {"PsatT",&JSteam::PsatT,NULL,NULL,NULL,NULL}, 
        {" - Quality (fraction)",NULL,NULL,NULL,NULL,NULL},
        {"XPH",NULL,&JSteam::XPH,NULL,NULL,NULL},
        {"XPS",NULL,&JSteam::XPS,NULL,NULL,NULL},
        {"XTS",NULL,&JSteam::XTS,NULL,NULL,NULL}, 
        {" - Temperature (default C)",NULL,NULL,NULL,NULL,NULL},
        {"THS",NULL,&JSteam::THS,NULL,NULL,NULL},
        {"TPH",NULL,&JSteam::TPH,NULL,NULL,NULL},
        {"TPS",NULL,&JSteam::TPS,NULL,NULL,NULL},
        {"TSX",NULL,&JSteam::TSX,NULL,NULL,NULL},
        {"TsatP",&JSteam::TsatP,NULL,NULL,NULL,NULL},
        {" - Volume (default m3/kg)",NULL,NULL,NULL,NULL,NULL},
        {"VPH",NULL,&JSteam::VPH,NULL,NULL,NULL},
        {"VPS",NULL,&JSteam::VPS,NULL,NULL,NULL},
        {"VPT",NULL,&JSteam::VPT,NULL,NULL,NULL},
        {"VPX",NULL,&JSteam::VPX,NULL,NULL,NULL},
        {"VTX",NULL,&JSteam::VTX,NULL,NULL,NULL},
        {" - Thermal Conductivity (default mW/(m.K))",NULL,NULL,NULL,NULL,NULL},
        {"KPT",NULL,&JSteam::KPT,NULL,NULL,NULL},
        {" - Viscosity (default uPa.s)",NULL,NULL,NULL,NULL,NULL},
        {"UPT",NULL,&JSteam::UPT,NULL,NULL,NULL},
        {"\n-------------------------------------------",NULL,NULL,NULL,NULL,NULL},
        {"PURE FLUID FUNCTIONS [REFPROP]\ne.g. JSteamMEX('HcPT','Methane',20,150)\n",NULL,NULL,NULL,NULL,NULL},
        {" - Isobaric Heat Capacity (default kJ/(kg.K))",NULL,NULL,NULL,NULL,NULL},
        {"CpcPH",NULL,NULL,NULL,NULL,&JSteam::CpcPH},
        {"CpcPS",NULL,NULL,NULL,NULL,&JSteam::CpcPS},
        {"CpcPT",NULL,NULL,NULL,NULL,&JSteam::CpcPT},
        {"CpcPX",NULL,NULL,NULL,NULL,&JSteam::CpcPX},
        {"CpcTX",NULL,NULL,NULL,NULL,&JSteam::CpcTX},
        {" - Isochoric Heat Capacity (default kJ/(kg.K))",NULL,NULL,NULL,NULL,NULL},
        {"CvcPH",NULL,NULL,NULL,NULL,&JSteam::CvcPH},
        {"CvcPS",NULL,NULL,NULL,NULL,&JSteam::CvcPS},
        {"CvcPT",NULL,NULL,NULL,NULL,&JSteam::CvcPT},
        {"CvcPX",NULL,NULL,NULL,NULL,&JSteam::CvcPX},
        {"CvcTX",NULL,NULL,NULL,NULL,&JSteam::CvcTX},
        {" - Enthalpy (default kJ/kg)",NULL,NULL,NULL,NULL,NULL},        
        {"HcPS",NULL,NULL,NULL,NULL,&JSteam::HcPS},
        {"HcPT",NULL,NULL,NULL,NULL,&JSteam::HcPT},
        {"HcPTvap",NULL,NULL,NULL,NULL,&JSteam::HcPTvap},
        {"HcPX",NULL,NULL,NULL,NULL,&JSteam::HcPX},
        {"HcTX",NULL,NULL,NULL,NULL,&JSteam::HcTX},
        {" - Entropy (default kJ/(kg K))",NULL,NULL,NULL,NULL,NULL},        
        {"ScPH",NULL,NULL,NULL,NULL,&JSteam::ScPH},
        {"ScPT",NULL,NULL,NULL,NULL,&JSteam::ScPT},
        {"ScPTvap",NULL,NULL,NULL,NULL,&JSteam::ScPTvap},
        {"ScPX",NULL,NULL,NULL,NULL,&JSteam::ScPX},
        {"ScTX",NULL,NULL,NULL,NULL,&JSteam::ScTX},
        {" - Pressure (default bar)",NULL,NULL,NULL,NULL,NULL},        
        {"PcHS",NULL,NULL,NULL,NULL,&JSteam::PcHS},
        {"PsatcT",NULL,NULL,NULL,&JSteam::PsatcT,NULL},
        {"PsatcD",NULL,NULL,NULL,&JSteam::PsatcD,NULL},
        {" - Quality (fraction)",NULL,NULL,NULL,NULL,NULL},        
        {"XcPH",NULL,NULL,NULL,NULL,&JSteam::XcPH},        
        {"XcPS",NULL,NULL,NULL,NULL,&JSteam::XcPS},        
        {"XcPT",NULL,NULL,NULL,NULL,&JSteam::XcPT},                
        {" - Temperature (default C)",NULL,NULL,NULL,NULL,NULL},        
        {"TcHS",NULL,NULL,NULL,NULL,&JSteam::TcHS},
        {"TcPS",NULL,NULL,NULL,NULL,&JSteam::TcPS},
        {"TcPH",NULL,NULL,NULL,NULL,&JSteam::TcPH},
        {"TsatcP",NULL,NULL,NULL,&JSteam::TsatcP,NULL},
        {"TsatcD",NULL,NULL,NULL,&JSteam::TsatcD,NULL},
        {" - Volume (default m3/kg)",NULL,NULL,NULL,NULL,NULL},
        {"VcPH",NULL,NULL,NULL,NULL,&JSteam::VcPH},
        {"VcPS",NULL,NULL,NULL,NULL,&JSteam::VcPS},
        {"VcPT",NULL,NULL,NULL,NULL,&JSteam::VcPT},
        {"VcPX",NULL,NULL,NULL,NULL,&JSteam::VcPX},
        {"VcTX",NULL,NULL,NULL,NULL,&JSteam::VcTX},
        {" - Thermal Conductivity (default mW/(m.K))",NULL,NULL,NULL,NULL,NULL},
        {"KcPH",NULL,NULL,NULL,NULL,&JSteam::KcPH},
        {"KcPS",NULL,NULL,NULL,NULL,&JSteam::KcPS},
        {"KcPT",NULL,NULL,NULL,NULL,&JSteam::KcPT},
        {"KcPX",NULL,NULL,NULL,NULL,&JSteam::KcPX},
        {"KcTX",NULL,NULL,NULL,NULL,&JSteam::KcTX},
        {" - Viscosity (default uPa.s)",NULL,NULL,NULL,NULL,NULL},
        {"UcPH",NULL,NULL,NULL,NULL,&JSteam::UcPH},
        {"UcPS",NULL,NULL,NULL,NULL,&JSteam::UcPS},
        {"UcPT",NULL,NULL,NULL,NULL,&JSteam::UcPT},
        {"UcPX",NULL,NULL,NULL,NULL,&JSteam::UcPX},
        {"UcTX",NULL,NULL,NULL,NULL,&JSteam::UcTX},
        {" - Heating Value (default kJ/kg)",NULL,NULL,NULL,NULL,NULL},
        {"NHV",NULL,NULL,&JSteam::NHV,NULL,NULL},
        {"GHV",NULL,NULL,&JSteam::GHV,NULL,NULL},
        {" - Other",NULL,NULL,NULL,NULL,NULL},
        {"MW",NULL,NULL,&JSteam::MW,NULL,NULL},
        {"[Tc,Pc,Vc,TTrip,TNBpt,w,TMax,TMin,DMax,PMax] = JSteamMEX('Info','Fluid')",NULL,NULL,NULL,NULL,NULL},
        {"\n-------------------------------------------",NULL,NULL,NULL,NULL,NULL},
        {NULL,NULL,NULL,NULL,NULL,NULL,}
    };         
    
    //Mixture Functions
    JSMFcn JSMFcns[] = {
        {"-------------------------------------------",NULL,NULL,NULL},
        {"MIXTURE FUNCTIONS [REFPROP]\ne.g. JSteamMEX('HmPT',{'nButane',0.5;'nPentane',0.5},20,150)\n",NULL,NULL,NULL},
        {" - Isobaric Heat Capacity (default kJ/(kg.K))",NULL,NULL,NULL},
        {"CpmPH",NULL,NULL,&Mixture::CpcPH},
        {"CpmPS",NULL,NULL,&Mixture::CpcPS},
        {"CpmPT",NULL,NULL,&Mixture::CpcPT},
        {"CpmPX",NULL,NULL,&Mixture::CpcPX},
        {"CpmTX",NULL,NULL,&Mixture::CpcTX},
        {" - Isochoric Heat Capacity (default kJ/(kg.K))",NULL,NULL,NULL},
        {"CvmPH",NULL,NULL,&Mixture::CvcPH},
        {"CvmPS",NULL,NULL,&Mixture::CvcPS},
        {"CvmPT",NULL,NULL,&Mixture::CvcPT},
        {"CvmPX",NULL,NULL,&Mixture::CvcPX},
        {"CvmTX",NULL,NULL,&Mixture::CvcTX},
        {" - Enthalpy (default kJ/kg)",NULL,NULL,NULL},        
        {"HmPS",NULL,NULL,&Mixture::HcPS},
        {"HmPT",NULL,NULL,&Mixture::HcPT},
        {"HmPTvap",NULL,NULL,&Mixture::HcPTvap},
        {"HmPX",NULL,NULL,&Mixture::HcPX},
        {"HmTX",NULL,NULL,&Mixture::HcTX},       
        {" - Entropy (default kJ/(kg.K))",NULL,NULL,NULL},        
        {"SmPH",NULL,NULL,&Mixture::ScPH},
        {"SmPT",NULL,NULL,&Mixture::ScPT},
        {"SmPTvap",NULL,NULL,&Mixture::ScPTvap},
        {"SmPX",NULL,NULL,&Mixture::ScPX},
        {"SmTX",NULL,NULL,&Mixture::ScTX},
        {" - Pressure (default bar)",NULL,NULL,NULL},        
        {"PmHS",NULL,NULL,&Mixture::PcHS},
        {"PsatmT",NULL,&Mixture::PsatcT,NULL},
        {"PsatmD",NULL,&Mixture::PsatcD,NULL},
        {" - Quality (fraction)",NULL,NULL,NULL},        
        {"XmPH",NULL,NULL,&Mixture::XcPH},        
        {"XmPS",NULL,NULL,&Mixture::XcPS},        
        {"XmPT",NULL,NULL,&Mixture::XcPT},                
        {" - Temperature (default C)",NULL,NULL,NULL},        
        {"TmHS",NULL,NULL,&Mixture::TcHS},
        {"TmPS",NULL,NULL,&Mixture::TcPS},
        {"TmPH",NULL,NULL,&Mixture::TcPH},
        {"TsatmP",NULL,&Mixture::TsatcP,NULL},
        {"TsatmD",NULL,&Mixture::TsatcD,NULL},
        {" - Volume (default m3/kg)",NULL,NULL,NULL},
        {"VmPH",NULL,NULL,&Mixture::VcPH},
        {"VmPS",NULL,NULL,&Mixture::VcPS},
        {"VmPT",NULL,NULL,&Mixture::VcPT},
        {"VmPX",NULL,NULL,&Mixture::VcPX},
        {"VmTX",NULL,NULL,&Mixture::VcTX},
        {" - Thermal Conductivity (default mW/(m.K))",NULL,NULL,NULL},
        {"KmPH",NULL,NULL,&Mixture::KcPH},
        {"KmPS",NULL,NULL,&Mixture::KcPS},
        {"KmPT",NULL,NULL,&Mixture::KcPT},
        {"KmPX",NULL,NULL,&Mixture::KcPX},
        {"KmTX",NULL,NULL,&Mixture::KcTX},
        {" - Viscosity (default uPa.s)",NULL,NULL,NULL},
        {"UmPH",NULL,NULL,&Mixture::UcPH},
        {"UmPS",NULL,NULL,&Mixture::UcPS},
        {"UmPT",NULL,NULL,&Mixture::UcPT},
        {"UmPX",NULL,NULL,&Mixture::UcPX},
        {"UmTX",NULL,NULL,&Mixture::UcTX},
        {" - Heating Value (default kJ/kg)",NULL,NULL,NULL},
        {"NHV",&Mixture::NHV,NULL,NULL},
        {"GHV",&Mixture::GHV,NULL,NULL},
        {" - Other",NULL,NULL,NULL},
        {"MW",&Mixture::MW,NULL,NULL},
        {"[Tc,Pc,Vc,TTrip,TNBpt,w,TMax,TMin,DMax,PMax] = JSteamMEX('Info',{Mixture})",NULL,NULL,NULL},
        {"\n-------------------------------------------",NULL,NULL,NULL},
        {NULL,NULL,NULL,NULL}
    };
    
    void JSteamSolve::Solve(JSteam *JStm, char *name, char *component, double *in1, double *in2, double *sol, size_t m1, size_t n1)
    {
        char buf[1024];
        int mode = 0;
        //Check if we are solving steam or general fun
        if(component==NULL) {//steam
            //Find Function
            int fidx = matchFcn(name,JSFcns,mode);
            if(fidx < 0) {
                sprintf(buf,"Could not match requested function [%s] to JSteam Water/Steam Function. Use JSteamMEX('help') to see all functions.",name);
                mexErrMsgTxt(buf);
            }
            switch(mode)
            {
                case 0: //D1
                    if(in1 == NULL) {
                        sprintf(buf,"%s requires one input",name); mexErrMsgTxt(buf);
                    }
                    parallelSolve(JStm,JSFcns[fidx].fcnD1,in1,sol,m1,n1);
                    break;
                case 1: //D2
                    if(in1 == NULL || in2 == NULL) {
                        sprintf(buf,"%s requires two inputs",name); mexErrMsgTxt(buf);
                    }
                    parallelSolve(JStm,JSFcns[fidx].fcnD2,in1,in2,sol,m1,n1);
                    break;  
                default:
                     sprintf(buf,"%s is a general component function - please pass a component name as the second argument.",name); mexErrMsgTxt(buf);
                     break;
            }
        }
        else { //general
            //Match user component
            int cidx = JSteam::GetComponentIndex(component);
            if(cidx < 0) {
                sprintf(buf,"Could not match requested component [%s] to JSteam component list",component);
                mexErrMsgTxt(buf);
            }
            //Find Function
            int fidx = matchFcn(name,JSFcns,mode);
            if(fidx < 0) {
                sprintf(buf,"Could not match requested function [%s] to JSteam General Thermodynamic Function. Use JSteamMEX('help') to see all functions.",name);
                mexErrMsgTxt(buf);
            }
            switch(mode)
            {
                case 2: //C1 NOT VECTORIZED
                    sol[0] = (*JStm.*(JSFcns[fidx].fcnC1))((JSteam::Component)cidx);
                    break;
                case 3: //C1D1
                    if(in1 == NULL) {
                        sprintf(buf,"%s requires one input",name); mexErrMsgTxt(buf);
                    }
                    parallelSolve(JStm,JSFcns[fidx].fcnCD,(JSteam::Component)cidx,in1,sol,m1,n1);
                    break;
                case 4: //C1D2
                    if(in1 == NULL || in2 == NULL) {
                        sprintf(buf,"%s requires two inputs",name); mexErrMsgTxt(buf);
                    }
                    parallelSolve(JStm,JSFcns[fidx].fcnCD2,(JSteam::Component)cidx,in1,in2,sol,m1,n1);
                    break;  
                default:
                     sprintf(buf,"%s is a water/steam function - do not pass a component name.",name); mexErrMsgTxt(buf);
                     break;
            }
        }
    }     
    
    void JSteamSolve::SolveMix(JSteam *JStm, char *name, const mxArray *mix, double *in1, double *in2, double *sol, size_t m1, size_t n1)
    {
        char buf[1024];
        int mode = 0;      
        //Find Function
        int fidx = matchFcn(name,JSMFcns,mode);
        if(fidx < 0) {
            sprintf(buf,"Could not match requested function [%s] to JSteam Mixture Thermodynamic Function. Use JSteamMEX('help') to see all functions.",name);
            mexErrMsgTxt(buf);
        }
        //Generate Mixture
        Mixture JSMix; cellToMix(mix,JSMix);
        //Solve Using Parallelized Solver
        switch(mode)
        {
            case 0: //M1 NOT VECTORIZED
                sol[0] = (JSMix.*(JSMFcns[fidx].fcnM1))();
                break;
            case 1: //M1D1
                if(in1 == NULL) {
                    sprintf(buf,"%s requires one input",name); mexErrMsgTxt(buf);
                }
                parallelSolve(JSMFcns[fidx].fcnMD1,JSMix,in1,sol,m1,n1);
                break;
            case 2: //M1D2
                if(in1 == NULL || in2 == NULL) {
                    sprintf(buf,"%s requires two inputs",name); mexErrMsgTxt(buf);
                }
                parallelSolve(JSMFcns[fidx].fcnMD2,JSMix,in1,in2,sol,m1,n1);
                break;  
            default:
                 mexErrMsgTxt("Error determining mixture operation.");
                 break;
        }
    }
    
    void JSteamSolve::Info(JSteam *JStm, char *name, const mxArray *mix, mxArray *plhs[])
    {
        double *Tc, *Pc, *Vc, *TTrip, *TNBpt, *w, *TMax, *TMin, *DMax, *PMax;
        char buf[1024];
        //Create Outputs [Tc,Pc,Vc,TTrip,TNBpt,w,TMax,TMin,DMax,PMax]
        plhs[0] = mxCreateDoubleScalar(NaN); Tc = mxGetPr(plhs[0]);
        plhs[1] = mxCreateDoubleScalar(NaN); Pc = mxGetPr(plhs[1]);
        plhs[2] = mxCreateDoubleScalar(NaN); Vc = mxGetPr(plhs[2]);
        plhs[3] = mxCreateDoubleScalar(NaN); TTrip = mxGetPr(plhs[3]);
        plhs[4] = mxCreateDoubleScalar(NaN); TNBpt = mxGetPr(plhs[4]);
        plhs[5] = mxCreateDoubleScalar(NaN); w = mxGetPr(plhs[5]);
        plhs[6] = mxCreateDoubleScalar(NaN); TMax = mxGetPr(plhs[6]);
        plhs[7] = mxCreateDoubleScalar(NaN); TMin = mxGetPr(plhs[7]);
        plhs[8] = mxCreateDoubleScalar(NaN); DMax = mxGetPr(plhs[8]);
        plhs[9] = mxCreateDoubleScalar(NaN); PMax = mxGetPr(plhs[9]);
        //Call Based on whether Mixture or Pure Fluid
        if(!mxIsCell(mix)) { //pure fluid
            char *fluid = mxArrayToString(mix);
            int cidx = JSteam::GetComponentIndex(fluid);
            double Zc,MW,dip,Rgas;
            if(cidx < 0) {
                sprintf(buf,"Could not match requested component [%s] to JSteam component list",fluid);
                mexErrMsgTxt(buf);
            }
            JStm->ComponentInfo((JSteam::Component)cidx,&MW,TTrip,TNBpt,Tc,Pc,Vc,&Zc,w,&dip,&Rgas);
            JStm->ComponentLimits((JSteam::Component)cidx,TMin,TMax,DMax,PMax);            
            mxFree(fluid);
        }
        else { //mixture            
            Mixture JSMix; cellToMix(mix,JSMix);
            JSMix.GenSatSplines();
            JSMix.CritParams(Tc,Pc,Vc);
            JSMix.Limits(TMin,TMax,DMax,PMax);
        }
    }
    

    int JSteamSolve::matchFcn(char *name, JSFcn *fcns, int &mode)
    {
        JSFcn *j = &fcns[0];
        int idx = 0;
        while(j && j->name) {
            if(strcmpi(name,j->name)) {
                if(j->fcnD1!=NULL)
                    mode = 0;
                else if(j->fcnD2!=NULL)
                    mode = 1;
                else if(j->fcnC1!=NULL)
                    mode = 2;
                else if(j->fcnCD!=NULL)
                    mode = 3;
                else if(j->fcnCD2!=NULL)
                    mode = 4;
                else
                    mexErrMsgTxt("Error identifying function mode...");
                return idx;
            }
            j++;
            idx++;
        }
        return -1; //no luck
    }
    
    int JSteamSolve::matchFcn(char *name, JSMFcn *fcns, int &mode)
    {
        JSMFcn *j = &fcns[0];
        int idx = 0;
        while(j && j->name) {
            if(strcmpi(name,j->name)) {
                if(j->fcnM1!=NULL)
                    mode = 0;
                else if(j->fcnMD1!=NULL)
                    mode = 1;
                else if(j->fcnMD2!=NULL)
                    mode = 2;
                else
                    mexErrMsgTxt("Error identifying function mode...");
                return idx;
            }
            j++;
            idx++;
        }
        return -1; //no luck
    }
            
    //Print Thermo Funcs Help
    void JSteamSolve::Print()
    {
        //Print Steam & General Functions
        JSFcn *j = &JSFcns[0];
        while(j && j->name) {
            mexPrintf("%s\n",j->name);
            j++;
        }        
        //Print Available Components
        mexPrintf("AVAILABLE FLUIDS\n\n%15s         %17s     %17s\n","Short Name","Full Name","Formula");
        for(int i = 0; i < JSteam::GetNoComponents(); i++)
            mexPrintf("%17s [%34s] (%s)\n",JSteam::GetComponentName((JSteam::Component)i,0).c_str(),JSteam::GetComponentName((JSteam::Component)i,1).c_str(),JSteam::GetComponentName((JSteam::Component)i,2).c_str());
        
        mexPrintf("\n");
        //Print Mixture Functions
        JSMFcn *jm = &JSMFcns[0];
        while(jm && jm->name) {
            mexPrintf("%s\n",jm->name);
            jm++;
        }
    }

    //Solving Routines
    void JSteamSolve::parallelSolve(JSteam *JStm, double (JSteam::*fcn)(double),double *in1, double *sol, size_t m1, size_t n1)
    {
        //Scalar
        if(m1 == 1 && n1 == 1)
        {
            sol[0] = (*JStm.*fcn)(in1[0]);
            return;
        }        
        //Vector
        if((m1 > 1 && n1 == 1) || (n1 > 1 && m1 == 1))
        {
            size_t m = max(m1,n1);
            parallel_for((size_t)0,m,(size_t)1,[&](size_t i)
            {     
                sol[i] = (*JStm.*fcn)(in1[i]);
            });
            return;
        }
        //Matrix
        parallel_for((size_t)0,m1,(size_t)1,[&](size_t i)
        {
            for(size_t j = 0; j < n1; j++)
            {
                size_t ind = i+j*m1;
                sol[ind] = (*JStm.*fcn)(in1[ind]);
            }   
        });      
    }
    void JSteamSolve::parallelSolve(JSteam *JStm, double (JSteam::*fcn)(double,double),double *in1, double *in2, double *sol, size_t m1, size_t n1)
    {
        //Scalar
        if(m1 == 1 && n1 == 1)
        {
            sol[0] = (*JStm.*fcn)(in1[0],in2[0]);
            return;
        }        
        //Vector
        if((m1 > 1 && n1 == 1) || (n1 > 1 && m1 == 1))
        {
            size_t m = max(m1,n1);
            parallel_for((size_t)0,m,(size_t)1,[&](size_t i)
            {     
                sol[i] = (*JStm.*fcn)(in1[i],in2[i]);
            });
            return;
        }
        //Matrix
        parallel_for((size_t)0,m1,(size_t)1,[&](size_t i)
        {
            for(size_t j = 0; j < n1; j++)
            {
                size_t ind = i+j*m1;
                sol[ind] = (*JStm.*fcn)(in1[ind],in2[ind]);
            }   
        });      
    }
    void JSteamSolve::parallelSolve(JSteam *JStm, double (JSteam::*fcn)(JSteam::Component,double),JSteam::Component c, double *in1, double *sol, size_t m1, size_t n1)
    {
        //Scalar
        if(m1 == 1 && n1 == 1)
        {
            sol[0] = (*JStm.*fcn)(c,in1[0]);
            return;
        }        
        //Vector
        if((m1 > 1 && n1 == 1) || (n1 > 1 && m1 == 1))
        {
            size_t m = max(m1,n1);
            parallel_for((size_t)0,m,(size_t)1,[&](size_t i)
            {     
                sol[i] = (*JStm.*fcn)(c,in1[i]);
            });
            return;
        }
        //Matrix
        parallel_for((size_t)0,m1,(size_t)1,[&](size_t i)
        {
            for(size_t j = 0; j < n1; j++)
            {
                size_t ind = i+j*m1;
                sol[ind] = (*JStm.*fcn)(c,in1[ind]);
            }   
        });      
    }
    void JSteamSolve::parallelSolve(JSteam *JStm, double (JSteam::*fcn)(JSteam::Component,double,double),JSteam::Component c, double *in1, double *in2, double *sol, size_t m1, size_t n1)
    {
        //Scalar
        if(m1 == 1 && n1 == 1)
        {
            sol[0] = (*JStm.*fcn)(c,in1[0],in2[0]);
            return;
        }        
        //Vector
        if((m1 > 1 && n1 == 1) || (n1 > 1 && m1 == 1))
        {
            size_t m = max(m1,n1);
            parallel_for((size_t)0,m,(size_t)1,[&](size_t i)
            {
                sol[i] = (*JStm.*fcn)(c,in1[i],in2[i]);
            });
            return;
        }
        //Matrix
        parallel_for((size_t)0,m1,(size_t)1,[&](size_t i)
        {
            for(size_t j = 0; j < n1; j++)
            {
                size_t ind = i+j*m1;
                sol[ind] = (*JStm.*fcn)(c,in1[ind],in2[ind]);
            }   
        });
    }
    
    void JSteamSolve::parallelSolve(double (Mixture::*fcn)(double),Mixture &JSMix, double *in1, double *sol, size_t m1, size_t n1)
    {
        //Scalar
        if(m1 == 1 && n1 == 1)
        {
            sol[0] = (JSMix.*fcn)(in1[0]);
            return;
        }        
        //Vector
        if((m1 > 1 && n1 == 1) || (n1 > 1 && m1 == 1))
        {
            size_t m = max(m1,n1);
            parallel_for((size_t)0,m,(size_t)1,[&](size_t i)
            {     
                sol[i] = (JSMix.*fcn)(in1[i]);
            });
            return;
        }
        //Matrix
        parallel_for((size_t)0,m1,(size_t)1,[&](size_t i)
        {
            for(size_t j = 0; j < n1; j++)
            {
                size_t ind = i+j*m1;
                sol[ind] = (JSMix.*fcn)(in1[ind]);
            }   
        });      
    }
    void JSteamSolve::parallelSolve(double (Mixture::*fcn)(double,double),Mixture &JSMix, double *in1, double *in2, double *sol, size_t m1, size_t n1)
    {
        //Scalar
        if(m1 == 1 && n1 == 1)
        {
            sol[0] = (JSMix.*fcn)(in1[0],in2[0]);
            return;
        }  
        //Vector
        if((m1 > 1 && n1 == 1) || (n1 > 1 && m1 == 1))
        {
            size_t m = max(m1,n1);
            parallel_for((size_t)0,m,(size_t)1,[&](size_t i)
            {
                sol[i] = (JSMix.*fcn)(in1[i],in2[i]);
            });
            return;
        }
        //Matrix
        parallel_for((size_t)0,m1,(size_t)1,[&](size_t i)
        {
            for(size_t j = 0; j < n1; j++)
            {
                size_t ind = i+j*m1;
                sol[ind] = (JSMix.*fcn)(in1[ind],in2[ind]);
            }   
        });
    }
}