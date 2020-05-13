#include <iostream>
#include <scenegraph.h>

int main(int argc, char **argv)
{
    std::cout << "Start the Program" << std::endl;

    SceneGraph wholegraph;
    wholegraph.readin("/home/sunp/Schreibtisch/SpectralClustering-master/Examples/0/all.txt");
    wholegraph.convert2graph();
    wholegraph.getsubgraphs();
    wholegraph.getbinsk();
    wholegraph.getmatrix();
    wholegraph.calceigen();
    wholegraph.getthefirstgroup();
    for (int i = 0; i < wholegraph.Laplace.size(); i++)
    {
        std::cout << "Group :" << i << std::endl;
        int eig_number;
        //choose the number of eigenvectors to consider
        if (wholegraph.Laplace[i].n_rows > 4)
        {
            eig_number = 2;
        }
        else
        {
            eig_number = 1;
        }
        wholegraph.calcgroups(wholegraph.Eigenvector[i], wholegraph.Subgraph[i], eig_number);
    }
    wholegraph.savegroups("/home/sunp/Schreibtisch/SpectralClustering-master/Examples/0/list.txt");
}
