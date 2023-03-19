/* JSTEAM MATLAB MEX INTERFACE
 * Utility Functions
 * J.Currie 2012-2015 */

#include <mex.h>
#include "include\JSteam.h"
#include "include\JSteamUtils.h"

using namespace JSteamDLL;

namespace JSteamMEX
{   
    //---- STRING FUNCTIONS ----//
    //Convert input string to uppercase
    void upper(char *str)
    {
        int i = 0;
        while(str[i]) {
            str[i] = toupper(str[i]);  
            i++;
        }
    }
    
    //Returns true if both strings are the same (case insensitive) [max 1024 length]
    bool strcmpi(char *str1, char *str2)
    {
        char s1[1024], s2[1024];
        if(strlen(str1) > 1024)
            mexErrMsgTxt("String1 too long for comparison with strcmpi");
        if(strlen(str2) > 1024)
            mexErrMsgTxt("String2 too long for comparison with strcmpi");
        strcpy(s1,str1); strcpy(s2,str2);
        //Convert both to upper
        upper(s1); upper(s2);
        if(strcmp(s1,s2)==0)
            return true;
        else 
            return false;
    }
    
    //Returns true if str is contained in refStr (case insensitive)
    bool strstri(char *refStr, char *str)
    {
        char rs[1024], s[1024];
        if(strlen(refStr) > 1024)
            mexErrMsgTxt("RefString too long for comparison with strstri");
        if(strlen(str) > 1024)
            mexErrMsgTxt("String too long for comparison with strstri");
        strcpy(rs,refStr); strcpy(s,str);
        //Convert both to upper
        upper(rs); upper(s);
        if(strstr(rs,s)!=NULL)
            return true;
        else 
            return false;
    }
    
    //String Array to Cell Array
    void StrArr2CellArr(mxArray *plhs[], std::vector<std::string> strArr)
    {
        //Create Cell Array to hold info
        plhs[0] = mxCreateCellMatrix(strArr.size(),1);
        //Fill in info
        for(int i = 0; i < strArr.size(); i++)
            mxSetCell(plhs[0],i,mxCreateString(strArr[i].c_str()));        
    }
    
    //---- MIXTURE FUNCTIONS ----//
    //Create Mixture with specified Pure Fluid
    Mixture *createMix(char *fluid)
    {
        char buf[1024];
        int idx = JSteam::GetComponentIndex(fluid);
        if(idx < 0) {
            sprintf(buf,"Could not match requested fluid [%s] to JSteam fluid list. See JSteamMEX('help') for a list of supported fluids.",fluid);
            mexErrMsgTxt(buf);
        }
        Mixture *Mix = new Mixture();
        std::vector<double> frac = Mix->CreateFracVec();
        frac[idx] = 1.0;
        Mix->SetMoleFrac(frac);
        return Mix;
    }
    
    //Convert Cell Array of Fluids and Fractions to Mixture
    void cellToMix(const mxArray *cell, Mixture &mix, bool moleBasis)
    {
        char buf[256];
        if(!mxIsCell(cell))
            mexErrMsgTxt("Mixtures supplied to this function must be a N x 2 cell array, where N is the number of fluids.");
        if(mxGetN(cell) != 2)
        	mexErrMsgTxt("When suppling a mixture as a cell array, the cell array should be N x 2, where N is the number of fluids.");
        
        std::vector<double> fracs = mix.CreateFracVec();
        int nfluids = (int)mxGetM(cell), idx;
        for(int i = 0; i < nfluids; i++) {
            //Check fluid name
            if(!mxIsChar(mxGetCell(cell,i)) || mxIsEmpty(mxGetCell(cell,i)))
                mexErrMsgTxt("All elements in the first column of the mixture cell array should be fluid names (strings).");
            //Get fluid name
            char *fluid = mxArrayToString(mxGetCell(cell,i));
            //Check Index
            idx = JSteam::GetComponentIndex(std::string(fluid));
            if(idx < 0) {
                sprintf(buf,"Could not match request component \"%s\" to JSteam component list.",fluid);
                mxFree(fluid);
                mexErrMsgTxt(buf);
            }
            //Check fraction
            mxArray *t1 = mxGetCell(cell,i + nfluids);
            if(mxIsEmpty(t1) || !mxIsDouble(t1) || mxIsSparse(t1) || mxIsComplex(t1))
                mexErrMsgTxt("All elements in the second column of the mixture cell array should be fractions (real, dense, doubles).");
            double frac = *mxGetPr(t1); 
            if(frac < 0 || frac > 100) {
                sprintf(buf,"The mole fraction associated with fluid \"%s\" is < 0 or > 100.",fluid);
                mxFree(fluid);
                mexErrMsgTxt(buf);
            }
            fracs[idx] = frac;
            mxFree(fluid);
        }
        if(moleBasis)
            mix.SetMoleFrac(fracs);
        else
            mix.SetMassFrac(fracs);
    }
    
    //Convert Mixture to Cell Array of Fluids and Fractions
    void mixToCell(Mixture &mix, mxArray *cell[], bool moleBasis)
    {
        std::vector<double> fracs;
        //Determine number of non-zero entries in mixture
        if(moleBasis)
            fracs = mix.GetMoleFrac();
        else
            fracs = mix.GetMassFrac();
        int nfluids = 0;
        for(int i = 0; i < fracs.size(); i++)
            if(fracs[i] > 0)
                nfluids++;
        //Create Output Cell Array
        cell[0] = mxCreateCellMatrix(nfluids,2);
        //Enter Details
        int idx = 0;
        for(int i = 0; i < fracs.size(); i++) {
            if(fracs[i] > 0) {
                mxSetCell(cell[0],idx,mxCreateString(JSteam::GetComponentName((JSteam::Component)i,0).c_str()));
                mxSetCell(cell[0],idx+nfluids,mxCreateDoubleScalar(fracs[i]));
                idx++;
            }
        }
    }
    
    //---- PROPERTY FUNCTIONS ----//
    //Match property name to propenum
    int matchString2Prop(char *prop)
    {   
        //enum propenum{Temp=0,Pres,MasF,MolF,Powr,Ener,Volu,Mass,Mole,Visc,ThmC,Basi,Enth,Entr,HCap};
        char buf[1024];
        
        if(strstri(prop,"PRES"))
            return Pres;
        else if(strstri(prop,"TEMP"))
            return Temp;
        else if(strstri(prop,"MASSF"))
            return MasF;            
        else if(strstri(prop,"MOLEF"))
            return MolF;            
        else if(strstri(prop,"POW"))
            return Powr;            
        else if(strstri(prop,"ENE"))
            return Ener;            
        else if(strstri(prop,"MASS"))
            return Mass;            
        else if(strstri(prop,"MOLE"))
            return Mole;
        else if(strstri(prop,"VOL"))
            return Volu;
        else if(strstri(prop,"VIS"))
            return Visc;
        else if(strstri(prop,"THERM"))
            return ThmC;
        else if(strstri(prop,"BAS"))
            return Basi;
        else if(strstri(prop,"ENTH"))
            return Enth;
        else if(strstri(prop,"ENTR"))
            return Entr;
        else if(strstri(prop,"HEAT") || strstri(prop,"CAP"))
            return HCap;
        else {
            sprintf(buf,"Unknown Property: '%s'. Examples are 'Pressure', 'Temperature', etc",prop);
            mexErrMsgTxt(buf);
        }
        return -1;
    }
        
    //---- MISC FUNCTIONS ----//
    //Print License Information
    void printLicense(JSteam *JStm, int code)
    {
        mexPrintf("-------------------------------------------\n");
        std::string details = JStm->LicenseDetails(code);
        mexPrintf(details.c_str());
        mexPrintf("-------------------------------------------\n");
    }
    
    //Get Root MATLAB Directory
    void getMATLABRoot(std::string &dir)
    {
        mxArray *rootDir = mxCreateString("dummy");
        mxArray *pcType = mxCreateString("dummy");
        mexCallMATLAB(1,&rootDir,0,NULL,"matlabroot");
        mexCallMATLAB(1,&pcType,0,NULL,"computer");
        char *crootDir = mxArrayToString(rootDir);
        char *cpcType = mxArrayToString(pcType);    
        std::string srootDir(crootDir);
        if(strcmp(cpcType,"PCWIN64")==0)
            dir = srootDir + "\\bin\\win64";
        else
            dir = srootDir + "\\bin\\win32";
        mxFree(crootDir);
        mxFree(cpcType);
        mxDestroyArray(rootDir);
        mxDestroyArray(pcType);
    }
}