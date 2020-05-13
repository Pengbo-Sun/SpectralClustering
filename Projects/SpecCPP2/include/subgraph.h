#ifndef SUBGRAPH_H
#define SUBGRAPH_H

#include <armadillo>
#include <iostream>

namespace SubGraph
{
class SubGraph
{
public:
  SubGraph(unsigned int size, double minCost);
  ~SubGraph();
  void addweight(unsigned int indexFirst,unsigned int indexSecond, double weight);
  void calcMatrices();
  void calcEigen();

  void findSplit();
  void findBestSplit(std::vector<unsigned int> allIndexxes, unsigned int eigenValueNumber);

  double splitCosts(std::vector<unsigned int> lowerIndexxes, std::vector<unsigned int> upperIndexxes);
  void printClasses();

 // bool compareValue(std::pair<unsigned int, double> &a, std::pair<unsigned int, double> &b);

  std::vector<unsigned int> originalIndex;
  std::vector<unsigned int> classIds;

  arma::mat weights;
  arma::mat degree;
  arma::mat laplace;
  arma::mat eigenvector;

  unsigned int elements;
  unsigned int eigenValueNumber;

  unsigned int DEBUG;
  unsigned int classAddOn;

  double minCosts;
};

} // namespace SubGraph

#endif // SUBGRAPH_H
