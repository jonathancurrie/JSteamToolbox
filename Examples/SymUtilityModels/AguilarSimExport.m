function AguilarSimExport(data,path,nlcon)

global TG1_Eff TG2_Eff TG1_Qfnc TG2_Qfnc COST_FG COST_BELEC COST_SELEC COST_WATER HPU_Q MPU_Q
global BLR1_Eff BLR2_Eff BLR3_Eff

if(length(data)==5)
    [TG1_M,TG2_M,B1F,B2F,B3F] = disperse(data);
    BT1 = 1; BT2 = 0;
else
    [TG1_M,TG2_M,BT1,BT2,B1F,B2F,B3F] = disperse(data);
end
%Check file exists
if(exist(path,'file') ~= 2)
    error('Cannot find file "%s"',path);
end

%Check we can open the model
try
    fid = fopen(path,'r+');
catch
    error('Cannot current open the file for writing. Ensure it is not already open!');
end
if(fid == -1)
    error('Cannot current open the file for writing. Ensure it is not already open!');
else
    fclose(fid);
end

TG_Q = [TG1_Qfnc(TG1_M) TG2_Qfnc(TG2_M)];
TG_E = [TG1_Eff(TG1_M),TG2_Eff(TG2_M)];
TG_Q(TG_Q < 0) = 0;
TG_E(isnan(TG_E) | isinf(TG_E)) = 1e-8;

fprintf('Exporting Excel Data...');

xlswrite(path,double(TG_Q > 0),'Control','H39:I39');
xlswrite(path,TG_Q,'Control','H40:I40');
xlswrite(path,TG_E,'Control','H41:I41');
xlswrite(path,[BT1,BT2],'Control','H44:I44');

xlswrite(path,double([B1F,B2F,B3F] > 0),'Control','H33:J33');
xlswrite(path,[B1F,B2F,B3F],'Control','H34:J34');
xlswrite(path,[BLR1_Eff(nlcon(end-2)) BLR2_Eff(nlcon(end-1)) BLR3_Eff(nlcon(end))],'Control','H36:J36');

xlswrite(path,[HPU_Q,MPU_Q],'Control','H49:I49');
xlswrite(path,[COST_FG COST_BELEC COST_SELEC COST_WATER],'Control','H53:K53');
fprintf(' Done!\n'); 