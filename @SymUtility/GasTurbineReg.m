function [a,b,Ffnc,Efffnc,maxEff,maxFuel,Ag,Bg] = GasTurbineReg(Fuel,maxQ,Lg)
% GASTURBINEREG  Regression Based Gas Turbine Calculation
% Follows Varbanov MODELLING AND OPTIMIZATION OF UTILITY SYSTEMS
%
% [a,b,Ffnc,Efffnc] = GasTurbineReg(Fuel,maxQ,Lg)
%
%   maxQ in kW, returns Fuel in kg/s

%Loss Fraction
if(nargin < 3 || isempty(Lg)), Lg = 0.2; end

%Check input mixture
SymUtility.checkMexMixture(Fuel,'Gas Turbine Fuel');
%Save Current Units
cunits = JSteamMEX('GetCurrentUnits');
%This function assumes JSteam defaults
JSteamMEX('SetDefaultUnits');

%Max Efficiency Coeffs [Q in MW]
ae = 1.116; be = 0.01389; ce = -0.8416;
%Range of Interest
if(length(maxQ) > 1)
    minQ = maxQ(1); maxQ = maxQ(end);
else
    minQ = 0.3*maxQ;
end
%Fit linearization just over range of interest
Q = linspace(minQ,maxQ)';
linfit = optifit(Q,Q./(ae.*(Q./1e3).^be + ce),'poly1');
Ag = linfit.theta(2); Bg = linfit.theta(1);
%Fuel Energy (mass basis)
Qfuel = JSteamMEX('NHV',Fuel);

%Max Efficiency for given Turbine Size [MW]
maxEff = ae.*(maxQ./1e3).^be + ce;
maxFuel = maxQ/(maxEff*Qfuel);

%Gradient
b = (Lg + 1)/Bg * (Qfuel - Ag/maxFuel);
%Intercept
a = Lg/Bg*(Qfuel * maxFuel - Ag);

%Plottable Functions
Ffnc = @(Q) (Q + a)./b;
Efffnc = @(Q) Q./(Ffnc(Q)*Qfuel);

%Return to user units
JSteamMEX('SetCurrentUnits',cunits);


%% Regression Calcs
% % Rated Efficiency Curve Aguilar [MW Eff%]
% aeff=[1648.0891	27.309092
% 2350.3184	28.181818
% 3066.879	28.945454
% 3883.758	29.672728
% 5030.255	30.254545
% 5961.783	30.50909
% 6835.9873	30.690908
% 7853.5034	30.8
% 8527.07	30.872726];
% afit = fit(aeff(:,1)/1000,aeff(:,2)/100,'exp2');
% %Varbanov Coeffs [26.1MW -> 255.6MW]
% va = 21.9917; vb = 2.6683;
% %Fit Points
% WA = linspace(2,8,20)';
% WV = linspace(80,255.6,1e2)';
% WF = [WA;WV];
% EF = [afit(WA);WV./(va + vb.*WV)];
% completefit = fit(WF,EF,'power2')
% %Now Find Linear Coeffs
% Wc = linspace(2,255,1e3)'; 
% lincompfit = fit(Wc,Wc./completefit(Wc),'poly1');
% Ag = lincompfit.p2
% Bg = lincompfit.p1





