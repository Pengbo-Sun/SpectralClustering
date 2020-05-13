#include <iostream>
#include <scenegraph.h>

#include <chrono>  // for high_resolution_clock

int main(int argc, char **argv)
{
    std::cout << "Start the Program" << std::endl;

    SceneGraph wholegraph;
    wholegraph.readin("all.txt");
    auto start = std::chrono::high_resolution_clock::now();
    wholegraph.convert2graph();
    wholegraph.getsubgraphs();
    wholegraph.calcAllEigen();
    wholegraph.getClasses();
    wholegraph.saveToFile("list.txt");
    auto finish = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time all: " << elapsed.count() << " s\n";
}