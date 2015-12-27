function PHI = rearrange(u , y , u_max , y_max)
n = length(u);  % = length(y)
phi_1 = [u(2) ; u(3:n-1)] / u_max;
phi_2 = [u(1) ; u(2) ; u(3:n-2)]/ u_max;
phi_3 = [y(2) ; y(3:n-1)]/y_max;
phi_4 = [y(1) ; y(2); y(3:n-2)]/y_max;
PHI =   [phi_1 , phi_2 , phi_3 , phi_4];
end
