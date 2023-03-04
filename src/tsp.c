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

/* Returns the report of the TSP instance. */
Report* tsp_report(TSP* tsp) {
  return tsp->report;
}

/* Sets the initial solution of the TSP instance. */
void tsp_set_initial_solution(TSP* tsp, City** initial) {
  tsp->initial_solution = initial;
}

/* Sets the final solution of the TSP instance. */
void tsp_set_current_solution(TSP* tsp, City** current) {
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

/* Sets the report of the TSP instance. */
void tsp_set_report(TSP* tsp, Report* report) {
  tsp->report = report;
}

/* /\* En clase sa *\/ */
/* /\* Computes the set of solutions. *\/ */
/* double* compute_set(TSP* tsp) { */
/*   int t = tsp->t; */
/*   Path* sol = tsp->sol, *n; */
/*   double* set = tsp->set; */
  
/*   int c = 0, m = tsp->m; */
/*   double r = 0.0; */

/*   while (c < tsp->l && m--) { */
/*     n = path_neighbour(sol); */
/*     c++; */
/*     r += path_cost_function(); */
/*   } */
/*   *set = r/tsp->l; */
/*   *(set+1) = n; */
  
/*   return set; */
/* } */

/* void threshold_accepting(TSP* tsp) { */
  
/* } */
