/* JSTEAM MATLAB MEX INTERFACE
 * Utility Header
 * J.Currie 2015 */

#ifndef JSTEAMUTIL_INC
#define JSTEAMUTIL_INC

#include "include\JSteam.h"

using namespace JSteamDLL;

namespace JSteamMEX
{
    //Property Enum
    enum propenum{Temp=0,Pres,MasF,MolF,Powr,Ener,Volu,Mass,Mole,Visc,ThmC,Basi,Enth,Entr,HCap};

    //String Functions
    void upper(char * str);                             //Convert Pass String to Upper Case
    bool strcmpi(char *str1, char *str2);               //Compare two strings (case insensitive)
    bool strstri(char *refStr, char *str);              //Find if a string is contained within refStr (case insensitive)
    void StrArr2CellArr(mxArray *plhs[], std::vector<std::string> strArr);  //String Array to Cell Array
    //Property Functions
    int matchString2Prop(char *prop);                   //Match property name to enum above    
    //Unit Functions    
    void printJSteamUnits();                            //Print All Possible JSteam Units
    void setJSteamUnit(char *prop, char *unit);         //Set a JSteam Unit
    void setAllCurrentUnits(const mxArray *prhs);       //Set all current units
    std::string getJSteamUnit(char *prop);              //Get current JSteam Unit for a given property
    void getAllCurrentUnits(mxArray *plhs[]);           //Get all current units
    void getUnitsForProp(mxArray *plhs[], char *prop);  //Get all units for given property
    void getAllPossibleUnits(mxArray *plhs[]);          //Get all possible units for all properties
    void enterUnits(mxArray *plhs, int idx, std::string unit, std::vector<std::string> units);
    void printUnits(std::string unit, std::string func, std::vector<std::string> units);        
    void printUnitHelp();
    //Mixture Functions
    Mixture *createMix(char *fluid);                    //Create a Mixture with a Single Fluid      
    void cellToMix(const mxArray *cell, Mixture &mix, bool moleBasis=true);  //Create a Mixture from a Cell Array of Fluids and Fractions
    void mixToCell(Mixture &mix, mxArray *cell[], bool moleBasis=true);  //Create a Cell Array of Fluids and Fractions from a Mixture    
    //Misc Functions
    void printLicense(JSteam *JStm, int code);          //Prints License Information    
    void getMATLABRoot(std::string &dir);               //Gets the MATLAB root directory (for placing license)
}

#endif