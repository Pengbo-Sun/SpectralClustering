function [Index_value, Klasse_number] = get_class(vox_index,eigen_vecs,eig_number,whole_graph,row_number)

global class_counter;
global percent_sub;
if eig_number < length(eigen_vecs)
    % Ermittlung Schnittpunkt
    sort_eig_werte = sort(eigen_vecs(:,eig_number));
    differs = diff(sort_eig_werte);
    [max_gap, max_index] = max(differs);

    % Ermittlung der Untergraphen
    U_gr_idx = find(eigen_vecs(:,eig_number)>sort_eig_werte(max_index));
    U_kl_idx = find(eigen_vecs(:,eig_number)<=sort_eig_werte(max_index));
    
    %Pruefung auf Sschnittkosten
    percent = calc_cut(whole_graph,row_number(U_kl_idx'),row_number(U_gr_idx'));
else
    percent = 1;
end

%if (eig_number==9)
%    percent = 1;
%    
%end

% Entweder weitere Berechnungen (mit hoeheren Eigenwerten)
% oder Untergraphen zu einer Klasse zusammenfuegen
if (percent <= percent_sub)
    
    [Index_kl, Klasse_kl] = get_class(vox_index,eigen_vecs(U_kl_idx,:), ...
        eig_number+1,whole_graph,row_number(U_kl_idx));
    [Index_gr, Klasse_gr] = get_class(vox_index,eigen_vecs(U_gr_idx,:),...
        eig_number+1,whole_graph,row_number(U_gr_idx));
    Index_value = [Index_kl Index_gr];
    Klasse_number = [Klasse_kl Klasse_gr];
else
    Index_value = vox_index(row_number);
    Klasse_number = zeros(size(row_number));
    Klasse_number(:) = class_counter;
    class_counter = class_counter +1;
end

end
