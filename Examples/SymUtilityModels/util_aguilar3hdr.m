%% Aguilar 3HDR
% (Not Really Aguilar's Model but the name stuck)
clc
clear
JSteamMEX('Load');
JSteamMEX('SetDefaultUnits');
ExcelModel = 'Aguilar3HDR.xlsx';
format short g

global HP_P MP_P LP_P HP_T MP_T HP_H MP_H HPU_Q MPU_Q BFW_H BT1_Q BT2_Q BT1_Eff BT2_Eff TG1_QMax TG2_QMax
global COST_FG COST_BELEC COST_SELEC COST_WATER HPU_F MPU_F HPU_H MPU_H MU_H PMP_P PMP_Eff FUEL_NHV
global BLR1_rangeM BLR2_rangeM BLR3_rangeM TG1_Eff TG2_Eff BLR1_Eff BLR2_Eff BLR3_Eff TG1_Qfnc TG2_Qfnc 
global DRTR_P DRTR_VT optfcount
global AIR_T AIR_P AIR_RELHUM

% Fuel
Fuel = {'Methane', 1.0};

% Globals
HP_P = 40; MP_P = 11; LP_P = 4; HP_T = 400; MP_T = 278;
HP_H = JSteamMEX('HPT',HP_P,HP_T); MP_H = JSteamMEX('HPT',MP_P,MP_T);
HPU_Q = 10e3; MPU_Q = 15e3; HPU_F = 0.8; MPU_F = 0.6;
HPU_H = JSteamMEX('HPX',HP_P,0); MPU_H = JSteamMEX('HPX',MP_P,0);
BT1_Q = 600; BT2_Q = 200; BT1_Eff = 0.6; BT2_Eff = 0.6;
BFW_H = 511.58; MU_H = JSteamMEX('HPT',LP_P,40); PMP_P = 47; PMP_Eff = 0.7;
DRTR_P = 2; DRTR_VT = 0.01;
BLR1_rangeM = [15 60]; BLR2_rangeM = [10 40]; BLR3_rangeM = [5 20];
AIR_T = 30; AIR_P = 1.01325; AIR_RELHUM = 0;
TG1_QMax = 2000; TG2_QMax = 2000;
COST_FG = 800; COST_WATER = 0.8;
COST_BELEC = 0.27; COST_SELEC = 0.20;
FUEL_NHV = JSteamMEX('NHV',Fuel);

% Generate Efficiency Curves for Sim Model
[~,~,TG1_maxM,~,TG1_Qfnc,~,TG1_Eff] = SymUtility.SteamTurbineReg(TG1_QMax,HP_P,HP_H,MP_P);
[~,~,TG2_maxM,~,TG2_Qfnc,~,TG2_Eff] = SymUtility.SteamTurbineReg(TG2_QMax,HP_P,HP_H,LP_P);
[~,~,~,BLR1_Eff] = SymUtility.SteamBoilerReg(Fuel,BLR1_rangeM./3.6,30,AIR_T,AIR_P,AIR_RELHUM,200,BFW_H,400,40,0.01,0.01);
[~,~,~,BLR2_Eff] = SymUtility.SteamBoilerReg(Fuel,BLR2_rangeM./3.6,30,AIR_T,AIR_P,AIR_RELHUM,200,BFW_H,400,40,0.01,0.01);
[~,~,~,BLR3_Eff] = SymUtility.SteamBoilerReg(Fuel,BLR3_rangeM./3.6,30,AIR_T,AIR_P,AIR_RELHUM,200,BFW_H,400,40,0.01,0.01);
TG1_maxM = TG1_maxM*3.6; TG2_maxM = TG2_maxM*3.6;
TG1_Eff = @(m) TG1_Eff(m/3.6); TG2_Eff = @(m) TG2_Eff(m/3.6);
TG1_Qfnc = @(m) TG1_Qfnc(m/3.6); TG2_Qfnc = @(m) TG2_Qfnc(m/3.6);
BLR1_Eff = @(m) BLR1_Eff(m/3.6); BLR2_Eff = @(m) BLR2_Eff(m/3.6); BLR3_Eff = @(m) BLR3_Eff(m/3.6);

% Case Studies
cstudy = 3;
switch(cstudy)
    case 1
        COST_BELEC = 1.5;
        COST_SELEC = 2.5;
    case 2
        HPU_Q = 20e3; 
        MPU_Q = 25e3;
    case 3
        COST_BELEC = 1.5;
        COST_SELEC = 2.5;
        HPU_Q = 20e3; 
        MPU_Q = 45e3;
end

% Simulation Model (for Initial Symbuilder Point)
optfcount = 0;
%Optimmization Vector [TG1_Q,TG2_Q,BT1,BT2,B1F,B2F,B3F]
ox = [18.323267,7.3376,1,0,1/3,1/3,1/3];
%Converge Model
[cost,xX,ii] = AguilarSim(0,ox);
cost
ii
optfcount

%Create Initial SymBuilder Vec
x0 = [double(ox(end-2) > 0);
      double(ox(end-1) > 0);
      double(ox(end) > 0);
      ~double(xX(end) >= 0);
      ox(3);ox(4);
      double(ox(1) > 0);
      double(ox(2) > 0);
      xX(1:end-1)'];


%% Optimize Simulation Model (no binary vars)
clc
%Objective
fun = @(x) AguilarSim(0,x);
%Nonlinear Constraints (same model, diff result)
nlcon = @(x) AguilarSim(1,x);
nlrhs = [0;2000;2000;60;40;20];
nle = [1;-1;-1;-1;-1;-1]; %[HP_Vent Blr1M Blr2M Blr3M]

xtype = 'CCCCC';
ox = [18.323267,7.3376,1/3,1/3,1/3];

%Bounds
lb = [0;0;0;0;0]; 
ub = [TG1_maxM;TG2_maxM;1;1;1];

%Options
opts = optiset('display','iter','solver','nlopt','maxtime',240,'solverOpts',nloptset('algorithm','LN_COBYLA'));
%Solve
optfcount = 0;
O = opti('fun',fun,'bounds',lb,ub,'nlmix',nlcon,nlrhs,nle,'xtype',xtype,'options',opts,'x0',ox)
[x,sf,e,i] = solve(O);
i
% i.FuncEvals
optfcount

%% Export Results to Excel
AguilarSimExport(x,ExcelModel,nlcon(x))

%% Optimize Simulation Model (WITH binary vars)
clc
%Objective
fun = @(x) AguilarSim(0,x);
%Nonlinear Constraints (same model, diff result)
nlcon = @(x) AguilarSim(1,x);
nlrhs = [0;2000;2000;60;40;20];
nle = [1;-1;-1;-1;-1;-1]; %[HP_Vent Blr1M Blr2M Blr3M]

xtype = 'CCBBCCC';
ox = [18.323267,7.3376,1,0,1/3,1/3,1/3];

%Bounds
lb = [0;0;0;0;0;0;0]; 
ub = [TG1_maxM;TG2_maxM;1;1;1;1;1];

%Options
opts = optiset('display','iter','solver','bonmin','maxtime',240,'tolrfun',1e-2,'tolint',1e-2,'solverOpts',bonminset('algorithm','b-bb','constr_viol_tol',1,'acceptable_constr_viol_tol',1,'acceptable_tol',1,...
               'acceptable_tol',1,'required_infeasibility_reduction',0.1,'start_with_resto','yes'));
% opts = optiset('display','iter','solver','nomad','maxtime',240);           
%Solve
optfcount = 0;
O = opti('fun',fun,'bounds',lb,ub,'nlmix',nlcon,nlrhs,nle,'xtype',xtype,'options',opts,'x0',ox)
[x,sf,e,i] = solve(O);
i
% i.FuncEvals
optfcount

%% Export Results to Excel
AguilarSimExport(x,ExcelModel,nlcon(x))

%% Build SymUtility Model
clc
tic
U = SymUtility;
U.AddSteamGroups();

% Mass Balance Equations
U.AddCon('m1-m2-m3-m4-m5-m6-m7 = 0');       %HP Header
U.AddCon('m10+m3+m5-m11-m12-m13-m14 = 0');  %MP Header
U.AddCon('m9+m16-m18 = 0');                 %User Mixer
U.AddCon('m17+m12+m4+m19-m20-m21 = 0');     %LP Header
U.AddCon('m24-m22-m23 = 0');                %Condensate Mixer
U.AddCon('m26-m15-m8-m27 = 0');             %BFW Splitter
U.AddCon('m27-m28-m30-m32 = 0');            %Boiler Splitter
U.AddCon('m1-m29-m31-m33 = 0');             %Steam Mixer
% Energy Balance Equations
U.AddCon('h1*m1 - h2*(m2+m3+m4+m5+m6+m7) = 0'); %HP Header
U.AddCon('h11*m10 + TG1_H*m3 + BT1_H*m5 - h11*(m11+m12+m13+m14) = 0'); %MP Header
U.AddCon('HPU_H*m9 + MPU_H*m16 - h18*m18 = 0'); %User Mixer
U.AddCon('h20*m17 + BT2_H*m12 + TG2_H*m4 + FLSH_VH*m19 - h20*(m20+m21) = 0'); %LP Header
U.AddCon('FLSH_LH*m22 + MU_H*m23 - h24*m24 = 0');               %Condensate Mixer
% General Bounds
U.AddBound('0 <= m <= 150');
U.AddBound('100 <= h <= 3500');

% Fired Boilers
U.AddBlrFrd({'BLR1','m29','m28','h1','bblr1'},Fuel,BLR1_rangeM,30,AIR_T,AIR_P,AIR_RELHUM,200,BFW_H,400,40,0.01,0.01);
U.AddBlrFrd({'BLR2','m31','m30','h1','bblr2'},Fuel,BLR2_rangeM,30,AIR_T,AIR_P,AIR_RELHUM,200,BFW_H,400,40,0.01,0.01);
U.AddBlrFrd({'BLR3','m33','m32','h1','bblr3'},Fuel,BLR3_rangeM,30,AIR_T,AIR_P,AIR_RELHUM,200,BFW_H,400,40,0.01,0.01);
% Turbo Generators
U.AddTurboGen({'TG1','m3','h2','btg1'},HP_P,MP_P,TG1_QMax,HP_H);
U.AddTurboGen({'TG2','m4','h2','btg2'},HP_P,LP_P,TG2_QMax,HP_H);
% Back Pressure Turbines
U.AddBPT({'BT1','m5','h2','bt1'},HP_P,MP_P,BT1_Q,BT1_Eff,HP_H);
U.AddBPT({'BT2','m12','h11','bt2'},MP_P,LP_P,BT2_Q,BT2_Eff,MP_H);
% Desuperheaters
U.AddDesuper({'HPDsp','m2','m8','m10','h2','BFW_H','h11'}); %HP Desuperheater
U.AddDesuper({'MPDsp','m11','m15','m17','h11','BFW_H','h20'}); %MP Desuperheater
% Deaerator
U.AddDeaerator({'DRTR','m20','m24','m25','m26','h20','h24'},DRTR_P,DRTR_VT);
% Steam Users
U.AddUser({'HPU','m6','h2','m9'},HPU_Q,HPU_H,HPU_F);
U.AddUser({'MPU','m13','h11','m16'},MPU_Q,MPU_H,MPU_F);
% Flash Drum
U.AddFlash({'FLSH','m18','m19','m22','h18'},LP_P);
% Pump
U.AddFixPump({'PMP','m26','BFW_H',},JSteamMEX('HPX',2,0),2,PMP_P,PMP_Eff);
% Make Up Water
U.AddWater({'Make Up','m23','MU_H'},MU_H);
% Headers
U.AddHeader({'HP','h2','m7'},JSteamMEX('HPT',HP_P,400));
U.AddHeader({'MP','h11','m14'},JSteamMEX('HPX',MP_P,1));
U.AddHeader({'LP','h20','m21'},JSteamMEX('HPX',LP_P,1));

% Costs
U.AddConstant('Cost_FG',COST_FG,'Cost_Water',COST_WATER);
U.AddConstant('Cost_BElec',COST_BELEC,'Cost_SElec',COST_SELEC);

% Power Balance
U.AddExpression('PWR = TG1_Q + TG2_Q - BT1RQ*(1-bt1) - BT2RQ*(1-bt2) - PMP_Q');
U.AddPwrBal({'PWR','bp'},60e3);
% Objective
U.AddObj('Cost_FG*(BLR1_FuelM+BLR2_FuelM+BLR3_FuelM) + Cost_Water*m23 - (Cost_SElec*(1-bp) + Cost_BElec*bp)*PWR');

Build(U)

opts = symbset('optiOpts',optiset('solver','bonmin','solverOpts',bonminset('algorithm','b-oa'),'maxnodes',1e6,'maxtime',3000,'tolrfun',1e-10)); %
toc

%%
opts = symbset('cbmode','mcode','display','iter');
[x,f,e,i] = Solve(U,x0,opts);
i
%
Results(U)

%%
ExcelExport(U,ExcelModel,AguilarExport)
