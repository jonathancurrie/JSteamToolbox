%% GENERATE JSTEAM TEST RESULTS

%% Generate Steam Thermo Test Results
clear
JStm = JSteam();
n = 30;

%-- Sequential Tests (Vector)--%
%PT Tests
P = linspace(1,1000,n)';
T = linspace(1,800,n)';
H = zeros(n,1); S = H; Cp = H; Cv = H; V = H;
for i = 1:n
    H(i) = JStm.HPT(P(i),T(i));
    S(i) = JStm.SPT(P(i),T(i));
    Cp(i) = JStm.CpPT(P(i),T(i));
    Cv(i) = JStm.CvPT(P(i),T(i));
    V(i) = JStm.VPT(P(i),T(i));
end

%PX Tests
Psat = linspace(1,200,n)';
X = linspace(0,1,n)';
Hsat = zeros(n,1); Ssat = Hsat; Cpsat = Hsat; Cvsat = Hsat; Vsat = Hsat;
for i = 1:n
    Hsat(i) = JStm.HPX(Psat(i),X(i));
    Ssat(i) = JStm.SPX(Psat(i),X(i));
    Cpsat(i) = JStm.CpPX(Psat(i),X(i));
    Cvsat(i) = JStm.CvPX(Psat(i),X(i));
    Vsat(i) = JStm.VPX(Psat(i),X(i));
end

%-- Parallel Tests (Matrix)--%
%PT Tests
pP = linspace(1,1000,n)';
pT = linspace(1,800,n)';
[pP,pT] = meshgrid(pP,pT);
pH = JStm.HPT(pP,pT);
pS = JStm.SPT(pP,pT);
pCp = JStm.CpPT(pP,pT);
pCv = JStm.CvPT(pP,pT);
pV = JStm.VPT(pP,pT);

%PX Tests
pPsat = linspace(1,200,n)';
pX = linspace(0,1,n)';
[pPsat,pX] = meshgrid(pPsat,pX);
pHsat = JStm.HPX(pPsat,pX);
pSsat = JStm.SPX(pPsat,pX);
pCpsat = JStm.CpPX(pPsat,pX);
pCvsat = JStm.CvPX(pPsat,pX);
pVsat = JStm.VPX(pPsat,pX);

clear i n JStm
save SteamThermoTest

%% Generate Fuel Thermo Test Results
clear
JStm = JSteam();
n = 20;

%-- Sequential Tests (Vector)--%
%PT Tests
P = linspace(1,30,n)';
T = linspace(1,1000,n)';
comps = {'Methane','Ethylene','Ethane','Propylene','Propane','IsoButene','nButane','IsoButane',...
		  'nPentane','IsoPentane','nHexane','CarbonMonoxide','Hydrogen','CarbonDioxide',...
		  'Nitrogen','Oxygen','Water','HydrogenSulfide','SulfurDioxide','mdm','r134a','r245fa'};
len = length(comps);
H = zeros(n,len); S = H; Hn = H; Sn = H; Cpn = H;
for i = 1:len    
    for j = 1:n
        H(j,i) = JStm.HcPT(comps{i},P(j),T(j));
        S(j,i) = JStm.ScPT(comps{i},P(j),T(j));
        Cp(j,i) = JStm.CpcPT(comps{i},P(j),T(j));
    end
end

%-- Parallel Tests (Matrix)--%
%PT Tests
pP = linspace(1,30,n)';
pT = linspace(1,1000,n)';
[pP,pT] = meshgrid(pP,pT);
pH = zeros(n,n,len); pS = pH; pHn = pH; pSn = pH; pCpn = pH;
for i = 1:len
    pH(:,:,i) = JStm.HcPT(comps{i},pP,pT);
    pS(:,:,i) = JStm.ScPT(comps{i},pP,pT);
    pCp(:,:,i) = JStm.CpcPT(comps{i},pP,pT);
end

clear i j len n JStm
save FuelThermoTest

%% Unit Operation Results
clear
clc
JStm = JSteam();

[status,StmH,BFWM,FirDuty] = JStm.UnitOp_Boiler(300,10,410,42,0.01,0.93)

[status,outM,outH] = JStm.UnitOp_CompressorA(JStm.HPT(3,150),3,15,0.75,500)
[status,outM,outH1,outH2] = JStm.UnitOp_CompressorA2(JStm.HPT(3,150),3,5,0.75,2,15,0.5,500)
[status,Pwr,outH] = JStm.UnitOp_CompressorB(JStm.HPT(3,150),3,15,0.75,3.6322)
[status,TPwr,outH1,outH2] = JStm.UnitOp_CompressorB2(JStm.HPT(3,150),3,5,0.75,2,15,0.5,2.1952)
[status,outH,Duty] = JStm.UnitOp_CoolerA(JStm.HPT(10,250),10,200,10)
[status,outH,outM] = JStm.UnitOp_CoolerB(JStm.HPT(10,250),10,200,319.3184)
[status,outH,outT] = JStm.UnitOp_CoolerC(JStm.HPT(10,250),10,10,319.3184)
[status,StmM,WatM,LiqH] = JStm.UnitOp_Deaerator(3,2800,300,10,0.01)
[status,WatInM,StmInM,StmOutH] = JStm.UnitOp_DesuperheaterA(3200,300,10,190,10)
[status,WatInM,StmOutM,StmOutH] = JStm.UnitOp_DesuperheaterB(3200,300,10,190,8.6328)
[status,VapM,VapH,LiqM,LiqH] = JStm.UnitOp_FlashDrum(4,JStm.HPX(10,0),10)
[status,outH,Pwr] = JStm.UnitOp_Pump(350,3,10,42,0.7)
[status,outH,Duty] = JStm.UnitOp_HeaterA(JStm.HPT(10,250),10,300,10)
[status,outH,outM] = JStm.UnitOp_HeaterB(JStm.HPT(10,250),10,300,301.3362)
[status,outH,outT] = JStm.UnitOp_HeaterC(JStm.HPT(10,250),10,10,301.3362)

[status,outM,outH] = JStm.UnitOp_TurbineA(JStm.HPT(30,350),30,5,0.75,500)
[status,outM,outH1,outH2] = JStm.UnitOp_TurbineA2(JStm.HPT(30,350),30,15,0.6,2,5,0.7,1500)
[status,Pwr,outH] = JStm.UnitOp_TurbineB(JStm.HPT(30,350),30,5,0.75,5.9984)
[status,TPwr,outH1,outH2] = JStm.UnitOp_TurbineB2(JStm.HPT(30,350),30,15,0.6,2,5,0.7,18.995)

[status,outT] = JStm.UnitOp_Valve(2500,10,5)
