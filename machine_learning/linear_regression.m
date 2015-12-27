%%definitions
% the inputs to this code si u y u_max y_max as follows
% u is the input vector
% y is the output vector
% y_max & u_max used to give y and u the same weight
% uTR & yTR are the training set equals 2/3 from u & y
% uV & yV are the validation set equals 1/3 from u & y
% the transfer function to identify is:
%  a1 s + a2 
% ----------------
% s^2 + b1 s + b2

% in the discrete form: z transform with zero order hold approximation:
% y         a1 z^-1 + a2 z^-2
% -- = ------------------------
% u     1 - b1 z^-1 - b2 z^-2
%
% then finally y = a1 u(z^-1) + a2 u(z^-2) + b1 y(z^-1) + b2 y(z^-2)
%to be scaled to the same weight to each feature:
%              y = A1 phi_1   + A2 phi_2   + B1 phi_3   + B2 phi_4
% where phi_1 = u(z^-1) / u_max , phi_3 = y(z^-1) / y_max
%       phi_2 = u(z^-2) / u_max , phi_4 = y(z^-2) / y_max
% then : a1 = A1 / u_max  , b1 = B1 / y_max
%        a2 = A2 / u_max  , b2 = B2 / y_max
L =double( uint64(length(u) / 3));
n = length(u);
uTR = u(1:2*L); uV = u(2*L : n);
yTR = y(1:2*L); yV = y(2*L : n);
dt = 0.01;
u_max = max(u);
y_max = max(y);
%% training set
nTR = 2*L;
PHI_TR = rearrange(uTR , yTR , u_max , y_max );
THETA = (PHI_TR' * PHI_TR) \ PHI_TR' * yTR(3:2*L);   % global minimum for least squares costTR function
theta = [1/u_max 0 0 0 ; 0 1/u_max 0 0 ; 0 0 1/y_max 0 ; 0 0 0 1/y_max]*THETA;
a1 = theta(1);
a2 = theta(2);
b1 = theta(3);
b2 = theta(4);


% costTR function
costTR = 0;
i = 3;

while i <= nTR-2
costTR = costTR + (yTR(i) + PHI_TR(i,:)*THETA)^2;
i = i + 1;
end
costTR = costTR/(nTR-2);


%compare output
yTR_m = yTR(1:2);
i = 3;
while i <= nTR
yTR_m(i) = [uTR(i-1) , uTR(i-2) , yTR_m(i-1) , yTR_m(i-2)]*theta;
i = i + 1;
end
t = 0:dt:dt*(nTR -1);
plot(t , yTR_m , t , yTR );
legend('Hypothesis' , 'Real model')
title('training set')

%% Validation set
nV = n - 2*L + 1;
PHI_V = rearrange(uV , yV , u_max , y_max);

% costV function
costV = 0;
i = 3;

while i <= nV-2
costV = costV + (yV(i) + PHI_V(i,:)*THETA)^2;
i = i + 1;
end
costV = costV/(nV-2);


%compare output
yV_m = yV(1:2);
i = 3;
while i <= nV
yV_m(i) = [uV(i-1) , uV(i-2) , yV_m(i-1) , yV_m(i-2)]*theta;
i = i + 1;
end
t = 0:dt:dt*(nV -1);
figure
plot(t , yV_m , t , yV);
legend('Hypothesis' , 'Real model')
title('vlaidation set')

%% output 
costTR
costV
theta

z = tf('z', dt);
G_discrete = (a1 *z + a2)/(z^2 - b1 *z - b2)
G_continuous = d2c(G_discrete)