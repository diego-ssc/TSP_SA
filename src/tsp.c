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
  City** initial_solution;
  City** current_solution;
  City** final_solution;
  Database_loader* loader;
  Normalizer* normalizer;
  double* distances;
  Report* report;
  char* report_name;
  int seed;
  int n;
  int *ids;
};

/* Creates a new TSP instance. */
TSP* tsp_new(int n, int* ids) {
  TSP* tsp = malloc(sizeof(struct _TSP));
  tsp->n = n;
  tsp->ids = calloc(1,sizeof(int)*n);
  memcpy(tsp->ids, ids, tsp->n * sizeof(int));
  tsp->initial_solution = city_array(n);
  tsp->current_solution = city_array(n);
  tsp->final_solution = city_array(n);
  tsp->loader = loader_new();
  loader_open(tsp->loader);
  loader_load(tsp->loader);
  tsp->normalizer = normalizer_new
    (n, ids, loader_adj_matrix(tsp->loader));
  tsp->distances = malloc(sizeof(double)*n*(n-1)/2);
  srandom(time(0));
  long int r = random();
  tsp->report_name = malloc(snprintf(NULL, 0, "%ld", r)+ 1);
  sprintf(tsp->report_name,"%ld",r);
  tsp->report = report_new(tsp->report_name, r);
  return tsp;
}

/* Creates a new TSP instance. */
TSP* tsp_new_s(int n, int* ids, int seed) {
  TSP* tsp = malloc(sizeof(struct _TSP));
  tsp->n = n;
  tsp->ids = malloc(sizeof(int)*n);
  tsp->initial_solution = city_array(n);
  tsp->current_solution = city_array(n);
  tsp->final_solution = city_array(n);
  tsp->loader = loader_new();
  loader_open(tsp->loader);
  loader_load(tsp->loader);
  tsp->normalizer = normalizer_new
    (n, ids, loader_adj_matrix(tsp->loader));
  tsp->distances = malloc(sizeof(double)*n);
  tsp->seed = seed;
  srandom(tsp->seed);
  long int r = random();
  tsp->report_name = malloc(snprintf(NULL, 0, "%ld", r)+ 1);
  sprintf(tsp->report_name,"%ld",r);
  tsp->report = report_new(tsp->report_name, r);
  return tsp;
}

/* Frees the memory used by the tsp instance. */
void tsp_free(TSP* tsp) {
  if (tsp->ids)
    free(tsp->ids);
  if (tsp->initial_solution)
    free(tsp->initial_solution);
  if (tsp->current_solution)
    free(tsp->current_solution);
  if (tsp->final_solution)
    free(tsp->final_solution);
  if (tsp->loader)
    loader_free(tsp->loader);
  if (tsp->normalizer)
    normalizer_free(tsp->normalizer);
  if (tsp->distances)
    free(tsp->distances);
  if (tsp->report_name)
    free(tsp->report_name);
  if (tsp->report)
    report_free(tsp->report);
  free(tsp);
}

/* Returns the initial solution of the TSP instance. */
City** tsp_initial_solution(TSP* tsp) {
  return tsp->initial_solution;
}

/* Returns the current solution of the TSP instance. */
City** tsp_current_solution(TSP* tsp) {
  return tsp->current_solution;
}

/* Returns the final solution of the TSP instance. */
City** tsp_final_solution(TSP* tsp) {
  return tsp->final_solution;
}

/* Returns the database loader of the TSP instance. */
Database_loader* tsp_database_loader(TSP* tsp) {
  return tsp->loader;
}

/* Returns the normalizer of the TSP instance. */
Normalizer* tsp_normalizer(TSP* tsp) {
  return tsp->normalizer;
}

/* Returns the report of the TSP instance. */
Report* tsp_report(TSP* tsp) {
  return tsp->report;
}

/* Sets the initial solution of the TSP instance. */
void tsp_set_initial_solution(TSP* tsp, City** initial) {
  tsp->initial_solution = initial;
}

/* Sets the final solution of the TSP instance. */
void tsp_set_final_current_solution(TSP* tsp, City** current) {
  tsp->current_solution = current;
}

/* Sets the final solution of the TSP instance. */
void tsp_set_final_solution(TSP* tsp, City** final) {
  tsp->final_solution = final;
}

/* Sets the database loader of the TSP instance. */
void tsp_set_database_loader(TSP* tsp, Database_loader* loader) {
  tsp->loader = loader;
}

/* Sets the normalizer of the TSP instance. */
void tsp_set_normalizer(TSP* tsp, Normalizer* normalizer) {
  tsp->normalizer = normalizer;
}

/* Sets the report of the TSP instance. */
void tsp_set_report(TSP* tsp, Report* report) {
  tsp->report = report;
}

/* Computes the cost of the initial solution. */
double tsp_initial_path_cost(TSP* tsp) {
  int* a = tsp->ids;
  City** cities = loader_cities(tsp->loader);
  int i;
  double cost = 0.0;
  int n = tsp->n;
  for (i = 0; i+1 < n; ++i)
    cost += weight_function(tsp, *(cities + *(a+i)), *(cities + *(a+i+1)));
  return cost;
}

/* Returns if the edge in G exists; 0, otherwise. */
static int edge_exists(TSP* tsp, City* c_1, City* c_2) {
  double (*matrix)[CITY_NUMBER+1] = loader_adj_matrix(tsp->loader);
  return (*(*(matrix + city_id(c_1))+city_id(c_2))) != 0.0;
}

/* Computes the weight of an edge between two cities. */
double weight_function(TSP* tsp, City* c_1, City* c_2) {
  return edge_exists(tsp, c_1, c_2) ?
    *(*(loader_adj_matrix(tsp->loader) + city_id(c_1))+city_id(c_2))
    : city_distance(c_1, c_2) * tsp_max_distance(tsp);
}

/* Computes the maximum distance of a problem instance. */
double tsp_max_distance(TSP* tsp) {
  int* a = tsp->ids;
  double (*m)[CITY_NUMBER+1] = loader_adj_matrix(tsp->loader);
  int i,j;
  double max = 0.0;
  int n = tsp->n;
  for (i = 0; i < n; ++i)
    for (j = i+1; j < n; ++j)
      max = max < *(*(m+ *(a+i)) + *(a+j)) ? *(*(m+ *(a+i)) + *(a+j)) : max;

  return max;
}

/* Determines the order of double numbers. */
static int fequal(const void* n, const void* m) {
  return *(double*)n-*(double*)m < 0;
}

/* Normalizes the path weights. */
double tsp_normalize(TSP* tsp) {
  int i,j,k=0,n=tsp->n;
  int* ids = tsp->ids;
  City** cities = loader_cities(tsp_database_loader(tsp));
  double (*m)[CITY_NUMBER + 1] = loader_adj_matrix(tsp_database_loader(tsp));
  double sum = 0.0;

  for (i = 0; i < n; ++i)
    for (j = i+1; j < n; ++j)
      if (edge_exists(tsp, *(cities + *(ids+i)), *(cities + *(ids+j)))) {
        *(tsp->distances + k) =  *(*(m + *(ids+i))+ *(ids+j));
        ++k;
      }

  qsort(tsp->distances, tsp->n * (tsp->n-1)/2, sizeof(double), fequal);
  for (i = 0; i < tsp->n-1; i++)
    sum += *(tsp->distances + i);

  return sum;
}

/* Computes the cost function. */
double cost_function(TSP* tsp) {
  City** cities = loader_cities(tsp_database_loader(tsp));
  int* ids = tsp->ids;
  int i;
  double sum = 0.0;
  for (i = 0; i+1 < tsp->n; ++i)
    sum += weight_function(tsp, *(cities + *(ids+i)), *(cities + *(ids+i+1)));

  return sum/tsp_normalize(tsp);
}
