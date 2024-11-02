clear all; clc; close all;
f_SCLK_MAX = 1/(20e-9);
disp(['Max SCLK ', num2str(f_SCLK_MAX/1e6), ' MHz'])
%% Checking timing
T_TEENSY = 250e-6;
T_MATLAB = 4e-3;
f_SCLK = 4e6;
SDEL = 1e-6;
T_SCLK = 1/f_SCLK*1e6
%% Simulate model to get a feel of interrupt handling
%% Load parameters
load('ECP_values.mat');
% Physical system parameters
J_1 = ECP_values(1);            % Disk 1 inertia kgm^2
J_2 = ECP_values(2);            % Disk 2 inertia kgm^2
J_3 = ECP_values(3);            % Disk 3 inertia kgm^2
k_1 = ECP_values(4);            % Shaft 1-2 stiffness Nm/rad
k_2 = ECP_values(5);            % Shaft 2-3 stiffness Nm/rad
b_1 = mean(ECP_values([6 7]));  % Disk 1 damping and friction Nms/rad
b_2 = mean(ECP_values([8 9]));  % Disk 2 damping and friction Nms/rad
b_3 = mean(ECP_values([10 11]));% Disk 3 damping and friction Nms/rad
T_Cp = ECP_values(12);          % Disk 1 Coulomb friction in pos direction
T_Cm = ECP_values(13);          % Disk 1 Coulomb friction in neg direction
atan_scale = 100;               % Sign approximation factor
w_th = 0.75;                    % Threshold angular velocity rad/s

% The system states are [theta_1;omega_1;theta_2;omega_2;theta_3;omega_3]
x_0 = [0;0;0;0;0;0];            % Initial conditions
T_s = 0.004;                    % Sampling period
sigma_meas = 0.0093*eye(3);     % Measurements covariance matrix
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% State space representation

A =   [
      0        1        0              0        0        0;
      -k_1/J_1 -b_1/J_1 k_1/J_1        0        0        0;
      0        0        0              1        0        0;
      k_1/J_2  0        (-k_1-k_2)/J_2 -b_2/J_2 k_2/J_2  0;
      0        0        0              0        0        1;
      0        0        k_2/J_3        0        -k_2/J_3 -b_3/J_3
      ];
B =   [
      0     0;
      1/J_1 0;
      0     0;
      0     1/J_2;
      0     0;
      0     0;
      ];
C =   [
      1 0 0 0 0 0
      0 0 1 0 0 0
      0 0 0 0 1 0 
      ];
D =   [
      0 0;
      0 0;
      0 0
      ];
E_x = [
      0;
      -1/J_1;
      0;
      0;
      0;
      0
      ];
E_y = [
      0;
      0;
      0
      ];
F_x = [
      0     0     0 0 0;
      1/J_1 0     0 0 0;
      0     0     0 0 0;
      0     1/J_2 0 0 0;
      0     0     0 0 0;
      0     0     0 0 0
      ];
F_y = [
      0 0 1 0 0;
      0 0 0 1 0;
      0 0 0 0 1];
% Discrete time
sysd = c2d(ss(A,[B E_x],C,[D E_y]),T_s);
F = sysd.A;
G = sysd.B(:,1:2);
E_x_d = sysd.B(:,3);
F_d = F;
G_d = G;
% Check controllability and observability of base system
if rank(ctrb(F,G)) == size(F,2)
    disp('System is controllable');
else
    disp('System is not controllable');
end

if rank(obsv(F,C)) == size(F,2)
    disp('System is observable');
else
    disp('System is not observable');
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% State-feedback LQR design
Q_c = diag([2 0 2 0 2.5 0.0024]);
R_c = diag([10 10]);
[K_discr,S,CLP] = dlqr(F,G,Q_c*T_s,R_c*T_s);
% Scaling of reference
C_3 = C(3,:);
C_ref = pinv(C_3*inv(eye(6) - F + G*K_discr)*G*K_discr);
% Kalman filter with friction estimation - DO NOT MODIFY
F_kf_aug = [F_d G_d(:,1);zeros(1,6) 1];
G_kf_aug = [G_d;0 0];
C_kf_aug = [C zeros(3,1)];
% Kalman gain
L_kf_aug = dlqe(F_kf_aug,eye(7),C_kf_aug,1e-3*eye(7), ...
                sigma_meas(1,1).^2*eye(3));
L_kf_o = L_kf_aug(1:6,:);
L_kf_d = L_kf_aug(7,:);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Simulate model
clc;
mdl = "ecpclosedloop.slx";
simTime = 3;
sim(mdl);
figure;
plot(t,)

