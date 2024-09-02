mdl = 'mod2';
sim(mdl);
%%
A = rx.signals.values(2)
dec2hex(A)
F = typecast(A,'single')
% B = 0;
% for i=1:8
%     B = B + A(i)*2^((i-1)*4)
% end