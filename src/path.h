/*
 * This file is part of TSP_SA.
 *
 * Copyright © 2023 Diego Sebastián Sánchez Correa
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

/**
 * Creates a new Path.
 * @param cities the initial permutation.
 * @param n the number of cities.
 * @param ids the ids of the cities.
 * @param matrix the adjacency amtrix.
 */
Path* path_new(City** cities, int n, int* ids,
               double (*matrix)[CITY_NUMBER+1]);

/**
 * Frees the memory used by the path.
 * @param path the path.
 */
void path_free(Path* path);

/**
 * Normalizes the path weights.
 * @param path the path.
 * @return the normalized number.
 */
double path_normalize(Path* path);

/**
 * Computes the weight of an edge between two cities.
 * @param path the path.
 * @param c_1 the first city.
 * @param c_2 the second city.
 * @return the weight.
 */
double path_weight_function(Path* path, City* c_1, City* c_2);

/**
 * Computes the maximum distance of a problem instance.
 * @param path the path.
 * @return the maximum distance.
 */
double path_max_distance(Path* path);

/**
 * Computes the sum of the costs.
 * @param path the path.
 * @return the cost sum.
 */
double path_cost_sum(Path* path);

/**
 * Computes the cost function.
 * @param path the path.
 * @return the cost.
 */
double path_cost_function(Path* path);

/**
 * Randomizes the initial path.
 * @param path the path.
 */
void path_randomize(Path* path);

/**
 * Swaps two cities in the path.
 * @param path the path.
 */
void path_swap(Path* path);

/**
 * Returns the city array.
 * @param path the path.
 * @return the array.
 */
City** path_array(Path* path);

/**
 * Returns the array of ids.
 * @param path the path.
 * @return the array.
 */
int* path_ids(Path* path);

/**
 * Returns the array of randomized ids.
 * @param path the path.
 * @return the array.
 */
int* path_ids_r(Path* path);
