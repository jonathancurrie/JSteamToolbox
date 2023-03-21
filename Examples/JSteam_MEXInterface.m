%% JSteamMEX Examples
clc
clear all
%Load JSteam Interface 
JSteamMEX('Load')

% NOTE - Other than the Steam Thermodynamics Functions below, all others
% require a valid JSteam license.

%% See all possible routines
clc
JSteamMEX('help')

%% Water/Steam Thermodynamics [IAPWS-97]
% - Use the first argument to specify the function you would like to call
clc
% Forward Functions
H = JSteamMEX('HPT',1,100)      % Specific Enthalpy
S = JSteamMEX('SPT',1,100)      % Specific Entropy 
Cp = JSteamMEX('CpPT',1,100)    % Isobaric Heat Capacity
Cv = JSteamMEX('CvPT',1,100)    % Isochoric Heat Capacity
V = JSteamMEX('VPT',1,100)      % Specific Volume

% Reverse Functions
P = JSteamMEX('PHS',H,S)        % Pressure
T = JSteamMEX('THS',H,S)        % Temperature

% - The MEX interface is parallelized as well
p = 1;
t = linspace(100,500,10)';
Hv1 = JSteamMEX('HPT',p,t)

p = linspace(1,10,10)';
t = linspace(100,500,10)';
[P,T] = meshgrid(p,t);
Hv2 = JSteamMEX('HPT',P,T)

%% Water/Steam Thermodynamics with REFPROP
clc
H = JSteamMEX('HcPT','water',1,100) % Specific Enthalpy of Steam
S = JSteamMEX('ScPT','water',1,100) % Specific Entropy of Steam

P = JSteamMEX('PcHS','water',H,S)   % Reverse Pressure
T = JSteamMEX('TcHS','water',H,S)   % Reverse Temperature

%% Vapour-Liquid Sat Curve of Water
clc
% Set Example Units
JSteamMEX('SetUnit','Temperature','C')
JSteamMEX('SetUnit','Pressure','kPa')

% Compute Vapour Pressure (Saturated Pressure) across this temp range
t = 10:10:180;
Psat = JSteamMEX('PSatT',t)

% Plot a nice curve
plot(t,Psat,'.-'); grid on;
xlabel('T_{sat} [^{\circ}C]')
ylabel('P_{sat} [kPa]')
title('Vapour-Liquid Saturation Curve of Water')

% Return to default units
JSteamMEX('SetDefaultUnits')

%% Compute Enthalpy of Vaporization (Latent Heat of Vaporization)
clc

% Reference Pressure
P = JSteamMEX('ConvUnit',1,'atm','bar'); % 1atm in bar

% Determine Enthalpy of Saturated Vapour (X=1) at 1 atm
HsatV = JSteamMEX('HPX',P,1)
X = JSteamMEX('XPH',P,HsatV) % check quality
TsatV = JSteamMEX('TPH',P,HsatV)

% Determine Enthalpy of Saturated Liquid (X=0) at 1 atm
HsatL = JSteamMEX('HPX',P,0)
X = JSteamMEX('XPH',P,HsatL) % check
TsatL = JSteamMEX('TPH',P,HsatL)

% Determine Enthalpy of Vaporization
HVap = HsatV - HsatL

% IAPWS 97 Temperature Error
tempError = TsatV-TsatL % should be 0

%% Water/Steam Transport Properties
clc
K = JSteamMEX('KPT',1,100) % Thermal Conductivity of Steam [mW/(m.K)]
U = JSteamMEX('UPT',1,100) % Viscosity of Steam [uPa.s]

%% See Fluids Available
% - The interface will recognise short or full name, or chemical formula
clc
JSteamMEX('PrintFluids')

%% Pure Fluid Thermodynamics [REFPROP] 
% - Use the second argument to specify which component
clc
H = JSteamMEX('HcPT','Methane',10,100)      % Specific Enthalpy
S = JSteamMEX('ScPT','Methane',10,100)      % Specific Entropy
Cp = JSteamMEX('CpcPT','Methane',10,100)    % Isobaric Heat Capacity
Cv = JSteamMEX('CvcPT','Methane',10,100)    % Isochoric Heat Capacity
V = JSteamMEX('VcPT','Methane',10,100)      % Specific Volume

%% Vapour-Liquid Sat Curve of Oxygen
clc
% Set Example Units
JSteamMEX('SetUnit','Temperature','C')
JSteamMEX('SetUnit','Pressure','kPa')

% Compute Vapour Pressure (Saturated Pressure) across this temp range
t = linspace(-210,-120,100);
Psat = JSteamMEX('PSatcT','Oxygen',t)

% Plot a nice curve
plot(t,Psat,'.-'); grid on;
xlabel('T_{sat} [^{\circ}C]')
ylabel('P_{sat} [kPa]')
title('Vapour-Liquid Saturation Curve of Oxygen')

% Return to default units
JSteamMEX('SetDefaultUnits')

%% Pure Fluid Transport Properties [REFPROP] 
clc
K = JSteamMEX('KcPT','Methane',10,100) % Thermal Conductivity of Methane [mW/(m.K)]
U = JSteamMEX('UcPT','Methane',10,100) % Viscosity of Methane [uPa.s]

%% Other Properties [REFPROP]
clc
NHV = JSteamMEX('NHV','Methane')    % Net (Lower) Heating Value
GHV = JSteamMEX('GHV','Methane')    % Gross (Higher) Heating Value
MW = JSteamMEX('MW','Methane')      % Molecular Weight

%% Fluid Info [REFPROP]
clc
% Temperature, Pressure, Volume of Critical Point (Tc, Pc, Vc)
% Temperature of the Triple Point (TTrip)
% Normal Boiling Temperature (TNBpt)
% Acentric Factor (w)
% Temperature Validity Range (Tmin -> TMax)
% Maximum Valid Density (DMax)
% Maximum Valid Pressure (PMax)
[Tc,Pc,Vc,TTrip,TNBpt,w,TMax,TMin,DMax,PMax] = JSteamMEX('Info','Methane')

%% Mixture Thermodynamics [REFPROP] 
% - Use the second argument to specify the mixture as a cell array [mole fractions]
% {fluid 1, mole frac of fluid 1;
%  fluid 2, mole frac of fluid 2...};
%
% Mixture fractions are always normalized (internally to sum to 1).
clc
gasMix = {'nButane',0.5;'nPentane',0.5};
H  = JSteamMEX('HmPT',gasMix,10,100)    % Specific Enthalpy
S  = JSteamMEX('SmPT',gasMix,10,100)    % Specific Entropy
Cp = JSteamMEX('CpmPT',gasMix,10,100)   % Isobaric Heat Capacity
Cv = JSteamMEX('CvmPT',gasMix,10,100)   % Isochoric Heat Capacity
V  = JSteamMEX('VmPT',gasMix,10,100)    % Specific Volume

%OR
fluids  = {'nButane','nPentane'}; %clearer for larger mixtures
fracs   = [0.5;0.5];
gasMix2 = [fluids(:),num2cell(fracs(:))];
H = JSteamMEX('HmPT',gasMix2,10,100)
%etc

%% Converting between Mass and Mole Fractions
clc
% Mole Fractions
gasMixMoleFrac = {'nButane',0.5;'nPentane',0.5}

% Compute Mass Fractions
gasMixMassFrac = JSteamMEX('mole2mass',gasMixMoleFrac)

% Back to Mole Fractions (just to check)
gasMixMoleFrac_check = JSteamMEX('mass2mole',gasMixMassFrac)

%% Mixture Transport Properties [REFPROP] 
clc
gasMix = {'nButane',0.5;'nPentane',0.5};
K = JSteamMEX('KmPT',gasMix,10,100) % Thermal Conductivity of Mixture [mW/(m.K)]
U = JSteamMEX('UmPT',gasMix,10,100) % Viscosity of Mixture [uPa.s]

%% Steam Unit Operations [IAPWS-97]
clc
[status,stmH,bfwM,bdM,bdH,duty] = JSteamMEX('UnitOp_Boiler',250,10,410,42,0.01,0.9)
[status,outM,outH] = JSteamMEX('UnitOp_Turbine1Q',3500,42,3,0.7,500)


%% Combustion Unit Operations [REFPROP] 
clc
% Fuel
fluids = {'methane','hydrogen','nitrogen','water','hydrogensulfide'};
fracs = [0.95,0.01,0.02,0.01,0.01];
Fuel = [fluids(:) num2cell(fracs(:))];
% Operating Specs
fuelT = 30;
airT = 30;
airP = 1.01325;
airRelHum = 0.5;
minStackT = 1000;
O2 = 0.1;
O2Mode = 0;
fuelM = 1;
% Solve Unit
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

