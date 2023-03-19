function [Q,Eff,H,maxEff,maxM] = SteamTurbineCalc(maxQ,inM,inP,inH,outP)
%STEAMTURBINECALC  Calculate Steam Turbine Part-Load Values
%
%   [Q,Eff,H,maxEff,maxM] = SteamTurbineCalc(maxQ,inM,inP,inH,outP)
%
%   Q in kW, M in kg/s, P in bar, H in kJ/kg


[~,~,maxM,maxEff,Qfnc,Hfnc,Efffnc] = SymUtility.SteamTurbineReg(maxQ,inP,inH,outP);

Q = Qfnc(inM);
Eff = Efffnc(inM);
H = Hfnc(inM);
