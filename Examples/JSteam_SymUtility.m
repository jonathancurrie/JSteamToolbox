%% JSteam SymUtility Examples
% J.Currie Mar 2023
clc
clear
JSteamMEX('Load');

%% Fixed Efficiency Back Pressure Steam Turbine Model
clc

% Load SymUtility
U = SymUtility;

% Specifications
inT  = 410; % Input Temperature [C]
inP  = 42;  % Input Pressure [bar]
inH  = JSteamMEX('HPT',inP,inT); % Reference Input Enthalpy [kJ/kg]
outP = 15;  % Output Pressure [bar]
duty = 10;  % Rated Output Power [kW]
eff  = 0.7; % Isentropic Efficiency

% Naming
name        = 'SteamTurbine1'; % Name of this turbine
massvar     = 'm1';   % Input mass flow variable
enthalpySteamVar = 'h1';   % Input enthalpy variable
onoffvar    = 'bST1'; % Variable used to switch on/off this turbine

[expQ,expH,conQ] = U.AddBPT({name,massvar,enthalpySteamVar,onoffvar},inP,outP,duty,eff,inH)

%% Check Model against JSteam
clc
% Reference operating point
m1 = 0.2; % example input mass flow rate
h1 = inH; % for subs
[~,Pwr,outH] = JSteamMEX('UnitOp_Turbine1M',h1,inP,outP,eff,m1)

% Check output power
subs(str2sym(expQ))
% Check output enthalpy
subs(str2sym(expH))

% Repeat at a slightly lower input enthalpy
h1 = inH - 100;
[~,Pwr,outH] = JSteamMEX('UnitOp_Turbine1M',h1,inP,outP,eff,m1)

% Check output power
subs(str2sym(expQ))
% Check output enthalpy
subs(str2sym(expH))

%% Check constraint (for interest)
clc
m1 = 0.2; % example input mass flow rate (not exactly 10kW output)
h1 = inH; % reference point
bST1 = 1; % turbine "on"

% Check constraint
subs(str2sym(conQ))


%% View SymUtility Class with Model
clc
constraints = U.eqs
expressions = U.exprsn
bounds = U.bnds
vartypes = U.vartypes


%% Turbo Generator Steam Turbine Part Load Model
clc
clf

% Specifications
inT  = 410; % Input Temperature [C]
inP  = 42;  % Input Pressure [bar]
inH  = JSteamMEX('HPT',inP,inT); % Reference Input Enthalpy [kJ/kg]
outP = 15;  % Output Pressure [bar]
maxQ = 200; % Maximum Output Power [kW]

% Start with the underlying regression model
[A,B,maxM,maxEff,Qfnc,Hfnc,Efffnc] = SymUtility.SteamTurbineReg(maxQ,inP,inH,outP)

% Plot the Part Load Behaviour
m = linspace(0.1*maxM,maxM,100);
yyaxis left
plot(m*3.6,Qfnc(m));
xlim([0.1*maxM maxM]*3.6); ylim([0 250])
ylabel('Output Power [kW]');

yyaxis right
plot(m*3.6,Efffnc(m)*100);
xlim([0.1*maxM maxM]*3.6); ylim([0 50])
ylabel('Isentropic Efficiency [%]');
grid on; xlabel('Mass Flow Rate [tonne/hr]')
title(sprintf('%d kW Turbogenerator SymUtility Part-Load Model',maxQ))

% Compare to JSteam Model
[~,Pwr,outH] = JSteamMEX('UnitOp_Turbine1M',inH,inP,outP,maxEff,maxM*3.6)

% Load SymUtility (new one for ease)
U = SymUtility;

% Naming
name        = 'TurboGen1'; % Name of this turbine
massvar     = 'm1';   % Input mass flow variable
enthalpySteamVar = 'h1';   % Input enthalpy variable
onoffvar    = 'bTG1'; % Variable used to switch on/off this turbine

[expQ,expH,con1,con2,conQ] = U.AddTurboGen({name,massvar,enthalpySteamVar,onoffvar},inP,outP,maxQ,inH)

% Check output power
subs(str2sym(expQ))
% Check output enthalpy
subs(str2sym(expH))

%% View SymUtility Class with Model
clc
constraints = U.eqs
expressions = U.exprsn
bounds = U.bnds
vartypes = U.vartypes

%% Fired Boiler Part Load Model
clc
clf

% Specifications
fuel  = {'Methane', 1}; % Assume 100% Methane
fuelT = -20;        % Fuel Temperature [C]
airT  = 30;         % Air Temerature [C]
airP  = 1.01325;    % Air Pressure [bar]
airRelHum = 0.75;   % Air Relative Humidity [fraction]
rangeM = [5 50];    % Operating mass flow rate range of the boiler [tonne/hr]
stackT = 200;       % Stack Temperature [C]
bfwH  = 350;        % Boiler Feed Water Enthalpy [kJ/kg]
stmT  = 410;        % Steam Temperature [C]
stmP  = 42;         % Steam Pressure [bar]
bdr   = 0.01;       % Blowdown Ratio [fraction]
exO2  = 0.1;        % Inlet excess O2 mole fraction for complete combustion

[~,~,Ffnc,Efffnc,maxEff] = SymUtility.SteamBoilerReg(fuel,rangeM,fuelT,airT,airP,airRelHum,stackT,bfwH,stmT,stmP,bdr,exO2)

% Plot the Part Load Behaviour
m = linspace(rangeM(1),rangeM(2),100);
yyaxis left
plot(m,Ffnc(m));
xlim(rangeM); 
ylabel('Fuel Demand [tonne/hr]');

yyaxis right
plot(m,Efffnc(m)*100);
xlim(rangeM); %ylim([0 50])
ylabel('NHV Efficiency [%]');
grid on; xlabel('Steam Mass Flow Rate [tonne/hr]')
title('Fired Boiler SymUtility Part-Load Model')

% Compare to JSteam Model
[~,~,~,~,fuelM,FEff,Duty,~] = JSteamMEX('UnitOp_BoilerFired',bfwH, rangeM(2), stmT, stmP, bdr, fuel, fuelT, airT, airP, airRelHum, stackT, exO2, 0)

% Load SymUtility (new one for ease)
U = SymUtility;

% Naming
name             = 'Boiler1';
massSteamVar     = 'm1';
massBFWVar       = 'm2';
enthalpySteamVar = 'h1';
onoffvar         = 'bBLR1';
names = {name, massSteamVar, massBFWVar, enthalpySteamVar, onoffvar};

[expM,con1,con2,con3] = U.AddBlrFrd(names,fuel,rangeM,fuelT,airT,airP,airRelHum,stackT,bfwH,stmT,stmP,bdr,exO2)

% Check mass flow and constraints
subs(str2sym(expM))
subs(str2sym(con1))
subs(str2sym(con2))

%% View SymUtility Class with Model
clc
constraints = U.eqs
expressions = U.exprsn
bounds = U.bnds
vartypes = U.vartypes

%% Optimization Example
% Steam Boiler connected to Steam Turbo Generator & Back Pressure Turbine
clc

% Mass Var Naming:
% m1 = Boiler Out -> HP Header In
% m2 = HP Header Out -> Turbo Gen In
% m3 = HP Header Out -> BPT In
% m4 = Boiler In
% m5 = HP Header Vent
% Enthalpy Var Naming:
% h1 = Boiler Out Enthalpy
% h2 = HP Header Enthalpy

% Load SymUtility & Add 
U = SymUtility;
U.AddSteamGroups(); % Allows Results(U)

% Mass Balance Equations
U.AddCon('m1-m2-m3-m5 = 0'); %HP Header

% Energy Balance Equations
U.AddCon('h1*m1 - h2*(m2+m3+m5) = 0'); %HP Header

% General Bounds (helps optimizer)
U.AddBound('0 <= m <= 150');
U.AddBound('100 <= h <= 3500');

% Fired Boiler
fuel  = {'Methane', 1}; % Assume 100% Methane
fuelT = -20;        % Fuel Temperature [C]
airT  = 30;         % Air Temerature [C]
airP  = 1.01325;    % Air Pressure [bar]
airRelHum = 0.75;   % Air Relative Humidity [fraction]
rangeM = [5 50];    % Operating mass flow rate range of the boiler [tonne/hr]
stackT = 200;       % Stack Temperature [C]
bfwH  = 350;        % Boiler Feed Water Enthalpy [kJ/kg]
stmT  = 410;        % Steam Temperature [C]
stmP  = 42;         % Steam Pressure [bar]
stmH  = JSteamMEX('HPT',stmP,stmT); % Steam Enthalpy [kJ/kg]
bdr   = 0.01;       % Blowdown Ratio [fraction]
exO2  = 0.1;        % Inlet excess O2 mole fraction for complete combustion
U.AddBlrFrd({'BLR1','m1','m4','h1','bblr1'},fuel,rangeM,fuelT,airT,airP,airRelHum,stackT,bfwH,stmT,stmP,bdr,exO2);

% Turbo Generator
outP = 11;          % Output pressure [bar]
maxQ = 1000;        % Maximum output power [kW]
U.AddTurboGen({'TG1','m2','h2','btg1'},stmP,outP,maxQ,stmH);

% Back Pressure Turbine
power = 500;        % Output power [kW]
eff   = 0.8;        % Isentropic efficiency [fraction]
U.AddBPT({'BT1','m3','h2','bt1'},stmP,outP,power,eff,stmH);

% Steam Header
U.AddHeader({'HP','h2','m5'},stmH);

% Costs
U.AddConstant('Cost_FG',300);
U.AddConstant('Cost_BElec',0.27,'Cost_SElec',0.2);

% Power Balance
U.AddExpression('PWR = TG1_Q - BT1RQ*(1-bt1)');
U.AddPwrBal({'PWR','bp'},60e3);
% Objective
U.AddObj('Cost_FG*(BLR1_FuelM) - (Cost_SElec*(1-bp) + Cost_BElec*bp)*PWR');

% Build Optimization Model
Build(U)

% Inspect variables (and order)
U.vars

% Generate Initial Guess & Solve
x0 = zeros(10,1);
[x,f,e,i] = Solve(U,x0);

Results(U)

%% Repeat with higher fuel cost
clc

% Increase fuel cost
U.AddConstant('Cost_FG',320);

% Build Optimization Model
Build(U)

% Solve and print results
[x,f,e,i] = Solve(U,x0);
Results(U)

%% Repeat with higher electricity sell price
clc

% Return fuel price to default, increase electricity sell price
U.AddConstant('Cost_FG',300);
U.AddConstant('Cost_SElec',0.4);

% Build Optimization Model
Build(U)

% Solve and print results
[x,f,e,i] = Solve(U,x0);
Results(U)
