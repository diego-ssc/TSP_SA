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
 * Creates a new TSP instance.
 * @param n the number of cities.
 * @param ids the ids of the cities.
 */
TSP* tsp_new(int n, int* ids);

/**
 * Creates a new TSP instance.
 * @param n the number of cities.
 * @param ids the ids of the cities.
 * @param seed the requested seed.
 */
TSP* tsp_new_s(int n, int* ids, int seed);

/**
 * Frees the memory used by the tsp instance.
 * @param tsp the tsp instance to be freed.
 */
void tsp_free(TSP* tsp);

/**
 * Returns the initial solution of the TSP instance.
 * @param tsp the TSP instance.
 * @return the initial solution of the TSP instance.
 */
City** tsp_initial_solution(TSP* tsp);

/**
 * Returns the current solution of the TSP instance.
 * @param tsp the TSP instance.
 * @return the current solution of the TSP instance.
 */
City** tsp_current_solution(TSP* tsp);

/**
 * Returns the final solution of the TSP instance.
 * @param tsp the TSP instance.
 * @return the final solution of the TSP instance.
 */
City** tsp_final_solution(TSP* tsp);

/**
 * Returns the database loader of the TSP instance.
 * @param tsp the TSP instance.
 * @return the database loader of the TSP instance.
 */
Database_loader* tsp_database_loader(TSP* tsp);

/**
 * Returns the normalizer of the TSP instance.
 * @param tsp the TSP instance.
 * @return the normalizer of the TSP instance.
 */
Normalizer* tsp_normalizer(TSP* tsp);

/**
 * Returns the report of the TSP instance.
 * @param tsp the TSP instance.
 * @return the report of the TSP instance.
 */
Report* tsp_report(TSP* tsp);

/**
 * Sets the initial solution of the TSP instance.
 * @param tsp the TSP instance.
 * @param initial the initial solution.
 */
void tsp_set_initial_solution(TSP* tsp, City** initial);

/**
 * Sets the current solution of the TSP instance.
 * @param tsp the TSP instance.
 * @param current the current solution.
 */
void tsp_set_current_solution(TSP* tsp, City** current);

/**
 * Sets the final solution of the TSP instance.
 * @param tsp the TSP instance.
 * @param final the final solution.
 */
void tsp_set_final_solution(TSP* tsp, City** final);

/**
 * Sets the database loader of the TSP instance.
 * @param tsp the TSP instance.
 * @param loader the database loader.
 */
void tsp_set_database_loader(TSP* tsp, Database_loader* loader);

/**
 * Sets the normalizer of the TSP instance.
 * @param tsp the TSP instance.
 * @param normalizer the normalizer.
 */
void tsp_set_normalizer(TSP* tsp, Normalizer* normalizer);

/**
 * Sets the report of the TSP instance.
 * @param tsp the TSP instance.
 * @param report the report.
 */
void tsp_set_report(TSP* tsp, Report* report);

/**
 * Computes the cost of the initial solution.
 * @param tsp the TSP instance.
 * @return the cost.
 */
double tsp_initial_tour_cost(TSP* tsp);

/**
 * Computes the weight of an edge between
 * two cities.
 * @param tsp the TSP instance.
 * @param c_1 the first city.
 * @param c_2 the second city.
 * @return the weight.
 */
double weight_function(TSP* tsp, City* c_1, City* c_2);

/**
 * Computes the maximum distance of a problem instance.
 * @param tsp the TSP instance.
 * @return the maximum distance.
 */
double tsp_max_distance(TSP* tsp);
