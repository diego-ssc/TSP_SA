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
 * Creates a new Simulated Annealing Heuristic.
 * @param cities the cities array.
 * @param ids the instance's cities ids.
 * @param n the number of cities in the instance.
 */
SA* sa_new();

/**
 * Frees the memory used by the simulated annealing
 * heuristic.
 * @param sa the heuristic.
 */
void sa_free(SA* sa);

/**
 * Computes the set of solutions.
 * @param sa the heuristic.
 * @return the batch.
 */
Batch* compute_batch(SA* sa);

/**
 * Main routine to accept solutions.
 * @param sa the heuristic.
 */
void threshold_accepting(SA* sa);

/**
 * Computes the best neighbour of the final solution
 * of the thresold accepting algorithm.
 * @param sa the heuristic.
 * @return the best neighbour.
 */
Path* sweep(SA* sa);
