#ifndef NODE_SWAP_H
#define NODE_SWAP_H

#include "perturbative_search.h"
#include <algorithm>

class NodeSwap : public PerturbativeSearch {
public:
    NodeSwap(int size, vector<City> cities, vector<int> tour)
        : PerturbativeSearch(size, cities, tour) {}

    void swapNodes(int i, int j) {
        swap(improved_tour[i], improved_tour[j]);
    }

    void optimizeTour() override {
        bool improvement = true;

        while (improvement) {
            improvement = false;
            double bestDistance = calculateTotalDistance();

            for (int i = 1; i < size - 1; ++i) {
                for (int j = i + 1; j < size; ++j) {
                    swapNodes(i, j);
                    double newDistance = calculateTotalDistance();
                    if (newDistance < bestDistance) {
                        bestDistance = newDistance;
                        improvement = true;
                    } else {
                        swapNodes(i, j);
                    }
                }
            }
        }
    }
};

#endif // NODE_SWAP_H
