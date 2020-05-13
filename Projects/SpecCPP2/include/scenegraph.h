#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include <iostream>
#include <string.h>
#include <misc.h>
#include <subgraph.h>


#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/connected_components.hpp>
#include <armadillo>

typedef boost::property<boost::edge_weight_t, double> weights;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, weights, boost::property<boost::edge_index_t, std::size_t>> Graph;

class SceneGraph
{
public:
  SceneGraph();
  ~SceneGraph();
  void readin(std::string filename);
  void convert2graph();
  void getsubgraphs();
  void calcAllEigen();
  void getClasses();

  void saveToFile(std::string filename);

  std::vector<std::pair<Misc::input_edge, weights>> input;

  Graph global_graph;

  int total_subgraphs;
  std::vector<SubGraph::SubGraph> SubGraphs;

  std::vector<unsigned int> classNumberAll;

  unsigned int indexMax;

};

#endif // SCENE_GRAPH_H
