function p = extract(path, percentage_main)
%function p = extract(tabelle)
%extract graphs
global class_counter;
class_counter = 2;

global percent_main;% 0.70;
%percent_main = 0.7;
%percent_main = str2double(percentage_main);
percent_main = percentage_main;
display(percent_main);

global percent_sub;
%percent_sub = 0.15;

percent_sub = percent_main;
%disp('Load data');

%path_list = '/weight.txt';
%path_list = strcat(path,path_list);
%weights = load_weight(path_list);
%path_list = '/neigh.txt';
%path_list = strcat(path,path_list);
%voxels = load_neigh(path_list);

%disp(path);
%disp('Calc graph entries');
%tic;
%G = calc_graph(voxels, weights);
%toc;
tic;
% disp('Calc graph entries second');
% 
path_list = '/all.txt';
path_list = strcat(path,path_list);
display(path_list);
tabelle = csvread(path_list);
%tabelle = tabelle.';
%display(tabelle);
EdgeTable = table(tabelle(:,1:2), ...
    tabelle(:,3),...
    'VariableNames',{'EndNodes','Weight'});
G = graph(EdgeTable);
toc;
%LWidths = 5*G.Edges.Weight/max(G.Edges.Weight);
%plot(G,'EdgeLabel',G.Edges.Weight,'LineWidth',LWidths)

disp('Get subgraphs');

% Extrahieren der Untergraphen
bins = conncomp(G);
count_g = max(bins);
graph_cell = cell(count_g,6);

disp('Eigen calculation');

for i=1:count_g
    graph_cell{i,1} = subgraph(G,find(bins==i));
    [graph_cell{i,3},graph_cell{i,2}] = find(bins==i);
    [graph_cell{i,4}, graph_cell{i,5}] = calc_eigen(graph_cell{i,1});
    graph_cell{i,6} = 1:length(graph_cell{i,2});
end

% Iteration durch alle Untergraphen
disp('get clusters');
tic;
for count=1:size(graph_cell,1)
    % Auswahl des Eigenvekotors (zu kleine Graphen besitzen keine h??heren
    % Eigenbvektoren
    if (length(graph_cell{count,4}) > 4)
        eig_number = 2;
    else
        eig_number = 1;
    end
    if (length(graph_cell{count,4})> 2)
        [sor_eig_werte, In] = sort(graph_cell{count,4}(:,eig_number));
        
        % Ermittlung des Schnittpunktes
        differs = diff(sor_eig_werte);
        [max_gap, max_index] = max(differs);
        
        % Ermittlung der Untergraphen
        U_1_gr_idx = find(graph_cell{count,4}(:,eig_number) ...
        >sor_eig_werte(max_index));
        U_1_kl_idx = find(graph_cell{count,4}(:,eig_number) ...
            <=sor_eig_werte(max_index));
        
        % Berechnung der Schnittkosten
        percent = calc_cut(graph_cell{count,1},U_1_kl_idx',U_1_gr_idx');
        
        
        if (percent <= percent_main)
            
            % Rekursive Ermittlung weiterer Untergraphen mit h??heren
            % Eigenwerten
            [Index_kl, KLasse_kl] = get_class(graph_cell{count,2}, ...
                graph_cell{count,4}(U_1_kl_idx,:),eig_number+1, ...
                graph_cell{count,1},graph_cell{count,6}(U_1_kl_idx));
            [Index_gr, KLasse_gr] = get_class(graph_cell{count,2}, ....
                graph_cell{count,4}(U_1_gr_idx,:),eig_number+1, ....
                graph_cell{count,1},graph_cell{count,6}(U_1_gr_idx));
            indexxe = [Index_kl Index_gr; KLasse_kl KLasse_gr]';
        else
            Index_value = graph_cell{count,2};
            Klasse_number = zeros(size(Index_value));
            Klasse_number(:) = class_counter;
            class_counter = class_counter +1;
            indexxe = [graph_cell{count,2}' Klasse_number'];
        end
        
    else
        Index_value = graph_cell{count,2};
        Klasse_number = zeros(size(Index_value));
        Klasse_number(:) = 1;
        indexxe = [graph_cell{count,2}' Klasse_number'];
    end
    
    
    cluster_combined = sortrows(indexxe,1);
    graph_cell{count,3} = cluster_combined(:,2)';
end
toc;
tic;
% Zusammenfassung der ermittelten Regionen
tmp = [graph_cell{1,2}' graph_cell{1,3}' ];
%tmp_number = max(graph_cell{1,3});

for i=2:count_g
    
    tmp = [tmp; [graph_cell{i,2}' [graph_cell{i,3}]']];
    %tmp_number = max(tmp(:,2));
end

cluster = sortrows(tmp,1);
disp('Save them');
%Abspeichern der Cluster
p = cluster(:,2);

path_list = '/list.txt';
path_list = strcat(path,path_list);
%display(path_list);
%path_list = 'list.txt';
fid = fopen(path_list,'w');
fprintf(fid,'%d \n',p);
fclose(fid);
 
tmp_class_counter = class_counter;
toc;

end
