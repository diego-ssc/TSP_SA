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

#define CITY_NUMBER 1092

/**
 * Creates a new Normalizer.
 * @param city_n the number of cities.
 * @param ids the ids of the cities.
 * @param matrix the adjacency matrix.
 */
Normalizer* normalizer_new(int city_n, int* ids,
                           double(*matrix)[CITY_NUMBER+1]);

/**
 * Frees the memory used by the normalizer.
 * @param normalizer the normalizer to be freed.
 */
void normalizer_free(Normalizer* normalizer);

/**
 * Normalizes the provided cost.
 * @param normalizer the normalizer.
 * @return the normalized cost.
 */
double normalizer_normalize(Normalizer* normalizer);
