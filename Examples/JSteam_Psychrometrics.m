%% JSteam Psychrometric Calculations
clc
clear
JSteamMEX('Load');
JSteamMEX('SetDefaultUnits');

%% Draw Psychrometric Chart
clc
% User Parameters
Pamb = JSteamMEX('OneAtm'); %bar
Tamb = linspace(5,60); %C
RelHum = 0:10:100; % percent

% Constants
f = 1.0045; %enhancement factor (at approx 1 atm, 25C)
B = 0.6219907; %ratio of molecular weight

% Calculation Loop
nh = length(RelHum); nt = length(Tamb);
w = zeros(nt,nh);
for i = 1:nh
    for j = 1:nt
        % Saturated Water Vapour Pressure
        Pws = JSteamMEX('PsatcT','Water',Tamb(j));
        % Mole Fraction of Water
        Xw = RelHum(i)/100*f*Pws/Pamb;
        % Humidity Ratio
        w(j,i) = B * Xw/(1-Xw); %[kg/kg]
    end
end

% Plot
plot(Tamb,w*1e3,'color',[0 0.447 0.7410]); axis('tight');
ylim([0 30]); grid on;
xlabel('Dry Bulb Temperature [^\circC]');
ylabel('Humidity Ratio [g_w/kg_a]');
title(sprintf('Psychrometric Chart for Pressure = %g bar',Pamb));


%% Creating a Humid Air Mixture
clc
% Specifications
Tamb = min(Tamb):5:max(Tamb); %C

% For each humidity point
for i = 1:length(RelHum)
    % For each temperature point
    for j = 1:length(Tamb)
        % Set as Spec Above use built in JSteam Function
        humAir = JSteamMEX('humidair',RelHum(i)/100,Pamb,Tamb(j));

        % Manually Calculate Humidity Ratio and Compare to Psychrometric Chart
        massFrac = JSteamMEX('mole2mass',humAir);
        idx = strcmpi(massFrac(:,1),'Water');
        if(any(idx))
            fracs = [massFrac{:,2}]';
            %kg water / kg of air
            wman = sum(fracs(idx))/sum(fracs(~idx));
        else
            wman = 0;
        end

        % Add to our existing plot
        hold on
        plot(Tamb(j),wman*1e3,'r.','markersize',15)
        hold off
    end
end

%Note differences are due to rounding of the fractions as JSteam does not
%account for Argon or other small fractions in Air.

