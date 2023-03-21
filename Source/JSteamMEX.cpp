/* JSTEAM MATLAB MEX INTERFACE
 * Thermo Interface
 * J.Currie November 2012 */

#include <mex.h>
#include <iostream>
#include "include\JSteam.h"
#include "include\JSteamSolve.h"
#include "include\JSteamUtils.h"

using namespace JSteamDLL;

//Argument Enumeration (in expected order of arguments)
enum {eFUNC, eCOMP, eIN1, eIN2, eMW};
//PRHS Defines    
#define pFUNC   prhs[eFUNC]
#define pCOMP   prhs[eCOMP]
#define pIN1    prhs[eIN1]
#define pIN2    prhs[eIN2]
#define pMW     prhs[eMW]

namespace JSteamMEX
{     
    //Variables saved between calls
    static JSteam *JStm = NULL;
    static JSteamSolve *JSolve = NULL;
    //Prototypes
    bool loadJSteam(bool printWarn, bool verb=true);
    void MexExit(void);
    int readInstr(const mxArray *prhs[], int nrhs, mxArray *plhs[], int nlhs);
    int checkInputs(const mxArray *prhs[], int nrhs, size_t &m1, size_t &n1, size_t &m2, size_t &n2);
    
    void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
    {
        //Variables are assumed in the following order
        char *func, *comp = NULL;
        double *in1 = NULL, *in2 = NULL;        
        //Outputs
        double *sol;
        
        //Internal Variables
        size_t m1 = 1, n1 = 1, m2 = 0, n2 = 0, i;
        mxArray *p = NULL;
        double *p1 = NULL, *p2 = NULL;
        
        //Check for general instruction (i.e. Load, SetUnit...)
        int rc = readInstr(prhs,nrhs,plhs,nlhs);
        if(rc == 0)
            return; //no further execution required
        else if(rc == 2) {
            //Solve Unit Operation then return
            JSolve->SolveUO(JStm,prhs,nrhs,plhs);
            return;
        }
        //Check Inputs & Get Sizes
        if(!checkInputs(prhs,nrhs,m1,n1,m2,n2))
            return;       
        
        //Collect Inputs
        func = mxArrayToString(pFUNC);
        //Check for component string
        if(nrhs > eCOMP) {
            if(mxIsChar(pCOMP) || mxIsCell(pCOMP)) { //general thermo function OR mixture         
                if(!mxIsCell(pCOMP))
                    comp = mxArrayToString(pCOMP);
                if(nrhs > eIN1)
                    in1 = mxGetPr(pIN1);
                if(nrhs > eIN2)
                    in2 = mxGetPr(pIN2);
            }
            else {//steam function            
                in1 = mxGetPr(pCOMP);
                if(nrhs > eIN1)
                    in2 = mxGetPr(pIN1);
            }
        }
        
        //Optional Scalar Expansion
        if(m2 > 0 && n2 > 0) {
            if(m1==1 && n1==1 && (m2 > 1 || n2 > 1)) { //in2 vector/matrix
                p = mxCreateDoubleMatrix(m2,n2,mxREAL); p1 = mxGetPr(p);
                for(i=0;i<m2*n2;i++)
                    p1[i] = *in1;
                p2 = in2;
                m1 = m2; n1 = n2;               
            }
            else if(m2==1 && n2==1 && (m1 > 1 || n1 > 1)) { //in1 vector/matrix
                p = mxCreateDoubleMatrix(m1,n1,mxREAL); p2 = mxGetPr(p);
                for(i=0;i<m1*n1;i++)
                    p2[i] = *in2;
                p1 = in1;
            }
            else { //no changes required
                p1 = in1;
                p2 = in2;
            } 
        }
        else {
            p1 = in1;
            p2 = in2;
        }          
        
        //Process Info Functions Separately
        if(strcmpi("Info",func)) {
            //Routine Creates Outputs
            JSolve->Info(JStm,func,pCOMP,plhs);
        }
        else {        
            //Create Output
            plhs[0] = mxCreateDoubleMatrix(m1,n1,mxREAL);
            sol = mxGetPr(plhs[0]);

            //Call Parallel Solver based on pure fluid or mixture
            if(mxIsCell(pCOMP))
                JSolve->SolveMix(JStm,func,pCOMP,p1,p2,sol,m1,n1);
            else //pure fluid
                JSolve->Solve(JStm,func,comp,p1,p2,sol,m1,n1);
        }
               
        //Free Memory
        mxFree(func);
        if(comp!=NULL){mxFree(comp); comp=NULL;}
        if(p!=NULL) {mxDestroyArray(p); p = NULL;}
    }
    
    //Check User Arguments
    int checkInputs(const mxArray *prhs[], int nrhs, size_t &m1, size_t &n1, size_t &m2, size_t &n2)
    {
        //We always require at least two inputs
        if(nrhs < 2)
            mexErrMsgTxt("JSteamMEX requires at least two input arguments");
        if(!mxIsChar(pFUNC))
            mexErrMsgTxt("The function name must be a string!");
        //Ensure JSteam is Loaded
        loadJSteam(true);
        char *func = mxArrayToString(pFUNC);
        m2 = 0; n2 = 0;
        
        //Check steam functions
        if(!mxIsChar(pCOMP) && !mxIsCell(pCOMP))
        {
            //Always get this size
            m1 = mxGetM(pCOMP);
            n1 = mxGetN(pCOMP);
            //Check data type
            if(mxIsEmpty(pCOMP) || !mxIsDouble(pCOMP) || mxIsSparse(pCOMP) || mxIsComplex(pCOMP))
                mexErrMsgTxt("Input 1 must be a real, dense, double scalar/vector/matrix");            
            
            //If 1 input argument, then ASSUME TsatP or PsatT, already got size so return
            if(nrhs == (eCOMP + 1))                
                return 1;
            //Else check dimensions of both inputs
            else
            {
                if(mxIsEmpty(pIN1) || !mxIsDouble(pIN1) || mxIsSparse(pIN1) || mxIsComplex(pIN1))
                    mexErrMsgTxt("Input 2 must be a real, dense, double scalar/vector/matrix"); 
                
                m2 = mxGetM(pIN1);
                n2 = mxGetN(pIN1);
                if((m1 > 1 || n1 > 1) && (m2 > 1 || n2 > 1)) {
                    if(m1*n1 != m2*n2)
                        mexErrMsgTxt("If both inputs are non-scalar, they must have the same number of elements");
                    if(m1!=m2 || n1!=n2)
                        mexErrMsgTxt("If both inputs are non-scalar, they must have the same size dimensions");
                }
            }     
        }
        //Check fuel/mixture functions
        else
        {
            m1 = 1; n1 = 1; //defaults           
            //If no further args, must be MW, NHV, etc [JSteamMEX('MW','methane')]
            if(nrhs == (eCOMP + 1))
                return 1;
            //Else if one input argument, PsatT, etc, get sizes and return [JSteamMEX('PsatT','methane',100)]
            else if(nrhs == (eIN1 + 1))
            {
                //Check data type
                if(mxIsEmpty(pIN1) || !mxIsDouble(pIN1) || mxIsSparse(pIN1) || mxIsComplex(pIN1))
                    mexErrMsgTxt("Input 1 must be a real, dense, double scalar/vector/matrix");  

                m1 = mxGetM(pIN1);
                n1 = mxGetN(pIN1);
                return 1;
            }
            //Else two args, PT, PS, etc, check both dimensions [JSteamMEX('HPT','methane',1,100)]
            else
            {
                m1 = mxGetM(pIN1);
                n1 = mxGetN(pIN1);
                if(mxIsEmpty(pIN1) || !mxIsDouble(pIN1) || mxIsSparse(pIN1) || mxIsComplex(pIN1))
                    mexErrMsgTxt("Input 1 must be a real, dense, double scalar/vector/matrix"); 
                if(mxIsEmpty(pIN2) || !mxIsDouble(pIN2) || mxIsSparse(pIN2) || mxIsComplex(pIN2))
                    mexErrMsgTxt("Input 2 must be a real, dense, double scalar/vector/matrix"); 
                
                m2 = mxGetM(pIN2);
                n2 = mxGetN(pIN2);
                if((m1 > 1 || n1 > 1) && (m2 > 1 || n2 > 1)) {
                    if(m1*n1 != m2*n2)
                        mexErrMsgTxt("If both inputs are non-scalar, they must have the same number of elements");
                    if(m1!=m2 || n1!=n2)
                        mexErrMsgTxt("If both inputs are non-scalar, they must have the same size dimensions");
                }
            }             
        } 
        mxFree(func);
        return 1;
    }   
    
    //Called when MATLAB closes or MEX file released from memory
    static void MexExit()
    {
        if(JStm) {delete JStm; JStm = NULL;}
        if(JSolve) {delete JSolve; JSolve = NULL;}
    }
    
    //Check for general instruction
    int readInstr(const mxArray *prhs[], int nrhs, mxArray *plhs[], int nlhs)
    {
        char buf[2048];
        //Check for version request       
        if(nrhs < 1) {
            if(nlhs < 1) {
                if(JStm != NULL)
                    mexPrintf("\nThis is the JSteam MEX Interface built against JSteam v%2.2f and REFPROP v%2.4f\n",JSTEAMVER,JStm->GetRefpropVer());
                else
                    mexPrintf("\nThis is the JSteam MEX Interface built against JSteam v%2.2f\n",JSTEAMVER);
            }
            else
            {
                sprintf(buf,"%2.2f",JSTEAMVER);
                plhs[0] = mxCreateString(buf);
            }
            return 0;
        }
        
        //Check we have an instruction
        if(!mxIsChar(pFUNC))
            mexErrMsgTxt("The function name must be a string!");
        //Read It
        char *func = mxArrayToString(pFUNC);
        
        //Attempt to decode instruction
        if(strcmpi(func,"LOAD")) {
            //Get verbosity input
            bool verb = true;
            if(nrhs > 1 && !mxIsEmpty(pCOMP))
                verb = (bool)*mxGetPr(pCOMP);            
            bool ok = loadJSteam(false,verb);
            //Create output
            plhs[0] = mxCreateDoubleScalar((double)ok);
            return 0;
        }
        else if(strstri(func,"UNITOP"))
        {
            //Ensure JSteam is Loaded
            loadJSteam(true);
            return 2; //exit to unit op
        }
        else if(strcmpi(func,"HUMIDAIRMIX") || strcmpi(func,"HUMIDAIR"))
        {
            double hum, p, t;
            //Ensure JSteam is Loaded
            loadJSteam(true);
            //Check Inputs
            if(nrhs < 3)
                mexErrMsgTxt("JSteam Humid Air Mixture Creation requires at least three input arguments: JSteamMEX('HumidAirMix',rel_hum,pres,temp)");
            if(mxIsEmpty(pCOMP) || mxIsSparse(pCOMP) || !mxIsDouble(pCOMP) || mxIsComplex(pCOMP) || mxGetNumberOfElements(pCOMP) != 1)
                mexErrMsgTxt("Relative Humidity must be a real dense double scalar");
            if(mxIsEmpty(pIN1) || mxIsSparse(pIN1) || !mxIsDouble(pIN1) || mxIsComplex(pIN1) || mxGetNumberOfElements(pIN1) != 1)
                mexErrMsgTxt("Pressure must be a real dense double scalar");
            if(mxIsEmpty(pIN2) || mxIsSparse(pIN2) || !mxIsDouble(pIN2) || mxIsComplex(pIN2) || mxGetNumberOfElements(pIN2) != 1)
                mexErrMsgTxt("Temperature must be a real dense double scalar");
            //Collect input values
            hum = *mxGetPr(pCOMP);
            p = *mxGetPr(pIN1);
            t = *mxGetPr(pIN2);
            //Create Mixture
            Mixture humidair;
            humidair.SetAsAir(hum,p,t);
            //Create output cell array with mixture
            mixToCell(humidair,&plhs[0]);
            //Clean up
            mxFree(func);
            return 0;            
        }
        else if(strcmpi(func,"TYPNATGAS") || strcmpi(func,"TYPICALNATURALGAS"))
        {
            //Ensure JSteam is Loaded
            loadJSteam(true);
            //Create Mixture
            Mixture natgas;
            natgas.SetAsTypNatGas();
            //Create output cell array with mixture
            mixToCell(natgas,&plhs[0]);
            //Clean up
            mxFree(func);
            return 0;            
        }
        else if(strcmpi(func,"ONEATM"))
        {
            plhs[0] = mxCreateDoubleScalar(JSteamUnits::OneAtm());
            //Clean up
            mxFree(func);
            return 0;  
        }
        else if(strcmpi(func,"MOLE2MASS") || strcmpi(func,"MOLETOMASS"))
        {
            //Ensure JSteam is Loaded
            loadJSteam(true);
            //Check Inputs
            if(nrhs < 1)
                mexErrMsgTxt("JSteam mixture basis conversion requires at least one input argument: JSteamMEX('Mole2Mass',{mix})");
            //Convert to mixture
            Mixture mix;
            cellToMix(pCOMP, mix);
            //Convert back to cell array, taking mass fractions
            mixToCell(mix,&plhs[0],false);
            //Clean up
            mxFree(func);
            return 0;  
        }
        else if(strcmpi(func,"MASS2MOLE") || strcmpi(func,"MASSTOMOLE"))
        {
            //Ensure JSteam is Loaded
            loadJSteam(true);
            //Check Inputs
            if(nrhs < 1)
                mexErrMsgTxt("JSteam mixture basis conversion requires at least one input argument: JSteamMEX('Mass2Mole',{mix})");
            //Convert to mixture, as mass fractions
            Mixture mix;
            cellToMix(pCOMP, mix,false);
            //Convert back to cell array, taking mole fractions
            mixToCell(mix,&plhs[0]);
            //Clean up
            mxFree(func);
            return 0;  
        }
        else if(strcmpi(func,"CONVUNIT") || strcmpi(func,"UNITCONV"))
        {
            double *sol, *val, mw = 0; char *uin1, *uin2;
            size_t m1, n1, i;
            //We always require at least three inputs
            if(nrhs < 3)
                mexErrMsgTxt("JSteam UnitConversion requires at least three input arguments: JSteamMEX('ConvUnit',value,'in_unit','out_unit')");
            if(mxIsEmpty(pCOMP) || mxIsSparse(pCOMP) || !mxIsDouble(pCOMP) || mxIsComplex(pCOMP))
                mexErrMsgTxt("The input value must be a real dense double scalar/vector/matrix");
            if(!mxIsChar(pIN1) || mxIsEmpty(pIN1))
                mexErrMsgTxt("The input unit name must be a string!");
            if(!mxIsChar(pIN2) || mxIsEmpty(pIN2))
                mexErrMsgTxt("The output unit name must be a string!");
            //Check MW if specified
            if(nrhs > eMW) {
                if(mxIsEmpty(pMW) || mxIsSparse(pMW) || !mxIsDouble(pMW) || mxIsComplex(pMW) || mxGetNumberOfElements(pMW) > 1)
                    mexErrMsgTxt("The molecular weight value must be a real dense double scalar");
                mw = *mxGetPr(pMW);
            }
            //Collect input value(s)
            m1 = mxGetM(pCOMP); n1 = mxGetN(pCOMP); val = mxGetPr(pCOMP);

            //Collect Unit Names
            uin1 = mxArrayToString(pIN1); uin2 = mxArrayToString(pIN2);
            //Create Output Memory
            plhs[0] = mxCreateDoubleMatrix(m1,n1,mxREAL);
            sol = mxGetPr(plhs[0]);
            //Convert
            try {
                for(i = 0; i < m1*n1; i++)
                    sol[i] = JSteamUnits::ConvUnit(val[i],mw,std::string(uin1),std::string(uin2));
            }
            catch(JSteamException &ex) {
                sprintf(buf,"Error Converting Unit!\n - %s\n\n",ex.what());
                mexErrMsgTxt(buf);
            }

            //Free Memory
            mxFree(uin1); mxFree(uin2); mxFree(func);
            return 0;
        }
        else if(strcmpi(func,"GETUNIT"))
        {
            //Ensure JSteam is Loaded
            loadJSteam(true);
            //Check Inputs
            if(nrhs < 2)
                mexErrMsgTxt("You must supply at least 2 arguments to Get a Unit - e.g. JSteamMEX('GetUnit','Pressure')");
            if(!mxIsChar(pCOMP) || mxIsEmpty(pCOMP))
                mexErrMsgTxt("JSteam GetUnit: The property name must be a string!");
            char *prop = mxArrayToString(pCOMP);
            //Get the Unit String
            plhs[0] = mxCreateString(getJSteamUnit(prop).c_str());            
            //Clean up
            mxFree(func);
            mxFree(prop);
            return 0;
        }
        else if(strcmpi(func,"SETUNIT"))
        {            
            //Ensure JSteam is Loaded
            loadJSteam(true);
            //Check Inputs
            if(nrhs < 3)
                mexErrMsgTxt("You must supply at least 3 arguments to Set a Unit - e.g. JSteamMEX('SetUnit','Pressure','bar')");
            if(!mxIsChar(pCOMP) || mxIsEmpty(pCOMP))
                mexErrMsgTxt("JSteam SetUnit: The property name must be a string!");
            if(!mxIsChar(pIN1) || mxIsEmpty(pIN1))
                mexErrMsgTxt("JSteam SetUnit: The unit name must be a string!");
            char *prop = mxArrayToString(pCOMP);
            char *unit = mxArrayToString(pIN1);
            //Set the Unit
            setJSteamUnit(prop,unit);            
            //Clean up
            mxFree(func);
            mxFree(prop);
            mxFree(unit);
            return 0;
        }      
        else if(strcmpi(func,"SETDEFAULTUNITS"))
        {            
            //Ensure JSteam is Loaded
            loadJSteam(true);
            JSteamUnits::SetDefaultUnits();
            mxFree(func);
            return 0;
        }
        else if(strcmpi(func,"SETCURRENTUNITS") || strcmpi(func,"SETALLCURRENTUNITS"))
        {
            //Ensure JSteam is Loaded
            loadJSteam(true);
            //Check Inputs
            if(nrhs < 2)
                mexErrMsgTxt("You must supply at least 2 arguments to Set all Current Units - e.g. JSteamMEX('SetCurrentUnits',{unit-list})");
            if(!mxIsCell(pCOMP) || mxIsEmpty(pCOMP))
                mexErrMsgTxt("JSteam SetCurrentUnits: The list of units to set must be a cell array!");
            if(mxGetNumberOfElements(pCOMP) != 12)
                mexErrMsgTxt("JSteam SetCurrentUnits: The list of units must contain 12 cells!");
            //Set the units
            setAllCurrentUnits(pCOMP);
            mxFree(func);
            return 0;
        }
        else if(strcmpi(func,"GETCURRENTUNITS") || strcmpi(func,"GETALLCURRENTUNITS"))
        {
            //Ensure JSteam is Loaded
            loadJSteam(true);
            //Get All Current Units
            getAllCurrentUnits(plhs);
            mxFree(func);
            return 0;
        }
        else if(strcmpi(func,"GETALLUNITS"))
        {
            if(nrhs == 2) { //specific property
                if(!mxIsChar(pCOMP) || mxIsEmpty(pCOMP))
                    mexErrMsgTxt("The property name must be a string!");
                char *prop = mxArrayToString(pCOMP);
                //Get Units for specified Property
                getUnitsForProp(plhs,prop);                
                mxFree(prop);
            }
            else //return all possibilities
                getAllPossibleUnits(plhs);
            mxFree(func);
            return 0;
        }
        else if(strcmpi(func,"PRINTUNITS"))
        {
            printJSteamUnits();            
            return 0;
        }        
        else if(strcmpi(func,"GETFLUIDS"))
        {
            int no = JSteam::GetNoComponents();
            //Create Cell Array to hold info
            plhs[0] = mxCreateCellMatrix(no,1); 
            std::vector<std::string> onames;
            //Fill in info
            for(int i = 0; i < no; i++) {
                onames.push_back(JSteam::GetComponentName((JSteam::Component)i,1));
                onames.push_back(JSteam::GetComponentName((JSteam::Component)i,2));
                onames.push_back(JSteam::GetComponentName((JSteam::Component)i,5));
                enterUnits(plhs[0],i,JSteam::GetComponentName((JSteam::Component)i),onames);
                onames.clear();
            }
            return 0;
        }        
        else if(strcmpi(func,"PRINTFLUIDS") || strcmpi(func,"PRINTALLFLUIDS"))
        {
            //Print Available Components
            mexPrintf("AVAILABLE FLUIDS\n\n%15s         %17s     %17s\n","Short Name","Full Name","Formula");
            for(int i = 0; i < JSteam::GetNoComponents(); i++)
                mexPrintf("%17s [%35s] (%s)\n",JSteam::GetComponentName((JSteam::Component)i,0).c_str(),JSteam::GetComponentName((JSteam::Component)i,1).c_str(),JSteam::GetComponentName((JSteam::Component)i,2).c_str());
            mexPrintf("\n");
            return 0;
        }       
        else if(strcmpi(func,"VERIFY"))
        {
            //Ensure JSteam is Loaded
            loadJSteam(true);
            //Check Inputs
            if(nrhs < 2)
                mexErrMsgTxt("You must supply at least 2 arguments to Verify");
            if(!mxIsChar(pCOMP))
                mexErrMsgTxt("The second argument to Verify must be a string");
            //Get Verification Mode
            char *mode = mxArrayToString(pCOMP);
            upper(mode);
            plhs[0] = mxCreateDoubleScalar(0);
            double *sol = mxGetPr(plhs[0]);
            if(strcmpi(mode,"STEAM_THERMO"))
                *sol = (double)JStm->VerifySteamThermo();
            else if(strcmpi(mode,"GEN_THERMO"))
                *sol = (double)JStm->VerifyGenThermo();
            else if(strcmpi(mode,"STEAM_UNITOPS"))
                *sol = (double)JStm->VerifySteamUnitOps();            
            else if(strcmpi(mode,"COM_UNITOPS"))
                *sol = (double)JStm->VerifyCombustUnitOps();
            else if(strcmpi(mode,"ORC_UNITOPS"))
                *sol = (double)JStm->VerifyORCUnitOps();
            else if(strcmpi(mode,"HX_UNITOPS"))
                *sol = (double)JStm->VerifyHXUnitOps();
            else if(strcmpi(mode,"UNIT_CONV"))
                *sol = (double)JStm->VerifyUnitConvs();
            else
            {
                sprintf(buf,"Unknown verification mode: %s",mode);
                mexErrMsgTxt(buf);
            }
            return 0;                
        }
        else if(strcmpi(func,"HELP"))
        {
            //Print JSteam Help
            mexPrintf("-------------------------------------------\n");
            mexPrintf(" JSTEAM MEX INTERFACE HELP\n");
            if(JStm == NULL)
                mexPrintf(" Built against JSteam v%2.2f\n",JSTEAMVER);
            else
                mexPrintf(" Built against JSteam v%2.2f and REFPROP v%2.4f\n",JSTEAMVER,JStm->GetRefpropVer());
            mexPrintf(" (C) Jonathan Currie 2011-2023\n Control Engineering\n https://controlengineering.co.nz\n\n");
            JSolve->Print();
            JSolve->PrintUO();
            printUnitHelp();
            mexPrintf("-------------------------------------------\n");
            return 0;
        }                            
        return 1;
    }
    
    //Load JSteam
    bool loadJSteam(bool printWarn, bool verb)
    {
        char buf[2048]; bool ok = true;
        //Check if JSteam is Loaded
        if(JStm == NULL) {
            if(printWarn) {
                mexPrintf("Loading JSteam Interface - Please Wait. Avoid this message by calling JSteamMEX('Load') first.\n");
                mexEvalString("drawnow;pause(1e-8);");
            }                    
            //Create JSteam & Solver Object
            try {
                JStm = new JSteam();
            }
            catch(JSteamException &ex)
            {
                delete JStm; JStm = NULL;
                sprintf(buf,"Error Loading JSteam Engine!\n\n%s",ex.what());
                mexErrMsgTxt(buf);
            }
            catch(std::exception *e) {
                delete JStm; JStm = NULL;
                sprintf(buf,"Error Loading JSteam Engine!\n\n%s",e->what());
                mexErrMsgTxt(buf);
            }
            catch(...) {
                delete JStm; JStm = NULL;
                mexErrMsgTxt("Error Loading JSteam Engine!");
            }
            JSolve = new JSteamSolve();
            int licode = 0;
            //Attempt to license JSteam
            try 
            {
                licode = JStm->LicenseValidate();
            }
            catch(JSteamException &ex)
            {
                delete JStm; JStm = NULL;
                if(strstr(ex.what(),"JSteam.lic") == NULL) {
                    sprintf(buf,"Error Loading JSteam License!\n\n%s",ex.what());
                    mexErrMsgTxt(buf);
                }
                //otherwise license error handled below
            }
            catch(std::exception *e) {
                delete JStm; JStm = NULL;
                sprintf(buf,"Error Loading JSteam License!\n\n%s",e->what());
                mexErrMsgTxt(buf);
            }
            catch(...) {
                delete JStm; JStm = NULL;
                mexErrMsgTxt("Error Loading JSteam License!");
            }
            //Check result
            if(licode <= 0) {
                sprintf(buf,"A valid JSteam License could not be found - only steam thermodynamic functions are available.\n");
                mexWarnMsgTxt(buf);
                std::string dir; getMATLABRoot(dir);
                sprintf(buf,"To License the JSteam Toolbox, please place your JSteam.lic file in the following directory:\n %s\nThe directory should have opened automatically for you. Please Restart MATLAB after copying the license file.\n",dir.c_str());
                mexWarnMsgTxt(buf);
                ok = false; //no good
                mexEvalString((std::string("!explorer.exe \"") + dir + std::string("\"")).c_str()); //open it for the user (on windows)  
                sprintf(buf,"If you have not received (or requested) a JSteam License file, please email the below MAC address (Physical Address/Host-ID) to license@controlengineering.co.nz to request one.\n\nNote: You can also use the same license file from the JSteam Excel Add In if you are using that.\n");
                mexWarnMsgTxt(buf);
                mexEvalString("!getmac");  
            }
            //Print License Info if Load OK and Verb = true
            if(ok && verb)            
                printLicense(JStm,licode);               
            //Register memory freeing routine
            mexAtExit(MexExit);
            //Set Default Units
            JSteamUnits::SetDefaultUnits();            
        }
        //Return OK status
        return ok;
    }
    
    
}