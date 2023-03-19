function [Ag,Bg,As,Bs,Ffnc,Sfnc,EffFnc,GTEffFnc,CCEffFnc,QFnc,maxGTEff] = HRSGReg(Fuel,maxQ,fuelT,airT,airP,airRelHum,exhaustT,Lg,stackT,bfwH,stmT,stmP,bdr,outP)
% HRSGREG  Regression Based GTG->HRSG Calculation (Base Load)
%
% [Ag,Bg,As,Bs,Cs,Ffnc,Sfnc,EffFnc,GTEfffnc] = HRSGReg(Fuel,[GTG Specs],maxQ,fuelT,airT,airP,airRelHum,Lg,GTG_Lg,stackT,bfwH,stmT,stmP,bdr)
%
%   maxQ in kW, returns Fuel,Steam in kg/s

%Check input mixture
SymUtility.checkMexMixture(Fuel,'HRSG Primary Fuel');
%Save Current Units
cunits = JSteamMEX('GetCurrentUnits');
%This function assumes JSteam defaults
JSteamMEX('SetDefaultUnits');

if(nargin < 14), outP = 0.05; end

%Range of Interest
if(length(maxQ) > 1)
    minQ = maxQ(1); maxQ = maxQ(end);
else
    minQ = 0.3*maxQ;
end
%Constants
nQ = 20; %Number of Q fitting points
minAppT = 20; %C
presRatio = 20; %bar

%Find GTG Regression Model
[Ag,Bg,Ffnc,GTEffFnc,maxGTEff] = SymUtility.GasTurbineReg(Fuel,[minQ maxQ],Lg);

%Set Mass Flow Unit
JSteamMEX('SetUnit','MassFlow','kg/s');

%Fit Steam production as a function of gas turbine output
Q = linspace(minQ,maxQ,nQ)';
stmM = zeros(nQ,1);
%Power vs Steam Flow & Fuel Flow
for i = 1:nQ
    [~, ~, ~, exhaustM, Exhaust] = JSteamMEX('UnitOp_GasTurbine',Fuel, fuelT, airT, airP, airRelHum, exhaustT, presRatio, 0.8, Q(i), GTEffFnc(Q(i)));
    [~, stmM(i)] = JSteamMEX('UnitOp_HRSG',Exhaust, exhaustT, exhaustM, minAppT, stackT, bfwH, stmT, stmP, bdr);
end
%Fit Steam Production
sfit = optifit(Q,stmM,'poly1');
As = sfit.theta(2); Bs = sfit.theta(1);
Sfnc = @(Q) Bs.*Q + As;

if(nargout > 6)
    %Calc Total Efficiency
    stmH = JSteamMEX('HPT',stmP,stmT); bdH = JSteamMEX('HPX',stmP,0); 
    [~,~,~,~,Qfnc] = SymUtility.SteamTurbineReg(0.33*maxQ,stmP,stmH,outP);
    EffFnc = @(Q) (stmH*Sfnc(Q) + bdH*(Sfnc(Q)*bdr) + Q - bfwH*(1 + bdr)*Sfnc(Q))./(Fuel.NHV()*Ffnc(Q));
    %Combined Cycle Efficiency + Power
    CCEffFnc = @(Q) (Qfnc(Sfnc(Q)) + Q)./(JSteamMEX('NHV',Fuel)*Ffnc(Q));
    QFnc = @(Q) (Qfnc(Sfnc(Q)) + Q);
end

%Return to user units
JSteamMEX('SetCurrentUnits',cunits);


