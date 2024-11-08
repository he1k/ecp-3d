clc;
close all

theta1 = y(:,1);
theta2 = y(:,2);
theta3 = y(:,3);

figure;
hold on;
plot(t, theta1,'b');
plot(t, theta2);
plot(t, theta3);
plot(t, miss > 1)
plot(t,u,'--r')
legend('$\theta_1$','$\theta_2$','$\theta_3$','miss','$u$');
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