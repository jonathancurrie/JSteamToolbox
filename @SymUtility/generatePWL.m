function [f,Aeq,beq,sos,xb,yb,res,stat] = generatePWL(fun,x0,xdata,method,draw)
%GENERATEPWL  Generate Piecewise Linear Approximation of Univariate Nonlinear Function
%
%   [f,Aeq,beq,sos,xb,yb,res,stat] = generatePWL(fun,x0,xdata,method,draw)
%
%   Input arguments:
%       fun - nonlinear function to fit a piecewise linear approximation to
%       x0 - initial guess of breakpoints (must be in order and feasible values)
%       xdata - x data that the nonlinear function exists across (should be continuous)
%       method - see below (optional)
%       draw - draw the resulting piecewise linear fit (optional - 0/1)
%
%   Output arguments:
%       f - new linear objective function
%       Aeq - new linear equalities LHS
%       beq - new linear equalities RHS
%       sos - structure of SOS type 2
%       xb - x values of breakpoints
%       yb - y values of breakpoints
%       res - final residual
%       stat - optimizer status structure
%
%   Possible methods:
%       {'fixed'}         Under approximation with fixed values from user
%       'optimal'         Under approximation with optimal values from NLS
%       'outer-fixed'     Outer approximation with fixed values from user
%       'outer-optimal'   Outer approximation with optimal values from NLS
%
%   Under approximation - x0 are the linear segment intersections AND lie on f(x) 
%   Outer approximation - x0 are the points where the linear segments tangent to f(x) intersect

%Input checks
if(nargin < 3), error('You must supply fun, x0 and xdata!'); end
if(isempty(x0) || isempty(xdata)), error('x0 and xdata must not be empty!'); end
if(~isa(fun,'function_handle')), error('fun must be a function handle!'); end
if(~issorted(x0)), error('x0 is not in order!'); end

%Default args
if(nargin < 4 || isempty(method)), method = 'fixed'; draw = 0; end
if(nargin < 5 || isempty(draw)), draw = 0; end
%Transposes
[r,c] = size(x0);
if(c > r), x0 = x0'; end
[r,c] = size(xdata);
if(c > r), xdata = xdata'; end

%Get breakpoints
switch(lower(method))
    %Fixed (user selected) methods
    case {'fixed','outer-fixed'}
        [xb,yb,res] = fixedPWL(fun,x0,xdata,method);
        stat = [];
    %Optimal (NLS) methods
    case {'optimal','outer-optimal'}   
        [xb,yb,res,stat] = optimalPWL(fun,x0,xdata,method);
        
    otherwise
        error('Unknown method: %s',method);
end

%Generate MILP matrices
[f,Aeq,beq,sos] = genMILP(xb,yb);

%Plot if required
if(draw)
%     clf;
    plot(xdata,fun(xdata));
    xlabel('x');
    ylabel('y');
    if(isempty(strfind(method,'outer')))
        title(sprintf('Piecewise Under Linear Approximation of Nonlinear Function\n%d segements, SSE %5.5g',length(xb)-1,res));
    else
        title(sprintf('Piecewise Outer Linear Approximation of Nonlinear Function\n%d segements, SSE %5.5g',length(xb)-1,res));
    end

    %Draw Tanget Lines
    no = length(xb);
    hold on;
    for i = 1:no
        plot(xb(i),yb(i),'ro');
        if(i < no)
            plot([xb(i) xb(i+1)],[yb(i) yb(i+1)],'r'); 
        end
    end
    hold off;
end



function [xb,yb,fval] = fixedPWL(fun,x0,xdata,method)
%User breakpoints

%Check range
lb = min(xdata); ub = max(xdata);
if(any(x0 > ub) || any(x0 < lb))
    error('A value in the supplied x0 is outside the range of values supplied in xdata');
end

%Generate residual based on method selected
switch(lower(method))
    case 'fixed'
        %Complete breakpoint vector
        [r,c] = size(x0);
        if(c > r), x0 = x0'; end
        xb = [xdata(1); x0; xdata(end)];
        yb = fun(xb); %on f(x) curve
        ylin = underError(fun,x0,xdata);
    case 'outer-fixed'
        [ylin,xb,yb] = outerError(fun,x0,xdata);
end

%Assign output
fval = sum((fun(xdata)-ylin).^2);



function [xb,yb,fval,stat] = optimalPWL(fun,x0,xdata,method)
%NLS breakpoints

%Generate objective based on method selected
switch(lower(method))
    case 'optimal'
        fx = @(x,xdata) underError(fun,x,xdata);
    case 'outer-optimal'
        fx = @(x,xdata) outerError(fun,x,xdata);
end

%Check initial f(x) doesn't contain an error (nan, inf)
y = fx(x0,xdata);
if(any(isinf(y)) || any(isnan(y)))
    error('Initial x0 results in a NaN or Inf from linear approximation, please reselect x0');
end
        
%Run optimizer
[xb,fval,ef,stat] = opti_lsqcurvefit(fx,x0,xdata,fun(xdata));

%If error - display
if(ef ~= 1)
    fprintf('There was an error running the optimizer - please check your problem!\n\n');
    display(stat);
end

%Generate yb
switch(lower(method))
    case 'optimal'
        %Complete breakpoint vector
        [r,c] = size(xb);
        if(c > r), xb = xb'; end
        xb = [xdata(1); xb; xdata(end)];
        yb = fun(xb); %on f(x) curve
    case 'outer-optimal'
        [~,xb,yb] = outerError(fun,xb,xdata);
end


function [f,Aeq,beq,sos] = genMILP(xb,yb)
%Generate MILP from breakpoints

%dec var = [x, lambda1, lambda2,.., lambdan]

[r,c] = size(yb);
if(c > r), yb = yb'; end

f = [0;yb];
Aeq = [0  ones(1,length(xb));
       -1 xb'];
beq = [1;0];
sos.sostype = '2';
sos.sosind = [2:length(xb)+1]'; %#ok<*NBRAK>
sos.soswt = [1:length(xb)]';


%Optimizer function for under approximation
function y = underError(fun,x,xdata)

%Generate segment vector
x = [xdata(1);x;xdata(end)];
%Optimizer chosen break points function values
ylin = fun(x);

%Calculate gradients & intercepts
m = (ylin(1:end-1)-ylin(2:end))./(x(1:end-1)-x(2:end));
c = ylin(2:end)-(m.*x(2:end));

%Generate y
j = 1;
y = zeros(size(xdata));
for i = 1:length(xdata)
    %Check which segment we're in
    if(xdata(i) > x(j+1))
        j = j + 1;
        if(j > length(x)-1)
            break;
        end
    end
    %Function calc
    y(i) = m(j)*xdata(i) + c(j);
end


%Optimizer function for outer approximation
function [y,xb,yb] = outerError(fun,x,xdata)

%Gradient
dy = @(x) mklJac(fun,x);
%Linear Tangent Lines
m = diag(dy(x));
c = fun(x) - m.*x;
%Intersections
it = (-c(1:end-1)+c(2:end))./(m(1:end-1)-m(2:end));

%Generate segment vector
xb = [xdata(1);it;xdata(end)];
%Generate yb
yb = zeros(size(xb));
for i = 1:length(xb)-1
    yb(i) = m(i)*xb(i)+c(i);
end
yb(end) = m(end)*xb(end)+c(end);

%Generate y
j = 1;
y = zeros(size(xdata));
for i = 1:length(xdata)
    %Check which segment we're in
    if(xdata(i) > xb(j+1))
        j = j + 1;
        if(j > length(xb)-1)
            break;
        end
    end
    %Function calc
    y(i) = m(j)*xdata(i) + c(j);
end




