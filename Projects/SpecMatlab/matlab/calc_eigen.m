function [V, U] = calc_eigen(tmp_graph)

nn = numnodes(tmp_graph);
[s,t] = findedge(tmp_graph);
W = sparse(s,t,tmp_graph.Edges.Weight,nn,nn);
W_Weights = full(W);
W_Weights = 0.5*(W_Weights+W_Weights');

% Degree Matrix
Degree = zeros(length(W_Weights),length(W_Weights));
Reihensumme = sum(W_Weights,2);
for i=1:length(Degree)
    Degree(i,i) = Reihensumme(i);
end

Laplacian = Degree - W_Weights;
allnan = Laplacian(isnan(Laplacian)==1);
if (length(allnan) > 0)
    display('error')
    Laplacian = 0;
end
if Laplacian == 0
    [V, U] = eig(Laplacian);
else
    %Degree(Degree == 0) = eps;
    % Falls normalisierte Matrix
    % calculate D^(-1/2)
    %D = spdiags(1./(Degree.^0.5), 0, size(Degree, 1), size(Degree, 2));
    %Laplacian = D * Laplacian * D;
    
    [V, U] = eig(Laplacian);
end

end