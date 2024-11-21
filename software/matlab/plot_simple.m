clc;
close all

theta1 = y(:,1);
theta2 = y(:,2);
theta3 = y(:,3);

figure;
hold on;
stairs(t, theta1,'b');
stairs(t, theta2);
stairs(t, theta3);
stairs(t, miss > 1)
stairs(t,u,'--r')
stairs(t,rx_cmd);
legend('$\theta_1$','$\theta_2$','$\theta_3$','miss','$u$','fo');
ylim([-1 1])
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