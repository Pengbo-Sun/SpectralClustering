function percent = calc_cut(whole_graph,lower,upper)

% Berechnung der einzelnen Graphen und Untergrpahen
graph_whole = subgraph(whole_graph,[lower upper]);
graph_lower = subgraph(whole_graph,lower);
graph_upper = subgraph(whole_graph,upper);

% Ermittlung der Schnittkosten
weight = graph_whole.Edges{:,2};
weight_whole = sum(weight);
number_whole = length(weight);
weight = graph_lower.Edges{:,2};
weight_lower = sum(weight);
number_lower = length(weight);
weight = graph_upper.Edges{:,2};
weight_upper = sum(weight);
number_upper = length(weight);

weight_subgraphs = weight_lower + weight_upper;
number_cuts = number_whole-number_lower-number_upper;
percent = (weight_whole - weight_subgraphs) / number_cuts; 

end