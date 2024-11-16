#ifndef GREEDY_HEURISTIC_H
#define GREEDY_HEURISTIC_H

#include "constructive_search.h"
#include <iostream>
#include <algorithm>
#include <vector>

class GreedyHeuristic : public ConstructiveSearch {
public:
    vector<pair<pair<City, City>, double>> edges; 
    vector<int> degree;
    vector<int> parent;

    GreedyHeuristic(int size, vector<City> cities) : ConstructiveSearch(size, cities) {
        parent.resize(size+1);
        //iota(parent.begin(), parent.end(), 0);
        degree.resize(size+1,0);
    }

    void createEdges() {
        edges.clear();
        for (int i = 0; i < size; ++i) {
            for (int j = i + 1; j < size; ++j) {
                edges.push_back({{cities[i], cities[j]}, cities[i].distanceTo(cities[j])});
            }
        }
    }

    
    int find_parents(int u) {
        if (u == parent[u]) return u;
        return parent[u] = find_parents(parent[u]);
    }

    
    void join_sets(int u, int v) {
        int parent_u = find_parents(u);
        int parent_v = find_parents(v);
        if (parent_u != parent_v) {
            parent[parent_v] = parent_u;
        }
    }


    vector<int> findTour() override {
        if (size == 0) return {};

        for (int i = 1; i <= size; ++i) {
            parent[i] = i; 
        }

        createEdges();

        sort(edges.begin(), edges.end(), [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

        vector<pair<int, int>> tourEdges;
        

        for (const auto& edge : edges) {
            int u = edge.first.first.id;
            int v = edge.first.second.id;

            if(degree[u] < 2 && degree[v] < 2 && find_parents(u) != find_parents(v)) {
                tourEdges.push_back({u, v});
                degree[u]++;
                degree[v]++;
                join_sets(u, v);
            }
            
            if (tourEdges.size() == size-1) break;
        }

        
        // if (tourEdges.size() != size) {
        //     std::cerr << "Error: Could not construct a valid tour!" << std::endl;
        //     return {};
        // }

        for(const auto& edge : edges) {
            int u = edge.first.first.id;
            int v = edge.first.second.id;
            if(degree[u] < 2 && degree[v] < 2 && find_parents(u) == find_parents(v)) {
                tourEdges.push_back({u, v});
            }
        }

        
        tour.clear();
        unordered_map<int, vector<int>> list;
        for (const auto& edge : tourEdges) {
            list[edge.first].push_back(edge.second);
            list[edge.second].push_back(edge.first);
        }

        int currentCity = cities[0].id;
        unordered_set<int> visitedCities;

        while (tour.size() < size) {
            tour.push_back(currentCity);
            visitedCities.insert(currentCity);
            for (int neighbor : list[currentCity]){
                if (visitedCities.find(neighbor) == visitedCities.end()){
                    currentCity = neighbor;
                    break;
                }
            }
        }
        tour.push_back(cities[0].id);
        return tour;
    }
};

#endif // GREEDY_HEURISTIC_H
