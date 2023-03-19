function [fuelM,Eff,maxEff,maxFuel] = GasTurbineCalc(Fuel,maxQ,Q,Lg)
%GASTURBINECALC  Calculate Gas Turbine Part-Load Values
%
%   [fuelM,Eff,maxEff,maxFuel] = GasTurbineCalc(Fuel,maxQ,Q,Lg)
%
%   Q in kW, M in kg/s, P in bar, H in kJ/kg

if(nargin < 4), Lg = []; end

[~,~,Ffnc,Efffnc,maxEff,maxFuel] = SymUtility.GasTurbineReg(Fuel,maxQ,Lg);

fuelM = Ffnc(Q);
Eff = Efffnc(Q);