function ExcelExport(U,path,exports)
%EXCELEXPORT  Automatically Export Optimization Results to a JSteam Excel Model
%
%   ExcelExport(U,path,exports)

path = which(path);

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

%For each variable in the model, assign its value from the solution
for i = 1:length(U.vars)
    eval(sprintf('%s = %1.15g;',char(U.vars(i)),U.Opt.sol(i)));
end
%Eval constants
for i = 1:length(U.constnt)
    eval(sprintf('%s = %1.15g;',U.constnt{i,1},U.constnt{i,2}));
end
%Eval expressions until no errors (missing vars occur) (poor form Jonny)
done = false;
while ~done
    done = true;
    %Eval constants
    for i = 1:length(U.exprsn)
        try
            eval(sprintf('%s = %1.15g;',U.exprsn{i,1},eval(U.exprsn{i,2})));
        catch
            done = false;
        end
    end        
end

%Begin writing data
fprintf('Exporting Excel Data...');
for i = 1:size(exports,1)
    if(exist(exports{i,1},'var') || exports{i,1}(1) == '[')
        val = eval(exports{i,1});
        val(isnan(val)) = 1e-8;
        xlswrite(path,val,'MATLAB',exports{i,2});
    end           
end
fprintf(' Done!\n'); 

