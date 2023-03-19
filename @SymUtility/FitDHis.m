function [a,b,DHis] = FitDHis(inP,outP,inH,rangeH,weight)
%FITDHIS  Fit Isentropic Enthalpy Drop Across Steam Turbine
%
%   [a,b] = FitDHis(inP,outP,inH,rangeH,weight)
%
%   DHis = a + b*InH
%
%   rangeH (scalar): +- of inH, (vector), range to fit over

%Number of fitting points
nH = 30;
%Default Weighting
if(nargin < 5), weight = 1; end
%Range of Fit if Not Specified
if(nargin < 4 || isempty(rangeH)), rangeH = 50; end

%If we have a range of H, or is true, must fit isentropic enthalpy drop
if(length(rangeH) > 1)
    Hmin = max(rangeH(1),JSteamMEX('HPX',inP,1)); %min is sat
    Hmax = rangeH(2); %no limit here
else 
    Hmin = max(inH-rangeH,JSteamMEX('HPX',inP,1)); %min is sat
    Hmax = inH + rangeH; %no limit here
end

%Fit isentropic enthalpy drop
Hfit = linspace(Hmin,Hmax,nH)';
P1 = inP*ones(nH,1); P2 = outP*ones(nH,1);
S = JSteamMEX('SPH',P1,Hfit);
iH = JSteamMEX('HPS',P2,S);
%Create Weighting Vector (weight middle 3rd)
w = ones(size(Hfit));
w(floor(nH/3):ceil(2*nH/3)) = weight;
DHis = optifit(Hfit.*w,(Hfit-iH).*w,'poly1');
%Extract Coeffs
a = DHis.theta(2); b = DHis.theta(1);

%Plot if No Output
if(~nargout)
    subplot(211)
    plot(Hfit,(Hfit-iH),Hfit,a + Hfit.*b,'o');
    ylabel('\Delta H_{isen}');
    title(sprintf('Fitted Function: \\Delta H_{isen} = %1.3g + %1.3g*inH',a,b));
    if(length(rangeH) == 1)
        hold on; plot([inH inH],ylim,'k:'); hold off;
    end
    xlim([min(Hfit),max(Hfit)]);
    subplot(212)
    plot(Hfit,(Hfit-iH) - (a + Hfit.*b));
    xlabel('InH [kJ/kg]');  ylabel('Residual');
    xlim([min(Hfit),max(Hfit)]);
end