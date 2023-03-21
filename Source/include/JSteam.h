/* -------------- JSteam Utility Modelling Package --------------- */
/*                  Main JSteam Class Header File                  */
/*                                                                 */
/*             Copyright � 2010-2015 Jonathan Currie               */
/* --------------------------------------------------------------- */

#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <windows.h>

//JSteam Version
#define JSTEAMVER 3.26

#define DLL __declspec(dllexport)

namespace JSteamDLL
{	
	//Solid Fuel Composition Object
	class SolidFuel
	{
	private:
		//Properties		
		std::vector<double> moleFrac;
		std::vector<double> massFrac;
		bool isCombusted;
		//Methods
		void CalcMass();
		void CalcMole();
		double SumArray(std::vector<double> ar);
		double SumArrayPosElem(std::vector<double> ar);

	public:
		//Enumerations
		enum Component{Carbon,Hydrogen,Sulfur,Oxygen,Nitrogen,Water,Ash,CarbonDioxide,SulfurDioxide};
		//Constructor
		DLL SolidFuel();
		DLL ~SolidFuel();
		DLL SolidFuel* MixMass(SolidFuel *B, double massFA, double massFB);
		DLL SolidFuel* MixMole(SolidFuel *B, double moleFA, double moleFB);
		DLL std::string Status();

		//--Fraction Methods--//
		DLL std::vector<double> CreateFracVec();
		DLL int SetMoleFrac(std::vector<double> frac);
		DLL int SetMassFrac(std::vector<double> frac);
		DLL std::vector<double> GetMoleFrac();
		DLL std::vector<double> GetMassFrac();
		DLL void SetAsAir();
		DLL void SetAsAir(double Humidity, double P, double T);
		DLL double GetO2MoleFrac();
		DLL double GetCO2MoleFrac();
		DLL double GetSO2MoleFrac();
		DLL int GetNoComp();

		//--Other Component Properties--//
		DLL double NHV();
		DLL double GHV();
		DLL double MW();

		//--Combustion Methods--//
		DLL SolidFuel* StoichCombust();
		DLL SolidFuel* StoichCombust(double *Hc);
		DLL SolidFuel* Combust(double stackT, double mFlow, double *duty, bool isMass=false);
		DLL SolidFuel* AdCombust(double moleF, double *outH, double *moleFOut, bool isMass=false);
		DLL double InExcessO2();
		DLL double StackO2();
		DLL double SolveAirMassF(SolidFuel *Air,double O2, int O2Mode,double fuelMassF);
		DLL double SolveAirMoleF(SolidFuel *Air,double O2, int O2Mode,double fuelMoleF);
		DLL double SolveFuelMassF(SolidFuel *Air, double O2, double airMassF);
		DLL double SolveFuelMoleF(SolidFuel *Air, double O2, double airMoleF);
		DLL double AcidDewPoint();
	};

	//Natural Gas / Refrigerant Mixture Object
	class Mixture
	{
	private:
		//Properties		
		std::vector<double> moleFrac;
		std::vector<double> massFrac;
		bool isCombusted;
		bool isNaturalGas;
		//Methods
		void CalcMass();
		void CalcMole();
		double SumArray(std::vector<double> ar);
		double SumArrayPosElem(std::vector<double> ar);
		int CheckMixedNGRefrig(std::vector<double> frac);

	public:
		//Constructor
		DLL Mixture();
		DLL Mixture(SolidFuel *SF);
		DLL ~Mixture();
		//--Composition Methods--//
		DLL void MixMass(Mixture *B, double massFA, double massFB, Mixture *OutMix);
		DLL void MixMole(Mixture *B, double moleFA, double moleFB, Mixture *OutMix);
		DLL std::string Status();
		DLL bool IsNaturalGas();
		DLL bool IsEmpty();

		//--Fraction Methods--//
		DLL std::vector<double> CreateFracVec();
		DLL int SetMoleFrac(std::vector<double> frac);
		DLL int SetMoleFrac(std::string fracStr);
		DLL int SetMassFrac(std::vector<double> frac);
		DLL std::vector<double> GetMoleFrac();
		DLL std::string GetMoleFracString();
		DLL double *GetMoleFracPtr();
		DLL std::vector<double> GetMassFrac();
		DLL void SetAsAir();
		DLL void SetAsAir(double Humidity, double P, double T);
		DLL void SetAsTypNatGas();
		DLL void SetAsEmpty();		
		DLL double GetO2MoleFrac();
		DLL double GetCO2MoleFrac();
		DLL double GetCO2MassFrac();
		DLL double GetSO2MoleFrac();
		DLL double GetSO2MassFrac();
		DLL int GetNoComp();
		DLL void Print();

		//--Thermo Methods--//
		DLL double HcPT(double P, double T);
		DLL double HcPS(double P, double S);
		DLL double HcPX(double P, double X);
		DLL double HcTX(double T, double X);
		DLL double ScPT(double P, double T);
		DLL double ScPH(double P, double H);
		DLL double ScPX(double P, double X);
		DLL double ScTX(double T, double X);
		DLL double CpcPT(double P, double T);
		DLL double CpcPH(double P, double H);
		DLL double CpcPS(double P, double S);
		DLL double CpcPX(double P, double X);
		DLL double CpcTX(double T, double X);
		DLL double CvcPT(double P, double T);
		DLL double CvcPH(double P, double H);
		DLL double CvcPS(double P, double S);
		DLL double CvcPX(double P, double X);
		DLL double CvcTX(double T, double X);
		DLL double VcPT(double P, double T);
		DLL double VcPH(double P, double H);
		DLL double VcPS(double P, double S);
		DLL double VcPX(double P, double X);
		DLL double VcTX(double T, double X);
		DLL double TcPH(double P, double H);
		DLL double TcPS(double P, double S);
		DLL double TcHS(double H, double S);
		DLL double TsatcP(double P);
		DLL double TsatcD(double d);
		DLL double PcHS(double H, double S);
		DLL double PsatcT(double T);
		DLL double PsatcD(double d);
		DLL double XcPT(double P, double T);
		DLL double XcPH(double P, double H);
		DLL double XcPS(double P, double S);		
		DLL double UcPT(double P, double T);
		DLL double UcPS(double P, double S);
		DLL double UcPH(double P, double H);
		DLL double UcPX(double P, double X);
		DLL double UcTX(double T, double X);
		DLL double KcPT(double P, double T);
		DLL double KcPS(double P, double S);
		DLL double KcPH(double P, double H);
		DLL double KcPX(double P, double X);
		DLL double KcTX(double T, double X);
		//--Thermo (Vapour Only) Methods--//
		DLL double HcPTvap(double P, double T);
		DLL double ScPTvap(double P, double T);
		//--Other Component Properties--//
		DLL double NHV();
		DLL double GHV();
		DLL double MW();
		DLL bool GenSatSplines();
		/// <summary>
		/// Reset the use of splines for saturated calculations
		/// </summary>
		DLL void ResetSatSplines();
		DLL int CritParams(double *Tc, double *Pc, double *Vc);
		DLL int SatGV(double *P, double *T, double X, double B, int ipv, int ityp, int isp, double *RhoL, double *RhoV, Mixture *Liq, Mixture *Vap);
		DLL void Limits(double *Tmin, double *Tmax, double *Dmax, double *Pmax, int Mode = 0);
		//--General Flash Routines--//		
		DLL int FlashPT(double P, double T, double *H, double *S, double *Cp, double *Cv, double *Rho, double *RhoL, double *RhoV, double *X, Mixture *Liq, Mixture *Vap);
		DLL int FlashPH(double P, double H, double *T, double *S, double *Cp, double *Cv, double *Rho, double *RhoL, double *RhoV, double *X, Mixture *Liq, Mixture *Vap);
		DLL int FlashPS(double P, double S, double *T, double *H, double *Cp, double *Cv, double *Rho, double *RhoL, double *RhoV, double *X, Mixture *Liq, Mixture *Vap);

		//--Combustion Methods--//
		DLL void StoichCombust(Mixture *CmbMix);
		DLL void StoichCombust(Mixture *CmbMix, double *Hc);
		DLL void Combust(double inP, double inT, double stackT, double mFlow, Mixture *CmbMix, double *duty);
		DLL void AdCombust(double inP, double inT, double mFlowIn, Mixture *CmbMix, double *outH, double *mFlowOut);
		DLL double InExcessO2();
		DLL double StackO2();
		DLL double SolveAirMassF(Mixture *Air,double O2, int O2Mode, double fuelMassF);
		DLL double SolveAirMoleF(Mixture *Air,double O2, int O2Mode, double fuelMoleF);
		DLL double SolveFuelMassF(Mixture *Air, double O2, double airMassF);
		DLL double SolveFuelMoleF(Mixture *Air, double O2, double airMoleF);
		DLL double AcidDewPoint();		
		//-- Internal Combustion methods --//
		void _Combust(double inP, double inT, double stackT, double moleF, Mixture *CmbMix, double *duty);
		void _AdCombust(double inP, double inT, double moleF, Mixture *CmbMix, double *outH, double *moleFOut);
		void _StoichCombust(Mixture *CmbMix, double *Hc);
	};

	/// <summary>
	/// The JSteam class is used for all steam and pure fluid thermodynamics, as well as all unit operations.
	/// All typical uses of JSteam will require a JSteam object to be created.
	/// </summary>
	class JSteam
	{
	public:
		//-- ENUMERATIONS --//		
		/// <summary>
		/// All JSteam fluids listed in the order they appear in mixtures.
		/// </summary>
#ifdef VATHNA_GEOTHERM_BUILD
		enum Component {Methane, Ethylene, Ethane, Propylene, Propane, IsoButene, nButane, IsoButane,
						nPentane, IsoPentane, nHexane, Hydrogen, CarbonMonoxide, CarbonDioxide,
						Nitrogen, Oxygen, Water, HydrogenSulfide, SulfurDioxide,
						Ammonia, Benzene, CF3I, D4, D5, D6, MM, MDM, MD2M, MD3M, MD4M, PerFluoroButane, PerFluoroPentane, NeoPentane, Propyne,
						R21, R22, R23, R32, R41, R116, R123, R124, R125, R134A, R141B, R142B, R143A, R152A, R218,
						R227EA, R236EA, R236FA, R245CA, R245FA, RC270, RC318, RE347MCC, Toluene};
#else
		enum Component{Methane,Ethylene,Ethane,Propylene,Propane,IsoButene,nButane,IsoButane,
						nPentane,IsoPentane,nHexane,Hydrogen,CarbonMonoxide,CarbonDioxide,
						Nitrogen,Oxygen,Water,HydrogenSulfide,SulfurDioxide,
						CF3I,D4,D5,D6,MM,MDM,MD2M,MD3M,MD4M,PerFluoroPentane,NeoPentane,
						R32,R41,R125,R134A,R143A,R152A,R218,R227EA,R236EA,R236FA,R245CA,R245FA,RC270,RC318,RE347MCC,Toluene};
#endif
		/// <summary>
		/// Thermodynamic and transport function return types.
		/// </summary>
		enum ThermoType{Enthalpy,Entropy,HeatCapCp,HeatCapCv,Volume,Quality,Temperature,Pressure,Viscosity,ThermalConductivity,HeatNHV,HeatGHV};

		//-- CONSTRUCTOR --//
		/// <summary>
		/// Initializes a new instance of the <see cref="JSteam"/> class. 		
		/// LicenseValidate() must be called after creating the JSteam object in order to activate the licensed modules.
		/// Note this operation is fast - no REFPROP code is called from the constructor.
		/// </summary>
		DLL JSteam();

		//-- STEAM THERMODYNAMIC FUNCTIONS --//
		//--Enthalpy--//
		/// <summary>
		/// Calculates Specific Enthalpy as a function of Pressure and Temperature for Water/Steam [IF-97].
		/// </summary>
		/// <param name="P">Pressure [user units]</param>
		/// <param name="T">Temperature [user units]</param>
		/// <returns>Specific Enthalpy [user units]</returns>
		DLL double HPT(double P, double T);
		/// <summary>
		/// Calculates Specific Enthalpy as a function of Pressure and Specific Entropy for Water/Steam [IF-97].
		/// </summary>
		/// <param name="P">Pressure [user units]</param>
		/// <param name="S">Specific Entropy [user units]</param>
		/// <returns>Specific Enthalpy [user units]</returns>
		DLL double HPS(double P, double S);
		/// <summary>
		/// Calculates Specific Enthalpy as a function of Pressure and Quality for Water/Steam [IF-97].
		/// </summary>
		/// <param name="P">Pressure [user units]</param>
		/// <param name="X">Quality [fraction]</param>
		/// <returns>Specific Enthalpy [user units]</returns>
		DLL double HPX(double P, double X);
		/// <summary>
		/// Calculates Specific Enthalpy as a function of Temperature and Quality for Water/Steam [IF-97].
		/// </summary>
		/// <param name="T">Temperature [user units]</param>
		/// <param name="X">Quality [user units]</param>
		/// <returns>Specific Enthalpy [user units]</returns>
		DLL double HTX(double T, double X);
		/// <summary>
		/// Calculates Specific Enthalpy as a function of Temperature and Specific Entropy for Water/Steam [IF-97].
		/// </summary>
		/// <param name="T">Temperature [user units]</param>
		/// <param name="S">Specific Entropy [user units]</param>
		/// <returns>Specific Enthalpy [user units]</returns>
		DLL double HTS(double T, double S);
		//--Entropy--//
		DLL double SPT(double P, double T);
		DLL double SPH(double P, double H);
		DLL double SPX(double P, double X);
		DLL double STX(double T, double X);
		//-- Isobaric Heat Capacity--//
		DLL double CpPT(double P, double T);
		DLL double CpPH(double P, double H);
		DLL double CpPS(double P, double S);
		DLL double CpPX(double P, double X);
		DLL double CpTX(double T, double X);
		//-- Isochoric Heat Capacity--//
		DLL double CvPT(double P, double T);
		DLL double CvPH(double P, double H);
		DLL double CvPS(double P, double S);
		DLL double CvPX(double P, double X);
		DLL double CvTX(double T, double X);
		//--Volume--//
		DLL double VPT(double P, double T);
		DLL double VPH(double P, double H);
		DLL double VPS(double P, double S);
		DLL double VPX(double P, double X);
		DLL double VTX(double T, double X);
		//--Temperature--//
		DLL double TPH(double P, double H);
		DLL double TPS(double P, double S);
		DLL double THS(double H, double S);
		DLL double TSX(double S, double X);
		DLL double TsatP(double P);
		//--Pressure--//
		/// <summary>
		/// Calculates Pressure as a function of Temperature and Specific Entropy for Water/Steam [IF-97].
		/// </summary>
		/// <param name="T">Temperature [user units]</param>
		/// <param name="S">Specific Entropy [user units]</param>
		/// <returns>Pressure [user units]</returns>
		DLL double PTS(double T, double S);
		/// <summary>
		/// Calculates Pressure as a function of Specific Enthalpy and Specific Entropy for Water/Steam [IF-97].
		/// </summary>
		/// <param name="H">Specific Enthalpy [user units]</param>
		/// <param name="S">Specific Entropy [user units]</param>
		/// <returns>Pressure [user units]</returns>
		DLL double PHS(double H, double S);
		/// <summary>
		/// Calculates Saturated Pressure as a function of Temperature for Water/Steam [IF-97].
		/// </summary>
		/// <param name="T">Temperature [user units]</param>
		/// <returns>Pressure [user units]</returns>
		DLL double PsatT(double T);
		//--Vapour Fraction--//
		/// <summary>
		/// Calculates Quality as a function of Pressure and Specific Enthalpy for Water/Steam [IF-97].
		/// </summary>
		/// <param name="P">Pressure [user units]</param>
		/// <param name="H">Specific Enthalpy [user units]</param>
		/// <returns>Quality [fraction]</returns>
		DLL double XPH(double P, double H);
		/// <summary>
		/// Calculates Quality as a function of Pressure and Specific Entropy for Water/Steam [IF-97].
		/// </summary>
		/// <param name="P">Pressure [user units]</param>
		/// <param name="S">Specific Entropy [user units]</param>
		/// <returns>Quality [fraction]</returns>
		DLL double XPS(double P, double S);
		/// <summary>
		/// Calculates Quality as a function of Temperature and Specific Entropy for Water/Steam [IF-97].
		/// </summary>
		/// <param name="T">Temperature [user units]</param>
		/// <param name="S">Specific Entropy [user units]</param>
		/// <returns>Quality [fraction]</returns>
		DLL double XTS(double T, double S);
		//--Isothermal Compressibility--//
		/// <summary>
		/// Calculates Isothermal Compressibility as a function of Pressure and Temperature for Water/Steam [IF-97].
		/// </summary>
		/// <param name="P">Pressure [user units]</param>
		/// <param name="T">Temperature [user units]</param>
		/// <returns>Isothermal Compressibility (1/MPa)</returns>
		DLL double KtPT(double P, double T);
		//--Viscosity--//
		/// <summary>
		/// Calculates Dynamic Viscosity as a function of Pressure and Temperature for Water/Steam [IF-97].
		/// </summary>
		/// <param name="P">Pressure [user units]</param>
		/// <param name="T">Temperature [user units]</param>
		/// <returns>Dynamic Viscosity [user units]</returns>
		DLL double UPT(double P, double T);
		//--Thermal Conductivity--//
		/// <summary>
		/// Calculates Thermal Conductivity as a function of Pressure and Temperature for Water/Steam [IF-97].
		/// </summary>
		/// <param name="P">Pressure [user units]</param>
		/// <param name="T">Temperature [user units]</param>
		/// <returns>Thermal Conductivity [user units]</returns>
		DLL double KPT(double P, double T);

		//-- REFPROP THERMODYNAMIC FUNCTIONS --//
		//--Enthalpy--//
		/// <summary>
		/// Calculates Specific Enthalpy as a function of Pressure and Temperature for a Specified Pure Fluid [REFPROP].
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <param name="P">Pressure [user units]</param>
		/// <param name="T">Temperature [user units]</param>
		/// <returns>Specific Enthalpy [user units]</returns>
		DLL double HcPT(Component C, double P, double T);
		/// <summary>
		/// Calculates Specific Enthalpy as a function of Pressure and Specific Entropy for a Specified Pure Fluid [REFPROP].
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <param name="P">Pressure [user units]</param>
		/// <param name="S">Specific Entropy [user units]</param>
		/// <returns>Specific Enthalpy [user units]</returns>
		DLL double HcPS(Component C, double P, double S);
		/// <summary>
		/// Calculates Saturated Specific Enthalpy as a function of Pressure and Quality for a Specified Pure Fluid [REFPROP].
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <param name="P">Pressure [user units]</param>
		/// <param name="X">Quality [fraction]</param>
		/// <returns>Specific Enthalpy [user units]</returns>
		DLL double HcPX(Component C, double P, double X);
		/// <summary>
		/// Calculates Saturated Specific Enthalpy as a function of Temperature and Quality for a Specified Pure Fluid [REFPROP].
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <param name="T">Temperature [user units]</param>
		/// <param name="X">Quality [fraction]</param>
		/// <returns>Specific Enthalpy [user units]</returns>
		DLL double HcTX(Component C, double T, double X);
		//--Entropy--//
		DLL double ScPT(Component C, double P, double T);
		DLL double ScPH(Component C, double P, double H);
		DLL double ScPX(Component C, double P, double X);
		DLL double ScTX(Component C, double T, double X);
		//--Isobaric Heat Capacity--//
		DLL double CpcPT(Component C, double P, double T);
		DLL double CpcPS(Component C, double P, double S);
		DLL double CpcPH(Component C, double P, double H);
		DLL double CpcPX(Component C, double P, double X);
		DLL double CpcTX(Component C, double T, double X);
		//--Isochoric Heat Capacity--//
		DLL double CvcPT(Component C, double P, double T);
		DLL double CvcPS(Component C, double P, double S);
		DLL double CvcPH(Component C, double P, double H);
		DLL double CvcPX(Component C, double P, double X);
		DLL double CvcTX(Component C, double T, double X);
		//--Volume--//
		DLL double VcPT(Component C, double P, double T);
		DLL double VcPS(Component C, double P, double S);
		DLL double VcPH(Component C, double P, double H);
		DLL double VcPX(Component C, double P, double X);
		DLL double VcTX(Component C, double T, double X);
		//--Quality--//
		DLL double XcPT(Component C, double P, double T);
		DLL double XcPH(Component C, double P, double H);
		DLL double XcPS(Component C, double P, double S);
		//--Temperature--//
		DLL double TcPH(Component C, double P, double H);
		DLL double TcPS(Component C, double P, double S);
		DLL double TcHS(Component C, double H, double S);
		DLL double TsatcP(Component C, double P);
		DLL double TsatcD(Component C, double d);
		//--Pressure--//
		DLL double PcHS(Component C, double H, double S);
		DLL double PsatcT(Component C, double T);
		DLL double PsatcD(Component C, double d);

		//--Thermo (Vapour Only) Methods--//
		/// <summary>
		/// Calculates Specific Enthalpy (Assuming Vapour) as a function of Pressure and Temperature for a Specified Pure Fluid [REFPROP].
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <param name="P">Pressure [user units]</param>
		/// <param name="T">Temperature [user units]</param>
		/// <returns>Specific Enthalpy [user units]</returns>
		DLL double HcPTvap(Component C, double P, double T);
		/// <summary>
		/// Calculates Specific Entropy (Assuming Vapour) as a function of Pressure and Temperature for a Specified Pure Fluid [REFPROP].
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <param name="P">Pressure [user units]</param>
		/// <param name="T">Temperature [user units]</param>
		/// <returns>Specific Entropy [user units]</returns>
		DLL double ScPTvap(Component C, double P, double T);

		//-- GENERAL TRANSPORT PROPERTY FUNCTIONS --//
		//--Dynamic Viscosity--//
		/// <summary>
		/// Calculates Dynamic Viscosity as a function of Pressure and Temperature for a Specified Pure Fluid [REFPROP].
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <param name="P">Pressure [user units]</param>
		/// <param name="T">Temperature [user units]</param>
		/// <returns>Dynamic Viscosity [user units]</returns>
		DLL double UcPT(Component C, double P, double T);
		/// <summary>
		/// Calculates Dynamic Viscosity as a function of Pressure and Specific Entropy for a Specified Pure Fluid [REFPROP].
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <param name="P">Pressure [user units]</param>
		/// <param name="S">Specific Entropy [user units]</param>
		/// <returns>Dynamic Viscosity [user units]</returns>
		DLL double UcPS(Component C, double P, double S);
		/// <summary>
		/// Calculates Dynamic Viscosity as a function of Pressure and Specific Enthalpy for a Specified Pure Fluid [REFPROP].
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <param name="P">Pressure [user units]</param>
		/// <param name="H">Specific Enthalpy [user units]</param>
		/// <returns>Dynamic Viscosity [user units]</returns>
		DLL double UcPH(Component C, double P, double H);
		/// <summary>
		/// Calculates Saturated Dynamic Viscosity as a function of Pressure and Quality for a Specified Pure Fluid [REFPROP].
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <param name="P">Pressure [user units]</param>
		/// <param name="X">Quality [fraction]</param>
		/// <returns>Dynamic Viscosity [user units]</returns>
		DLL double UcPX(Component C, double P, double X);
		/// <summary>
		/// Calculates Saturated Dynamic Viscosity as a function of Temperature and Quality for a Specified Pure Fluid [REFPROP].
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <param name="T">Temperature [user units]</param>
		/// <param name="X">Quality [fraction]</param>
		/// <returns>Dynamic Viscosity [user units]</returns>
		DLL double UcTX(Component C, double T, double X);
		//--Thermal Conductivity--//
		/// <summary>
		/// Calculates Thermal Conductivity as a function of Pressure and Temperature for a Specified Pure Fluid [REFPROP].
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <param name="P">Pressure [user units]</param>
		/// <param name="T">Temperature [user units]</param>
		/// <returns>Thermal Conductivity [user units]</returns>
		DLL double KcPT(Component C, double P, double T);
		/// <summary>
		/// Calculates Thermal Conductivity as a function of Pressure and Specific Entropy for a Specified Pure Fluid [REFPROP].
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <param name="P">Pressure [user units]</param>
		/// <param name="S">Specific Entropy [user units]</param>
		/// <returns>Thermal Conductivity [user units]</returns>
		DLL double KcPS(Component C, double P, double S);
		/// <summary>
		/// Calculates Thermal Conductivity as a function of Pressure and Specific Enthalpy for a Specified Pure Fluid [REFPROP].
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <param name="P">Pressure [user units]</param>
		/// <param name="H">Specific Enthalpy [user units]</param>
		/// <returns>Thermal Conductivity [user units]</returns>
		DLL double KcPH(Component C, double P, double H);
		/// <summary>
		/// Calculates Saturated Thermal Conductivity as a function of Pressure and Quality for a Specified Pure Fluid [REFPROP].
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <param name="P">Pressure [user units]</param>
		/// <param name="X">Quality [fraction]</param>
		/// <returns>Thermal Conductivity [user units]</returns>
		DLL double KcPX(Component C, double P, double X);
		/// <summary>
		/// Calculates Saturated Thermal Conductivity as a function of Temperature and Quality for a Specified Pure Fluid [REFPROP].
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <param name="T">Temperature [user units]</param>
		/// <param name="X">Quality [fraction]</param>
		/// <returns>Thermal Conductivity [user units]</returns>
		DLL double KcTX(Component C, double T, double X);

		//-- OTHER PROPERTIES --//
		/// <summary>
		/// Calculates the Net Heating Value (NHV) / Lower Heating Value (LHV) for a Specified Pure Fluid [JSteam Routine].
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <returns>Net Heating Value [user units]</returns>
		DLL double NHV(Component C);
		/// <summary>
		/// Calculates the Gross Heating Value (GHV) / Higher Heating Value (HHV) for a Specified Pure Fluid [JSteam Routine]].
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <returns>Gross Heating Value [user units]</returns>
		DLL double GHV(Component C);
		/// <summary>
		/// Calculates the Molecular Weight of a Specified Pure Fluid [REFPROP].
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <returns>Molecular Weight (g/mol)</returns>
		DLL double MW(Component C);
		/// <summary>
		/// Calculates the Specific Enthalpy of Formation for a Specific Pure Fluid [JSteam Routine].
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <returns>Specific Enthalpy of Formation [user units]</returns>
		DLL double Hf(Component C, double T);
		/// <summary>
		/// Calculates the Specific Enthalpy of Formation for a Specific Pure Fluid [REFPROP].
		/// Note there are some small differences between this routine and Hf - being investigated.
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <returns>Specific Enthalpy of Formation [user units]</returns>
		DLL double RefHf(Component C, double T);
		/// <summary>
		/// Return the limits on temperature, pressure and density for accurate calculations using REFPROP of the specified fluid.
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <param name="Tmin">Minimum Temperature [user units] [out]</param>
		/// <param name="Tmax">Maximum Temperature [user units] [out]</param>
		/// <param name="Dmax">Maximum Density [user units] [out]</param>
		/// <param name="Pmax">Maximum Pressure [user units] [out]</param>
		/// <param name="Mode">Limit Mode: 0 Equation of State (normal), 1 Viscosity, 2 Thermal Conductivity, 3 Surface Tension.</param>
		DLL void ComponentLimits(Component C, double *Tmin, double *Tmax, double *Dmax, double *Pmax, int Mode = 0);
		/// <summary>
		/// Return information on the specified fluid from the REFPROP database.
		/// </summary>
		/// <param name="C">Fluid</param>
		/// <param name="MW">Molecular Weight (g/mol) [out]</param>
		/// <param name="TTrip">Triple Point Temperature [user units] [out]</param>
		/// <param name="TNBpt">Normal Boiling Point Temperature [user units] [out]</param>
		/// <param name="Tc">Critical Point Temperature [user units] [out]</param>
		/// <param name="Pc">Critical Point Pressure [user units] [out]</param>
		/// <param name="Vc">Critical Point Volume [user units] [out]</param>
		/// <param name="Zc">Critical Point Compressibility [fraction] [out]</param>
		/// <param name="w">Acentric Factor [out]</param>
		/// <param name="dip">Dipole Moment (debye) [out]</param>
		/// <param name="Rgas">The Universal Gas Constant (J/mol-K) [out]</param>
		DLL void ComponentInfo(Component C, double *MW, double *TTrip, double *TNBpt, double *Tc, double *Pc, double *Vc, double *Zc, double *w, double *dip, double *Rgas);

		//-- STEAM UNIT OPERATIONS --//
		DLL int UnitOp_Boiler(double BFWH, double StmM, double StmT, double StmP, double BDr, double Eff, double *StmH, double *BFWM, double *BDM, double *BDH, double *FirDuty);
		DLL int UnitOp_Compressor1Q(double inH, double inP, double outP, double Eff, double Pwr, double *outM, double *outH);
		DLL int UnitOp_Compressor2Q(double inH, double inP, double outP1, double Eff1, double outM1, double outP2, double Eff2, double TPwr, double *outM2, double *outH1, double *outH2);
		DLL int UnitOp_Compressor1M(double inH, double inP, double outP, double Eff, double outM, double *Pwr, double *outH);
		DLL int UnitOp_Compressor2M(double inH, double inP, double outP1, double Eff1, double outM1, double outP2, double Eff2, double outM2, double *TPwr, double *outH1, double *outH2);
		DLL int UnitOp_CoolerMQ(double inH, double outP, double outM, double Duty, double *outH, double *outT);
		DLL int UnitOp_CoolerTM(double inH, double outP, double outT, double outM, double *outH, double *Duty);
		DLL int UnitOp_CoolerHM(double inH, double outP, double outH, double outM, double *outT, double *Duty);
		DLL int UnitOp_CoolerTQ(double inH, double outP, double outT, double Duty, double *outH, double *outM);				
		DLL int UnitOp_Deaerator(double OpP, double StmH, double WatH, double LiqM, double VTr, double *StmM, double *WatM, double *LiqH, double *VapM);
		DLL int UnitOp_DesuperheaterMout(double StmInH, double WatInH, double outP, double outT, double StmOutM, double *WatInM, double *StmInM, double *StmOutH);
		DLL int UnitOp_DesuperheaterMin(double StmInH, double WatInH, double outP, double outT, double StmInM, double *WatInM, double *StmOutM, double *StmOutH);
		DLL int UnitOp_FlashDrum(double OpP, double inH, double inM, double *VapM, double *VapH, double *LiqM, double *LiqH);
		DLL int UnitOp_HeaterMQ(double inH, double outP, double outM, double Duty, double *outH, double *outT);
		DLL int UnitOp_HeaterTM(double inH, double outP, double outT, double outM, double *outH, double *Duty);
		DLL int UnitOp_HeaterHM(double inH, double outP, double outH, double outM, double *outT, double *Duty);
		DLL int UnitOp_HeaterTQ(double inH, double outP, double outT, double Duty, double *outH, double *outM);		
		DLL int UnitOp_Pump(double inH, double inP, double inM, double outP, double Eff, double *outH, double *Pwr);
		DLL int UnitOp_Turbine1Q(double inH, double inP, double outP, double Eff, double Pwr, double *outM, double *outH);
		DLL int UnitOp_Turbine2Q(double inH, double inP, double outP1, double Eff1, double outM1, double outP2, double Eff2, double TPwr, double *outM2, double *outH1, double *outH2);
		DLL int UnitOp_Turbine3Q(double inH, double inP, double outP1, double Eff1, double outM1, double outP2, double Eff2, double outM2, double outP3, double Eff3, double TPwr, double *outM3, double *outH1, double *outH2, double *outH3);
		DLL int UnitOp_Turbine1M(double inH, double inP, double outP, double Eff, double outM, double *Pwr, double *outH);
		DLL int UnitOp_Turbine2M(double inH, double inP, double outP1, double Eff1, double outM1, double outP2, double Eff2, double outM2, double *TPwr, double *outH1, double *outH2);
		DLL int UnitOp_Turbine3M(double inH, double inP, double outP1, double Eff1, double outM1, double outP2, double Eff2, double outM2, double outP3, double Eff3, double outM3, double *TPwr, double *outH1, double *outH2, double *outH3);
		DLL int UnitOp_Valve(double inH, double inP, double outP, double *outT, double *outX);

		//-- COMBUSTION UNIT OPERATIONS --//
		DLL int UnitOp_BoilerFired(double BFWH, double StmM, double StmT, double StmP, double BDr, Mixture* Fuel, Mixture* Air, double fuelT, double airT, double stackT, double O2, int O2Mode, Mixture *Stack, double *StmH, double *BFWM, double *airM, double *fuelM, double *eff, double *duty);
		DLL int UnitOp_FurnaceM(Mixture* Fuel, Mixture* Air, double fuelT, double airT, double stackT, double O2, int O2Mode, double fuelM, Mixture *Stack, double *airM, double *duty, double *eff);
		DLL int UnitOp_FurnaceQ(Mixture* Fuel, Mixture* Air, double fuelT, double airT, double stackT, double O2, int O2Mode, double duty, Mixture *Stack, double *airM, double *fuelM, double *eff);
		DLL int UnitOp_GasTurbine(Mixture* Fuel, Mixture* Air, double fuelT, double airT, double exhaustT, double airP, double presRatio, double compEff, double GTPower, double GTEff, Mixture *Exhaust, double *airM, double *fuelM, double *exhaustM);
		DLL int UnitOp_GasTurbineInj(Mixture* Fuel, Mixture* Air, double fuelT, double airT, double exhaustT, double airP, double presRatio, double compEff, double StmT, double StmP, double StmM, double GTPower, double GTEff, Mixture *Exhaust, double *airM, double *fuelM, double *exhaustM);
		DLL int UnitOp_HRSG(Mixture* GTExhaust, double exhaustT, double exhaustM, double minAppT, double minStackT, double BFWH, double StmT, double StmP, double BDr, Mixture *Stack, double *StmM, double *econDT, double *sprHDT, double *stackT);
		DLL int UnitOp_HRSGSup(Mixture* GTExhaust, Mixture* SecFuel, int OpMode, double exhaustT, double exhaustM, double secFuelT, double minAppT, double minStackT, double minStackO2, double StmDemand, double BFWH, double StmT, double StmP, double BDr, Mixture *Stack, double *StmM, double *BaseStmM, double *MaxStmM, double *SecFuelM, double *econDT, double *sprHDT, double *stackT, double *stackM);

		//-- ORGANIC RANKINE CYCLE OPERATIONS --//
		DLL int UnitOp_Compressor1Q(Mixture *Fluid, double inH, double inP, double outP, double Eff, double Pwr, double *outM, double *outH);
		DLL int UnitOp_Compressor2Q(Mixture *Fluid, double inH, double inP, double outP1, double Eff1, double outM1, double outP2, double Eff2, double TPwr, double *outM2, double *outH1, double *outH2);
		DLL int UnitOp_Compressor1M(Mixture *Fluid, double inH, double inP, double outP, double Eff, double outM, double *Pwr, double *outH);
		DLL int UnitOp_Compressor2M(Mixture *Fluid, double inH, double inP, double outP1, double Eff1, double outM1, double outP2, double Eff2, double outM2, double *TPwr, double *outH1, double *outH2);
		DLL int UnitOp_CoolerMQ(Mixture *Fluid, double inH, double outP, double outM, double Duty, double *outH, double *outT);
		DLL int UnitOp_CoolerTM(Mixture *Fluid, double inH, double outP, double outT, double outM, double *outH, double *Duty);
		DLL int UnitOp_CoolerHM(Mixture *Fluid, double inH, double outP, double outH, double outM, double *outT, double *Duty);
		DLL int UnitOp_CoolerTQ(Mixture *Fluid, double inH, double outP, double outT, double Duty, double *outH, double *outM);	
		DLL int UnitOp_FlashDrum(Mixture *Fluid, double OpP, double inH, double inM, Mixture *Vap, double *VapM, double *VapH, Mixture *Liq, double *LiqM, double *LiqH);
		DLL int UnitOp_HeaterMQ(Mixture *Fluid, double inH, double outP, double outM, double Duty, double *outH, double *outT);
		DLL int UnitOp_HeaterTM(Mixture *Fluid, double inH, double outP, double outT, double outM, double *outH, double *Duty);
		DLL int UnitOp_HeaterHM(Mixture *Fluid, double inH, double outP, double outH, double outM, double *outT, double *Duty);
		DLL int UnitOp_HeaterTQ(Mixture *Fluid, double inH, double outP, double outT, double Duty, double *outH, double *outM);		
		DLL int UnitOp_Pump(Mixture *Fluid, double inH, double inP, double inM, double outP, double Eff, double *outH, double *Pwr);
		DLL int UnitOp_Turbine1Q(Mixture *Fluid, double inH, double inP, double outP, double Eff, double Pwr, double *outM, double *outH);
		DLL int UnitOp_Turbine2Q(Mixture *Fluid, double inH, double inP, double outP1, double Eff1, double outM1, double outP2, double Eff2, double TPwr, double *outM2, double *outH1, double *outH2);
		DLL int UnitOp_Turbine3Q(Mixture *Fluid, double inH, double inP, double outP1, double Eff1, double outM1, double outP2, double Eff2, double outM2, double outP3, double Eff3, double TPwr, double *outM3, double *outH1, double *outH2, double *outH3);
		DLL int UnitOp_Turbine1M(Mixture *Fluid, double inH, double inP, double outP, double Eff, double outM, double *Pwr, double *outH);
		DLL int UnitOp_Turbine2M(Mixture *Fluid, double inH, double inP, double outP1, double Eff1, double outM1, double outP2, double Eff2, double outM2, double *TPwr, double *outH1, double *outH2);
		DLL int UnitOp_Turbine3M(Mixture *Fluid, double inH, double inP, double outP1, double Eff1, double outM1, double outP2, double Eff2, double outM2, double outP3, double Eff3, double outM3, double *TPwr, double *outH1, double *outH2, double *outH3);
		DLL int UnitOp_Valve(Mixture *Fluid, double inH, double inP, double outP, double *outT, double *outX);

		//--Verification Methods--//		
		/// <summary>
		/// Runs all verification tests including unit conversion, thermo and all unit operations. Returns 1 if all tests passed, or 0 if any fail.
		/// Note: This function is slow!
		/// </summary>
		/// <param name="verbose">Set to true to enable command print out to the console.</param>
		/// <returns>Integer Status Code</returns>
		DLL int VerifyAll(bool verbose=false); 
		/// <summary>
		/// Runs all verification tests including unit conversion, thermo and all unit operations, and writes all results to a specified dump file. 
		/// Also returns 1 if all tests passed, or 0 if any fail.
		/// Note: This function is slow!
		/// </summary>
		/// <param name="filename">FULL path of the dump file to write to.</param>
		/// <exception>JSteamException</exception>
		/// <returns>Integer Status Code</returns>
		DLL int VerifyAllDump(std::string filename); 
		/// <summary>
		/// Verifies all Unit Conversions. Returns 1 if all tests passed, or 0 if any fail.
		/// </summary>
		/// <param name="verbose">Set to true to enable command print out to the console.</param>
		/// <returns>Integer Status Code</returns>
		DLL int VerifyUnitConvs(bool verbose=false);
		/// <summary>
		/// Verifies Steam Thermodynamics. Returns 1 if all tests passed, or 0 if any fail.
		/// </summary>
		/// <param name="verbose">Set to true to enable command print out to the console.</param>
		/// <returns>Integer Status Code</returns>
		DLL int VerifySteamThermo(bool verbose=false);
		/// <summary>
		/// Verifies General Thermodynamics. Returns 1 if all tests passed, or 0 if any fail.
		/// Note: This function is slow!
		/// </summary>
		/// <param name="verbose">Set to true to enable command print out to the console.</param>
		/// <param name="checkThermoResults">Set to true to enable testing of the thermodynamic results (slow part).</param>
		/// <returns>Integer Status Code</returns>	
		DLL int VerifyGenThermo(bool verbose=false, bool checkThermoResults=true);
		/// <summary>
		/// Verifies Steam Unit Operations. Returns 1 if all tests passed, or 0 if any fail.
		/// </summary>
		/// <param name="verbose">Set to true to enable command print out to the console.</param>
		/// <returns>Integer Status Code</returns>
		DLL int VerifySteamUnitOps(bool verbose=false);
		/// <summary>
		/// Verifies Combustion Unit Operations. Returns 1 if all tests passed, or 0 if any fail.
		/// </summary>
		/// <param name="verbose">Set to true to enable command print out to the console.</param>
		/// <returns>Integer Status Code</returns>
		DLL int VerifyCombustUnitOps(bool verbose=false);
		/// <summary>
		/// Verifies ORC Unit Operations. Returns 1 if all tests passed, or 0 if any fail.
		/// </summary>
		/// <param name="verbose">Set to true to enable command print out to the console.</param>
		/// <returns>Integer Status Code</returns>
		DLL int VerifyORCUnitOps(bool verbose=false);
		/// <summary>
		/// Verifies HX Unit Operations. Returns 1 if all tests passed, or 0 if any fail.
		/// </summary>
		/// <param name="verbose">Set to true to enable command print out to the console.</param>
		/// <returns>Integer Status Code</returns>
		DLL int VerifyHXUnitOps(bool verbose=false);
		/// <summary>
		/// BROKEN - DO NOT USE
		/// </summary>
		/// <param name="verbose">Set to true to enable command print out to the console.</param>
		/// <returns>Unknown</returns>
		DLL int Benchmark(bool verbose=false);

		//--License Activation--//
		/// <summary>
		/// Attemps to License JSteam by looking in the current directory for JSteam.lic, then passing it to path based License Validate method.
		/// The methods return the JSteam License Code if successful, otherwise the corresponding error code (ERROR_OPEN, ERROR_READ, ERROR_MAC, ERROR_DATE, ERROR_UNLIM).
		/// Note this method can be slow (seconds) - as validating the license also initializes REFPROP.
		/// </summary>
		/// <exception>JSteamException</exception>
		/// <returns>Integer Status Code</returns>
		DLL int LicenseValidate();
		/// <summary>
		/// Attemps to License JSteam by opening the supplied path to JSteam.lic.
		/// The methods return the JSteam License Code if successful, otherwise the corresponding error code (ERROR_OPEN, ERROR_READ, ERROR_MAC, ERROR_DATE, ERROR_UNLIM).
		/// Note this method can be slow (seconds) - as validating the license also initializes REFPROP.
		/// </summary>
		/// <param name="licpath">FULL path to the JSteam.lic license file.</param>
		/// <exception>JSteamException</exception>
		/// <returns>Integer Status Code</returns>
		DLL int LicenseValidate(std::string licpath);
		/// <summary>
		/// From the supplied encrypted license content (supplied as a char array), decrypt it and extract the license code and details.
		/// If the license is extracted correctly, enable the corresponding license (Eval, Full, etc) including module activation. This
		/// function will also check MAC address and elapsed evaluation time. It will return the license code if successful, otherwise it
		/// will return the corresponding error code (ERROR_READ, ERROR_MAC, ERROR_DATE, ERROR_UNLIM).
		/// Note: This method also initializes REFPROP if it was enabled, thus can be quite slow (seconds).
		/// </summary>
		/// <param name="bytes">The encrypted license bytes.</param>
		/// <param name="blen">The number of bytes in the bytes array.</param>
		/// <param name="details">If decrypted successfully, the details of the license.</param>
		/// <param name="licensee">If decrypted successfully, the name of the licensee.</param>
		/// <returns>Integer Status Code</returns>
		DLL int LicenseValidate(char *licbytes, int len, std::string &details, std::string &licensee);
		/// <summary>
		/// From a supplied license code, build a return string of the functionality enabled by the license code.
		/// Note if the license code is an error, this function will report what the error is.
		/// </summary>
		/// <param name="code">License code</param>
		/// <returns>The license information.</returns>
		DLL std::string LicenseDetails(int code);
		/// <summary>
		/// Checks if a module is currently licensed. Returns true if it is licensed.
		/// </summary>
		/// <param name="module">The module number to check: 
		///	0 - General Thermo + Steam Ops, 
		/// 1 - Combustion Ops, 
		/// 2 - ORC Ops, 
		/// 3 - HX Ops
		/// </param>
		/// <returns>True/False if licensed</returns>
		DLL bool LicenseCheck(int module);

		//--Other--//
		/// <summary>
		/// Sets the REFPROP Fluids folder - the folder where REFPROP looks for the .fld files.
		/// Note: This function does not normally need to be called, as JSteam looks intelligently for the fluid files.
		/// </summary>
		/// <param name="path">The FULL path to the Fluid Files.</param>
		DLL void SetRefpropFluidsFolder(std::string path); 
		/// <summary>
		/// Resets Refprop to initial state (useful after spline calculations).
		/// </summary>
		DLL void ResetRefprop(void);
		/// <summary>
		/// Get the current JSteam Version as a Double
		/// </summary>
		/// <returns>JSteam Version Number</returns>
		DLL double GetJSteamVer();
		/// <summary>
		/// Gets the current REFPROP Version as a Double
		/// </summary>
		/// <returns>REFPROP Version Number</returns>
		DLL double GetRefpropVer();
		/// <summary>
		/// Return the number of components supported. 
		/// </summary>
		/// <param name="mode">Mode 0 - All components, Mode 1 - Natural Gas Components, Mode 2 - Solid Fuel Components</param>
		/// <returns>An integer containing the number of components.</returns>
		DLL static int GetNoComponents(int Mode=0);
		/// <summary>
		/// Match a Fluid Name to a JSteam Fluid Name and return an integer index of it within JSteam Component Enumeration (same index as used in mixtures).
		/// By default this function checks the short names, then the long names, then the alternative names, then the refrigeration names, then the chemical formula.
		/// If the fluid cannot be found the function returns -1, or if the fluid name contains an error, the function returns -2.
		/// NOTE: When comparing chemical formulae there are multiple fluids with the same formula - it is best to use the fluid name.
		/// </summary>
		/// <param name="name">The fluid name to look up.</param>
		/// <returns>Integer index of where the fluid resides with the JSteam component enumeration.</returns>
		DLL static int GetComponentIndex(std::string fluid);
		/// <summary>
		/// Return the name, formula or REFPROP fluid name of a JSteam fluid from the component enumeration.
		/// </summary>
		/// <param name="c">The fluid to look up.</param>
		/// <param name="Mode">The search mode: 
		///	0 - Short Name (no spaces, refrigerants use R codes), 
		///	1 - Full Name, 
		/// 2 - Chemical Formula, 
		/// 3 - Refrigeration Code, 
		/// 4 - REFPROP Name,
		/// 5 - Alternative Name (e.g. nPentane -> Pentane)
		/// </param>
		/// <returns>The fluid name in the specified format.</returns>
		DLL static std::string GetComponentName(Component c, int Mode=0);
	};

	
	/// <summary>
	/// A class with all static methods, used for changing the engineering units across JSteam, Mixture and SolidFuel classes.
	/// </summary>
	class JSteamUnits
	{
	public:
		//Enumerations
		enum PresUnits{bar,barg,psi,psig,kPa,MPa,atm};
		enum TempUnits{C,F,K,R};
		enum MassFUnits{tonnehr,kghr,kgs,lbmhr,lbms,tonUKhr,tonUShr};
		enum MoleFUnits{kmolhr,kmols,molhr,mols};
		enum PowerUnits{kW,MW,W,Btuhr,MMBtuhr,kcalhr,kcals,ftlbfhr,ftlbfs,hpMech,hpMetric,hpElec,hpBlr};
		enum EnergyUnits{kJ,J,cal,kcal,calth,kcalth,Btu,MMBtu};
		enum MassUnits{kg,g,lbm};
		enum MoleUnits{kmol,mol,lbmol};
		enum VolUnits{m3,cm3,in3,ft3,galUK,galUS}; 
		enum VisUnits{uPs,mPs,Ps,uP,mP,cP,P,lbfth,lbfts,lbfsft2,lbfsin2};
		enum ThermCUnits{mWmK,WmK,WcmK,calscmK,kcalhmK,Btuinhft2F,Btuinsft2F,Btufthft2F};	
		enum Basis{mass,mole};
		enum Property{Enthalpy,Entropy,Volume,SpecVolume,Temperature,Pressure,Viscosity,ThermalConductivity,Power,MassF,MoleF,Mass,Mole,Energy,ErrorUnit};

	private:
		//Methods
		static void throwUnknownUnitError(std::string prop, std::string unit, std::vector<std::string> possUnits, bool checkSpecific = false);
		static int MatchUnit(std::vector<std::string> AllUnits, std::string unitname);
		static Property MatchUnitToProperty(std::string unit, std::string &prop);
		static double ConvUnit(double m, double mw, JSteamUnits::Property prop, std::string current_unit, std::string new_unit);	
		static std::string SplitSpecificUnit(std::string unitname, Basis &basisname, MassUnits &massname, MoleUnits &molename);
		static std::vector<std::string> JSteamUnits::GetSpecificUnitComb(std::vector<std::string> num, std::vector<std::string> den, bool isEntr = false);
		static std::string ToLower(std::string str);

	public:				
		//Set Methods		
		static DLL void SetPresUnit(PresUnits P);
		static DLL void SetTempUnit(TempUnits T);
		static DLL void SetMassFUnit(MassFUnits M);
		static DLL void SetMoleFUnit(MoleFUnits M);
		static DLL void SetPowerUnit(PowerUnits P);
		static DLL void SetEnergyUnit(EnergyUnits E);
		static DLL void SetMassUnit(MassUnits M);
		static DLL void SetMoleUnit(MoleUnits M);
		static DLL void SetVolUnit(VolUnits V);
		static DLL void SetVisUnit(VisUnits V);
		static DLL void SetThermCUnit(ThermCUnits T);
		static DLL void SetBasis(Basis B);
		static DLL int SetPresUnit(std::string P);
		static DLL int SetTempUnit(std::string T);
		static DLL int SetMassFUnit(std::string M);
		static DLL int SetMoleFUnit(std::string M);
		static DLL int SetPowerUnit(std::string P);
		static DLL int SetEnergyUnit(std::string E);
		static DLL int SetMassUnit(std::string M);
		static DLL int SetMoleUnit(std::string M);
		static DLL int SetVolUnit(std::string V);
		static DLL int SetVisUnit(std::string U);
		static DLL int SetThermCUnit(std::string K);
		static DLL int SetBasis(std::string B);
		static DLL void SetDefaultUnits();
		static DLL void SetAllUnits(std::vector<std::string> units);

		//Get Methods
		static DLL PresUnits GetPresUnit();
		static DLL TempUnits GetTempUnit();
		static DLL MassFUnits GetMassFUnit();
		static DLL MoleFUnits GetMoleFUnit();
		static DLL PowerUnits GetPowerUnit();
		static DLL EnergyUnits GetEnergyUnit();
		static DLL MassUnits GetMassUnit();
		static DLL MoleUnits GetMoleUnit();
		static DLL VolUnits GetVolUnit();
		static DLL VisUnits GetVisUnit();
		static DLL ThermCUnits GetThermCUnit();
		static DLL Basis GetBasis();
		static DLL std::string GetPresUnitStr();
		static DLL std::string GetTempUnitStr();
		static DLL std::string GetMassFUnitStr();
		static DLL std::string GetMoleFUnitStr();
		static DLL std::string GetFlowUnitStr();
		static DLL std::string GetEnthUnitStr();
		static DLL std::string GetEntrUnitStr();
		static DLL std::string GetHCapUnitStr();
		static DLL std::string GetPowerUnitStr();
		static DLL std::string GetEnergyUnitStr();
		static DLL std::string GetMassUnitStr();
		static DLL std::string GetMoleUnitStr();
		static DLL std::string GetVolUnitStr(bool isSpecific = false);
		static DLL std::string GetVisUnitStr();
		static DLL std::string GetThermCUnitStr();
		static DLL std::string GetBasisStr();
		static DLL std::vector<std::string> GetCurrentUnits();
		/// <summary>
		/// Return an array of all the JSteam Default Units.
		/// [Pres,Temp,MassF,MoleF,Power,Energy,Mass,Mole,Vol,Visc,ThermC,Basis]
		/// </summary>
		/// <returns>Array of Strings</returns>
		static DLL std::vector<std::string> GetDefaultUnits();
		static DLL std::vector<std::string> GetAllPresUnits();
		static DLL std::vector<std::string> GetAllTempUnits();
		static DLL std::vector<std::string> GetAllMassFUnits();
		static DLL std::vector<std::string> GetAllMoleFUnits();
		static DLL std::vector<std::string> GetAllFlowUnits();
		static DLL std::vector<std::string> GetAllPowerUnits();
		static DLL std::vector<std::string> GetAllEnergyUnits();
		static DLL std::vector<std::string> GetAllEnthalpyUnits();
		static DLL std::vector<std::string> GetAllEntropyUnits();
		static DLL std::vector<std::string> GetAllMassUnits();
		static DLL std::vector<std::string> GetAllMoleUnits();
		static DLL std::vector<std::string> GetAllMassAndMoleUnits();
		static DLL std::vector<std::string> GetAllVolUnits(bool isSpecific = false);
		static DLL std::vector<std::string> GetAllVisUnits();
		static DLL std::vector<std::string> GetAllThermCUnits();
		static DLL std::vector<std::string> GetAllBasis();		

		//Conversion Methods
		static DLL double ConvPressure(double P, std::string current_unit, std::string new_unit);
		static DLL double ConvTemperature(double T, std::string current_unit, std::string new_unit);
		static DLL double ConvMassFlow(double m, std::string current_unit, std::string new_unit);
		static DLL double ConvMoleFlow(double m, std::string current_unit, std::string new_unit);
		static DLL double ConvFlow(double m, double mw, std::string current_unit, std::string new_unit);
		static DLL double ConvPower(double pwr, std::string current_unit, std::string new_unit);
		static DLL double ConvEnthalpy(double H, double mw, std::string current_unit, std::string new_unit);
		static DLL double ConvEntropy(double S, double mw, std::string current_unit, std::string new_unit);
		static DLL double ConvHeatCap(double cp, double mw, std::string current_unit, std::string new_unit);
		static DLL double ConvVolume(double v, double mw, std::string current_unit, std::string new_unit);
		static DLL double ConvViscosity(double u, std::string current_unit, std::string new_unit);
		static DLL double ConvThermCond(double k, std::string current_unit, std::string new_unit);
		static DLL double ConvMass(double m, std::string current_unit, std::string new_unit);
		static DLL double ConvMole(double m, std::string current_unit, std::string new_unit);	
		static DLL double ConvUnit(double m, double mw, std::string current_unit, std::string new_unit);
		//Processing Methods	
		static DLL void ProcFlowUnit(std::string current_unit, Basis &b, MassFUnits &ma, MoleFUnits &mo);
		static DLL void ProcEnergyUnit(std::string current_unit, Basis &b, EnergyUnits &e, MassUnits &ma, MoleUnits &mo);
		static DLL void ProcSpecVolUnit(std::string current_unit, Basis &b, VolUnits &v, MassUnits &ma, MoleUnits &mo);

		//Misc
		static DLL std::string MatchUnitToProperty(std::string unit);
		static DLL void PrintAllConv(double v, double mw, std::string current_unit);		
		static DLL double OneAtm();
	};

	//High Performance (uS) Timer
	class HPTimer
	{
		private:
			LARGE_INTEGER start;
			LARGE_INTEGER stop;
			LONGLONG frequency;
			double mul;

		public:
			//Create a new High Performance Timer
			DLL HPTimer();
			//Start Timer
			DLL void Start();
			//Stop Timer and return Time Elapsed in mS
			DLL double Stop();
	};

	//Composition Dec2Base Conversion Class
	class CompConvertor
	{
		public:
			//Main Conversion Routines
			static DLL std::string Dec2Base(double num);
			static DLL double Base2Dec(std::string str);
			//Wrappers
			static DLL std::string Comp2Base(std::vector<double> moleFrac);
			static DLL std::vector<double> Base2Comp(std::string str);			
	};

	//JSteam Exception Class
	class JSteamException: public std::runtime_error
	{
		public:
        DLL JSteamException(std::string const& msg): std::runtime_error(msg)
        {}
	};
	//Exception String Formatting
	class ExFmt
	{
	public:
		ExFmt() {}
		~ExFmt() {}

		template <typename Type>
		ExFmt & operator << (const Type & value)
		{
			stream_ << value;
			return *this;
		}

		std::string str() const         { return stream_.str(); }
		operator std::string () const   { return stream_.str(); }

		enum ConvertToString 
		{
			to_str
		};
		std::string operator >> (ConvertToString) { return stream_.str(); }

	private:
		std::stringstream stream_;

		ExFmt(const ExFmt &);
	};
}