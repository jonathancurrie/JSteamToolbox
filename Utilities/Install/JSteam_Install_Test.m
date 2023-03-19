function ok = JSteam_Install_Test(varargin)
% Installation Test File
%
% A collection of tests to verify the installation of the toolbox has been
% successful. 

%   Jonathan Currie (C)
%   Control Engineering 2023

%Set Verbosity
if(nargin < 1)
    verb = 1; %default show all messages
else
    verb = varargin{1};
end

%Set default ok
ok = 1;

fprintf('\nChecking JSteam Toolbox Installation [this will take a few seconds]:\n');

%Load JSteam Interface
try
    fprintf('Loading JSteam... ');
    ok = JSteamMEX('Load',0); 
    if(ok)
        fprintf('Ok\n');
    else
        fprintf(2,'Error Loading JSteam Interface.\n');
        return;
    end
catch
    ok = 0;
    fprintf(2,'Failed to Load JSteam Interface.\n');
    return;
end

%% TEST 1 - Check Main Paths
if(verb); fprintf('Checking Paths... '); end
paths = {'MEX'};
len = length(paths);
for i = 1:len
    pass = exist(paths{i},'dir');
    if(~pass)
        fprintf('\nFailed Path Check on %s\n',paths{i});
        ok = 0;
        return
    end
end
if(verb); fprintf('Ok\n'); end

%% TEST 2 - Check Unit Conversion Results
if(verb); fprintf('Checking Unit Conversion Results... '); end
if(~JSteamMEX('Verify','Unit_Conv'))
    fprintf('\nFailed Internal Unit Conversion Verification\n');
    ok = 0;
    return;
end
if(verb); fprintf('Ok\n'); end

%% TEST 3 - Check Steam Thermo Results
if(verb); fprintf('Checking Steam Thermo Results... '); end

if(~JSteamMEX('Verify','Steam_Thermo'))
    fprintf('\nFailed Internal Steam Thermo Verification\n');
    ok = 0;
    return;
end
%Load PreSolved Steam Results
load SteamThermoTest
stmfnc = {'HPT','SPT','CpPT','CvPT','VPT'};
stmvars = {'H','S','Cp','Cv','V'};
satfnc = {'HPX','SPX','CpPX','CvPX','VPX'}; fnclen = length(satfnc);
satvars = {'Hsat','Ssat','Cpsat','Cvsat','Vsat'};

%Seq Check
len = length(P); val = zeros(len,1);
for i = 1:fnclen
    for j = 1:len
        val(j) = eval(['JSteamMEX(''',stmfnc{i},''',P(',num2str(j),'),T(',num2str(j),'))']);
    end
    if(norm(eval(stmvars{i})-val) > 1e-6)
        fprintf('\nFailed Sequential Steam Thermo Result Check on %s\n',stmfnc{i});
        ok = 0;
        return
    end
    for j = 1:len
        val(j) = eval(['JSteamMEX(''',satfnc{i},''',Psat(',num2str(j),'),X(',num2str(j),'))']);
    end
    if(norm(eval(satvars{i})-val) > 1e-6)
        fprintf('\nFailed Sequential Steam Thermo Result Check on %s\n',satfnc{i});
        ok = 0;
        return
    end
end
%Par Check
stmvars = {'pH','pS','pCp','pCv','pV'};
satvars = {'pHsat','pSsat','pCpsat','pCvsat','pVsat'};
for i = 1:fnclen
    val = eval(['JSteamMEX(''',stmfnc{i},''',pP,pT)']);
    if(norm(eval(stmvars{i})-val) > 1e-6)
        fprintf('\nFailed Parallel Steam Thermo Result Check on %s\n',stmfnc{i});
        ok = 0;
        return
    end
    val = eval(['JSteamMEX(''',satfnc{i},''',pPsat,pX)']);
    if(norm(eval(satvars{i})-val) > 1e-6)
        fprintf('\nFailed Parallel Steam Thermo Result Check on %s\n',satfnc{i});
        ok = 0;
        return
    end
end
if(verb); fprintf('Ok\n'); end  

%% TEST 4 - Check Fuel Thermo Results
if(verb); fprintf('Checking General Thermo Results... '); end
% if(~JSteamMEX('Verify','GEN_THERMO')) %takes toooooo long
%     fprintf('\nFailed Internal General Thermo Verification\n');
%     ok = 0;
%     return;
% end
%Load PreSolved Steam Results
load FuelThermoTest
fuelfnc = {'HcPT','ScPT','CpcPT'}; fnclen = length(fuelfnc);
fuelvars = {'H','S','Cp'};
%Seq Check
len = length(T); nocomp = length(comps); val = zeros(len,nocomp); 
for i = 1:fnclen
    for c = 1:nocomp
        for j = 1:len
            if(strcmp(fuelfnc{i}(end-1:end),'PT'))
                val(j,c) = eval(['JSteamMEX(''',fuelfnc{i},''',''',comps{c},''',P(',num2str(j),'),T(',num2str(j),'))']);
            else
                val(j,c) = eval(['JSteamMEX(''',fuelfnc{i},''',''',comps{c},''',T(',num2str(j),'))']);
            end
        end
    end
    if(norm(eval(fuelvars{i})-val) > 1e-6)
        fprintf('\nFailed Sequential General Thermo Result Check on %s\n',fuelfnc{i});
        ok = 0;
        return
    end
end
%Par Check
fuelvars = {'pH','pS','pCp'};
for i = 1:fnclen
    for c = 1:nocomp
        if(strcmp(fuelfnc{i}(end-1:end),'PT'))
            val = eval(['JSteamMEX(''',fuelfnc{i},''',''',comps{c},''',pP,pT)']);
        else
            val = eval(['JSteamMEX(''',fuelfnc{i},''',''',comps{c},''',pT)']);
        end
        if(norm(eval([fuelvars{i} '(:,:,c)'])-val) > 1e-6)
            fprintf('\nFailed Parallel General Thermo Result Check on %s with %s\n',fuelfnc{i},comps{c});
            ok = 0;
            return
        end
    end
end
if(verb); fprintf('Ok\n'); end  

%% TEST 5 - Check Unit Operation Results
if(verb); fprintf('Checking Unit Operation Results... '); end
if(~JSteamMEX('Verify','Steam_UnitOps'))
    fprintf('\nFailed Internal Steam UnitOps Verification\n');
    ok = 0;
    return;
end
if(~JSteamMEX('Verify','Com_UnitOps'))
    fprintf('\nFailed Internal Combustion UnitOps Verification\n');
    ok = 0;
    return;
end
if(~JSteamMEX('Verify','ORC_UnitOps'))
    fprintf('\nFailed Internal ORC UnitOps Verification\n');
    ok = 0;
    return;
end
if(verb); fprintf('Ok\n'); end

%% Final
fprintf('JSteam Toolbox Checked Out Ok! - Enjoy\n');