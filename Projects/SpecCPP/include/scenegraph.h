#include <iostream>
#include <string.h>
#include <misc.h>
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
  void getmatrix();
  void getbinsk();
  void calceigen();
  void getthefirstgroup();
  void calcgroups(arma::mat vec, Graph subgraph, int eig_number);
  void savegroups(std::string filename);

  std::vector<int> binsp;
  std::vector<int> binsk;
  std::vector<arma::mat> Laplace;
  std::vector<arma::mat> Eigenvector;
  std::vector<std::vector<int>> GruppePunkt;
  std::vector<Graph> Subgraph;
  std::vector<Graph> result;
  int NUM;
  double percent_main = 0.7;

  std::vector<std::pair<edge, weights>> input;

  Graph g;
};
