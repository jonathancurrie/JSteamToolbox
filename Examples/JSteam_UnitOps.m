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
