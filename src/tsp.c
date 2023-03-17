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

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "heuristic.h"
#include "tsp.h"

#define CITY_NUMBER 1092

/* The TSP structure */
struct _TSP {
  /*The best solution */
  Path* path;
  /* The database loader. */
  Database_loader* loader;
  /* The seed. */
  unsigned int seed;
  /* The number of cities in this instance. */
  int n;
  /* The ids of the cities in this instance. */
  int *ids;
  /* The filled adjacency matrix. */
  double (*fm)[CITY_NUMBER+1];
  /* RNG buffer. */
  struct drand48_data* buffer;
};

/* Creates a new TSP instance. */
TSP* tsp_new(int n, int* ids, unsigned int seed) {
  /* Heap allocation. */
  TSP* tsp         = malloc(sizeof( struct _TSP));
  tsp->ids         = calloc(1,sizeof(int)*n);
  tsp->fm          = calloc(1,(CITY_NUMBER+1)*sizeof(double[CITY_NUMBER+1]));

  /* Random number generator. */
  tsp->seed        = seed;

  /* Database intialization. */
  tsp->loader = loader_new();
  loader_open(tsp->loader);
  loader_load(tsp->loader);

  /* Value copies. */
  tsp->n = n;

  /* Heap memory intialization. */
  memcpy(tsp->ids, ids, tsp->n * sizeof(int));

  /* Structure creation. */
  tsp->path   = path_new(loader_cities(tsp->loader), n, ids,
                         tsp->seed, loader_adj_matrix(tsp->loader));

  return tsp;
}

/* Frees the memory used by the tsp instance. */
void tsp_free(TSP* tsp) {
  if (tsp->path)
    path_free(tsp->path);
  if (tsp->fm)
    free(tsp->fm);
  if (tsp->ids)
    free(tsp->ids);
  if (tsp->loader)
    loader_free(tsp->loader);
  free(tsp);
}

/* Returns the database loader of the TSP instance. */
Database_loader* tsp_database_loader(TSP* tsp) {
  return tsp->loader;
}

/* Returns the adjacency matrix of the TSP instance. */
double (*tsp_adj_matrix(TSP* tsp))[CITY_NUMBER+1] {
  return loader_adj_matrix(tsp->loader);
}

/* Returns the ids array of the TSP instance. */
int* tsp_ids(TSP* tsp) {
  return tsp->ids;
}

/* Returns the number of cities in the TSP instance. */
int tsp_city_number(TSP* tsp) {
  return tsp->n;
}

/* Returns the path of the TSP instance. */
Path* tsp_path(TSP* tsp) {
  return tsp->path;
}

/* Returns the seed of the TSP instance. */
unsigned int tsp_seed(TSP* tsp) {
  return tsp->seed;
}

/* Sets the best solution of the TSP instance. */
void tsp_set_solution(TSP* tsp, Path* path) {
  tsp->path = path;
}

/* Sets the database loader of the TSP instance. */
void tsp_set_database_loader(TSP* tsp, Database_loader* loader) {
  tsp->loader = loader;
}
