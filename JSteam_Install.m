function JSteam_Install
%% Installation File for JSteam

% In order to run this tool, please run this file to setup the required
% directories. You MUST be in the current directory of this file!

%   Copyright (C) 2012 Jonathan Currie (I2C2)

cpath = cd;
try
    cd('Utilities');
catch %#ok<CTCH>
    error('You don''t appear to be in the JSteam Toolbox directory');
end
%Get current versions    
cur_ver = JSteamver();

fprintf('\n------------------------------------------------\n')
fprintf(['  INSTALLING JSTEAM MATLAB INTERFACE ver ' sprintf('%1.2f',cur_ver) '\n\n'])

%Perform pre-req check
cd(cpath);
if(~preReqChecks(cpath))
    return;
end

%Uninstall previous versions
fprintf('\n- Checking for previous versions of JSteam Toolbox...\n');
no = JSteam_Uninstall('JSteam_Install.m',0);
if(no < 1)
    fprintf('Could not find a previous installation of JSteam Toolbox\n');
else
    fprintf('Successfully uninstalled previous version(s) of JSteam Toolbox\n');
end

%Add toolbox path to MATLAB
fprintf('\n- Adding JSteam Paths to MATLAB Search Path...');
genp = genpath(cd);
genp = regexp(genp,';','split');
%Folders to exclude from adding to Matlab path
i = 1;
rInd{:,:,i} = strfind(genp,'vti_cnf'); i = i + 1;
rInd{:,:,i} = strfind(genp,'vti_pvt'); i = i + 1;
rInd{:,:,i} = strfind(genp,'Source'); i = i + 1;
rInd{:,:,i} = strfind(genp,'Internal Tests'); i = i + 1;
rInd{:,:,i} = strfind(genp,'fluids'); i = i + 1;
rInd{:,:,i} = strfind(genp,'.git'); i = i + 1;
ind = NaN(length(rInd{1}),1);
%Track indices of paths to remove from list
for i = 1:length(rInd{1})
    for j = 1:size(rInd,3)
        if(any(rInd{j}{i}))
            ind(i) = 1;
        end
    end
end

%Remove paths from above and add to matlab path
genp(ind == 1) = [];
addpath(genp{:});
rehash
fprintf('Done\n\n');
in = input('- Would You Like To Save the Path Changes? (Recommended) (y/n): ','s');
if(strcmpi(in,'y'))
    try
        savepath;
    catch %#ok<CTCH>
        warning('opti:install',['It appears you do not have administrator rights on your computer to save the Matlab path. '...
                                'In order to run JSteam Toolbox you will need to install it each time you wish to use it. To fix '...
                                'this please contact your system administrator to obtain administrator rights.']);
    end
end

%Post Install Test if requested
in = input('\n- Would You Like To Run Post Installation Tests? (Recommended) (y/n): ','s');
if(strcmpi(in,'y'))
    ok = JSteam_Install_Test(1);
else
    ok = true;
end

if(ok)
    %Launch Help Browser
    web('https://www.controlengineering.co.nz/Wikis/JSteam/pmwiki.php/Matlab/Main','-helpbrowser');

    %Finished
    fprintf('\nJSteam Toolbox Installation Complete!\n');
end
disp('------------------------------------------------')


function no = JSteam_Uninstall(token,del)

%Check nargin in, default don't delete
if(nargin < 2 || isempty(del))
    del = 0;
end

%Check if we have anything to remove
paths = which(token,'-all');
len = length(paths);
if(~len) %should always be at least 1 if we are in correct directory
    error('Expected to find "%s" in the current directory - please ensure you are in the JSteam Toolbox directory');        
elseif(len == 1)
    %if len == 1, either we are in the correct folder with nothing to remove, or we are in the
    %wrong folder and there are files to remove, check CD
    if(any(strfind(paths{1},cd)))
        no = 0;
        return;
    else
        error('Expected to find "%s" in the current directory - please ensure you are in the JSteam Toolbox directory',token);
    end    
else %old ones to remove
    
    %Remove each folder found, and all subdirs under
    for n = 2:len
        %Absolute path to remove
        removeP = paths{n};
        %Search backwards for first file separator (we don't want the filename)
        for j = length(removeP):-1:1
            if(removeP(j) == filesep)
                break;
            end
        end
        removeP = removeP(1:max(j-1,1));        

        %Everything is lowercase to aid matching
        lrpath = lower(removeP);
        opath = regexp(lower(path),';','split');

        %Find & Remove Matching Paths
        no = 0;
        for i = 1:length(opath)
            %If we find it in the current path string, remove it
            fnd = strfind(opath{i},lrpath);        
            if(~isempty(fnd))  
                rmpath(opath{i});
                no = no + 1;
            end
        end
        
        %Check we aren't removing our development version
        rehash;
        if(isdir([removeP filesep 'Testing'])) %is this robust enough?
            fprintf('Found development version in "%s", skipping.\n',removeP);
            return;
        end

        %If delete is specified, also delete the directory
        if(del)
            stat = recycle; recycle('on'); %turn on recycling
            rmdir(removeP,'s'); %not sure if we dont have permissions here
            recycle(stat); %restore to original
        end
    end    
end


function OK = preReqChecks(cpath)
%Search for each required prereq
% Note we no longer search the registry, simply check if we can load a mex
% file which requires each runtime

if(~isempty(strfind(computer,'64')))
    arch = 'x64';
    icarch = 'intel64';
else
    arch = 'x86';
    icarch = 'ia32';
end

mver = ver('MATLAB');
fprintf('- Checking MATLAB version and operating system...\n');
vv = regexp(mver.Version,'\.','split');
if(str2double(vv{1}) < 8)
    if(str2double(vv{2}) < 12)
        if(str2double(vv{2}) < 10)
            error('MATLAB 2011a or above is required to run JSteam - sorry!');
        else %2010a/b/sp1
            fprintf(2,'JSteam is designed for MATLAB 2011a or above.\nIt will install into 2010a, but you may experience reliability problems.\nPlease upgrade to R2011a or later.\n');
        end
    end
end
switch(mexext)
    case 'mexw32'
%         fprintf('MATLAB %s 32bit (Windows x86) detected\n',mver.Release);
        error('From v1.72 JSteam Toolbox only supports 64bit (Windows x64) platforms.');
    case 'mexw64'
        fprintf('MATLAB %s 64bit (Windows x64) detected\n',mver.Release);
    otherwise
        error('JSteam Toolbox is compiled only for Windows systems - sorry!');
end

fprintf('\n- Checking for required pre-requisites...\n');
missing = false;
havVC = true; havIF = true;
%Check for VC++ 2012
if(~isempty(strfind(computer,'64')))
    cd('MEX/Win64');
else
    cd('MEX/Win32');
end
try
    a = vc19check; %#ok<NASGU>
catch
    havVC = false;
end
try
    a = JSteamMEX; %#ok<NASGU>
catch
    havIF = false;
end
cd(cpath);
%See if missing anything
if(~havVC || ~havIF)
    missing = true;
end

%Print Missing PreReqs
if(~havVC)
    fprintf(2,'Cannot find the Microsoft VC++ 2019 %s Redistributable!\n',arch); 
else
    fprintf('Found the Microsoft VC++ 2019 %s Redistributable\n',arch); 
end
if(~havIF)
    fprintf(2,'Cannot find the Intel Fortran XE 2019 %s Redistributable!\n',arch);
else
    fprintf('Found the Intel Fortran XE 2019 %s Redistributable\n',arch); 
end    

%Install Instructions for each Package
if(missing)
    fprintf(2,'\nYou are missing one or more pre-requisites. Please read the instructions below carefully to install them:\n\n');
    
    if(~havVC)
        fprintf(2,' Microsoft VC++ 2019:\n  - Download from: https://aka.ms/vs/17/release/vc_redist.x64.exe\n');
        fprintf(2,'  - When prompted, select the ''%s'' package. Once downloaded, install it.\n\n',arch);
        fprintf(2,['NOTE: If you have already downloaded and installed VC++ 2019 (and restarted MATLAB) - it may be that you are missing the Universal C Runtime (Universal CRT).\nThis is automatically installed '...
                    'with Windows Updates - but if you don''t have those turned on, you can download it from here:\nhttps://www.microsoft.com/en-us/download/details.aspx?id=48234\n\n']);
    end
    if(~havIF) 
        fprintf(2,' Intel Fortran XE 2019:\n  - Download from: https://software.intel.com/en-us/articles/redistributable-libraries-for-intel-c-and-fortran-2019-compilers-for-windows\n');
        fprintf(2,'  - The download page will contain multiple links. Download the latest (highest number) update from the ''Intel Fortran Compiler for Windows Table''\n');
        fprintf(2,'  - The download package will contain two files. Install the ''%s'' package.\n\n',icarch);
    end
    
    fprintf(2,'\nOnce you have downloaded AND installed all the above packages, you MUST restart MATLAB.\n\nIf this message appears again after installing the above packages, try restarting your computer.\n\n\n');
    
    OK = false;
else
    OK = true;
end