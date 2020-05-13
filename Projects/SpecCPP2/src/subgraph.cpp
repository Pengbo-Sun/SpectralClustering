#include <iostream>
#include <subgraph.h>

#include <algorithm>
#include <numeric>

bool compareValue(std::pair<unsigned int, double> &a, std::pair<unsigned int, double> &b)
{
    return a.second < b.second;
}

namespace SubGraph
{
SubGraph::SubGraph(unsigned int size, double minCost)
{
    elements = size;
    // eigenValueNumber 2 (Fiedler Vector), if enough elements
    if (elements > 4)
        eigenValueNumber = 2;
    else
        eigenValueNumber = 1;

    weights = arma::mat(size, size);
    degree = arma::mat(size, size);
    laplace = arma::mat(size, size);

    weights.zeros();
    degree.zeros();
    laplace.zeros();

    classIds.resize(size);
    std::fill(classIds.begin(), classIds.end(), 1);

    DEBUG = 0;

    minCosts = minCost;

    classAddOn = 0;
};
SubGraph::~SubGraph() = default;

void SubGraph::addweight(unsigned int indexFirst, unsigned int indexSecond, double weight)
{   
    //search the first index
    unsigned int a;
    auto it = std::find(originalIndex.begin(), originalIndex.end(), indexFirst);
    if (it == originalIndex.end())
    {
        // index first not in vector
        originalIndex.push_back(indexFirst);
        a = originalIndex.size();
    }
    else
    {
        auto index = std::distance(originalIndex.begin(), it);
        a = index + 1;
    }

    //search the second index
    unsigned int b;
    auto it2 = std::find(originalIndex.begin(), originalIndex.end(), indexSecond);
    if (it2 == originalIndex.end())
    {
        // index second not in vector
        originalIndex.push_back(indexSecond);
        b = originalIndex.size();
    }
    else
    {
        auto index = std::distance(originalIndex.begin(), it2);
        b = index + 1;
    }

    // add weight
    // -1 as arma starts with a zero
    weights(a - 1, b - 1) = weight;
};

void SubGraph::calcMatrices()
{
    // calc normalised laplacian
    arma::vec summe(elements);

    // sum of each row (traverse along columns)
    summe = arma::sum(weights, 1);
    degree.diag() = summe;
    // Lmatrix
    laplace = degree - weights;
};

void SubGraph::calcEigen()
{
    // calc Eigen with armadillo library
    arma::vec eigval;
    arma::eig_sym(eigval, eigenvector, laplace);

    //TODO: sort eigenvalues -> need to??

    if (DEBUG >= 1)
    {
        std::cout << "Next Eigenvector: " << std::endl;
        //std::cout << eigenvector << std::endl;
        std::cout << "Eigenvalues: " << std::endl;
        std::cout << eigval << std::endl;
    }
};

void SubGraph::findSplit()
{
    if (DEBUG >= 2)
    {
        std::cout << "Related Eigenvector" << std::endl;
        std::cout << eigenvector.col(eigenValueNumber - 1) << std::endl;
    }

    // if too less elemnts, no need of splitting
    if (elements < 2)
    {
        if (DEBUG >= 2)
        {
            std::cout << " To less elements" << std::endl;
        }
        return;
    }

    // get first indexxes
    std::vector<unsigned int> allIndexxes(elements);
    std::iota(allIndexxes.begin(), allIndexxes.end(), 0);

    SubGraph::findBestSplit(allIndexxes, eigenValueNumber);
};

void SubGraph::findBestSplit(std::vector<unsigned int> allIndexxes, unsigned int eigenValueNumberTmp)
{
    if (allIndexxes.size() > 1)
    {
        // iterate through coloumn and push to vector
        // 0 of eigenvector == eigenValueNumber 1
        std::vector<double> tmpVector;
        tmpVector.reserve(eigenvector.n_rows);

        auto it_end = eigenvector.end_col(eigenValueNumberTmp - 1);

        for (auto it = eigenvector.begin_col(eigenValueNumberTmp - 1); it != it_end; ++it)
        {
            tmpVector.push_back(*it);
        }

        std::vector<std::pair<unsigned int, double>> pairs;
        pairs.reserve(allIndexxes.size());

        for (unsigned int index = 0; index < allIndexxes.size(); index++)
        {
            //filteredVector.push_back(tmpVector[index]);
            pairs.push_back(std::make_pair(allIndexxes[index], tmpVector[allIndexxes[index]]));
        }

        // sort by values
        std::sort(pairs.begin(), pairs.end(), compareValue);

        // convert back to vectors
        std::vector<unsigned int> newIndexxes;
        std::vector<double> newEigenvector;

        for (std::vector<std::pair<unsigned int, double>>::const_iterator it = pairs.begin(), end_it = pairs.end(); it != end_it; ++it)
        {
            newIndexxes.push_back(it->first);
            newEigenvector.push_back(it->second);
        }

        if (DEBUG >= 2)
        {
            std::cout << "Split Eigenvector number: " << (eigenValueNumberTmp - 1) << std::endl;
            for (auto &i : newEigenvector)
                std::cout << i << std::endl;
        }

        std::vector<double> result(newEigenvector.size());
        // calc difference of two following, beginning from begin + 1 to avoid first value
        std::adjacent_difference(newEigenvector.begin(), newEigenvector.end(), result.data());

        double max_difference = 0;
        unsigned int maxElementIndex = 0;
        for (unsigned int i = 1; i < result.size(); i++)
        {
            if (fabs(result[i]) > max_difference)
            {
                maxElementIndex = i;
                max_difference = fabs(result[i]);
            }
        }

        // +1 as vector of difference has 1 fewer elements
        std::vector<unsigned int> lowerIndexxes(newIndexxes.begin(), newIndexxes.begin() + maxElementIndex);
        std::vector<unsigned int> upperIndexxes(newIndexxes.begin() + maxElementIndex, newIndexxes.end());

        double cost = SubGraph::splitCosts(lowerIndexxes, upperIndexxes);

        if (cost >= minCosts)
        {
            // if to good, set classes and increment number
            for (unsigned int index = 0; index < newIndexxes.size(); index++)
            {
                classIds[newIndexxes[index]] = classIds[newIndexxes[index]] + classAddOn;
            }
            classAddOn++;
        }
        else
        {
            if (DEBUG >= 2)
            {
                std::cout << "Lower part" << std::endl;
            }
            SubGraph::findBestSplit(lowerIndexxes, eigenValueNumberTmp + 1);
            if (DEBUG >= 2)
            {
                std::cout << "Upper part" << std::endl;
            }
            SubGraph::findBestSplit(upperIndexxes, eigenValueNumberTmp + 1);
        }
    }
    else
    {
        // if to less elements, set classes and increment number
        for (unsigned int index = 0; index < allIndexxes.size(); index++)
        {
            classIds[allIndexxes[index]] = classIds[allIndexxes[index]] + classAddOn;
        }
        classAddOn++;
    }
};

double SubGraph::splitCosts(std::vector<unsigned int> lowerIndexxes, std::vector<unsigned int> upperIndexxes)
{
    // return costs of the current split
    if (DEBUG >= 2)
    {
        std::cout << weights << std::endl;
    }
    double lowerCosts = 0;
    int edges_low = 0;
    double upperCosts = 0;
    int edges_upp = 0;

    unsigned int first, second;

    // loop through indices from first up to last -1
    for (first = 0; first < (lowerIndexxes.size() - 1); first++)
    {
        // loop through indices up to last
        for (second = (first + 1); second < lowerIndexxes.size(); second++)
        {
            lowerCosts = weights(lowerIndexxes[first], lowerIndexxes[second]) + lowerCosts;
            // if exist, then count up
            if (weights(lowerIndexxes[first], lowerIndexxes[second]) > 0)
            {
                edges_low++;
            }
        }
    }

    // loop through indices from first up to last -1
    for (first = 0; first < (upperIndexxes.size() - 1); first++)
    {
        // loop through indices up to last
        for (second = (first + 1); second < upperIndexxes.size(); second++)
        {
            upperCosts = weights(upperIndexxes[first], upperIndexxes[second]) + upperCosts;
            if (weights(upperIndexxes[first], upperIndexxes[second]) > 0)
            {
                edges_upp++;
            }
        }
    }

    double allCosts = 0;
    int edges_all = 0;
    std::vector<unsigned int> allIndexxes;
    allIndexxes.reserve(lowerIndexxes.size() + upperIndexxes.size());
    allIndexxes.insert(allIndexxes.end(), lowerIndexxes.begin(), lowerIndexxes.end());
    allIndexxes.insert(allIndexxes.end(), upperIndexxes.begin(), upperIndexxes.end());

    // loop through indices from first up to last -1
    for (first = 0; first < (allIndexxes.size() - 1); first++)
    {
        // loop through indices up to last
        for (second = (first + 1); second < allIndexxes.size(); second++)
        {
            allCosts = weights(allIndexxes[first], allIndexxes[second]) + allCosts;
            if (weights(allIndexxes[first], allIndexxes[second]) > 0)
            {
                edges_all++;
            }
        }
    }

    // calc subtract elements of low + up
    int cuts = (edges_all - edges_upp - edges_low);
    if (cuts == 0)
    {
        return 0;
    }
    double cutCosts = (allCosts - lowerCosts - upperCosts);
    return cutCosts / cuts;
}

void SubGraph::printClasses()
{
    for (auto &i : classIds)
    {
        std::cout << i << "\t";
    }
    std::cout << std::endl;
}

} // namespace SubGraph