#include <iostream>
#include <scenegraph.h>
#include <misc.h>

#include <boost/graph/graph_traits.hpp>

SceneGraph::SceneGraph() = default;
SceneGraph::~SceneGraph() = default;

void SceneGraph::readin(std::string filename)
{
    std::cout << "loading file  " << filename << std::endl;
    std::string charactersFilename(filename);
    std::ifstream file;
    file.open(charactersFilename);

    char delimiter[] = ",";
    char *ptr;

    std::string fileline;
    Misc::input_edge tmp_edge;
    // while not end -> read
    while (1)
    {
        getline(file, fileline); // read a line of strings
        if (file.eof())
            break;

        //std::cout << fileline << std::endl; // display value removing the first and the last character from it
        ptr = strtok(const_cast<char *>(fileline.c_str()), delimiter);
        // convert to int
        tmp_edge.begin = atoi(ptr);
        ptr = strtok(NULL, delimiter);
        // convert to int
        tmp_edge.end = atoi(ptr);
        ptr = strtok(NULL, delimiter);
        // convert to double
        double weight = atof(ptr);

        //std::cout << "End:  " << begin << "  " << end << "  " << weight << std::endl;

        input.push_back(std::make_pair(tmp_edge, weight));
    }
}
void SceneGraph::convert2graph()
{
    for (auto &elements : input)
    {
        // add edge between vertices begin and end with weight second into graph g
        boost::add_edge(elements.first.begin, elements.first.end, elements.second, global_graph); //add edges
    }
}

void SceneGraph::getsubgraphs() //get the connected components
{
    std::vector<int> component(num_vertices(global_graph));
    total_subgraphs = boost::connected_components(global_graph, &component[0]);

    SubGraphs.reserve(total_subgraphs);

    //std::cout << "Begin Subgraph Size is: " << SubGraphs.size() << std::endl;

    unsigned int index_component = 0;
    for (; index_component < total_subgraphs; index_component++)
    {
        unsigned int occurrences = std::count(component.begin(), component.end(), index_component);
        //std::cout << " Using index number " << index_component << " and they occur " << occurrences << std::endl;
        SubGraph::SubGraph subgraph_tmp(occurrences, 0.7);

        Graph::vertex_iterator vertexIt, vertexEnd;
        Graph::adjacency_iterator neighbourIt, neighbourEnd;
        boost::tie(vertexIt, vertexEnd) = boost::vertices(global_graph);
        indexMax = *vertexEnd;
        for (; vertexIt != vertexEnd; ++vertexIt)
        {
            if (component[*vertexIt] == index_component)
            {
                //std::cout << *vertexIt << " is connected with ";
                boost::tie(neighbourIt, neighbourEnd) = boost::adjacent_vertices(*vertexIt, global_graph);
                for (; neighbourIt != neighbourEnd; ++neighbourIt)
                {
                    //std::cout << *neighbourIt << " ";
                    //std::cout << "\n";
                    auto ed = boost::edge(*vertexIt, *neighbourIt, global_graph);
                    double weight = get(boost::edge_weight_t(), global_graph, ed.first);
                    //std::cout << "Weight " << weight << std::endl;
                    subgraph_tmp.addweight((*vertexIt), (*neighbourIt), weight);
                }
            }
        }

        SubGraphs.push_back(subgraph_tmp);
    }
}
void SceneGraph::calcAllEigen()
{
    for (auto &i : SubGraphs)
    {
        i.calcMatrices();
        i.calcEigen();
        i.findSplit();
        //i.printClasses();
    }
}

void SceneGraph::getClasses()
{
    std::vector<unsigned int> classnumber(indexMax);
    //std::iota(classnumber.begin(), classnumber.end(), 1);
    unsigned int global_class = 1;
    for (auto &i : SubGraphs)
    {
        auto &indexxes = i.originalIndex;
        auto &classes = i.classIds;

        unsigned int maxClass = 0;

        auto classUnique = classes;

        for (unsigned int j = 0; j < indexxes.size(); j++)
        {
            classnumber[indexxes[j]] = classes[j] + global_class;
            if (classes[j] > maxClass)
            {
                maxClass = classes[j];
            }
        }
        //TODO:
        global_class = global_class + maxClass;
    }
    classNumberAll = classnumber;
}

void SceneGraph::saveToFile(std::string filename)
{
    std::ofstream endfile;
    endfile.open(filename.c_str());

    //remove the first element of the vector (its a zero that doesn't appear in the supervoxels)
    classNumberAll.erase(classNumberAll.begin());

    for (auto &i : classNumberAll)
    {
        endfile << i + 1 << std::endl;
    }

    endfile.close();
}
