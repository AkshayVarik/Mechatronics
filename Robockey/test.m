%% test

[L_i, ids] = loss(X(300,:))

possible_assignment = [1,2,3,4; 2,1,3,4; 3,2,1,4; 4,2,3,1;
1,2,4,3; 2,1,4,3; 3,2,4,1; 4,2,1,3;
1,3,2,4; 2,3,1,4; 3,1,2,4; 4,3,2,1;
1,3,4,2; 2,3,4,1; 3,1,4,2; 4,3,1,2;
1,4,2,3; 2,4,1,3; 3,4,2,1; 4,1,2,3;
1,4,3,2; 2,4,3,1; 3,4,1,2; 4,1,3,2];

assign = possible_assignment(ids,:)


c = [50, 100, 150, 200];
figure;
scatter(X(300,1:4),X(300,5:8),[],c);

x1 = X(300,1);
x2 = X(300,2);
x3 = X(300,3);
x4 = X(300,4);
y1 = X(300,5);
y2 = X(300,6);
y3 = X(300,7);
y4 = X(300,8);


 possible_assignment_x = [x1,x2,x3,x4; x2,x1,x3,x4; x3,x2,x1,x4; x4,x2,x3,x1;
x1,x2,x4,x3; x2,x1,x4,x3; x3,x2,x4,x1; x4,x2,x1,x3;
x1,x3,x2,x4; x2,x3,x1,x4; x3,x1,x2,x4; x4,x3,x2,x1;
x1,x3,x4,x2; x2,x3,x4,x1; x3,x1,x4,x2; x4,x3,x1,x2;
x1,x4,x2,x3; x2,x4,x1,x3; x3,x4,x2,x1; x4,x1,x2,x3;
x1,x4,x3,x2; x2,x4,x3,x1; x3,x4,x1,x2; x4,x1,x3,x2];

     possible_assignment_y = [y1,y2,y3,y4; y2,y1,y3,y4; y3,y2,y1,y4; y4,y2,y3,y1;
y1,y2,y4,y3; y2,y1,y4,y3; y3,y2,y4,y1; y4,y2,y1,y3;
y1,y3,y2,y4; y2,y3,y1,y4; y3,y1,y2,y4; y4,y3,y2,y1;
y1,y3,y4,y2; y2,y3,y4,y1; y3,y1,y4,y2; y4,y3,y1,y2;
y1,y4,y2,y3; y2,y4,y1,y3; y3,y4,y2,y1; y4,y1,y2,y3;
y1,y4,y3,y2; y2,y4,y3,y1; y3,y4,y1,y2; y4,y1,y3,y2];

 permutation = [possible_assignment_x(ids,:) possible_assignment_y(ids,:)];

figure;
scatter(permutation(1:4),permutation(5:8),[],c);

