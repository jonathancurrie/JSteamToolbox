function [cost,xX,ii] = AguilarSim(mode,x)
%MATLAB Model to suit Aguilar 3HDr Model

%Maintains current operating point between optimization iterations
persistent x0

% %Code to skip resolving model just for a constraint
% persistent xX StmM HP_M Blr1_M Blr2_M Blr3_M
% %Check for same X as previous call and constraint eval
% if(mode > 0 && all(xX == x))
%     cost = [StmM-HP_M;Blr1_M;Blr2_M;Blr3_M];
%     iter = 0;
%     return;
% else
%     xX = x;
% end

global HP_P MP_P LP_P MP_T HP_H MP_H HPU_Q MPU_Q BFW_H BT1_Q BT2_Q BT1_Eff BT2_Eff
global COST_FG COST_BELEC COST_SELEC COST_WATER HPU_F MPU_F HPU_H MPU_H MU_H PMP_P PMP_Eff FUEL_NHV
global TG1_Eff TG2_Eff BLR1_Eff BLR2_Eff BLR3_Eff optfcount
global LP_H Con_H BFW_M Drtr_StmM Drtr_CondM PMP_Q HPU_M HPU_oM MPU_oM User_M User_H BT2_M TG2_Q FVapM FLiqM LP_Feed LP_Vent MPDsp_WM MPDsp_SM BT1_M TG1_Q MP_Feed MP_Vent HPDsp_WM HPDsp_SM HP_M Blr1_M Blr2_M Blr3_M
global Blr1_FM Blr1_FirDuty Blr2_FM Blr2_FirDuty Blr3_FM Blr3_FirDuty StmM HP_Vent

%Distribute Optimization Variables
if(length(x)==5)
    [TG1_M,TG2_M,B1F,B2F,B3F] = disperse(x);
    BT1 = 1; BT2 = 0;
else
    [TG1_M,TG2_M,BT1,BT2,B1F,B2F,B3F] = disperse(x);
end
%SPECIFICATIONS
%Boilers
Blr1_T = 400; Blr1_P = HP_P; Blr1_BDRatio = 0.01;
Blr2_T = 400; Blr2_P = HP_P; Blr2_BDRatio = 0.01;
Blr3_T = 400; Blr3_P = HP_P; Blr3_BDRatio = 0.01;
%Deaerator, Pump & Make Up
Drtr_P = 2; Drtr_Vt = 0.01;

%STARTING GUESSES
% if(isempty(x0) || any(isnan(x0)))
    %Header Enthalpies
    x0(1,1) = JSteamMEX('HPT',HP_P,400);
    x0(2,1) = JSteamMEX('HPT',MP_P,280);
    x0(3,1) = JSteamMEX('HPT',LP_P,170);
    x0(4,1) = JSteamMEX('HPT',LP_P,100);
    %BFW Mass Flow
    x0(5,1) = 50;
% end

%Converge model using multivariable root solver (updates current state)
opts = optiset('solver','lmder','warnings','off','maxtime',3);
opts = optimset('display','off');
try
    fcount = 0;
    [x0,~,ee,ii] = fsolve(@localModel,x0,opts);
    if(ee ~= 1)
        if(isfield(ii,'message'))
            display(ii.message); 
        else
            display(ii);
        end
    end
    ii.TotalFCount = fcount;
    optfcount = optfcount + fcount;
catch ME
  disp(ME);
end

%Power Balance
Tpow = TG1_Q + TG2_Q - BT1_Q*(1-BT1) - BT2_Q*(1-BT2) - PMP_Q;

%Depending if we are a constraint, or objective, calculate costs
if(mode == 0) %objective
    %Calculate Costs
    cFG = COST_FG*(Blr1_FirDuty+Blr2_FirDuty+Blr3_FirDuty)/FUEL_NHV*3.6;
    cW = COST_WATER*(Drtr_CondM-FLiqM);
    %Power Sum    
    if(Tpow > 0), cE = COST_SELEC * -Tpow; else cE = COST_BELEC * -Tpow; end
    %Total Cost
    cost = cFG + cW + cE;
else
   %Constraints
   cost = [ StmM - HP_M
            TG1_Q
            TG2_Q
            Blr1_M
            Blr2_M
            Blr3_M];        
end

%Collect return vars
if(nargout > 1)
    xX = [MP_H,User_H,HP_H,LP_H,Con_H,HP_M,MP_Feed,MPDsp_SM,BT2_M,HPU_M,MP_Vent,MPDsp_WM,MPU_oM,...
          LP_Feed,User_M,FVapM,HPDsp_SM,Drtr_StmM,LP_Vent,FLiqM,Drtr_CondM-FLiqM,Drtr_CondM,...
          BFW_M*Drtr_Vt,BFW_M,Blr1_FM+Blr2_FM+Blr3_FM,Blr1_FM,Blr1_M,TG1_M,Blr2_FM,Blr2_M,Blr3_FM,Blr3_M,...
          TG2_M,BT1_M,HPU_M,HP_Vent,HPDsp_WM,HPU_oM,Tpow];
end


    function f = localModel(x)
        fcount = fcount+1;
        [HP_H, MP_H, LP_H, Con_H, BFW_M] = disperse(x);
        %Deaerator & Pump
        [~,Drtr_StmM,Drtr_CondM,Drtr_H] = JSteamMEX('UnitOp_Deaerator',Drtr_P,LP_H,Con_H,BFW_M,Drtr_Vt);
        [~,BFW_H,PMP_Q] = JSteamMEX('UnitOp_Pump',Drtr_H,Drtr_P,BFW_M,PMP_P,PMP_Eff);
        %Condensate Collection
        HPU_M = HPU_Q/(HP_H-HPU_H)*3.6;
        MPU_M = MPU_Q/(MP_H-MPU_H)*3.6;
        HPU_oM = HPU_M*HPU_F; 
        MPU_oM = MPU_M*MPU_F;
        User_M = HPU_oM+MPU_oM; User_H = (HPU_H*HPU_oM+MPU_H*MPU_oM)/User_M;
        %BT2 + TG2
        [~,BT2_M,BT2_H] = JSteamMEX('UnitOp_Turbine1Q',MP_H, MP_P, LP_P, BT2_Eff, BT2_Q*BT2);
        [~,TG2_Q,TG2_H] = JSteamMEX('UnitOp_Turbine1M',HP_H, HP_P, LP_P, TG2_Eff(TG2_M), TG2_M);
        %Flash Drum
        [~,FVapM,FVapH,FLiqM,FLiqH] = JSteamMEX('UnitOp_FlashDrum',LP_P,User_H,User_M);
        %LP Feed & Vent
        LP_M = Drtr_StmM-BT2_M-TG2_M-FVapM;
        LP_Feed = max(LP_M,0);
        LP_Vent = abs(min(LP_M,0));
        %MP-LP Desuperheater
        LP_T = JSteamMEX('TPH',LP_P,LP_H);
        [~,MPDsp_WM,MPDsp_SM,MPDsp_H] = JSteamMEX('UnitOp_DesuperheaterMout',MP_H,BFW_H,LP_P,LP_T,LP_Feed);
        %-- Correct LP H & Con H
        LP_H = (MPDsp_H*LP_Feed+BT2_H*BT2_M+TG2_H*TG2_M+FVapH*FVapM)/(LP_Feed+BT2_M+TG2_M+FVapM);
        Con_H = (FLiqH*FLiqM+(Drtr_CondM-FLiqM)*MU_H)/Drtr_CondM;
        %MP Fixed Ins
        [~,BT1_M,BT1_H] = JSteamMEX('UnitOp_Turbine1Q',HP_H,HP_P,MP_P,BT1_Eff,BT1_Q*BT1);    
        [~,TG1_Q,TG1_H] = JSteamMEX('UnitOp_Turbine1M',HP_H, HP_P, MP_P, TG1_Eff(TG1_M), TG1_M);    
        %MP Feed & Vent
        MP_M = MPDsp_SM+BT2_M+MPU_M-TG1_M-BT1_M;
        MP_Feed = max(MP_M,0);
        MP_Vent = abs(min(MP_M,0));
        %HP-MP Desuperheater
        MP_T = JSteamMEX('TPH',MP_P,MP_H);
        [~,HPDsp_WM,HPDsp_SM,HPDsp_H] = JSteamMEX('UnitOp_DesuperheaterMout',HP_H,BFW_H,MP_P,MP_T,MP_Feed);
        %--Correct MP H
        MP_H = (HPDsp_H*MP_Feed+BT1_H*BT1_M+TG1_H*TG1_M)/(MP_Feed+BT1_M+TG1_M);
        %HP Mass Flow Required   
        HP_M = HPDsp_SM+BT1_M+HPU_M+TG1_M+TG2_M; 
        %Boilers
        Blr1_M = HP_M*B1F;
        Blr2_M = HP_M*B2F;
        Blr3_M = HP_M*B3F;
        [~,Blr1_H,Blr1_FM,~,~,Blr1_FirDuty] = JSteamMEX('UnitOp_Boiler',BFW_H,Blr1_M,Blr1_T,Blr1_P,Blr1_BDRatio,BLR1_Eff(Blr1_M));
        [~,Blr2_H,Blr2_FM,~,~,Blr2_FirDuty] = JSteamMEX('UnitOp_Boiler',BFW_H,Blr2_M,Blr2_T,Blr2_P,Blr2_BDRatio,BLR2_Eff(Blr2_M));
        [~,Blr3_H,Blr3_FM,~,~,Blr3_FirDuty] = JSteamMEX('UnitOp_Boiler',BFW_H,Blr3_M,Blr3_T,Blr3_P,Blr3_BDRatio,BLR3_Eff(Blr3_M));
        %--Correct HP H & BFW_M
        StmM = Blr1_M+Blr2_M+Blr3_M;
        HP_H = (Blr1_H*Blr1_M+Blr2_H*Blr2_M+Blr3_H*Blr3_M)/StmM;
        BFW_M = Blr1_FM+Blr2_FM+Blr3_FM+HPDsp_WM+MPDsp_WM;
        %--HP Vent
        HP_Vent = max(StmM-HP_M,0);

        f = [HP_H, MP_H, LP_H, Con_H, BFW_M]' - x;
    end           
end

function varargout = disperse(x)
% DISPERSE was created so that you can stop doing things like this:
%
%   x1 = array(1); % ...repetitive assignments from an array
%   x2 = array(2);
%   x3 = array(3);
%   x4 = array(4);
%
% and start doing things like this:
%
%   [x1 x2 x3 x4] = disperse(array);
%
% DISPERSE generalizes to arbitrary dimensions, and is extended to follow
% analogous behavior on cell arrays and structure arrays. See the html
% documentation for more details and examples.
%
% Example:
%   Grab the color channels from an RGB image:
%   [r g b] = disperse(im);

% Sam Hallman
% shallman@uci.edu
% May 26, 2010

% num2cell on column vectors is problematic
if ndims(x)==2 && size(x,2)==1
    x = x';
end

if isnumeric(x) || ischar(x) || islogical(x) || isstruct(x)
    dims = 1:ndims(x)-1;
    varargout = num2cell(x,dims);
elseif iscell(x)
    if size(x,1) == 1
        varargout = x;
    else
        dims = 1:ndims(x)-1;
        varargout = num2cell(x,dims);
    end
else
    error('unknown data type');
end
end