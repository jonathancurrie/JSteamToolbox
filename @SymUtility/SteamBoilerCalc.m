function [fuelM,Eff,maxEff] = SteamBoilerCalc(Fuel,maxM,stmM,fuelT,airT,airP,airRelHum,stackT,bfwH,stmT,stmP,bdr,exO2)
%STEAMBOILERCALC  Calculate Steam Boiler Part-Load Values
%
%   [fuelM,Eff,maxEff] = SteamBoilerCalc(Fuel,maxM,stmM,fuelT,airT,airP,airRelHum,stackT,bfwH,stmT,stmP,bdr,exO2)
%
%   M in kg/s, P in bar, T in C, H in kJ/kg


[~,~,Ffnc,Efffnc,maxEff] = SymUtility.SteamBoilerReg(Fuel,maxM,fuelT,airT,airP,airRelHum,stackT,bfwH,stmT,stmP,bdr,exO2);

fuelM = Ffnc(stmM);
Eff = Efffnc(stmM);