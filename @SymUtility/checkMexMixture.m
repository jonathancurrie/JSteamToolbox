function checkMexMixture(mix,name)
%Checks if a mixture is compatible with the JSteam MEX Interface

if(~iscell(mix) || size(mix,2) ~= 2)
    error('%s must be a cell array of the form {Component 1, Mole Fraction 1; Component 2, Mole Fraction 2; ...}',name);
end

for i = 1:size(mix,1)
    if(~ischar(mix{i,1}))
        error('%s Component %d name must be a string',name,i);
    end
    if(~isnumeric(mix{i,2}))
        error('%s Component %d mole fraction must be a double',name,i);
    end
end