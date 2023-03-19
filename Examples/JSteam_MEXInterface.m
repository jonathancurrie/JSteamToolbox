%% JSteamMEX Examples
% It is optional to use the JSteam class, and is in fact 2-3x faster to use 
% the MEX interface directly. The downside is slightly more obtuse code.

clc
clear all
format compact
%Load JSteam Interface 
JSteamMEX('Load')

% NOTE - Other than the Steam Thermodynamics Functions below, all others
% require a valid JSteam license.

%% See all possible routines
clc
JSteamMEX('help')

%% Steam Thermodynamics [IAPWS-97]
% - Use the first argument to specify the function you would like to call
clc
H = JSteamMEX('HPT',1,100)
S = JSteamMEX('SPT',1,100)

% - The MEX interface is parallelized as well
p = 1;
t = linspace(100,500,10)';
Hv = JSteamMEX('HPT',p,t)


%% General Fluid Thermodynamics [REFPROP] 
% - Use the second argument to specify which component
clc
H2 = JSteamMEX('HcPT','Methane',10,100)
S2 = JSteamMEX('ScPT','Methane',10,100)
NHV = JSteamMEX('NHV','Methane')


%% See Fluids Available
% - The interface will recognise short or full name, or chemical formula
clc
JSteamMEX('PrintFluids')


%% Mixture Thermodynamics [REFPROP] 
% - Use the second argument to specify the mixture as a cell array [mole fractions]
% {fluid 1, mole frac of fluid 1;
%  fluid 2, mole frac of fluid 2...};
%
% Mixture fractions are always normalized (internally to sum to 1).
clc
H3 = JSteamMEX('HmPT',{'nButane',0.5;'nPentane',0.5},10,100)
%OR
fluids = {'nButane','nPentane'}; %clearer for larger mixtures
fracs = [0.5;0.5];
H4 = JSteamMEX('HmPT',[fluids(:),num2cell(fracs(:))],10,100)


%% Steam Unit Operations [IAPWS-97]
clc
[status,stmH,bfwM,bdM,bdH,duty] = JSteamMEX('UnitOp_Boiler',250,10,410,42,0.01,0.9)
[status,outM,outH] = JSteamMEX('UnitOp_Turbine1Q',3500,42,3,0.7,500)


%% Combustion Unit Operations [REFPROP] 
clc
%Fuel
fluids = {'methane','hydrogen','nitrogen','water','hydrogensulfide'};
fracs = [0.95,0.01,0.02,0.01,0.01];
Fuel = [fluids(:) num2cell(fracs(:))];
%Operating Specs
fuelT = 30;
airT = 30;
airP = 1.01325;
airRelHum = 0.5;
minStackT = 1000;
O2 = 0.1;
O2Mode = 0;
fuelM = 1;
%Solve Unit
[status,airM,Duty,FEff,acidDp,Stack] = JSteamMEX('UnitOp_FurnaceM',Fuel,fuelT,airT,airP,airRelHum,minStackT,O2,O2Mode,fuelM)

%% Refrigerant Unit Operations [REFPROP]
clc
%Refrigerant
fluids = {'R134A','R143A'};
fracs = [0.9;0.1];
Refrig = [fluids(:) num2cell(fracs(:))];
%Operating Specs
inP = 10;
inH = JSteamMEX('HmPT',Refrig,inP,250)
outP = 1.5;
Eff = 0.8;
Pwr = 1000;
%Solve Unit
[status,outM,outH] = JSteamMEX('UnitOp_Turbine1QC',Refrig,inH,inP,outP,Eff,Pwr)
outX = JSteamMEX('XmPH',Refrig,outP,outH)

%% Customizable Units
% - The main units are customizable in JSteam, listed from the below
% function
clc
JSteamMEX('PrintUnits')

%% Changing Units Example
% - Simply pass the unit as a string to the corresponding function.
clc
JSteamMEX('SetUnit','Pressure','MPa')
JSteamMEX('SetUnit','Temperature','K')

H = JSteamMEX('HPT',0.1,373.15)

%Get Current Units [to check]
JSteamMEX('GetCurrentUnits')
%Revert to default
JSteamMEX('SetDefaultUnits')

H = JSteamMEX('HPT',1,100)

%% Unit Conversion
% - Convert between any of the available JSteam Units
clc
% Same Basis
JSteamMEX('ConvUnit',10,'kg/s','kg/hr')
% New Basis
JSteamMEX('ConvUnit',350,'kJ/kg','cal(th)/lbmol',18)


%% Pure Fluid Info
clc
[Tc,Pc,Vc,TTrip,TNBpt,w,TMax,TMin,DMax,PMax] = JSteamMEX('Info','Water')


%% Mixture Info
clc
fluids = {'nButane','nPentane'};
fracs = [0.5,0.5];
Fluid = [fluids(:) num2cell(fracs(:))];
[Tc,Pc,Vc,TTrip,TNBpt,w,TMax,TMin,DMax,PMax] = JSteamMEX('Info',Fluid)

%% TS Diagram (beta testing)
clc
n = 200;
fluids = {'nButane','nPentane'};
fracs = [0.5,0.5];
Fluid = [fluids(:) num2cell(fracs(:))];
%Enable Refprop Splines by calling Info Routine First! (important before using density routine!)
JSteamMEX('Info',Fluid);
%Use Density Routine to Find Sat T (kg/m3)
d = linspace(50,740,n); %problem is still how to find this range....
t = JSteamMEX('TsatmD',Fluid,d);

%Using calculated Sat T, draw TS
clf
plot(JSteamMEX('SmTX',Fluid,t,0),t)
hold on
plot(JSteamMEX('SmTX',Fluid,t,1),t,'r')
plot(JSteamMEX('SmTX',Fluid,t,0.25),t,'m')
plot(JSteamMEX('SmTX',Fluid,t,0.5),t,'m')
plot(JSteamMEX('SmTX',Fluid,t,0.75),t,'m')
xlabel('Entropy [kJ/(kg K)]');
ylabel('Temp [C]');

