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
  /* The results report. */
  Report* report;
  /* The report name. */
  char* report_name;
  /* The seed. */
  int seed;
  /* The number of cities in this instance. */
  int n;
  /* The ids of the cities in this instance. */
  int *ids;
  /* The filled adjacency matrix. */
  double (*fm)[CITY_NUMBER+1];
};

/* Creates a new TSP instance. */
TSP* tsp_new(int n, int* ids, int seed) {
  /* Random number generator. */
  seed ? srandom(seed) : srandom(time(0));
  long int r = random();

  /* Heap allocation. */
  TSP* tsp         = malloc(sizeof( struct _TSP));
  tsp->ids         = calloc(1,sizeof(int)*n);
  tsp->loader      = loader_new();
  tsp->path        = path_new(loader_cities(tsp->loader), n, ids,
                              loader_adj_matrix(tsp->loader));
  tsp->report_name = malloc(snprintf(NULL, 0, "%ld", r)+ 1);
  tsp->fm          = malloc((CITY_NUMBER+1)*sizeof(double[CITY_NUMBER+1]));
  tsp->report      = report_new(tsp->report_name, r);

  /* Database intialization. */
  loader_open(tsp->loader);
  loader_load(tsp->loader);

  /* Value copies. */
  tsp->n = n;
  tsp->seed = seed;

  /* Heap memory intialization. */
  sprintf(tsp->report_name,"%ld",r);
  memcpy(tsp->ids, ids, tsp->n * sizeof(int));
  tsp_fill_matrix(tsp);

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
  if (tsp->report_name)
    free(tsp->report_name);
  if (tsp->report)
    report_free(tsp->report);
  free(tsp);
}

/* Returns the database loader of the TSP instance. */
Database_loader* tsp_database_loader(TSP* tsp) {
  return tsp->loader;
}

/* Returns the report of the TSP instance. */
Report* tsp_report(TSP* tsp) {
  return tsp->report;
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

/* Sets the best solution of the TSP instance. */
void tsp_set_solution(TSP* tsp, Path* path) {
  tsp->path = path;
}

/* Sets the database loader of the TSP instance. */
void tsp_set_database_loader(TSP* tsp, Database_loader* loader) {
  tsp->loader = loader;
}

/* Sets the report of the TSP instance. */
void tsp_set_report(TSP* tsp, Report* report) {
  tsp->report = report;
}

/* Fills the nonexistent values of the adjacency matrix. */
void tsp_fill_matrix(TSP* tsp) {
  int i,j;
  double n;
  double (*m)[CITY_NUMBER+1] = loader_adj_matrix(tsp_database_loader(tsp));
  double (*fm)[CITY_NUMBER+1] = tsp->fm;
  City** cities = loader_cities(tsp->loader);
  for (i = 1; i < CITY_NUMBER+1; ++i)
    for (j = i+1; j < CITY_NUMBER+1; ++j) {
        n = *(*(m+ i)+ j) == 0.0 ? city_distance(*(cities + i), *(cities + j)) *
          path_max_distance(tsp->path) : *(*(m+ i)+ j);

      *(*(fm + i)+ j) = n;
      *(*(fm + j)+ i) = n;
    }
}
