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
#include <pthread.h>

#include "heuristic.h"

typedef struct {
  int n;
  int seed;
  int *ids;
} Data;

void* heuristic(void*) {
  /* printf("Seed: %d\n", seed); */
  /* TSP* tsp = tsp_new(n, ids, seed); */
  /* SA* sa = sa_new(tsp, 0, 0., 0, 0., 0.); */
  /* threshold_accepting(sa); */
  /* sa_free(sa); */
  /* tsp_free(tsp); */
  return NULL;
}

int main(int argc, char** argv) {
  int inst[] = {1,2,3,4,5,6,7,54,163,164,165,168,172,186,327,329,331,332,
                333,483,489,490,491,492,493,496,653,654,656,657,815,816,
                817,820,978,979,980,981,982,984};

  TSP* tsp = tsp_new(40, inst, 85);//71//66//17//82//52//47841368//584
  SA* sa = sa_new(tsp, 0, 0., 0, 0., 0.);

  /* int inst_1[] = {657,656,2,820,981,979,493,172,186,982,816,7,1,654,3,333,165, */
  /*             6,5,491,980,54,483,332,164,978,490,653,815,496,168,329,163, */
  /*             489,327,331,984,492,817,4}; */
  /* Path* path_1 = path_new(loader_cities(tsp_database_loader(tsp)), 40, inst_1, */
  /*                       loader_adj_matrix(tsp_database_loader(tsp))); */
  /* printf("Path_1 cost function: %.16Lf\n", path_cost_function(path_1)); */
  /* path_free(path_1); */

  threshold_accepting(sa);
  sa_free(sa);
  tsp_free(tsp);

  /* int s; */
  /* /\* Data* data =  *\/ */
  /* for (s = 1; s < 100; ++s) { */
  /*   pthread_t thread_id; */
  /*   pthread_create(&thread_id, NULL, heuristic, NULL); */
  /*   pthread_join(thread_id, NULL); */
  /* } */
  return 0;
}
