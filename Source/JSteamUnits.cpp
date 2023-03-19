/* JSTEAM MATLAB MEX INTERFACE
 * Unit Functions
 * J.Currie 2012-2015 */

#include <mex.h>
#include "include\JSteamUtils.h"

using namespace JSteamDLL;

namespace JSteamMEX
{     
    //---- UNIT FUNCTIONS ----//
    void printJSteamUnits()
    {
        printUnits("Pressure","JSteamMEX('SetUnit','Pressure',unit)",JSteamUnits::GetAllPresUnits());
        printUnits("Temperature","JSteamMEX('SetUnit','Temperature',unit)",JSteamUnits::GetAllTempUnits());
        printUnits("Mass Flow","JSteamMEX('SetUnit','MassFlow',unit)",JSteamUnits::GetAllMassFUnits());
        printUnits("Mole Flow","JSteamMEX('SetUnit','MoleFlow',unit)",JSteamUnits::GetAllMoleFUnits());
        printUnits("Power","JSteamMEX('SetUnit','Power',unit)",JSteamUnits::GetAllPowerUnits());    
        printUnits("Energy","JSteamMEX('SetUnit','Energy',unit)",JSteamUnits::GetAllEnergyUnits());
        printUnits("Volume","JSteamMEX('SetUnit','Volume',unit)",JSteamUnits::GetAllVolUnits());
        printUnits("Mass","JSteamMEX('SetUnit','Mass',unit)",JSteamUnits::GetAllMassUnits());
        printUnits("Mole","JSteamMEX('SetUnit','Mole',unit)",JSteamUnits::GetAllMoleUnits());
        printUnits("Viscosity","JSteamMEX('SetUnit','Viscosity',unit)",JSteamUnits::GetAllVisUnits());
        printUnits("Thermal Conductivity","JSteamMEX('SetUnit','ThermalConductivity',unit)",JSteamUnits::GetAllThermCUnits());
        printUnits("Basis","JSteamMEX('SetUnit','Basis',unit)",JSteamUnits::GetAllBasis());
        mexPrintf("\n");
    }
    
    //Set a Unit for a Specified Property
    void setJSteamUnit(char *prop, char *unit)
    {       
        char buf[1024];
        int pno = matchString2Prop(prop);
        try {
            //Switch on property
            switch(pno) 
            {
                case Pres: JSteamUnits::SetPresUnit(unit); break;
                case Temp: JSteamUnits::SetTempUnit(unit); break;
                case MasF: JSteamUnits::SetMassFUnit(unit); break;
                case MolF: JSteamUnits::SetMoleFUnit(unit); break;
                case Powr: JSteamUnits::SetPowerUnit(unit); break;
                case Ener: JSteamUnits::SetEnergyUnit(unit); break;
                case Volu: JSteamUnits::SetVolUnit(unit); break;
                case Mass: JSteamUnits::SetMassUnit(unit); break;
                case Mole: JSteamUnits::SetMoleUnit(unit); break;
                case Visc: JSteamUnits::SetVisUnit(unit); break;
                case ThmC: JSteamUnits::SetThermCUnit(unit); break;
                case Basi: JSteamUnits::SetBasis(unit); break;
                case Enth:
                case Entr:
                case HCap:
                    mexWarnMsgTxt("Specific Properties such as Enthalpy, Entropy, and Heat Capacity require you to set the Energy and Mass/Mole + Basis Separately.\n"
                                " For example to set Enthalpy as kJ/kmol:\n"
                                "  >> JSteamMEX('SetUnit','Energy','kJ');\n"
                                "  >> JSteamMEX('SetUnit','Mole','kmol');\n"
                                "  >> JSteamMEX('SetUnit','Basis','mole');\n\n"
                                " No unit was changed with this call.\n");
                    break;
            }
        }
        catch(JSteamException &ex)
        {
            sprintf(buf,"%s",ex.what());
            mexErrMsgTxt(buf);
        }
        catch(std::exception *e) {
            sprintf(buf,"Error Setting Unit!\n\n%s",e->what());
            mexErrMsgTxt(buf);
        }
        catch(...) {
            mexErrMsgTxt("Unknown Property to Set Unit. Examples are 'Pressure', 'Temperature', etc.");
        }
    }
    
    //Set all current units
    void setAllCurrentUnits(const mxArray *prhs)
    {
        char *unit;
        char buf[1024];
        std::vector<std::string> units;
        try {
            //Build an array of the units 
            for(int i = 0; i < 12; i++)
            {
                //Check unit
                if(!mxIsChar(mxGetCell(prhs,i)) || mxIsEmpty(mxGetCell(prhs,i)))
                    mexErrMsgTxt("Unit names must be strings");
                unit = mxArrayToString(mxGetCell(prhs,i));
                //Save into array
                units.push_back(std::string(unit));
                //Clear memory
                mxFree(unit);
            }
            //Set in JSteam
            JSteamUnits::SetAllUnits(units);            
        }
        catch(JSteamException &ex)
        {
            sprintf(buf,"%s",ex.what());
            mexErrMsgTxt(buf);
        }
        catch(std::exception *e) {
            sprintf(buf,"Error Setting Unit!\n\n%s",e->what());
            mexErrMsgTxt(buf);
        }
        catch(...) {
            mexErrMsgTxt("Unknown Property to Set Unit. Examples are 'Pressure', 'Temperature', etc.");
        }
    }
    
    //Get a Unit for a Specified Property
    std::string getJSteamUnit(char *prop)
    {       
        int pno = matchString2Prop(prop);
        //Switch on property
        switch(pno) 
        {
            case Pres: return JSteamUnits::GetPresUnitStr(); break;
            case Temp: return JSteamUnits::GetTempUnitStr(); break;
            case MasF: return JSteamUnits::GetMassFUnitStr(); break;
            case MolF: return JSteamUnits::GetMoleFUnitStr(); break;
            case Powr: return JSteamUnits::GetPowerUnitStr(); break;
            case Ener: return JSteamUnits::GetEnergyUnitStr(); break;
            case Volu: return JSteamUnits::GetVolUnitStr(); break;
            case Mass: return JSteamUnits::GetMassUnitStr(); break;
            case Mole: return JSteamUnits::GetMoleUnitStr(); break;
            case Visc: return JSteamUnits::GetVisUnitStr(); break;
            case ThmC: return JSteamUnits::GetThermCUnitStr(); break;
            case Basi: return JSteamUnits::GetBasisStr(); break;
            case Enth: return JSteamUnits::GetEnthUnitStr(); break;
            case Entr: return JSteamUnits::GetEntrUnitStr(); break;
            case HCap: return JSteamUnits::GetHCapUnitStr(); break;
        }   
        return "Not Found";
    }
    
    //Get All Current Units
    void getAllCurrentUnits(mxArray *plhs[])
    {
        //Create Cell Array to hold info
        plhs[0] = mxCreateCellMatrix(12,1);
        //Fill in info
        mxSetCell(plhs[0],0,mxCreateString(JSteamUnits::GetPresUnitStr().c_str()));
        mxSetCell(plhs[0],1,mxCreateString(JSteamUnits::GetTempUnitStr().c_str()));
        mxSetCell(plhs[0],2,mxCreateString(JSteamUnits::GetMassFUnitStr().c_str()));
        mxSetCell(plhs[0],3,mxCreateString(JSteamUnits::GetMoleFUnitStr().c_str()));
        mxSetCell(plhs[0],4,mxCreateString(JSteamUnits::GetPowerUnitStr().c_str()));
        mxSetCell(plhs[0],5,mxCreateString(JSteamUnits::GetEnergyUnitStr().c_str()));        
        mxSetCell(plhs[0],6,mxCreateString(JSteamUnits::GetMassUnitStr().c_str()));
        mxSetCell(plhs[0],7,mxCreateString(JSteamUnits::GetMoleUnitStr().c_str()));
        mxSetCell(plhs[0],8,mxCreateString(JSteamUnits::GetVolUnitStr().c_str()));
        mxSetCell(plhs[0],9,mxCreateString(JSteamUnits::GetVisUnitStr().c_str()));
        mxSetCell(plhs[0],10,mxCreateString(JSteamUnits::GetThermCUnitStr().c_str()));
        mxSetCell(plhs[0],11,mxCreateString(JSteamUnits::GetBasisStr().c_str()));
    }
    
    //Get all possible Units for a given property
    void getUnitsForProp(mxArray *plhs[], char *prop)
    {
        int pno = matchString2Prop(prop);
        switch(pno) 
        {
            case Pres: StrArr2CellArr(plhs,JSteamUnits::GetAllPresUnits()); break;
            case Temp: StrArr2CellArr(plhs,JSteamUnits::GetAllTempUnits()); break;
            case MasF: StrArr2CellArr(plhs,JSteamUnits::GetAllMassFUnits()); break;
            case MolF: StrArr2CellArr(plhs,JSteamUnits::GetAllMoleFUnits()); break;
            case Powr: StrArr2CellArr(plhs,JSteamUnits::GetAllPowerUnits()); break;
            case Ener: StrArr2CellArr(plhs,JSteamUnits::GetAllEnergyUnits()); break;
            case Volu: StrArr2CellArr(plhs,JSteamUnits::GetAllVolUnits()); break;
            case Mass: StrArr2CellArr(plhs,JSteamUnits::GetAllMassUnits()); break;
            case Mole: StrArr2CellArr(plhs,JSteamUnits::GetAllMoleUnits()); break;
            case Visc: StrArr2CellArr(plhs,JSteamUnits::GetAllVisUnits()); break;
            case ThmC: StrArr2CellArr(plhs,JSteamUnits::GetAllThermCUnits()); break;
            case Basi: StrArr2CellArr(plhs,JSteamUnits::GetAllBasis()); break;
            case Enth:
            case Entr:
            case HCap:
                mexErrMsgTxt("Specific Properties such as Enthalpy, Entropy, and Heat Capacity are not set (or read) directly, rather via Energy and Mass/Mole properties.");
                break;
        }
    }
    
    //Get all possible units for all properties
    void getAllPossibleUnits(mxArray *plhs[])
    {
        //Create Cell Array to hold info
        plhs[0] = mxCreateCellMatrix(12,1); 
        //Fill in info
        enterUnits(plhs[0],0,"Pressure",JSteamUnits::GetAllPresUnits());
        enterUnits(plhs[0],1,"Temperature",JSteamUnits::GetAllTempUnits());
        enterUnits(plhs[0],2,"Mass Flow",JSteamUnits::GetAllMassFUnits());
        enterUnits(plhs[0],3,"Mole Flow",JSteamUnits::GetAllMoleFUnits());
        enterUnits(plhs[0],4,"Power",JSteamUnits::GetAllPowerUnits());    
        enterUnits(plhs[0],5,"Energy",JSteamUnits::GetAllEnergyUnits());        
        enterUnits(plhs[0],6,"Mass",JSteamUnits::GetAllMassUnits());
        enterUnits(plhs[0],7,"Mole",JSteamUnits::GetAllMoleUnits());
        enterUnits(plhs[0],8,"Volume",JSteamUnits::GetAllVolUnits());
        enterUnits(plhs[0],9,"Viscosity",JSteamUnits::GetAllVisUnits());
        enterUnits(plhs[0],10,"Thermal Conductivity",JSteamUnits::GetAllThermCUnits());
        enterUnits(plhs[0],11,"Basis",JSteamUnits::GetAllBasis());
    }
    
    //Enter Strings into a Cell Array
    void enterUnits(mxArray *plhs, int idx, std::string unit, std::vector<std::string> units)
    {
        //Create Cell Array to hold info        
        mxSetCell(plhs,idx, mxCreateCellMatrix(1,units.size()+1)); //mxCreateString(JSteamUnits::GetPresUnitStr().c_str()));
        mxSetCell(mxGetCell(plhs,idx),0,mxCreateString(unit.c_str()));
        for(int i = 0; i < units.size(); i++)
            mxSetCell(mxGetCell(plhs,idx),i+1,mxCreateString(units[i].c_str()));        
    }
        
    //Print Unit Information
    void printUnits(std::string unit, std::string func, std::vector<std::string> units)
    {
        mexPrintf("%20s Units: ",unit.c_str());
        for(int i = 0; i < units.size()-1; i++) {
            mexPrintf("%s, ",units[i].c_str());
        }
        mexPrintf("%s  [Set via %s]\n",units[units.size()-1].c_str(),func.c_str());
    }  
    
    //Print Unit Documentation
    void printUnitHelp()
    {
        mexPrintf("-------------------------------------------\n");
        mexPrintf("USER CUSTOMIZABLE UNITS\n\n - Available Units\n");
        printJSteamUnits();
        mexPrintf(" - Unit Get/Set Methods\n");
        mexPrintf("SetUnit                  - Change the Unit for a Specified Property e.g. JSteamMEX('SetUnit','Pressure','atm')\n");
        mexPrintf("SetDefaultUnits          - Return all Units to JSteam Defaults e.g. JSteamMEX('SetDefaultUnits')\n");
        mexPrintf("SetCurrentUnits          - Set all Units for all Properties e.g. JSteamMEX('SetCurrentUnits',{unit-list}) [Same order as returned by GetCurrentUnits]\n");
        mexPrintf("GetUnit                  - Return the Current Unit for a Specified Property e.g. JSteamMEX('GetUnit','Pressure')\n");
        mexPrintf("GetCurrentUnits          - Return the Current Unit for all Properties e.g. JSteamMEX('GetCurrentUnits')\n");
        mexPrintf("GetAllUnits [1 Input]    - Return all Possible Units for all Properties e.g. JSteamMEX('GetAllUnits')\n");
        mexPrintf("GetAllUnits [2 Input]    - Return all Possible Units for a Specified Property e.g. JSteamMEX('GetAllUnits','Pressure')\n");
        mexPrintf("PrintUnits               - Print all Possible Units and Properties e.g. JSteamMEX('PrintUnits')\n");
        
        mexPrintf(" - Unit Conversion\n");
        mexPrintf("ConvUnit [Same Basis]    - Convert Value between Specified Units e.g. JSteamMEX('ConvUnit',1,'bar','atm')\n");
        mexPrintf("ConvUnit [New Basis]     - Convert Value between Specified Units And Basis e.g. JSteamMEX('ConvUnit',1,'kJ/kg','kJ/kmol',mw)\n");
    }
}