%% JSteam Unit Operation Examples
% J.Currie Mar 2023
clc
clear
JSteamMEX('Load');

%% Simple Steam Boiler (Boiler)
clc

% Inputs
BFWT = 25;  % Inlet Boiler Feed Water Temperature [C]
BFWP = 5;   % Inlet Boiler Feed Water Pressure [bar]
BFWH = JSteamMEX('HPT',BFWP,BFWT); % Inlet Boiler Feed Water Enthalpy [kJ/kg]

StmM = 5;   % Outlet Steam Mass Flow Rate [tonne/h]
StmT = 410; % Outlet Steam Temperature [C]
StmP = 42;  % Outlet Steam Pressure [bar]

BDr  = 0.01; % Blowdown Ratio [fraction]
Eff  = 0.9;  % Firing Efficiency [fraction]

[status,StmH,BFWM,BDM,BDH,Duty] = JSteamMEX('UnitOp_Boiler',BFWH,StmM,StmT,StmP,BDr,Eff)

%% Multi-Stage Turbine (Turbine2M)
clc

% Inputs
inT  = 410;     % Inlet Temperature [C]
inP  = 42;      % Inlet Pressure [bar]
inH  = JSteamMEX('HPT',inP,inT); % Inlet Enthalpy [kJ/kg]

outP1 = 15;     % First Stage Outlet Pressure [bar]
Eff1  = 0.4;    % First Stage Isentropic Efficiency [fraction]
outM1 = 10;     % First Stage Outlet Mass Flow [tonne/hr]

outP2 = 3;      % Second Stage Outlet Pressure [bar]
Eff2  = 0.5;    % Second Stage Isentropic Efficiency [fraction]
outM2 = 4;      % Second Stage Outlet Mass Flow [tonne/hr]

[status,TPwr,outH1,outH2] = JSteamMEX('UnitOp_Turbine2M',inH,inP,outP1,Eff1,outM1,outP2,Eff2,outM2)

%% Heat Exchanger 1
% Counter Flow Shell & Tube, Inlet Specified, No Phase Change, Liquid/Liquid
clc
clear

% Specifications
shell.inP = 10;     % Shell, Inlet Pressure [bar]
shell.inT = 60;     % Shell, Inlet Temperature [C]
shell.inM = 36;     % Shell, Inlet Mass Flow Rate [tonne/hr]

tube.inP  = 10;     % Tube, Inlet Pressure [bar]
tube.inT  = 120;    % Tube, Inlet Temperature [C]
tube.inM  = 36;     % Tube, Inlet Mass Flow Rate [tonne/hr]

duty = 50;          % Heat Exchanger Duty [kW]

% Solve Heater (shell) - Know M+Q, Solve H+T
shell.inH  = JSteamMEX('HPT',shell.inP,shell.inT); % Inlet Enthalpy [kj/kg]
shell.outP = shell.inP; % Assume no pressure drop, [bar]
shell.outM = shell.inM; % Assume no mass loss [tonne/hr]
[status,shell.outH,shell.outT] = JSteamMEX('UnitOp_HeaterMQ',shell.inH,shell.outP,shell.outM,duty);

% Solve Cooler (tube) - Know M+Q, Solve H+T
tube.inH  = JSteamMEX('HPT',tube.inP,tube.inT); % Inlet Enthalpy [kj/kg]
tube.outP = tube.inP; % Assume no pressure drop, [bar]
tube.outM = tube.inM; % Assume no mass loss [tonne/hr]
[status,tube.outH,tube.outT] = JSteamMEX('UnitOp_CoolerMQ',tube.inH,tube.outP,tube.outM,duty);

% View Results
shell
tube

%% Heat Exchanger 2
% Counter Flow Shell & Tube, Phase Change, Liquid/Steam
clc
clear

% Specifications
shell.inP = 1;      % Shell, Inlet Pressure [bar]
shell.inT = 90;     % Shell, Inlet Temperature [C]
shell.inM = 0.9;    % Shell, Inlet Mass Flow Rate [tonne/hr]

tube.inP  = 1;      % Tube, Inlet Pressure [bar]
tube.inT  = 500;    % Tube, Inlet Temperature [C]
tube.inM  = 3.6;    % Tube, Inlet Mass Flow Rate [tonne/hr]
tube.outT = 150;    % Tube, Outlet Temperature [C]

% Solve Cooler (tube) - Know T+M, Solve H+Q
tube.inH  = JSteamMEX('HPT',tube.inP,tube.inT); % Inlet Enthalpy [kj/kg]
tube.outP = tube.inP; % Assume no pressure drop, [bar]
tube.outM = tube.inM; % Assume no mass loss [tonne/hr]
[status,tube.outH,duty] = JSteamMEX('UnitOp_CoolerTM',tube.inH,tube.outP,tube.outT,tube.outM);

% Solve Heater (shell) - Know M+Q, Sovel H+T
shell.inH  = JSteamMEX('HPT',shell.inP,shell.inT); % Inlet Enthalpy [kj/kg]
shell.outP = shell.inP; % Assume no pressure drop, [bar]
shell.outM = shell.inM; % Assume no mass loss [tonne/hr]
[status,shell.outH,shell.outT] = JSteamMEX('UnitOp_HeaterMQ',shell.inH,shell.outP,shell.outM,duty);
shell.outX = JSteamMEX('XPH',shell.outP,shell.outH); % Check Phase

% View Results
duty
shell
tube

%% Heat Exchanger 2 with lower shell mass flow rate
% Counter Flow Shell & Tube, Phase Change, Liquid/Steam
clc
% Specifications
shell.inM = 0.8;    % Shell, Inlet Mass Flow Rate [tonne/hr]

% Solve Heater (shell) - Know M+Q, Sovel H+T
shell.inH  = JSteamMEX('HPT',shell.inP,shell.inT); % Inlet Enthalpy [kj/kg]
shell.outP = shell.inP; % Assume no pressure drop, [bar]
shell.outM = shell.inM; % Assume no mass loss [tonne/hr]
[status,shell.outH,shell.outT] = JSteamMEX('UnitOp_HeaterMQ',shell.inH,shell.outP,shell.outM,duty);
shell.outX = JSteamMEX('XPH',shell.outP,shell.outH); % Check Phase

% View Results
duty
shell
tube

%% Fired Steam Boiler (BoilerFired)
clc

% Inputs
BFWT = 25;   % Inlet Boiler Feed Water Temperature [C]
BFWP = 5;    % Inlet Boiler Feed Water Pressure [bar]
BFWH = JSteamMEX('HPT',BFWP,BFWT); % Inlet Boiler Feed Water Enthalpy [kJ/kg]

StmM = 5;    % Outlet Steam Mass Flow Rate [tonne/h]
StmT = 410;  % Outlet Steam Temperature [C]
StmP = 42;   % Outlet Steam Pressure [bar]

BDr  = 0.01; % Blowdown Ratio [fraction]

fuel = JSteamMEX('TypNatGas'); % Typical Natural Gas mixture
fuelT = -20; % Fuel Temperature [C]
airT = 30;   % Air Temperature [C]
airP = JSteamMEX('OneAtm'); % Air Pressure [bar]
airRelHum = 0.5; % Air Relative Humidity [fraction]

minStackT = 200; % Minimum Stack Temperature [C]
O2   = 0.01;     % Minimum Stack O2 Mole Fraction
O2Mode = 1;      % 0 Inlet Spec, 1 Outlet Spec

[status,StmH,BFWM,airM,fuelM,FEff,Duty,Stack] = JSteamMEX('UnitOp_BoilerFired',BFWH,StmM,StmT,StmP,BDr,fuel,fuelT,airT,airP,airRelHum,minStackT,O2,O2Mode)

%% Gas Turbine with Steam Injection coupled to HRSG with Supplemental Firing
clc

% Gas Turbine Inputs
fuel  = {'Methane',0.9;'Ethane',0.08;'Propane',0.02};   % Fuel Mixture Mole Fractions
fuelT = -20;     % Fuel Temperature [C]
airT  = 30;      % Air Temperature [C]
airP  = 1.01325; % Air Pressure [bar]
airRelHum = 0.5; % Air Relative Humidity [fraction]

exhaustT = 300;  % GTG Exhaust Temperature [C]
presRatio = 20;  % Ratio of inlet to outlet compressure pressure
compEff  = 0.75; % Compressor Isentropic Efficiency [fraction]

StmT = 410;      % Injected Steam Temperature [C]
StmP = 42;       % Injected Steam Pressure [bar]
StmM = 1;        % Injected Steam Mass Flow Rate [tonne/hr]

GTPower = 15000; % GTG Output Power [kW]
GTEff = 0.3;     % GTG Overall Mechanical+Electrical Efficiency [fraction]

% Solve Gas Turbine
[status,airM,fuelM,exhaustM,Exhaust] = JSteamMEX('UnitOp_GasTurbineInj',fuel,fuelT,airT,airP,airRelHum,exhaustT,presRatio,compEff,StmT,StmP,StmM,GTPower,GTEff)

% HRSG Inputs
secFuel    = fuel;  % Assume same fuel for both units
secFuelT   = fuelT;
OpMode     = 1;     % 0 Base Load Only, 1 Meet Demand, 2 Maximum Firing

minAppT    = 30;    % Minimum internal delta-T [C]
minStackT  = 200;   % Minimum stack temperature [C]
minStackO2 = 0.01;  % Minimum stack O2 mole fraction

StmDemand  = 50;    % Steam Demand [tonne/hr]
StmT       = 410;   % Steam Temperature [C]
StmP       = 42;    % Steam Pressure [bar]
BFWH       = 350;   % Boiler Feed Water Enthalpy [kJ/kg]
BDr        = 0.01;  % Blowdown Ratio

[status,StmM,BaseStmM,MaxStmM,SecFuelM,econDT,sprHDT,stackT,stackM,Stack] = JSteamMEX('UnitOp_HRSGSup',Exhaust,secFuel,OpMode,exhaustT,exhaustM,secFuelT,minAppT,minStackT,minStackO2,StmDemand,BFWH,StmT,StmP,BDr)


%% ORC Evaporator
clc
clear
% Follows: https://www.controlengineering.co.nz/Wikis/JSteam/pmwiki.php/Excel/TutorialOEC1
% Set Example Units
JSteamMEX('SetUnit','Pressure','MPa');
JSteamMEX('SetUnit','MassFlow','kg/s');

% Brine Specs
brine = {'water',1};    % Assume just water
brineInP  = 0.84;       % Brine input pressure [MPa]
brineInT  = 166.9;      % Brine input temperature [C]
brineInH  = JSteamMEX('HmPT',brine,brineInP,brineInT); % Inlet Enthalpy [kJ/kg]
brineInM  = 398.4/2;    % Brine input mass flow rate [kg/s] (50% each side)
brineOutT = 136.0;      % Brine outlet temperature [C]

% Working Fluid Specs
fluid = {'nPentane',1}; % 100% n-Pentane
fluidInP  = 1.64;       % Fluid input pressure [MPa]
fluidInT  = 61.1;       % Fluid input temperature [C]
fluidInH  = JSteamMEX('HmPT',fluid,fluidInP,fluidInT); % Inlet Enthalpy [kJ/kg]
fluidOutT = 147.78;     % Fluid outlet temperature [C]
fluidOutP = 1.53;       % Fluid outlet pressure [MPa]

% Solve Cooler (Brine, have T+M, solve H+Q):
brineOutP = brineInP;   % Assume no pressure drop
brineOutM = brineInM;   % Assume no mass loss
[status,brineOutH,Duty] = JSteamMEX('UnitOp_CoolerTMC',brine,brineInH,brineOutP,brineOutT,brineOutM)

% Solve Heater (Working Fluid, have T+Q, solve H+M):
[status,outH,outM] = JSteamMEX('UnitOp_HeaterTQC',fluid,fluidInH,fluidOutP,fluidOutT,Duty)

% Return to default units
JSteamMEX('SetDefaultUnits');

