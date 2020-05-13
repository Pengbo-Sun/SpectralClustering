#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <scenegraph.h>

#include <armadillo>

SceneGraph::SceneGraph() = default;
SceneGraph::~SceneGraph() = default;

void SceneGraph::readin(std::string filename) {
  std::cout << "loading file  " << filename << std::endl;
  std::string charactersFilename(filename);
  std::ifstream file;
  file.open(charactersFilename);

  char delimiter[] = ",";
  char *ptr;

  std::string fileline;
  edge tmp_edge;
  // while not end -> read
  while (1) {
    getline(file, fileline); // read a line of strings
    if (file.eof())
      break;

    // std::cout << fileline << std::endl; // display value removing the first
    // and the last character from it
    ptr = strtok(const_cast<char *>(fileline.c_str()), delimiter);
    // convert to int
    tmp_edge.begin = atoi(ptr);
    ptr = strtok(NULL, delimiter);
    // convert to int
    tmp_edge.end = atoi(ptr);
    ptr = strtok(NULL, delimiter);
    // convert to double
    double weight = atof(ptr);

    // std::cout << "End:  " << begin << "  " << end << "  " << weight <<
    // std::endl;

    input.push_back(std::make_pair(tmp_edge, weight));
  }
}
void SceneGraph::convert2graph() {
  for (auto &elements : input) {
    // add edge between vertices begin and end with weight second into graph g
    boost::add_edge(elements.first.begin, elements.first.end, elements.second,
                    g); // add edges
  }
}

void SceneGraph::getsubgraphs() // get the connected components
{
  std::vector<int> component(num_vertices(g));
  int num = connected_components(g, &component[0]);
  std::vector<int>::size_type i;
  std::cout << "Total number of components: " << num << std::endl;
  NUM = num - 1;
  for (i = 1; i != component.size(); ++i) {
    binsp.push_back(component[i]);
    std::cout << "Vertex " << i << " is in component " << component[i]
              << std::endl;
  }
  std::cout << std::endl;
}

void SceneGraph::getbinsk() { // get the bins_list of vertex

  for (int i = 0; i < num_edges(g); i++) // initialisation binslist zero
  {
    binsk.push_back(0);
  }
  typedef boost::graph_traits<Graph>::edge_iterator edge_iter;
  std::pair<edge_iter, edge_iter> es = boost::edges(g);
  typedef boost::graph_traits<Graph>::vertex_iterator vertex_iter;
  std::pair<vertex_iter, vertex_iter> vs = boost::vertices(g);

  typedef boost::graph_traits<Graph>::out_edge_iterator out_edge_iter;
  boost::graph_traits<Graph>::vertex_iterator vertexIterator, vend;

  typedef boost::graph_traits<Graph>::in_edge_iterator in_edge_iter;
  int z = 0;
  for (vs.first = vs.first + 1; vs.first != vs.second - 1;
       ++vs.first) // vs.first:the first egde,vs.second:the last edge
                   // count from the first edege until the last
  {
    std::pair<out_edge_iter, out_edge_iter> outEdges =
        boost::out_edges(*vs.first, g);
    for (; outEdges.first != outEdges.second; ++outEdges.first) {
      int count = 0;
      // count serves for index
      for (es = boost::edges(g); *es.first != *outEdges.first; ++es.first) {
        count++;
      }
      binsk[count] = binsp[z];
    }
    z++;
  }
}
void SceneGraph::getmatrix() {
  // find the subgraph
  for (int i = 1; i <= NUM; i++) {
    bool exist = false;
    for (int j : binsk) {
      if (j == i) {
        exist = true;
        break;
      }
    }
    if (exist) {
      // make matrix with Graph
      Graph subgraph;
      for (int p = 0; p < num_edges(g); p++) {
        if (binsk[p] == i) {
          int count = 0;
          typedef boost::graph_traits<Graph>::edge_iterator edge_iter;
          std::pair<edge_iter, edge_iter> es = boost::edges(g);
          for (; es.first != es.second; ++es.first) {
            if (count == p) {
              boost::property_map<Graph, boost::edge_weight_t>::type
                  EdgeWeightMap = get(boost::edge_weight_t(), g);
              // add edge as well as weight
              add_edge(source(*es.first, g), target(*es.first, g),
                       EdgeWeightMap[*es.first], subgraph);
              break;
            }
            count++;
          }
        }
      }
      Subgraph.push_back(subgraph);
      // punkt
      // find all Vertex form the xth grop
      std::set<int> Punkt;
      typedef boost::graph_traits<Graph>::edge_iterator edge_iter;
      std::pair<edge_iter, edge_iter> es = boost::edges(subgraph);
      for (; es.first != es.second; ++es.first) {
        Punkt.insert(source(*es.first, subgraph));
        Punkt.insert(target(*es.first, subgraph));
      }
      // generate weight matrix
      boost::property_map<Graph, boost::edge_weight_t>::type EdgeWeightMap =
          get(boost::edge_weight_t(), subgraph);
      unsigned int size = Punkt.size();
      arma::mat m(size, size);
      m.zeros();
      es = boost::edges(subgraph);
      for (; es.first != es.second; ++es.first) {
        int zeile, spalte;
        int count = 0;
        for (auto it = Punkt.cbegin();
             it != Punkt.find(source(*es.first, subgraph)); ++it) {
          count++;
        }
        zeile = count;
        count = 0;
        for (auto it = Punkt.cbegin();
             it != Punkt.find(target(*es.first, subgraph)); ++it) {
          count++;
        }
        spalte = count;
        m(zeile, spalte) = m(spalte, zeile) = EdgeWeightMap[*es.first];
      }
      arma::mat Deg(size, size);
      Deg.zeros();
      arma::mat Lapla(size, size);
      Lapla.zeros();
      // calc normalised laplacian
      arma::vec summe(size);
      summe = arma::sum(m, 1); // sum of each row
      Deg.diag() = summe;
      // Lmatrix
      Lapla = Deg - m;

      Laplace.push_back(Lapla);
    }
  }
}
void SceneGraph::calceigen() {
  for (int i = 0; i < Laplace.size(); i++) {
    arma::mat A = Laplace[i];
    arma::vec eigval;
    arma::mat eigvec;
    arma::eig_sym(eigval, eigvec, A);
     for (auto i=0;i<eigval.n_rows;i++)
    {
        std::cout <<eigval(i) << std::endl;
    }
    Eigenvector.push_back(eigvec);
  }
}

void SceneGraph::getthefirstgroup() {
  std::set<int> Punkt; // make the vertexlist of g(whole graph)
  std::vector<int> firstgoup;
  typedef boost::graph_traits<Graph>::edge_iterator edge_iter;
  std::pair<edge_iter, edge_iter> es = boost::edges(g);
  for (; es.first != es.second; ++es.first) {
    Punkt.insert(source(*es.first, g));
    Punkt.insert(target(*es.first, g));
  }
  typedef boost::graph_traits<Graph>::vertex_iterator vertex_iter;
  std::pair<vertex_iter, vertex_iter> vs = boost::vertices(g);
  for (vs.first = ++vs.first; vs.first != vs.second; ++vs.first) {
    if (Punkt.find(*vs.first) == Punkt.end()) {
      firstgoup.push_back(*vs.first);
    }
  }
  GruppePunkt.push_back(firstgoup);
}

void SceneGraph::calcgroups(
    arma::mat vec, Graph subgraph,
    int eig_number) { // find the vertex and store in the Punkt with Datatyp
                      // set(unique and sorted)
  std::set<int> Punkt;
  typedef boost::graph_traits<Graph>::edge_iterator edge_iter;
  std::pair<edge_iter, edge_iter> es = boost::edges(subgraph);
  for (; es.first != es.second; ++es.first) {
    Punkt.insert(source(*es.first, subgraph));
    Punkt.insert(target(*es.first, subgraph));
  }
  int numDims = Punkt.size();
  std::vector<std::pair<double, int>> sor_eig_werte;
  if (numDims > 2) {
    for (int k = 0; k < numDims; k++) {
      std::pair<double, int> In(vec(k, eig_number - 1), k + 1);
      sor_eig_werte.push_back(In);
    }
    // sort matrix
    std::sort(sor_eig_werte.begin(), sor_eig_werte.end());
    std::cout << " sortierte eigenvector" << std::endl;
    for (int j = 0; j < numDims; j++) {
      std::cout << sor_eig_werte[j].first << " " << sor_eig_werte[j].second
                << std::endl;
    }
    std::vector<std::pair<double, int>> diff;
    // calculate the difference value of the sorted matrix
    for (int q = 0; q < sor_eig_werte.size() - 1; q++) {
      std::pair<double, int> tmp_diff(
          sor_eig_werte[q + 1].first - sor_eig_werte[q].first, q + 1);
      diff.push_back(tmp_diff);
    }
    // find the max
    std::vector<std::pair<double, int>>::const_iterator it;
    it = std::max_element(diff.begin(), diff.end());
    std::cout << " max element is " << it->first << "  at position "
              << it->second << std::endl;
    // get the subgraph
    std::vector<int> U_1_gr_idx, U_1_kl_idx;
    for (int k = 0; k < sor_eig_werte.size(); k++) {
      if (vec(k, eig_number - 1) > sor_eig_werte[(it->second) - 1].first) {
        U_1_gr_idx.push_back(k);
      } else {
        U_1_kl_idx.push_back(k);
      }
    }
    // calculate cutcost
    std::vector<int> Punktvector(Punkt.begin(), Punkt.end());
    std::set<int> Punkt_gross, Punkt_klein;
    for (int j : U_1_gr_idx) {
      Punkt_gross.insert(Punktvector[j]);
    }
    for (int j : U_1_kl_idx) {
      Punkt_klein.insert(Punktvector[j]);
    }
    std::cout << "Punkt klein" << std::endl;
    for (int j : Punkt_klein) {
      std::cout << j << " ";
    }
    std::cout << std::endl << "Punkt gross" << std::endl;
    for (int j : Punkt_gross) {
      std::cout << j << " ";
    }
    std::cout << std::endl;
    Graph subgraphupper, subgraphlower;

    boost::property_map<Graph, boost::edge_weight_t>::type EdgeWeightMap =
        get(boost::edge_weight_t(), subgraph);
    for (es = boost::edges(subgraph); es.first != es.second; ++es.first) {
      if (Punkt_gross.find(target(*es.first, subgraph)) != Punkt_gross.end() &&
          Punkt_gross.find(source(*es.first, subgraph)) != Punkt_gross.end()) {
        add_edge(source(*es.first, subgraph), target(*es.first, subgraph),
                 EdgeWeightMap[*es.first], subgraphupper);
      }
      if (Punkt_klein.find(target(*es.first, subgraph)) != Punkt_klein.end() &&
          Punkt_klein.find(source(*es.first, subgraph)) != Punkt_klein.end()) {
        add_edge(source(*es.first, subgraph), target(*es.first, subgraph),
                 EdgeWeightMap[*es.first], subgraphlower);
      }
    }
    int numbercut = num_edges(subgraph) - num_edges(subgraphlower) -
                    num_edges(subgraphupper);
    double percent;
    if (!numbercut) {
      percent = 0;
    } else {
      double subgraphweight, subgraphupperweight, subgraphlowerweight;
      subgraphweight = subgraphupperweight = subgraphlowerweight = 0;
      es = boost::edges(subgraph);
      EdgeWeightMap = get(boost::edge_weight_t(), subgraph);
      for (; es.first != es.second; ++es.first) {
        subgraphweight = subgraphweight + EdgeWeightMap[*es.first];
      }
      EdgeWeightMap = get(boost::edge_weight_t(), subgraphupper);
      es = boost::edges(subgraphupper);
      for (; es.first != es.second; ++es.first) {
        subgraphupperweight = subgraphupperweight + EdgeWeightMap[*es.first];
      }
      EdgeWeightMap = get(boost::edge_weight_t(), subgraphlower);
      for (es = boost::edges(subgraphlower); es.first != es.second;
           ++es.first) {
        subgraphlowerweight = subgraphlowerweight + EdgeWeightMap[*es.first];
      }
      percent = (subgraphweight - subgraphupperweight - subgraphlowerweight) /
                numbercut;
    }
    std::cout << " Schnittkosten ist  " << percent << std::endl;
    if (U_1_gr_idx.size() == 0 || U_1_kl_idx.size() == 0 ||
        percent > percent_main) // if not give out the result
    {
      // result.push_back(subgraph);
      GruppePunkt.push_back(Punktvector);
    } else // if the cutcost less than 70%,calculate again until more than 70%
    {
      if (U_1_kl_idx.size() == 1) {
        std::vector<int> gruppeklein;
        gruppeklein.push_back(Punktvector[U_1_kl_idx[0]]);
        GruppePunkt.push_back(gruppeklein);
      }
      if (U_1_gr_idx.size() == 1) {
        std::vector<int> gruppegross;
        gruppegross.push_back(Punktvector[U_1_gr_idx[0]]);
        GruppePunkt.push_back(gruppegross);
      }
      arma::mat geteigenvectorkl(U_1_kl_idx.size(), vec.n_cols);
      geteigenvectorkl.zeros();
      arma::mat geteigenvectorgr(U_1_gr_idx.size(), vec.n_cols);
      geteigenvectorgr.zeros();
      // less in a group
      // more in a group
      for (int k = 0; k < U_1_kl_idx.size(); k++) {
        geteigenvectorkl.row(k) = vec.row(U_1_kl_idx[k]);
      }

      for (int k = 0; k < U_1_gr_idx.size(); k++) {
        geteigenvectorgr.row(k) = vec.row(U_1_gr_idx[k]);
      }
      SceneGraph::calcgroups(geteigenvectorkl, subgraphlower, eig_number + 1);
      SceneGraph::calcgroups(geteigenvectorgr, subgraphupper, eig_number + 1);
    }
  } else if (num_edges(subgraph) != 0) {
    // result.push_back(subgraph);
    std::vector<int> Punktvector(Punkt.begin(), Punkt.end());
    GruppePunkt.push_back(Punktvector);
  }
}
void SceneGraph::savegroups(std::string filename) {
  std::ofstream out_data(filename);
  for (int i = 1; i < num_vertices(g); i++) // Output
  {
    for (int j = 0; j < GruppePunkt.size(); j++) {
      std::vector<int>::iterator it;
      it = std::find(GruppePunkt[j].begin(), GruppePunkt[j].end(), i);
      if (it != GruppePunkt[j].end()) {
        out_data << j + 1 << std::endl;
      }
    }
  }
}
