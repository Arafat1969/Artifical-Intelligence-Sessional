#ifndef NODE_SHIFT_H
#define NODE_SHIFT_H

#include "perturbative_search.h"
#include <algorithm>

class NodeShift : public PerturbativeSearch {
public:
    NodeShift(vector<City> cities, vector<int> tour)
        : PerturbativeSearch(cities, tour) {}

    void shiftNode(int fromIndex, int toIndex) {
        int node = improved_tour[fromIndex];
        improved_tour.erase(improved_tour.begin() + fromIndex);
        improved_tour.insert(improved_tour.begin() + toIndex, node);
    }

    void optimizeTour() override {
        bool improvement = true;

        while (improvement) {
            improvement = false;
            double bestDistance = calculateTotalDistance();

            for (int i = 1; i < size - 1; ++i) {
                for (int j = 0; j < size; ++j) {
                    if (i == j) continue;
                    shiftNode(i, j);
                    double newDistance = calculateTotalDistance();
                    if (newDistance < bestDistance) {
                        bestDistance = newDistance;
                        improvement = true;
                    } else {
                        shiftNode(j, i);
                    }
                }
            }
        }
    }
};

#endif // NODE_SHIFT_H
