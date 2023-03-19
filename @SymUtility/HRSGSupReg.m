function [Af,Bf,Cf,Abs,Bbs,Ams,Bms,Ffnc,BSFnc,MSFnc,Q,StmD,TfuelM,baseM,maxM,maxM0,Ag,Bg] = HRSGSupReg(Fuel,maxQ,fuelT,airT,airP,airRelHum,exhaustT,Lg,SFuel,stackT,stackO2,bfwH,stmT,stmP,bdr,npts) 
% HRSGSUPREG  Regression Based GTG->HRSG Calculation (Sec Fired)
%
% [Af,Bf,Cf,Abs,Bbs,Ams,Bms,Ffnc,BSFnc,MSFnc,Q,StmD,TfuelM,baseM,maxM,maxM0,Ag,Bg] = HRSGSupReg(Fuel,maxQ,fuelT,airT,airP,airRelHum,exhaustT,Lg,SFuel,stackT,stackO2,bfwH,stmT,stmP,bdr)
%
%   maxQ in kW, returns Fuel,Steam in kg/s

if(nargin < 16), npts = 20; end

%Check input mixtures
SymUtility.checkMexMixture(Fuel,'HRSG Primary Fuel');
SymUtility.checkMexMixture(SFuel,'HRSG Secondary Fuel');
%Save Current Units
cunits = JSteamMEX('GetCurrentUnits');
%This function assumes JSteam defaults
JSteamMEX('SetDefaultUnits');


%Range of Interest
if(length(maxQ) > 1)
    minQ = maxQ(1); maxQ = maxQ(end);
else
    minQ = 0.3*maxQ;
end
%Constants
nQ = npts; %Number of Q fitting points
minAppT = 20; %C
presRatio = 20; %bar

%Find GTG Regression Model
[Ag,Bg,~,GTEffFnc] = SymUtility.GasTurbineReg(Fuel,[minQ maxQ],Lg);

%Set Mass Flow Unit
JSteamMEX('SetUnit','MassFlow','kg/s');

%Preallocate
TfuelM = zeros(nQ,nQ);
baseM = zeros(nQ,1);
maxM = zeros(nQ,1);
Q = linspace(minQ,maxQ,nQ)';
%Get Appropriate range of Steam Demand to fit Over (remember 50 is arbitrary)
%First Lowest Q
[~,~,~,exhaustM,Exhaust] = JSteamMEX('UnitOp_GasTurbine', Fuel, fuelT, airT, airP, airRelHum, exhaustT, presRatio, 0.8, min(Q), GTEffFnc(min(Q)));
[~, ~,baseM0] = JSteamMEX('UnitOp_HRSGSup', Exhaust, SFuel, 1, exhaustT, exhaustM, fuelT, minAppT, stackT, stackO2, 50, bfwH, stmT, stmP, bdr);
%Then Highest Q
[~,~,~,exhaustM,Exhaust] = JSteamMEX('UnitOp_GasTurbine', Fuel, fuelT, airT, airP, airRelHum, exhaustT, presRatio, 0.8, max(Q), GTEffFnc(max(Q)));
[~, ~,~,maxM0] = JSteamMEX('UnitOp_HRSGSup', Exhaust, SFuel, 1, exhaustT, exhaustM, fuelT, minAppT, stackT, stackO2, 50, bfwH, stmT, stmP, bdr);
%Generate steam demand fitting data
StmD = linspace(baseM0,maxM0,nQ);
%Power vs Total Fuel Flow, Base Steam and Max Steam
for i = 1:nQ
    [~, ~, fuelM,exhaustM,Exhaust] = JSteamMEX('UnitOp_GasTurbine', Fuel, fuelT, airT, airP, airRelHum, exhaustT, presRatio, 0.8, Q(i), GTEffFnc(Q(i)));
    [~, ~,baseM(i),maxM(i)] = JSteamMEX('UnitOp_HRSGSup', Exhaust, SFuel, 1, exhaustT, exhaustM, fuelT, minAppT, stackT, stackO2, 50, bfwH, stmT, stmP, bdr);
    for j = 1:nQ
        [stat, ~,~,~,secfM] = JSteamMEX('UnitOp_HRSGSup', Exhaust, SFuel, 1, exhaustT, exhaustM, fuelT, minAppT, stackT, stackO2, StmD(j), bfwH, stmT, stmP, bdr);
        if(stat==11 || stat ==12) %avoid > max steam or < base load to keep linear
            TfuelM(j,i) = NaN;
        else
            TfuelM(j,i) = secfM + fuelM;
        end
    end
end

%Fit Total Fuel Requirement
% [xData, yData, zData] = prepareSurfaceData( Q./1e3, StmD, TfuelM );
% ft = fittype('poly11');
% opts = fitoptions(ft);
% Fuelf = fit([xData, yData], zData, ft, opts);
% Af = Fuelf.p00; Bf = Fuelf.p10; Cf = Fuelf.p01;
Fuelf = optifit(Q./1e3,{StmD TfuelM},'poly11');
Af = Fuelf.theta(3); Bf = Fuelf.theta(1); Cf = Fuelf.theta(2);

%Fit Base and Max Steam
Basef = optifit(Q,baseM,'poly1');
Abs = Basef.theta(2); Bbs = Basef.theta(1);
Maxf = optifit(Q,maxM,'poly1');
Ams = Maxf.theta(2); Bms = Maxf.theta(1);

%Plottable Functions
BSFnc = @(Q) Abs + Bbs*Q;
MSFnc = @(Q) Ams + Bms*Q;
Ffnc = @(Q,St) Af + Bf*(Q./1e3) + Cf*St;

%Return to user units
JSteamMEX('SetCurrentUnits',cunits);


