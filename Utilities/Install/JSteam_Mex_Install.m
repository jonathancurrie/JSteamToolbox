function JSteam_Mex_Install()
%% Mex Install File

%%
%mex -setup
clc
% fprintf(2,'Please Specify Visual Studio 2019 as your Compiler...\n\n');
% mex -setup

%%
%Ensure you are in the JSteam Toolbox directory!

fprintf('\n------------------------------------------------\n');
fprintf('JSteam MEX FILE INSTALL\n\n');

clear JSteamMEX JSteamHPT vc19check

%Get Correct JSteam Libraries
switch(computer)
    case 'PCWIN64'
        post = ' -IInclude lib/Win64/JSteam.lib';
        fol = 'Win64';
    otherwise
        error('O/S Not Supported!');
end

%If compiling with VS2015 but pre R2015b, need to manually add in UCRT location
cc = mex.getCompilerConfigurations();
for i = 1:length(cc)
    if(~isempty(strfind(cc(i).Name,'Microsoft Visual C++')) && str2double(cc(i).Version) >= 14 && verLessThan('matlab','8.6'))
        post = [post opti_FindUCRT()];
        break;
    end
end

% Files to Compile
cdir = cd;
cd 'Source';
try    
    pre = 'mex -largeArrayDims JSteamMEX.cpp JSteamUtils.cpp JSteamUnits.cpp JSteamSolve.cpp JSteamSolveUO.cpp';
    eval([pre post]);
    fprintf('Build Complete\n\n');
    movefile(['JSteamMEX.' mexext],['../MEX/' fol '/'],'f') 
catch ME
    cd(cdir);
    error('jsteam:mex','Error Compiling JSteam!\n%s',ME.message);
end
cd '../';
fprintf('------------------------------------------------\n');

%% VC19 Check
cdir = cd;
cd 'Source';
try    
    eval(['mex -largeArrayDims vc19check.c' post]);
    fprintf('Build Complete\n\n');
    movefile(['vc19check.' mexext],['../MEX/' fol '/'],'f') 
catch ME
    cd(cdir);
    error('jsteam:mex','Error Compiling JSteam VC19 Check!\n%s',ME.message);
end
cd '../';
