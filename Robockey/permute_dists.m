%% permutations

function permutation = permute_dists(assign, X_i)

    x1 = X_i(1);
    x2 = X_i(2);
    x3 = X_i(3);
    x4 = X_i(4);
    y1 = X_i(5);
    y2 = X_i(6);
    y3 = X_i(7);
    y4 = X_i(8);

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

 permutation = [possible_assignment_x(assign,:) possible_assignment_y(assign,:)];
  
end
