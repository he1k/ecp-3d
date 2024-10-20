mdl = 'mod2';
sim(mdl);
Ts = 0.004;
%%
idx = find(miss>1,1,'last'); % Find index for synchronization
t_clean = t(idx:end);
theta1_clean = theta1(idx:end);
theta2_clean = theta2(idx:end);
theta3_clean = theta3(idx:end);
u_clean = u(idx:end);

figure;
subplot(4,1,1);
plot(t_clean, theta1_clean);
title('$\theta_1$')
xlabel('Time (s)');

subplot(4,1,2);
plot(t_clean, theta2_clean);
title('$\theta_2$')
xlabel('Time (s)');

subplot(4,1,3);
plot(t_clean, theta3_clean);
title('$\theta_3$')
xlabel('Time (s)');

subplot(4,1,4);
plot(t_clean, u_clean);
title('$u$')
xlabel('Time (s)');
%%
figure;
hold on;
stairs(t_clean,theta1_clean,'b','LineWidth',2);
stairs(t_clean,theta2_clean,':g','LineWidth',2);
stairs(t_clean,u_clean,'--r','LineWidth',2);
legend('u recieved','meas','u transmitted')
xlim([1.8 1.825])
%%
figure;
subplot(3,1,1);
plot(t, theta1);
% hold on;
% plot(t, theta2);
% plot(t, theta3);
% legend('$\theta_1$','$\theta_2$','$\theta_3$');
xlabel('Time (s)');
ylabel('Angle (deg)');
subplot(3,1,2)
plot(t, u);
legend('$u$');
xlabel('Time (s)');
ylabel('Torque (Nm)');
subplot(3,1,3);
stairs(t, miss);
xlabel('Time (s)');
ylabel('Miss cycles');
%%
figure;
stairs(t, theta1);
hold on;
stairs(t, u);
%xlim([0.7 1.1]);
%%
figure;
stairs(t, theta3*1e-6)
hold on;
stairs(t, miss > 0);
legend('Reported elapsed time for teensy since start command','Missed ticks');

%%
figure;
stairs(t, d_rdy_rx,'r');
hold on;
stairs(t, d_rdy_tx,'b');
hold on
stairs(t, miss);
stairs(t,theta2/4000,'k')
legend('rx','tx','miss','dt');
ylim([-1 2])

%%
tau = 0.1;
T = 0.001;
s = tf('s');
G = s/(1+tau*s);
H = c2d(G,T,'tustin')
z = tf('z',T);
t = 0:T:3;
y = zeros(1,length(t));
a1 = (T-2*tau)/(T+2*tau);
b0 = 2/(T+2*tau);
b1 = -2/(T+2*tau);
K = (b0+b1*z^(-1))/(1+a1*z^(-1));
figure;
[y,t] = step(H);
plot(t,y,'b');
hold on;
[y,t] = step(K);
plot(t,y,'--r')
