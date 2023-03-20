%% JSteam Toolbox: Units Examples
% J.Currie 2023
clc
clear
JSteamMEX('Load')

%% Customizable Units
% - The main units are customizable in JSteam, listed from the below
% function
clc
JSteamMEX('PrintUnits')

%Get Current Units [to check]
JSteamMEX('GetCurrentUnits')

%Get Current Unit
JSteamMEX('GetUnit','Pressure')

%% Changing Units Example
% - Simply pass the unit as a string to the corresponding function.
clc
JSteamMEX('SetUnit','Pressure','MPa')
JSteamMEX('SetUnit','Temperature','K')

%Get Current Units [to check]
JSteamMEX('GetCurrentUnits')

%Example thermo call
H = JSteamMEX('HPT',0.1,373.15)

%Revert to default
JSteamMEX('SetDefaultUnits')

%Example thermo call
H = JSteamMEX('HPT',1,100)

%% Moving between Unit Sets
% Save unit set
clc
myUnits = JSteamMEX('GetCurrentUnits');

% Example computation in different units
JSteamMEX('SetUnit','Pressure','MPa');
H = JSteamMEX('HPT',0.1,100)

% Return to original unit set
JSteamMEX('SetCurrentUnits',myUnits);

%% Unit Conversion
% - Convert between any of the available JSteam Units
clc
% Same Basis
JSteamMEX('ConvUnit',10,'kg/s','kg/hr')

% New Basis requires Molecular Weight
JSteamMEX('ConvUnit',350,'kJ/kg','cal(th)/lbmol',18)