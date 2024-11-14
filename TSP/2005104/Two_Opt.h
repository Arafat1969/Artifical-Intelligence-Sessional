#ifndef TWO_OPT_H
#define TWO_OPT_H

#include "perturbative_search.h"
#include <algorithm>

class TwoOpt : public PerturbativeSearch {
public:
    TwoOpt(int size, vector<City> cities, vector<int> tour)
        : PerturbativeSearch(size, cities, tour) {}

    void twoOptSwap(int i, int k) {
        reverse(improved_tour.begin() + i, improved_tour.begin() + k + 1);
    }

    void optimizeTour() override {
        bool improvement = true;

        while (improvement) {
            improvement = false;
            double bestDistance = calculateTotalDistance();

            for (int i = 1; i < size - 1; ++i) {
                for (int k = i + 1; k < size; ++k) {
                    twoOptSwap(i, k);
                    double newDistance = calculateTotalDistance();
                    if (newDistance < bestDistance) {
                        bestDistance = newDistance;
                        improvement = true;
                    } else {
                        twoOptSwap(i, k);
                    }
                }
            }
        }
    }
};

#endif // TWO_OPT_H
