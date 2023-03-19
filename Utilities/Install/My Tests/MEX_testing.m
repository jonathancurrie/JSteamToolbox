%% New JSTEAM MEX INTERFACE TESTING
clear all
clc

JSteamMEX('Load')

%%
JSteamMEX('SetUnit','Pressure','bar')
JSteamMEX('SetUnit','Temperature','C')

n = 500;
p = linspace(1,30,n)';
t = 100*ones(n,1);
[P,T] = meshgrid(p,t);

h = JSteamMEX('HPT',p,t);

t = JSteamMEX('TsatP',1)

b = JSteamMEX('MW','Methane')

d = JSteamMEX('NHV','nHexane')

h1 = JSteamMEX('HcPT','Methane',P,T);

cv = JSteamMEX('CvPT',P,T);

k = JSteamMEX('KPT',1,50)

%% Unit Operations
clc
clear all
format long g
JSteamMEX('Load')
[status,stmH,bfwM,bdM,bdH,duty] = JSteamMEX('UnitOp_Boiler',250,10,410,42,0.01,0.9);
[status,outM,outH] = JSteamMEX('UnitOp_Compressor1Q',3500,3,10,0.7,500);
[status,out2M,out1H,out2H] = JSteamMEX('UnitOp_Compressor2Q',3500,3,6,0.4,4,10,0.5,800);
[status,pwr,outH] = JSteamMEX('UnitOp_Compressor1M',3500,3,10,0.7,10);
[status,tpwr,out1H,out2H] = JSteamMEX('UnitOp_Compressor2M',3500,3,6,0.4,10,10,0.5,4);
[status,outH,outT] = JSteamMEX('UnitOp_CoolerMQ',3000,5,10,7000);
[status,outH,duty] = JSteamMEX('UnitOp_CoolerTM',3000,5,100,10);
[status,outH,outM] = JSteamMEX('UnitOp_CoolerTQ',3000,5,100,7000);
[status,outH,outT] = JSteamMEX('UnitOp_HeaterMQ',3000,5,25,3500);
[status,outH,duty] = JSteamMEX('UnitOp_HeaterTM',3000,5,500,10);
[status,outH,outM] = JSteamMEX('UnitOp_HeaterTQ',3000,5,500,3500);
[status,stmM,watM,liqH,vapM] = JSteamMEX('UnitOp_Deaerator',2,2800,250,10,0.01);
[status,watM,outM,outH] = JSteamMEX('UnitOp_DesuperheaterMin',3500,350,10.5,187,10);
[status,watM,stmM,outH] = JSteamMEX('UnitOp_DesuperheaterMout',3500,350,10.5,187,10);
[status,vapM,vapH,liqM,liqH] = JSteamMEX('UnitOp_FlashDrum',3,2400,10);
[status,outH,pwr] = JSteamMEX('UnitOp_Pump',300,3,10,42,0.7);
[status,outT,outX] = JSteamMEX('UnitOp_Valve',2400,42,10);
[status,outM,outH] = JSteamMEX('UnitOp_Turbine1Q',3500,42,3,0.7,500);
[status,out2M,out1H,out2H] = JSteamMEX('UnitOp_Turbine2Q',3500,42,15,0.4,10,3,0.5,800);
[status,out3M,out1H,out2H,out3H] = JSteamMEX('UnitOp_Turbine3Q',3500,42,15,0.4,10,8,0.5,5,3,0.4,1000);
[status,pwr,outH] = JSteamMEX('UnitOp_Turbine1M',3500,42,3,0.7,10);
[status,tpwr,out1H,out2H] = JSteamMEX('UnitOp_Turbine2M',3500,42,15,0.4,10,3,0.5,4);
[status,tpwr,out1H,out2H,out3H] = JSteamMEX('UnitOp_Turbine3M',3500,42,15,0.4,10,8,0.5,5,3,0.4,1);

%%
clc
% clear all
format long g
JSteamMEX('Load')
JSteamMEX('Verify','STEAM_THERMO')
JSteamMEX('Verify','STEAM_UNITOPS')
JSteamMEX('Verify','GEN_THERMO')
JSteamMEX('Verify','COM_UNITOPS')
JSteamMEX('Verify','ORC_UNITOPS')
JSteamMEX('Verify','UNIT_CONV')

%%
clc
clear 
JSteamMEX('Load')

fluids = {'nButane','nPentane'};
fracs = [0.5;0.5];
p = 10; t = 100;

JSteamMEX('HmPT',[fluids(:) num2cell(fracs(:))],p,t)

JSteamMEX('HmPT',{'nButane',0.5; 'nPentane', 0.5},1,100)

JSteamMEX('HmPT',[fluids(:) num2cell(fracs(:))],p,t)

JSteamMEX('MW','methane')
JSteamMEX('MW',{'methane',1})
JSteamMEX('PsatmT',{'methane',1},-150)
JSteamMEX('PsatcT','methane',-150)

JSteamMEX('HmPT',[fluids(:) num2cell(fracs(:))],p,t:t+5)

%% Evap on MDM
JSteamMEX('Load')
clc
fluid = {'mdm' 1.0};
inT = 80;
inP = 25;
inH = JSteamMEX('HmPT',fluid,inP,inT);
outT = 250;
outP = 25;
outM = 0.5;
[status,outH,Duty] = JSteamMEX('UnitOp_HeaterTMc',fluid,inH,outP,outT,outM)
outT = JSteamMEX('TmPH',fluid,outP,outH)

%% Furnace M
clc
format compact
format long g
JSteamMEX('Load')
fluids = {'methane','hydrogen','nitrogen','water','hydrogensulfide'};
fracs = [0.95,0.01,0.02,0.01,0.01];
Fuel = [fluids(:) num2cell(fracs(:))];
fuelT = -20;
airT = 25;
airP = 1.01325;
airRelHum = 0.5;
minStackT = 1000;
O2 = 0.01;
O2Mode = 1;
fuelM = 1;
[status,airM,Duty,FEff,acidDp,Stack] = JSteamMEX('UnitOp_FurnaceM',Fuel,fuelT,airT,airP,airRelHum,minStackT,O2,O2Mode,fuelM)
[status,airM,fuelM,FEff,acidDp,Stack] = JSteamMEX('UnitOp_FurnaceQ',Fuel,fuelT,airT,airP,airRelHum,minStackT,O2,O2Mode,Duty)

%
% JNet = LoadJSteamNET();
% Fuel = CreateMixNET(JNet,fluids,fracs);
% Stack = CreateMixNET(JNet,fluids,fracs);
% Air = CreateAirMixNET(JNet,airRelHum,airP,airT);
% [status,airM,duty,eff] = JNet.UnitOp_FurnaceM(Fuel,Air,fuelT,airT,minStackT,O2,O2Mode,fuelM,Stack)

%% Boiler Fired
clc
format compact
format long g
JSteamMEX('Load')
fluids = {'methane','hydrogen','nitrogen','water','hydrogensulfide'};
fracs = [0.95,0.01,0.02,0.01,0.01];
Fuel = [fluids(:) num2cell(fracs(:))];
fuelT = -20;
airT = 25;
airP = 1.01325;
airRelHum = 0.5;
minStackT = 200;
O2 = 0.1;
O2Mode = 0;
BFWH = 300;
StmM = 20;
StmT = 410;
StmP = 42;
BDr = 0.01;

[status,StmH,BFWM,airM,fuelM,FEff,Duty,Stack] = JSteamMEX('UnitOp_BoilerFired',BFWH,StmM,StmT,StmP,BDr,Fuel,fuelT,airT,airP,airRelHum,minStackT,O2,O2Mode)

% JNet = LoadJSteamNET();
% Fuel = CreateMixNET(JNet,fluids,fracs);
% Stack = CreateMixNET(JNet,fluids,fracs);
% Air = CreateAirMixNET(JNet,airRelHum,airP,airT);
% [status,StmH,BFWM,airM,fuelM,eff,duty] = JNet.UnitOp_BoilerFired(BFWH,StmM,StmT,StmP,BDr,Fuel,Air,fuelT,airT,minStackT,O2,O2Mode,Stack)

%% Gas Turbine
clc
format compact
format long g
JSteamMEX('Load')
fluids = {'methane','hydrogen','nitrogen','water','hydrogensulfide'};
fracs = [0.95,0.01,0.02,0.01,0.01];
Fuel = [fluids(:) num2cell(fracs(:))];
fuelT = -20;
airT = 25;
airP = 1.01325;
airRelHum = 0.5;
exhaustT = 550;
presRatio = 20;
compEff = 0.75;
GTPower = 15000;
GTEff = 0.3;

[status,airM,fuelM,exhaustM,Exhaust] = JSteamMEX('UnitOp_GasTurbine',Fuel,fuelT,airT,airP,airRelHum,exhaustT,presRatio,compEff,GTPower,GTEff)

% JNet = LoadJSteamNET();
% Fuel = CreateMixNET(JNet,fluids,fracs);
% Exhaust = CreateMixNET(JNet,fluids,fracs);
% Air = CreateAirMixNET(JNet,airRelHum,airP,airT);
% [status, airM, fuelM, exhaustM] = JNet.UnitOp_GasTurbine(Fuel, Air, fuelT, airT, exhaustT, airP, presRatio, compEff, GTPower, GTEff, Exhaust)

%% Gas Turbine Inj
clc
format compact
format long g
JSteamMEX('Load')
fluids = {'methane','hydrogen','nitrogen','water','hydrogensulfide'};
fracs = [0.95,0.01,0.02,0.01,0.01];
Fuel = [fluids(:) num2cell(fracs(:))];
fuelT = -20;
airT = 25;
airP = 1.01325;
airRelHum = 0.5;
exhaustT = 550;
presRatio = 20;
compEff = 0.75;
GTPower = 15000;
GTEff = 0.3;
StmT = 350; StmP = 10; StmM = 1;

[status,airM,fuelM,exhaustM,Exhaust] = JSteamMEX('UnitOp_GasTurbineInj',Fuel,fuelT,airT,airP,airRelHum,exhaustT,presRatio,compEff,StmT, StmP, StmM,GTPower,GTEff)

% JNet = LoadJSteamNET();
% Fuel = CreateMixNET(JNet,fluids,fracs);
% Exhaust = CreateMixNET(JNet,fluids,fracs);
% Air = CreateAirMixNET(JNet,airRelHum,airP,airT);
% [status, airM, fuelM, exhaustM] = JNet.UnitOp_GasTurbineInj(Fuel, Air, fuelT, airT, exhaustT, airP, presRatio, compEff, StmT, StmP, StmM, GTPower, GTEff, Exhaust)


%% Gas Turbine + HRSG
clc
format compact
format long g
JSteamMEX('Load')
fluids = {'methane','hydrogen','nitrogen','water','hydrogensulfide'};
fracs = [0.95,0.01,0.02,0.01,0.01];
Fuel = [fluids(:) num2cell(fracs(:))];
fuelT = -20;
airT = 25;
airP = 1.01325;
airRelHum = 0.5;
exhaustT = 550;
presRatio = 20;
compEff = 0.8;
GTPower = 15000;
GTEff = 0.3;
minAppT = 30;
minStackT = 200;
BFWH = 350;
StmT = 410;
StmP = 42;

[status,airM,fuelM,exhaustM,Exhaust] = JSteamMEX('UnitOp_GasTurbine',Fuel,fuelT,airT,airP,airRelHum,exhaustT,presRatio,compEff,GTPower,GTEff)
[status,StmM,econDT,sprHDT,stackT,Stack] = JSteamMEX('UnitOp_HRSG',Exhaust,exhaustT,exhaustM*0.95,minAppT,minStackT,BFWH,StmT,StmP,BDr)

% JNet = LoadJSteamNET();
% Fuel = CreateMixNET(JNet,fluids,fracs);
% Exhaust = CreateMixNET(JNet,fluids,fracs);
% Stack = CreateMixNET(JNet,fluids,fracs);
% Air = CreateAirMixNET(JNet,airRelHum,airP,airT);
% [status, airM, fuelM, exhaustM] = JNet.UnitOp_GasTurbine(Fuel, Air, fuelT, airT, exhaustT, airP, presRatio, compEff, GTPower, GTEff, Exhaust)
% [status,StmM,econDT,sprHDT,stackT] = JNet.UnitOp_HRSG(Exhaust,exhaustT,exhaustM*0.95,minAppT,minStackT,BFWH,StmT,StmP,BDr,Stack)

%% ORC Ops w Mixtures
clc
format compact
format long g
JSteamMEX('Load')
JSteamMEX('setunit','pres','mpa')
JSteamMEX('setunit','massf','kg/s')

fluids = {'nPentane'};
fracs = [1];
Fuel = [fluids(:) num2cell(fracs(:))];

inH = 530.9117;
inP = 1.53;
outP = 0.135;
Eff = 0.8271;
outM = 56.5195;

[status,Pwr,outH] = JSteamMEX('UnitOp_Turbine1MC',Fuel,inH,inP,outP,Eff,outM)

%% Parallel Testing
clc
format compact
format long g
JSteamMEX('Load')
n = 10000;
hs = zeros(n,1); ks = zeros(n,1);
p = linspace(0.1,1,n)'; t = linspace(100,110,n)';

for i = 1:n
    hs(i) = JSteamMEX('HcPT','Methane',p(i),t(i));
    ks(i) = JSteamMEX('KcPT','Methane',p(i),t(i));
end
hp = JSteamMEX('HcPT','Methane',p,t);
kp = JSteamMEX('KcPT','Methane',p,t);

herr = norm(hs-hp)
kerr = norm(ks-kp)

fluids = {'CarbonDioxide','Nitrogen','O2','H2O'};
fracs = [0.000307,0.763108,0.204719,0.031866];
Air = [fluids(:) num2cell(fracs(:))];

tic
for i = 1:n
    hs(i) = JSteamMEX('HmPT',Air,p(i),t(i));
    ks(i) = JSteamMEX('KmPT',Air,p(i),t(i));
end
toc

tic
hp = JSteamMEX('HmPT',Air,p,t);
kp = JSteamMEX('KmPT',Air,p,t);
toc

herr = norm(hs-hp)
kerr = norm(ks-kp)

plot(p,hs)