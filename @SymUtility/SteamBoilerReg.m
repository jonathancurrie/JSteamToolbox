function [a,b,Ffnc,Efffnc,maxEff,ar,br] = SteamBoilerReg(Fuel,maxM,fuelT,airT,airP,airRelHum,stackT,bfwH,stmT,stmP,bdr,exO2)
% STEAMBOILERREG  Regression Based Steam Boiler Calculation
% Follows Shang A transhipment model for the optimisation of steam levels of total site utility system for multiperiod operation
%
% [a,b,Ffnc,Efffnc,maxEff,ar,br] = SteamBoilerReg(Fuel,maxM,fuelT,airT,airP,airRelHum,stackT,bfwH,stmT,stmP,bdr,exO2)

%Check input mixture
SymUtility.checkMexMixture(Fuel,'Steam Boiler Fuel');
%Save Current Units
cunits = JSteamMEX('GetCurrentUnits');
%This function assumes JSteam defaults
JSteamMEX('SetDefaultUnits');

%Shang + Kokossis Regressed Heat Loss Data from Pattison & Sharma, 1980
a = 0.0126; b = 0.2156;
%Range of Interest
if(length(maxM) > 1)
    minM = maxM(1); maxM = maxM(end);
else
    minM = 0.1*maxM;
end
%Two Test Points minM and maxM
[~, ~, ~, ~, fuelMin] = JSteamMEX('UnitOp_BoilerFired',bfwH, minM, stmT, stmP, bdr, Fuel, fuelT, airT, airP, airRelHum, stackT, exO2, 0);
[~, ~, ~, ~, fuelMax,maxEff] = JSteamMEX('UnitOp_BoilerFired',bfwH, maxM, stmT, stmP, bdr, Fuel, fuelT, airT, airP, airRelHum, stackT, exO2, 0);
%Fuel Energy
FuelQ = JSteamMEX('NHV',Fuel);

%Thermo Props from manual calc
HBD = JSteamMEX('HPX',stmP,0); HST = JSteamMEX('HPT',stmP,stmT); inH = bfwH; M = [minM;maxM];
Qs = -(inH - HST - bdr*HBD + bdr*inH).*M; 
Qf = [fuelMin;fuelMax].*FuelQ;
%Fit Shang and Kokossis Model
pfit = optifit(M./maxM,(Qf-Qs)./Qs .* M./maxM,'poly1');
%New Coeffs
br = pfit.theta(1); ar = a*br/b; %note ar is just ratio of change

%Intercept
a = -maxM*ar*(inH - HST - bdr*HBD + bdr*inH) / FuelQ;
%Gradient
b = -(br + 1)*(inH - HST - bdr*HBD + bdr*inH) / FuelQ;

%Plottable Functions
Ffnc = @(m) (a + b*m);
Efffnc = @(m) m./(maxM*ar + m*(br + 1));

%Return to user units
JSteamMEX('SetCurrentUnits',cunits);

%% Derivation of Above
% syms M BD HBD HBF HST Mm a b
% 
% Q = (M*(1+BD)*(HBD - HBF) + M*(HST-HBD))./M.*((1+b)*M + a*Mm)
% 
% grad = simplify(diff(Q,M))
% int = simplify(Q - diff(Q,M)*M)