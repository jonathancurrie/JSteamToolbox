%% JSteam Mollier Diagram Example
% J.Currie Mar 2023
clc
clear
JSteamMEX('load')

%% Compute Data & Plot
clc
% Lines of Constant Pressure
s_range = 3:0.25:10;
p_range = [0.01 0.02 0.04 0.1 0.2 0.5 1 2 6 10 20 50 ...
    100 200 500 1e3];
[S_conP,P] = meshgrid(s_range,p_range);
H_conP = JSteamMEX('HPS',P,S_conP);

% Enthalpy & Entropy at Constant Quality
x_range = 1:-0.05:0;
p_range = [0.01:0.01:0.1 0.2:0.1:1 2:1:10 20:10:220];
[X,P] = meshgrid(x_range,p_range);
H_conX = JSteamMEX('HPX',P,X);
S_conX = JSteamMEX('SPH',P,H_conX);

% Lines of Constant Temperature
t_range = [10 50 100:100:800];
s_range = 10:-0.01:3;
[T,S_conT] = meshgrid(t_range,s_range);
H_conT = JSteamMEX('HTS',T,S_conT);

% Remove infeasible lines (ok, a little hacky!)
dH_conT = [diff(H_conT,1);ones(1,size(H_conT,2))];
S_idx = S_conT < 6;
H_conT(dH_conT==0 & S_idx) = NaN;

% Plot it Nicely
line_conP = plot(S_conP',H_conP','color',[0 0.447 0.7410]);
hold on;
line_conX = plot(S_conX,H_conX,'color',[0.7410 0.447 0],'linewidth',2);
line_conT = plot(S_conT,H_conT,'color',[0.447 0.7410 0]);
hold off;
grid on; ylim([1000 4000]); xlim([3 10]);
title('Mollier Diagram for Water & Steam')
xlabel('Entropy [$\frac{kJ}{kg.K}$]','Interpreter','latex')
ylabel('Enthalpy [$\frac{kJ}{kg}$]','Interpreter','latex')
legend([line_conP(1),line_conX(1),line_conT(1)],...
    {'Constant Pressure','Constant Quality','Constant Temperature'},...
    'location','se')