classdef SymUtility < SymBuilder
%SYMUTILITY  Create a SYMUTILITY object for creating symbolic utility system optimization problems
%
%   Copyright (C) 2023 Jonathan Currie (www.controlengineering.co.nz)

    methods
        
        function [expF,expS,con1,con2,con3,con4] = AddGTGHRSG(B,names,Fuel,fuelT,airT,airP,airRelHum,exhaustT,Lg,rangeQ,SFuel,stackT,stackO2,bfwH,stmT,stmP,bdr)
        %  ADDGTGHRSG  Add Gas Turbine Generator Connected to HRSG to Symbolic Utility Model [Includes Part-Load Model] 
        %   [expF,expS,con1,con2,con3,con4] = AddGTGHRSG(B,names,Fuel,fuelT,airT,airP,airRelHum,exhaustT,Lg,rangeQ,SFuel,stackT,stackO2,bfwH,stmT,stmP,bdr)
        %
        %   names are {Unit Name,q,msteam,mbfw,h,b}
        %   rangeQ is [Qmin Qmax] or Qmax (min assumed 20% of max)
        %   if SFuel is empty, assumed unfired
        %   
        %   Units: Duty kW, Mass Flow t/h, Pressure bar, Temperature C
        
            %Generate Part-Load Models
            if(length(rangeQ) == 1), rangeQ = [0.2*rangeQ rangeQ]; end  
            if(isempty(SFuel))
                [Ag,Bg,As,Bs] = SymUtility.HRSGReg(Fuel,rangeQ,fuelT,airT,exhaustT,Lg,stackT,bfwH,stmT,stmP,bdr);
                %Generate Q vs Fuel Expression
                expF = sprintf('%s_FuelM = (%s + %1.15g*%s)/%1.15g',names{1},names{2},Ag,names{end},Bg/3.6);
                %Generate Q vs Steam Expression
                expS = sprintf('%1.15g*%s + %1.15g*%s',As*3.6,names{end},Bs*3.6,names{2}); 
                %GTG Efficiency
                cunits = JSteamMEX('GetCurrentUnits');
                JSteamMEX('SetDefaultUnits');
                expE = sprintf('%s_Eff = %s/(((%s + %1.15g)/%s)*%1.15g)',names{1},names{2},names{2},Ag,Bg,JSteamMEX('NHV',Fuel));
                JSteamMEX('SetCurrentUnits',cunits);
                conM = [];
            else
                [Af,Bf,Cf,Abs,Bbs,Ams,Bms,~,~,~,~,~,~,~,~,maxM0,Ag,Bg] = SymUtility.HRSGSupReg(Fuel,rangeQ,fuelT,airT,airP,airRelHum,exhaustT,Lg,SFuel,stackT,stackO2,bfwH,stmT,stmP,bdr); 
                %Generate Q vs Fuel Expression
                expF = sprintf('%s_FuelM = %1.15g*%s + %1.15g*%s + %1.15g*%s',names{1},Af*3.6,names{end},Bf*3.6/1e3,names{2},Cf,names{3});
                expS = [];
                %GTG Efficiency
                cunits = JSteamMEX('GetCurrentUnits');
                JSteamMEX('SetDefaultUnits');
                expE = sprintf('%s_Eff = %s/(((%s + %1.15g)/%s)*%1.15g)',names{1},names{2},names{2},Ag,Bg,JSteamMEX('NHV',Fuel));
                JSteamMEX('SetCurrentUnits',cunits);
                %Constraint for no steam in mass flow var when binary off
                conM = sprintf('%s - %1.15g*%s <= 0',names{3},maxM0+10,names{end});
            end            
            %Create GTG Constraints
            bigM = rangeQ(2) + 5e3;
            con1 = sprintf('%s - %1.15g*%s <= 0',names{2},bigM,names{end}); %when off must equal 0
            con2 = sprintf('-%s + %1.15g*%s <= %1.15g',names{2},bigM,names{end},-rangeQ(1)+bigM); %when on must be greater than min Q
            %Create HRSG Constraints            
            if(~isempty(SFuel))
                %Max and Base Steam Functions
                expBS = sprintf('%1.15g*%s + %1.15g*%s',Abs*3.6,names{end},Bbs*3.6,names{2});
                expMS = sprintf('%1.15g*%s + %1.15g*%s',Ams*3.6,names{end},Bms*3.6,names{2});
                bigM = maxM0 + 10;
                %Create HRSG Constraints
                con3 = sprintf('%s + %1.15g*%s <= %s + %1.15g',names{3},bigM,names{end},expMS,bigM); %less than max steam M
                con4 = sprintf('-%s + %1.15g*%s <= -(%s) + %1.15g',names{3},bigM,names{end},expBS,bigM); %greater than base steam M
            else
                con3 = sprintf('%s - %s = 0',expS,names{3}); %steam production must match mass flow var
                con4 = [];
            end
            %Add To Model
            B.AddConstant([names{1} '_BD'],bdr);
            B.AddConstant(names{5},JSteamMEX('HPT',stmP,stmT)); %HRSG H
            B.AddCon(con1);
            B.AddCon(con2);
            B.AddCon(con3);
            B.AddCon(sprintf('%s-(1+%1.15g)*%s = 0',names{4},bdr,names{3})); %mass balance
            if(~isempty(SFuel)), B.AddCon(con4); end
            if(~isempty(conM)), B.AddCon(conM); end
            B.AddExpression(expF,['A:' names{1}]);
            B.AddResultExp(['D:' names{1}],names{2},names{end});
            B.AddResultExp(['F:' names{1}],names{3});
            B.AddExpression(expE,['M:' names{1}]);   
            B.AddBound(sprintf('0 <= %s <= %1.15g',names{2},rangeQ(2)));
            B.AddBound(sprintf('0 <= %s <= 1',names{end}));
            B.AddInteger(sprintf('%s = B',names{end}));
        end
        
        function [expF,con1,con2] = AddGTG(B,names,Fuel,rangeQ,Lg)
        %  ADDGTG  Add Gas Turbine Generator to Symbolic Utility Model [Includes Part-Load Model] 
        %   AddGTG(names,Fuel,rangeQ,Lg)
        %
        %   names are {Unit Name,q,b}
        %   rangeQ is [Qmin Qmax] or Qmax (min assumed 20% of max)
        %   
        %   Units: Duty kW, Mass Flow t/h
        
            %Default Loss Coefficient Internal
            if(nargin < 5), Lg = []; end
            %Generate Part-Load Model
            if(length(rangeQ) == 1), rangeQ = [0.2*rangeQ rangeQ]; end  
            [a,b] = SymUtility.GasTurbineReg(Fuel,rangeQ,Lg);
            %Generate Q vs Fuel Expression
            expF = sprintf('%s_FuelM = (%s + %1.15g*%s)/%1.15g',names{1},names{2},a,names{3},b/3.6);
            %Create Constraints
            bigM = rangeQ(2) + 5e3;
            con1 = sprintf('%s - %1.15g*%s <= 0',names{2},bigM,names{3}); %when off must equal 0
            con2 = sprintf('-%s + %1.15g*%s <= %1.15g',names{2},bigM,names{3},-rangeQ(1)+bigM); %when on must be greater than min Q
            %Add To Model
            B.AddCon(con1);
            B.AddCon(con2);
            B.AddExpression(expF,['A:' names{1}]);
            B.AddResultExp(['D:' names{1}],names{2},names{3});
            B.AddBound(sprintf('0 <= %s <= %1.15g',names{2},rangeQ(2)));
            B.AddBound(sprintf('0 <= %s <= 1',names{3}));
            B.AddInteger(sprintf('%s = B',names{3}));
        end
        
        function [expM,con1,con2,con3] = AddBlrFrd(B,names,Fuel,rangeM,fuelT,airT,airP,airRelHum,stackT,bfwH,stmT,stmP,bdr,exO2)
        % ADDBLRFRD  Add Fired Boiler To Symbolic Utility Model [Includes Part-Load Model]
        %   AddBlrFrd(names,Fuel,rangeM,fuelT,airT,airP,airRelHum,stackT,bfwH,stmT,stmP,bdr,exO2)
        %
        %   names are {Unit Name,msteam,mbfw,h,b}
        %   rangeM is either [Mmin Mmax] or Mmax (min assumed 10% of max)
        %
        %   Units: Mass Flow t/h, Pressure bar, Temperature C, Ratios fraction
        
            if(length(rangeM) == 1), rangeM = [0.1*rangeM rangeM]; end        
            %Generate Part-Load Model
            [a,b,~,~,~,ar,br] = SymUtility.SteamBoilerReg(Fuel,rangeM./3.6,fuelT,airT,airP,airRelHum,stackT,bfwH,stmT,stmP,bdr,exO2);
            %Generate Steam vs Fuel Expression
            expM = sprintf('%s_FuelM = %1.15g*%s + %1.15g*%s',names{1},a*3.6,names{5},b,names{2});
            %Generate Efficiency Expression (Exports Only) @(m) m./(maxM*ar + m*(br + 1));
            expE = sprintf('%s_Eff = (%s/3.6)/(%1.15g + %s*%1.15g)',names{1},names{2},rangeM(2)/3.6*ar,names{2},(br + 1)/3.6);
            %Create Constraints
            bigM = rangeM(2) + 10;
            con1 = sprintf('%s - %1.15g*%s <= 0',names{2},bigM,names{5}); %when off must equal 0
            con2 = sprintf('-%s + %1.15g*%s <= %1.15g',names{2},bigM,names{5},-rangeM(1)+bigM); %when on must be greater than min flow
            con3 = sprintf('%s-(1+%1.15g)*%s = 0',names{3},bdr,names{2}); %mass balance
            %Add To Model
            B.AddConstant(names{4},JSteamMEX('HPT',stmP,stmT));
            B.AddCon(con1); B.AddCon(con2); B.AddCon(con3);
            B.AddResultExp(['F:' names{1}],names{2},names{5});
            B.AddExpression(expM,['A:' names{1}]);   
            B.AddExpression(expE,['M:' names{1}]);   
            B.AddBound(sprintf('0 <= %s <= %1.15g',names{2},rangeM(2)));
            B.AddBound(sprintf('0 <= %s <= 1',names{5}));
            B.AddInteger(sprintf('%s = B',names{5}));
            
        end
        
        function [con1,con2] = AddWHB(B,names,duty,bfwH,stmT,stmP,bdr,eff)
        %ADDWHB Add Waste Heat Boiler to Symbolic Builder Model [Duty Based]
        %
        %   AddWHB(names,duty,bfwH,stmT,stmP,bdr,eff)
        %
        %   names are {Unit Name,msteam,mbfw,h}
        %   eff is optional, defaults to 0.9

            if(nargin < 8), eff = 0.9; end
            %Thermo Calcs
            WHB_H = JSteamMEX('HPT',stmP,stmT);
            WHB_BDH = JSteamMEX('HPX',stmP,0);
            WHB_eff = eff; %assume fixed efficiency
            %Energy Balance
            con1 = sprintf('%1.15g - %s = 0',(duty*WHB_eff)/(WHB_H + bdr*WHB_BDH - bfwH*(bdr + 1))*3.6,names{2});
            con2 = sprintf('%s-(1+%1.15g)*%s = 0',names{3},bdr,names{2}); %mass balance
            %Add To Model
            B.AddConstant(names{4},WHB_H);
            B.AddConstant([names{1} '_Q'],duty);
            B.AddConstant([names{1} '_Eff'],eff);
            B.AddCon(con1); B.AddCon(con2);
            B.AddResultExp(['F:' names{1}],names{2}); 
        end
        
        function con1 = AddWHBM(B,names,stmM,stmT,stmP,bdr)
        %ADDWHB Add Waste Heat Boiler to Symbolic Builder Model [Flow Based]
        %
        %   AddWHB(names,stmM,stmT,stmP,bdr)
        %
        %   names are {Unit Name,msteam,mbfw,h}
        %   eff is optional, defaults to 0.9

            if(nargin < 6), bdr = 0; end
            %Thermo Calcs
            WHB_H = JSteamMEX('HPT',stmP,stmT);
            %Mass Balance
            if(bdr > 0)
                con1 = sprintf('%s - %s*%1.15g = 0',names{3},names{2},1+bdr);
                B.AddCon(con1);
            elseif(~strcmp(names{2},names{3}))
                error('With no blowdown mass steam = mass bfw, consider renaming variables');
            end                
            %Add To Model
            B.AddConstant(names{4},WHB_H);            
            B.AddBound(sprintf('%1.15g <= %s <= %1.15g',stmM,names{2},stmM));
            B.AddResultExp(['F:' names{1}],names{2}); 
        end
        
        function [expQ,expH,con1,con2,conQ] = AddTurboGen(B,names,inP,outP,maxQ,inH,rangeH)
        % ADDTURBOGEN  Add Steam Turbo Generator to Symbolic Utility Model [Includes Part-Load Model]
        %   AddTurboGen(names,inP,outP,maxQ,inH,rangeH)
        %
        %   names are {Unit Name,m,h,b}
        %   rangeH is either [Hmin, Hmax] or inH +- rangeH (scalar)
        %
        %   Units: Duty kW, Mass Flow t/h, Pressure bar
       
            if(nargin < 7), rangeH = []; end
            if(isempty(inH) && isempty(rangeH)), error('You must supply inH or rangeH - or both!'); end
            %Generate Part-Load Model + Isentropic Enthalpy Drop
            if(isempty(inH))
                [Ac,Bc,maxM] = SymUtility.SteamTurbineReg(maxQ,inP,mean(rangeH),outP);
            else
                [Ac,Bc,maxM] = SymUtility.SteamTurbineReg(maxQ,inP,inH,outP);
            end
            [a,b] = SymUtility.FitDHis(inP,outP,inH,rangeH); 
            dHis = sprintf('(%1.15g + %1.15g*%s)',a,b,names{3});
            %Generate Shaftwork Expression Q = (6/5)*(1/B)*(dHis-A/maxM).*(m-(1/6)*maxM)
            expQ = sprintf('%s_Q = %1.15g*(%s - %1.15g)*%s/3.6 + %1.15g*(%1.15g - %1.15g*%s)*%s',names{1},6/(5*Bc),dHis,Ac/maxM,names{2},1/(5*Bc),Ac,maxM,dHis,names{4});
            %Generate Output Enthalpy Expression
            expH = sprintf('%s_H = %s - %1.15g*(%1.15g + %1.15g*%s)*(1 - %1.15g/((%1.15g + %1.15g*%s)*%1.15g))*(1 - %1.15g/(6*(%s + 1-%s)/3.6))',...
                            names{1},names{3},6/(5*Bc),a,b,names{3},Ac,a,b,names{3},maxM,maxM,names{2},names{4}); 
            %Generate Efficiency Expression (Exports Only)
            expE = sprintf('%s_Eff = %s_Q / (%s * %s/3.6)',names{1},names{1},dHis,names{2});
            %Generate either or constraints
            bigM = maxM*3.6+10; minM = 0.4*maxM*3.6;
            con1 = sprintf('%s - %1.15g*%s <= 0',names{2},bigM,names{end}); %mass flow = 0 when turbine off
            con2 = sprintf('-%s + %1.15g*%s <= %1.15g',names{2},bigM,names{end},- minM + bigM); %>= min flow
            %Generate max Q constraint
            conQ = sprintf('%s_Q <= %1.15g',names{1},maxQ);
            %Add To Model
            B.AddCon(con1);
            B.AddCon(con2);
            B.AddCon(conQ);
            B.AddExpression(expQ,['D:' names{1}],names{4});
            B.AddExpression(expH,['I:' names{1}],names{2});
            B.AddExpression(expE,['M:' names{1}]);
            B.AddBound(sprintf('0 <= %s <= 1',names{end}));
            B.AddInteger(sprintf('%s = B',names{end}));        
        end
        
        function [expQ,expH,conQ] = AddBPT(B,names,inP,outP,ratQ,eff,inH,rangeH)
        % ADDBPT  Add Back Pressure Turbine to Symbolic Utility Model [No Part-Load Model]
        %   AddBPT(names,inP,outP,ratQ,eff,inH,rangeH)
        %
        %   names are {Unit Name,m,h,b}
        %   rangeH is either [Hmin, Hmax] or inH +- rangeH (scalar)
        %
        %   Units: Duty kW, Mass Flow t/h, Pressure bar
        
            %Default Range
            if(nargin < 8), rangeH = []; end
            %Generate Isentropic Enthalpy Drop Expression
            [a,b] = SymUtility.FitDHis(inP,outP,inH,rangeH);
            dHis = sprintf('(%1.15g + %1.15g*%s)',a,b,names{3});
            %Generate Enthalpy & Shaftwork Expressions
            expQ = sprintf('%s_Q = %s*(1/3.6)*%1.15g*%s',names{1},names{2},eff,dHis);
            expH = sprintf('%s_H = %s - %1.15g*%s',names{1},names{3},eff,dHis);
            %Generate rated Q constraint
            conQ = sprintf('%s*%1.15g - %s*(1/3.6)*%1.15g*%s = 0',names{4},ratQ,names{2},eff,dHis);
            %Add to Model
            B.AddCon(conQ);
            B.AddConstant([names{1} 'RQ'],ratQ);
            B.AddExpression(expQ,['E:' names{1}],names{4});
            B.AddExpression(expH,['I:' names{1}],names{2});
            B.AddExpression(sprintf('%s_RQ = %1.15g',names{1},ratQ));
            B.AddExpression(sprintf('%s_Eff = %1.15g',names{1},eff));
            B.AddBound(sprintf('0 <= %s <= 1',names{end}));
            B.AddInteger(sprintf('%s = B',names{end}));
        end
        
        function [expQ1,expQ2,expQ3,expH1,expH2,expH3,conM,conQ] = AddBPT3(B,names,inP,outP1,outP2,outP3,ratQ,eff1,eff2,eff3,inH,rangeH)
        % ADDBPT3  Add Triple Stage Back Pressure Turbine to Symbolic Utility Model [No Part-Load Model]
        %   AddBPT3(names,inP,outP1,outP2,outP3,ratQ,eff1,eff2,eff3,inH,rangeH)
        %
        %   names are {Unit Name,inM,outM1,outM2,outM3,inH,b}
        %   rangeH is either [Hmin, Hmax] or inH +- rangeH
        %
        %   Units: Duty kW, Mass Flow t/h, Pressure bar
        
            %Default Range
            if(nargin < 12), rangeH = []; end
            %Sample Output Enthalpies (bit rough)
            [~, ~, outH1, outH2] = JSteamMEX('UnitOp_Turbine3M', inH, inP, outP1, eff1, 2, outP2, eff2, 2, outP3, eff3, 2);
            %Generate Isentropic Enthalpy Drop Expression
            [a,b] = SymUtility.FitDHis(inP,outP1,inH,rangeH);
            dHis1 = sprintf('(%1.15g + %1.15g*%s)',a,b,names{6});
            [a,b] = SymUtility.FitDHis(outP1,outP2,outH1,rangeH);
            dHis2 = sprintf('(%1.15g + %1.15g*%s_H1)',a,b,names{1});
            [a,b] = SymUtility.FitDHis(outP2,outP3,outH2,rangeH);
            dHis3 = sprintf('(%1.15g + %1.15g*%s_H2)',a,b,names{1});
            %Generate Enthalpy & Shaftwork Expressions
            expQ1 = sprintf('%s_Q1 = %s*(1/3.6)*%1.15g*%s',names{1},names{2},eff1,dHis1);
            expQ2 = sprintf('%s_Q2 = (%s+%s)*(1/3.6)*%1.15g*%s',names{1},names{4},names{5},eff2,dHis2);
            expQ3 = sprintf('%s_Q3 = %s*(1/3.6)*%1.15g*%s',names{1},names{5},eff3,dHis3);                        
            expH1 = sprintf('%s_H1 = %s - %1.15g*%s',names{1},names{6},eff1,dHis1);
            expH2 = sprintf('%s_H2 = %s_H1 - %1.15g*%s',names{1},names{1},eff2,dHis2);
            expH3 = sprintf('%s_H3 = %s_H2 - %1.15g*%s',names{1},names{1},eff3,dHis3);
            %Generate rated Q constraint
            conQ = sprintf('%s*%1.15g - %s_Q1 - %s_Q2 - %s_Q3 = 0',names{7},ratQ,names{1},names{1},names{1});            
            %Mass Balance
            conM = sprintf('%s-%s-%s-%s = 0',names{2},names{3},names{4},names{5});
            %Result Expression
            resQ = sprintf('%s_Q1+%s_Q2+%s_Q3',names{1},names{1},names{1});
            %Add to Model
            B.AddCon(conQ); B.AddCon(conM);
            B.AddConstant([names{1} 'RQ'],ratQ);
            B.AddExpression(expQ1);
            B.AddExpression(expQ2);
            B.AddExpression(expQ3);
            B.AddResultExp(['E:' names{1}],resQ,names{end});
            B.AddExpression(expH1,['I:' names{1}],names{3});
            B.AddExpression(expH2,['I:' names{1}],names{4});
            B.AddExpression(expH3,['I:' names{1}],names{5});
            B.AddExpression(sprintf('%s_RQ = %1.15g',names{1},ratQ));
            B.AddExpression(sprintf('%s_Eff1 = %1.15g',names{1},eff1));
            B.AddExpression(sprintf('%s_Eff2 = %1.15g',names{1},eff2));
            B.AddExpression(sprintf('%s_Eff3 = %1.15g',names{1},eff3));
            B.AddBound(sprintf('0 <= %s <= 1',names{end}));
            B.AddInteger(sprintf('%s = B',names{end}));
        end
            
        
        function expQ = AddFixPump(B,names,inH,inP,outP,eff)
        %ADDFIXPUMP Add Fixed Inlet Enthalpy Pump to Symbolic Utility Model
        %   AddFixPump(names,inP,outP,eff)
        %
        %   names are {Pump Name,m,outh}
        %
        %   If eff is omitted, defaults to 0.7
        %
        %   Units: Duty kW, Mass Flow t/h, Pressure bar

            if(nargin < 6), eff = 0.7; end
            %Calculate Output Enthalpy
            [~, outH] = JSteamMEX('UnitOp_Pump', inH, inP, 100, outP, eff); %massflow is arbitrary
            %Build Q expression
            expQ = sprintf('%s_Q = %s*%1.15g',names{1},names{2},(outH-inH)/3.6);
            B.AddExpression(expQ,['J:' names{1}],names{2});
            B.AddConstant(names{3},outH);
        end
        
        
        function con = AddUser(B,names,duty,outH,retFrac)
        %ADDUSER Add Duty Based User to Symbolic Utility Model
        %   AddUser(names,duty,outH,retFrac)
        %
        %   names are {User Name,inM,inH,outM}
        %
        %   If outH omitted, assumed full energy of stream used
        %   If retFrac omitted, assumed all returned
        %
        %   Units: Duty kW, Mass Flow t/h

            if(nargin < 5), retFrac = 1; end
            if(nargin < 4 || isempty(outH)), outH = 0; end

            if(outH > 0)
                con = sprintf('%1.15g/(%s - %1.15g)*3.6 - %s = 0',duty,names{3},outH,names{2});
            else
                con = sprintf('%1.15g/%s*3.6 - %s = 0',duty,names{3},names{2});
            end
            B.AddCon(con);
            B.AddCon(sprintf('%s/%1.15g - %s = 0',names{4},retFrac,names{2}));
            B.AddConstant([names{1} '_H'],outH);   
            if(length(names) < 4)
                B.AddResultExp(['G:' names{1}],names{2},names{2});
            else
                B.AddResultExp(['G:' names{1}],names{2},names{4});
            end
            B.AddExpression(sprintf('%s_F = %1.15g',names{1},retFrac));
            B.AddExpression(sprintf('%s_H = %1.15g',names{1},outH));
            B.AddExpression(sprintf('%s_Q = %1.15g',names{1},duty));
        end
    
        function AddHeader(B,names,H)
        %ADDHEADER Add Header to Symbolic Utility Model
        %   AddHeader(names,minH)
        %
        %   names are {Header Name,h,mVent}
        %
        %   minH  can also be [minH maxH]

            if(nargin > 2)
                if(length(H) > 1)
                    B.AddBound(sprintf('%1.15g <= %s <= %1.15g',H(1),names{2},H(2)));
                else
                    B.AddBound(sprintf('%1.15g <= %s <= %1.15g',H,names{2},3800));
                end
            end
            B.AddResultExp(['H:' names{1}],names{2});
            if(length(names) > 2)
                B.AddResultExp(['K:' names{1}],names{3});
            end
        end
        
        function AddFlash(B,names,P)
        %ADDFLASH Add Flash Drum to Symbolic Utility Model
        %   AddFlash(names,pressure)
        %
        %   names are {Unit Name, inM, vapM, liqM, inH}
        
            B.AddCon(sprintf('%s-%s-%s=0',names{2},names{3},names{4}));  %mass blance
            B.AddCon(sprintf('%s*%s - %s_VH*%s - %s_LH*%s = 0',names{5},names{2},names{1},names{3},names{1},names{4})); %energy balance
            B.AddConstant([names{1} '_VH'],JSteamMEX('HPX',P,1));
            B.AddConstant([names{1} '_LH'],JSteamMEX('HPX',P,0));
        end
        
        function AddDeaerator(B,names,P,Vtr)
        %ADDDEAERATOR Add Deaerator to Symbolic Utility Model
        %   AddDeaerator(names,pressure,vent_ratio)
        %
        %   names are {Unit Name, steamM,condM,vapM,liqM,steamH,condH}
        
            B.AddCon(sprintf('%s+%s-%s-%s = 0',names{2},names{3},names{4},names{5})); %mass balance
            B.AddCon(sprintf('%s*%1.15g-%s = 0',names{5},Vtr,names{4})); %vent mass balance
            B.AddCon(sprintf('%s*%s + %s*%s - %s_VH*%s - %s_LH*%s = 0',names{6},names{2},names{7},names{3},names{1},names{4},names{1},names{5}));  %energy balance
            B.AddConstant([names{1} '_VH'],JSteamMEX('HPX',P,1));
            B.AddConstant([names{1} '_LH'],JSteamMEX('HPX',P,0));
        end
        
        function AddDesuper(B,names)
        %ADDDESUPER  Add Desuperheated to Symbolic Utility Model
        %   AddDesuper(names)
        %
        %   names are {Unit Name,steamM,bfwM,outM,steamH,bfwH,outH}
        
            B.AddCon(sprintf('%s+%s-%s=0',names{2},names{3},names{4})); %mass balance
            B.AddCon(sprintf('%s*%s + %s*%s - %s*%s=0',names{5},names{2},names{6},names{3},names{7},names{4})); %energy balance   
            B.AddResultExp(['L:' names{1}],names{4});
        end

        
        function AddWater(B,names,H)
        %ADDWATER Add Make Up Water Stream to Symbolic Utility Model
        %   AddUser(names,H)
        %
        %   names are {Stream Name,m,h}
        %
        %   Mass Flow t/h
        
            B.AddConstant(names{3},H); 
            B.AddResultExp(['C:' names{1}],names{2});  
        end                
        
        function AddSteamGroups(B)
        %ADDSTEAMGROUPS  Add Default Steam Result Groups

            B.AddResultGroup('A','Fuel Gas Consumption [ton/h]');
            B.AddResultGroup('B','Electricity Balance [kW]');
            B.AddResultGroup('C','Water Consumption [ton/h]');
            B.AddResultGroup('D','GTG & Turbo Generators Output [kW]');
            B.AddResultGroup('E','Process Driver Shaftwork [kW]');
            B.AddResultGroup('F','Steam Generation [ton/h]');
            B.AddResultGroup('G','Steam Users: In M [ton/h]     Out M [ton/h]');
            B.AddResultGroup('H','Header Enthalpy [kJ/kg]');
            B.AddResultGroup('I','Turbines:   Out H [kJ/kg]     Out M [ton/h]');
            B.AddResultGroup('J','Pumps:      Q [kW]            Out M [ton/h]');
            B.AddResultGroup('K','Header Vents [ton/h]');
            B.AddResultGroup('L','Let Down Flows [ton/h]');
            B.AddResultGroup('M','Unit Efficiencies [%]');
        end
        
        function [con1,con2] = AddPwrBal(B,names,MaxCoGen)
        %ADDPWRBAL  Add Power Balance Equations to Symbolic Builder Model
        %   AddPwrBal(B,name,MaxCoGen)
        %
        %   names are {Power Expression Variable,b}
        %
        % When selling  TPOW >= 0  (var = 0)
        % When buying   TPOW <= 0  (var = 1)

            %BigM Constraint
            MaxCoGen = MaxCoGen + 10e3;            
            con1 = sprintf('-%s - %1.15g*%s <= 0',names{1},MaxCoGen,names{2}); %buy var = 1
            con2 = sprintf('%s + %1.15g*%s <= %1.15g',names{1},MaxCoGen,names{2},MaxCoGen); %sell var = 0
            %Add To Model
            B.AddCon(con1);
            B.AddCon(con2);
            B.AddBound(sprintf('0 <= %s <= 1',names{2}));
            B.AddInteger(sprintf('%s = B',names{2}));
            B.AddResultExp('B:PWR',names{1},names{2});
        end
        
    end 
    
    
    methods(Static)
        %Single Point Routines
        [Q,Eff,H,maxEff,maxM] = SteamTurbineCalc(maxQ,inM,inP,inH,outP);
        [fuelM,Eff,maxEff] = SteamBoilerCalc(Fuel,maxM,fuelT,airT,airP,airRelHum,stackT,bfwH,stmT,stmP,bdr,exO2);
        [fuelM,Eff,maxEff,maxFuel] = GasTurbineCalc(Fuel,maxQ,Q,Lg);
        %Regression Routines
        [a,b,DHis] = FitDHis(inP,outP,inH,rangeH,weight);
        [eff,p,data] = PetersonMannData();
        [A,B,maxM,maxEff,Qfnc,Hfnc,Efffnc,ar,br] = SteamTurbineReg(maxQ,inP,inH,outP,JStm);
        [a,b,Ffnc,Efffnc,maxEff,ar,br] = SteamBoilerReg(Fuel,maxM,fuelT,airT,airP,airRelHum,stackT,bfwH,stmT,stmP,bdr,exO2);
        [a,b,Ffnc,Efffnc,maxEff,maxFuel,Ag,Bg] = GasTurbineReg(Fuel,maxQ,Lg);
        [Ag,Bg,As,Bs,Ffnc,Sfnc,EffFnc,GTEffFnc,CCEffFnc,QFnc,maxGTEff] = HRSGReg(Fuel,maxQ,fuelT,airT,airP,airRelHum,exhaustT,Lg,stackT,bfwH,stmT,stmP,bdr,outP);
        [Af,Bf,Cf,Abs,Bbs,Ams,Bms,Ffnc,BSFnc,MSFnc,Q,StmD,TfuelM,baseM,maxM,maxM0,Ag,Bg] = HRSGSupReg(Fuel,maxQ,fuelT,airT,airP,airRelHum,exhaustT,Lg,SFuel,stackT,stackO2,bfwH,stmT,stmP,bdr,npts);   
        %Misc
        [f,Aeq,beq,sos,xb,yb,res,stat] = generatePWL(fun,x0,xdata,method,draw);
        checkMexMixture(mix,name);
    end
    
end