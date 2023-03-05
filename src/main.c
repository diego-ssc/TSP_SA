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

#include <stdio.h>

#include "database_loader.h"
#include "heuristic.h"

int main(int argc, char** argv) {
  /* Database_loader* loader = loader_new(); */
  /* loader_open(loader); */
  /* loader_load(loader); */
  /* double (*m)[1093] = loader_adj_matrix(loader); */
  /* City** cities = loader_cities(loader); */
  /* printf("%.12f :: %.12f\n", *(*(m + 1) + 7), city_distance(*(cities + 1), *(cities + 7))); */
  /* loader_free(loader); */

  /* int inst[] = {1,2,3,4,5,6,7,54,163,164,165,168,172,186,327,329,331,332, */
  /*               333,483,489,490,491,492,493,496,653,654,656,657,815,816, */
  /*               817,820,978,979,980,981,982,984}; */
  /* TSP* tsp = tsp_new(40, inst); */
  /* printf("Cost function: %.16f\n ", cost_function(tsp)); */
  /* tsp_free(tsp); */

  /* Database_loader* loader = loader_new(); */
  /* loader_open(loader); */
  /* loader_load(loader); */
  /* Path* path = path_new(loader_cities(loader), 40, */
  /*                       inst, loader_adj_matrix(loader)); */
  /* City** original = path_array(path); */
  /* int i; */
  /* printf("Cost: %0.16f\nShould be: %0.16f\n\nCost sum[path_cost_sum]:%0.16f\nNormalizer[path_normalize]: %0.16f\n",path_cost_function(path), */
  /*        path_cost_sum(path)/path_normalize(path), path_cost_sum(path), path_normalize(path)); */
  /* printf("\n\n"); */
  /* path_randomize(path); */
  /* for (i = 0; i < 40 ; ++i) */
  /*   printf("%s:%d |", city_name(*(original + i)), */
  /*          city_id(*(original + i))); */
  /* path_swap(path);; */
  /* for (i = 0; i < 40 ; ++i) */
  /*   printf("%s:%d |", city_name(*(original + i)), */
  /*          city_id(*(original + i))); */
  /* printf("\n\n"); */
  /* printf("Cost: %0.16f\nShould be: %0.16f\n\nCost sum[path_cost_sum]:%0.16f\nNormalizer[path_normalize]: %0.16f\n",path_cost_function(path), */
  /*        path_cost_sum(path)/path_normalize(path), path_cost_sum(path), path_normalize(path)); */
  /* path_free(path); */
  /* loader_free(loader); */
  return 0;
}
