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

#include "heuristic.h"

/* New: [817,492,3,186,981,333,493,489,978,491,327,984,165,331,982,483,654,54,490,172,815,164,980,6,496,820,7,979,657,653,329,5,168,656,1,4,332,816,2,163] */
/* cost: 0.2618081683482656 */
/* Original: [817,492,3,186,981,333,493,489,978,491,327,984,165,331,982,483,654,54,490,172,815,164,980,6,496,820,7,979,657,653,329,5,168,656,1,4,332,816,2,163] */
/* cost: 0.2618081684080158 */
/* T: 10.2912864687499971 */

int main(int argc, char** argv) {
  int inst[] = {1,2,3,4,5,6,7,54,163,164,165,168,172,186,327,329,331,332,
                333,483,489,490,491,492,493,496,653,654,656,657,815,816,
                817,820,978,979,980,981,982,984};
  TSP* tsp = tsp_new(40, inst, 102);
  SA* sa = sa_new(tsp, 0, 0., 0, 0., 0.);

  /* int inst_1[] = {657,656,2,820,981,979,493,172,186,982,816,7,1,654,3,333,165, */
  /*             6,5,491,980,54,483,332,164,978,490,653,815,496,168,329,163, */
  /*             489,327,331,984,492,817,4}; */
  /* int inst_2[] = {657,656,2,820,981,979,493,172,332,982,816,7,1,654,3,333,165, */
  /*                 6,5,491,980,54,483,186,164,978,490,653,815,496,168,329,163, */
  /*                 489,327,331,984,492,817,4}; */
  /* Path* path_1 = path_new(loader_cities(tsp_database_loader(tsp)), 40, inst_1, */
  /*                       loader_adj_matrix(tsp_database_loader(tsp))); */
  /* Path* path_2 = path_new(loader_cities(tsp_database_loader(tsp)), 40, inst_2, */
  /*                         loader_adj_matrix(tsp_database_loader(tsp))); */
  /* printf("Path_1 cost function: %.16Lf\n", path_cost_function(path_1)); */
  /* printf("Path_2 cost function: %.16Lf\n", path_cost_function(path_2)); */

  /* printf("%s\n", path_to_str(path)); */
  /* path_swap(path); */
  /* printf("%s\n", path_to_str(path)); */
  /* path_de_swap(path); */
  /* printf("%s\n", path_to_str(path)); */

  /* path_free(path_1); */
  /* path_free(path_2); */

  /* int inst_3[] = {1,2,3,4,5,6,7,54,163,164,165,168,172,186,327, */
  /*                 329,331,332,333,483,489,490,491,492,493,496, */
  /*                 653,654,656,657,815,816,817,820,978,979,980, */
  /*                 981,982,984}; */
  /* Path* path_3 = path_new(loader_cities(tsp_database_loader(tsp)), 40, inst_3, */
  /*                         loader_adj_matrix(tsp_database_loader(tsp))); */
  /* printf("Path_3 cost function: %.16Lf\n", path_cost_function(path_3)); */
  /* path_free(path_3); */
  
  threshold_accepting(sa);
  sa_free(sa);
  tsp_free(tsp);
  return 0;
}
