clc;
close all

theta1 = y(:,1);
omega1 = y(:,2);
theta2 = y(:,3);
omega2 = y(:,4);
theta3 = y(:,5);
omega3 = y(:,6);

% Plot control commands
figure;
title('Control Commands')
stairs(t, tx_cmd,'r')
hold on
stairs(t, rx_cmd,'--b')
legend('cmd(tx)','cmd(rx)')

% Plot data error
figure;
title('Data Error')
stairs(t, tx_err,'r')
hold on
stairs(t, rx_err,'--b')
legend('err(tx)','err(rx)')

% Plot data ready
figure;
title('Data Ready')
stairs(t, tx_rdy,'r')
hold on
stairs(t, rx_rdy,'--b')
legend('rdy(tx)','rdy(rx)')

% Plot sent and recieved data
figure;
title('Data Ready')
stairs(t, u,'r')
hold on
stairs(t, theta1,'--b')
legend('dat(tx)','dat(rx)')

% Plot recieved data
figure;
hold on;
plot(t, theta1);
plot(t, omega1);
plot(t, theta2);
plot(t, omega2);
plot(t, theta3);
plot(t, omega3);
legend('$\theta_1$','$\omega_1$','$\theta_2$','$\omega_2$','$\theta_3$','$\omega_3$')
