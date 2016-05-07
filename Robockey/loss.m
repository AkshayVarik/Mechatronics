%% loss

function [L, min_assignment] = loss(X_i)

AB = 13;
AC = 29;
AD = 16;
BC = 26;
BD = 23.0825;
CD = 20;

actual_dists = [AB AC AD BC BD CD];
actual_ratios = zeros(1, 15);
ratios = zeros(1, 15);
act_count = 0;

for i = 1:6
    for j = 1:6
        if i<j
            act_count = act_count + 1;
            i_over_j = actual_dists(i)/actual_dists(j);
            actual_ratios(act_count) = i_over_j;
        end
    end
end


min_score = 1000000000;
for k = 1:24
    
    permutation = permute_dists(k, X_i);
    
    x1 = permutation(1);
    x2 = permutation(2);
    x3 = permutation(3);
    x4 = permutation(4);
    y1 = permutation(5);
    y2 = permutation(6);
    y3 = permutation(7);
    y4 = permutation(8);

    dist_12 = distance(x1, y1, x2, y2);
    dist_13 = distance(x1, y1, x3, y3);
    dist_14 = distance(x1, y1, x4, y4);
    dist_23 = distance(x2, y2, x3, y3);
    dist_24 = distance(x2, y2, x4, y4);
    dist_34 = distance(x3, y3, x4, y4);
    
   distances = [dist_12 dist_13 dist_14 dist_23 dist_24 dist_34];
    
   count = 0;
    for i = 1:6
        for j = 1:6
            if i<j
                count = count + 1;
                i_over_j = distances(i)/distances(j);
                ratios(count) = i_over_j;
            end
        end
    end

    score = assignment_score(actual_ratios, ratios);
    if score < min_score
        min_score = min(min_score, score);
        min_assignment = k;
    end

end
   
L = min_score;

end