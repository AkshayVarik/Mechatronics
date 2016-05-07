A = [0 767*14.5/120];
B = [1023*11.655/230 767*8.741/120];
C = [0 -767*14.5/120];
D = [-1023*10.563/230 767*2.483/120];

AB = pdist2(A,B);
AC = pdist2(A,C);
AD = pdist2(A,D);

% clc
figure(1)
axis equal;
axis([-512 512 -384 384]);
hold on

R = rawStarData;

for k = 1:length(R)
o = 1;
    for o = 1:4
    if (R(k,o) ~= 1023 && R(k,o+4) ~= 1023)
        
   

w = [R(k,1) R(k,5)];
x = [R(k,2) R(k,6)];
y = [R(k,3) R(k,7)];
z = [R(k,4) R(k,8)];

wx = pdist2(w,x);
wy = pdist2(w,y);
wz = pdist2(w,z);
xy = pdist2(y,x);
xz = pdist2(z,x);
yz = pdist2(y,z);

i = [abs(AC-wx) abs(AC-wy) abs(AC-wz) abs(AC-xy) abs(AC-xz) abs(AC-yz)];
[L,a] = min(i);

j = [abs(AB-wx) abs(AB-wy) abs(AB-wz) abs(AB-xy) abs(AB-xz) abs(AB-yz)];
[M,b] = min(j);

if a == 1 && b == 2
    order = [w;y;x;z];
elseif a == 1 && b == 3
    order = [w;z;x;y];
elseif a == 1 && b == 4
    order = [x;y;w;z];
elseif a == 1 && b == 5
    order = [x;z;w;y];
elseif a == 1 && b == 6
    order = [w;z;x;y];
elseif a == 2 && b == 1
    order = [w;x;y;z];
elseif a == 2 && b == 3
    order = [w;z;y;x];
elseif a == 2 && b == 4
    order = [y;z;w;z];
elseif a == 2 && b == 6
    order = [y;z;w;x];
    elseif a == 3 && b == 1
    order = [w;x;z;y];
elseif a == 3 && b == 2
    order = [w;y;z;x];
elseif a == 3 && b == 5
    order = [z;x;w;y];
elseif a == 3 && b == 6
    order = [z;y;w;x];
    elseif a == 4 && b == 1
    order = [x;w;y;z];
elseif a == 4 && b == 2
    order = [y;w;x;z];
elseif a == 4 && b == 5
    order = [x;z;y;w];
elseif a == 4 && b == 6
    order = [y;z;x;w];
       elseif a == 5 && b == 1
    order = [x;w;z;y];
elseif a == 5 && b == 3
    order = [z;w;x;y];
elseif a == 5 && b == 4
    order = [x;y;z;w];
elseif a == 5 && b == 6
    order = [z;y;x;w];
       elseif a == 6 && b == 2
    order = [y;w;z;x];
elseif a == 6 && b == 3
    order = [z;w;y;x];
elseif a == 6 && b == 4
    order = [y;x;z;w];
elseif a == 6 && b == 5
    order = [z;x;y;w];    
end

% if R(o) == 0

% k = [abs(AD-wx) abs(AD-wy) abs(AD-wz) abs(AD-xy) abs(AD-xz) abs(AD-yz)];
% [N,c] = min(k);
% 
% if a == 1 && c == 2
%     order_2 = [w;x;y;z]
% elseif a == 1 && c == 3
%     order_2 = [w;x;z;y]
% elseif a == 1 && c == 4
%     order_2 = [x;w;y;z]
% elseif a == 1 && c == 5
%     order_2 = [x;w;z;y]
% elseif a == 1 && c == 6
%     order_2 = [w;x;z;y]
% elseif a == 2 && c == 1
%     order_2 = [w;y;x;z]
% elseif a == 2 && c == 3
%     order_2 = [w;y;z;x]
% elseif a == 2 && c == 4
%     order_2 = [y;w;x;z]
% elseif a == 2 && c == 6
%     order_2 = [y;w;z;x]
%     elseif a == 3 && c == 1
%     order_2 = [w;z;x;y]
% elseif a == 3 && c == 2
%     order_2 = [w;z;y;x]
% elseif a == 3 && c == 5
%     order_2 = [z;w;x;y]
% elseif a == 3 && c == 6
%     order_2 = [z;w;y;x]
%     elseif a == 4 && c == 1
%     order_2 = [x;y;w;z]
% elseif a == 4 && c == 2
%     order_2 = [y;x;w;z]
% elseif a == 4 && c == 5
%     order_2 = [x;y;z;w]
% elseif a == 4 && c == 6
%     order_2 = [y;x;z;w]
%        elseif a == 5 && c == 1
%     order_2 = [x;z;w;y]
% elseif a == 5 && c == 3
%     order_2 = [z;x;w;y]
% elseif a == 5 && c == 4
%     order_2 = [x;z;y;w]
% elseif a == 5 && c == 6
%     order_2 = [z;x;y;w]
%        elseif a == 6 && c == 2
%     order_2 = [y;z;w;x]
% elseif a == 6 && c == 3
%     order_2 = [z;y;w;x]
% elseif a == 6 && c == 4
%     order_2 = [y;z;x;w]
% elseif a == 6 && c == 5
%     order_2 = [z;y;x;w]  
% end
% 
% if order_2 == order
%     
% robot_position = [(order(1)+order(3))/2 (order(5)+order(7))/2 0]' ;
% O = [1 1 1]';
% origin = [0 0 0 1]';
% 
% X = (order(1)+order(3))/2;
% Y = (order(5)+order(7))/2;
% 
% t = pi/2 - atan2((order(1,2)-order(3,2)),(order(1,1)-order(3,1)));
% 
% % R_01 = dh_kuchenbe(X,0,0,t);
% % p = R_01*origin;
% x_cam = 512;
% y_cam = 384;
% 
% x = X-x_cam;
% y = Y-y_cam;
% 
% R_01 = [cos(t) -sin(t); sin(t) cos(t)];
% p_01 = R_01*[x y]';
% 
% % axis equal;
% % axis([-512 512 -384 384]);
% 
% % p_01 = p(1:3,1);
% 
% figure(1)
% scatter(p_01(1), p_01(2))
% hold on
% else
%     
%     robot_position = [(order(1)+order(3))/2 (order(5)+order(7))/2 0]' ;
% O = [1 1 1]';
% origin = [0 0 0 1]';
% 
% X = (order(1)+order(3))/2;
% Y = (order(5)+order(7))/2;
% 
% t = pi/2 - atan2((order(1,2)-order(3,2)),(order(1,1)-order(3,1)));
% 
% % R_01 = dh_kuchenbe(X,0,0,t);
% % p = R_01*origin;
% x_cam = 512;
% y_cam = 384;
% 
% x = X-x_cam;
% y = Y-y_cam;
% 
% R_01 = [cos(t) -sin(t); sin(t) cos(t)];
% p_01 = R_01*[x y]';
% 
% % axis equal;
% % axis([-512 512 -384 384]);
% 
% figure(1)
% scatter(p_01(1), p_01(2))
% hold on
%     
% end
% 
% else 
robot_position = [(order(1)+order(3))/2 (order(5)+order(7))/2 0]' ;
O = [1 1 1]';
origin = [0 0 0 1]';

X = (order(1)+order(3))/2;
Y = (order(5)+order(7))/2;

t = pi/2 - atan2((order(1,2)-order(3,2)),(order(1,1)-order(3,1)));

% R_01 = dh_kuchenbe(X,0,0,t);
% p = R_01*origin;
x_cam = 512;
y_cam = 384;

x = X-x_cam;
y = Y-y_cam;

R_01 = [cos(t) -sin(t); sin(t) cos(t)];
p_01 = R_01*[x y]';

% axis equal;
% axis([-512 512 -384 384]);

% if p_01(1)>100

figure(1)
scatter(p_01(1), p_01(2))
line([p_01(1) p_01(1)+50*cos(t)], [p_01(2) p_01(2)+50*sin(t)])
hold on

% end
    end
    end
end