function [A,B,maxM,maxEff,Qfnc,Hfnc,Efffnc] = SteamTurbineReg(maxQ,inP,inH,outP)
% STEAMTURBINEREG  Regression Based Steam Turbine Calculation
% Follows Mavromatis Conceptual Optimisation of Utility Networks I
%
% [A,B,maxM,maxEff,Qfnc,Hfnc,Efffnc] = SteamTurbineReg(maxQ,inP,inH,outP)
%
%   with maxQ in kW, inP/outP in bar, inH in kJ/kg and maxM in kg/s
%
%   MINIMUM maxQ is 200kW 

%Save Current Units
cunits = JSteamMEX('GetCurrentUnits');
%This function assumes JSteam defaults
JSteamMEX('SetDefaultUnits');

%Convert to MW
maxQ = maxQ/1e3;

%If maxQ is a vector, assumed [minQ,maxQ]
if(length(maxQ) > 1)
    minQ = maxQ(1); maxQ = maxQ(2);
else
    minQ = 0.4*maxQ;
end

%Get Peterson & Mann Data
[eff,p] = SymUtility.PetersonMannData();

%Generate Sample power points over region of operation
Qfit = logspace(log10(minQ),log10(maxQ),1e2)';

%Generate table of interpolated efficiency data
Efftable = [eff.p14(Qfit) eff.p28(Qfit) eff.p41(Qfit) eff.p61(Qfit) eff.p82(Qfit) eff.p102(Qfit)];

%Gridded Interpolate For Our Pressure
Effp = interp1(p,Efftable',inP,'pchip')';

%Arrange as Qmax./Effmax and solve for straight line fit
pfit = optifit(Qfit,Qfit./Effp,'poly1');
%Assign Coeffs
A = pfit.theta(2)*1e3; %converts for MW->kW
B = pfit.theta(1);

%Solve for maximum efficiency using a single point
maxEff = interp1(p,[eff.p14(maxQ),eff.p28(maxQ),eff.p41(maxQ),eff.p61(maxQ),eff.p82(maxQ),eff.p102(maxQ)],inP,'pchip');

%Estimate Typical Max Mass Flow
dHis = inH - JSteamMEX('HPS',outP,JSteamMEX('SPH',inP,inH));
maxM = (maxQ*1e3)/(dHis*maxEff);

%Build Plottable Functions
Qfnc = @(m) (6/5)*(1/B)*(dHis - A/maxM).*(m - (1/6)*maxM);
Efffnc = @(m) Qfnc(m)./(dHis.*(m));
Hfnc = @(m) inH - dHis*Efffnc(m);

%Return to user units
JSteamMEX('SetCurrentUnits',cunits);

