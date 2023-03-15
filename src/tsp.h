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
 * @param seed the requested seed.
 */
TSP* tsp_new(int n, int* ids, unsigned int seed);

/**
 * Frees the memory used by the tsp instance.
 * @param tsp the tsp instance to be freed.
 */
void tsp_free(TSP* tsp);

/**
 * Returns the database loader of the TSP instance.
 * @param tsp the TSP instance.
 * @return the database loader of the TSP instance.
 */
Database_loader* tsp_database_loader(TSP* tsp);

/**
 * Returns the adjacency matrix of the TSP instance.
 * @param tsp the TSP instance.
 * @return the adjacency matrix.
 */
double (*tsp_adj_matrix(TSP* tsp))[CITY_NUMBER+1];

/**
 * Returns the ids array of the TSP instance.
 * @param tsp the TSP instance.
 * @return the ids array.
 */
int* tsp_ids(TSP* tsp);

/**
 * Returns the number of cities in the TSP instance.
 * @param tsp the TSP instance.
 * @return the number of cities.
 */
int tsp_city_number(TSP* tsp);

/**
 * Returns the path of the TSP instance.
 * @param tsp the TSP instance.
 * @return the path.
 */
Path* tsp_path(TSP* tsp);

/**
 * Returns the seed of the TSP instance.
 * @param tsp the TSP instance.
 * @return the seed.
 */
unsigned int tsp_seed(TSP* tsp);

/**
 * Sets the best solution of the TSP instance.
 * @param tsp the TSP instance.
 * @param current the current solution.
 */
void tsp_set_solution(TSP* tsp, Path* path);

/**
 * Sets the database loader of the TSP instance.
 * @param tsp the TSP instance.
 * @param loader the database loader.
 */
void tsp_set_database_loader(TSP* tsp, Database_loader* loader);

/**
 * Fills the nonexistent values of the adjacency matrix.
 * @param tsp the TSP instance.
 */
void tsp_fill_matrix(TSP* tsp);
