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

#include "heuristic.h"
#include "sa.h"

/* #define T        8 */
#define T        14
#define M        2000
#define L        1200
#define EPSILON  0.002
#define PHI      0.95

#define BEST     0.263713276

/* The Batch structure. */
struct _Batch {
  double mean;
  Path* path;
};

/* The Simulated Annealing structure. */
struct _SA {
  /* The temperature */
  double t;
  double epsilon;
  double phi;
  /* The current solution */
  Path* sol;
  /* The final solution */
  Path* final;
  /* Maximum iterations of `compute_batch`
     if no better solution is found.       */
  int m;
  /* Accepted solutions average and last
     accepted solution. */
  Batch* batch;
  /* The number of iterations the computing of
     a batch should take.*/
  int l;
};


/* Creates a new Simulated Annealing Heuristic. */
SA* sa_new(TSP* tsp, double t, int m,
           int l, double epsilon, double phi) {
  SA* sa = malloc(sizeof(struct _SA));
  sa->batch = malloc(sizeof(struct _Batch));
  sa->sol = path_new(loader_cities(tsp_database_loader(tsp)), tsp_city_number(tsp),
                     tsp_ids(tsp), tsp_adj_matrix(tsp));
  sa->final = path_new(loader_cities(tsp_database_loader(tsp)), tsp_city_number(tsp),
                     tsp_ids(tsp), tsp_adj_matrix(tsp));
  sa->t = t ? t : T;
  sa->m = m ? m : M;
  sa->l = l ? l : L;
  sa->epsilon = epsilon ? epsilon : EPSILON;
  sa->phi = phi ? phi : PHI;
  return sa;
}

/* Frees the memory used by the simulated annealing heuristic. */
void sa_free(SA* sa) {
  if (sa->batch)
    free(sa->batch);
  if (sa->sol)
    path_free(sa->sol);
  if (sa->final)
    path_free(sa->final);
  free(sa);
}

/* Computes the set of solutions. */
Batch* compute_batch(SA* sa) {
  int t = sa->t;
  Path* n = 0;
  Batch* batch = sa->batch;

  int c = 0, m = sa->m;
  double r = 0.0, cost;

  while (c < sa->l && m--) {
    cost = path_cost_function(sa->sol);
    n = path_neighbour(sa->sol);
    if (path_cost_function(n) <= cost + t) {
      path_free(sa->sol);
      sa->sol = n;
      c++;
      r += path_cost_function(n);
    }
    /* else */
    /*   path_free(n); */
  }
  batch->mean = r/sa->l;
  batch->path = n;

  return batch;
}

void threshold_accepting(SA* sa) {
  double p = 0., q;
  Batch* batch;
  double cost;
  double b = 10000;
  while (sa->t > sa->epsilon) {
    q = p + 1;

    printf("T: %0.16f\n", sa->t);
    fflush(stdout);
    while (p <= q) {
      q = p;
      batch = compute_batch(sa);
      p = batch->mean;
      cost = path_cost_function(batch->path);
      b = b > cost ? cost : b;
      printf("Accepted: %.16f\n", cost);
      /* printf("["); */
      /* for (int i = 0; i < path_n(batch->path); ++i) */
      /*   if (i+1 < path_n(batch->path)) */
      /*     /\* printf("%d,", *(path_ids_r(batch->path)+i)); *\/ */
      /*     printf("%d,", city_id(*(path_array(batch->path)+i))); */
      /*   else */
      /*     /\* printf("%d]\n", *(path_ids_r(batch->path)+i)); *\/ */
      /*     printf("%d]\n", city_id(*(path_array(batch->path)+i))); */
      
      /* if (path_cost_function(batch->path) < path_cost_function(sa->final)) { */
      /*   printf("Accepted: %.16f\n", path_cost_function(batch->path)); */
      /*   fflush(stdout); */
      /* } */
    }
    
    sa->t *= sa->phi;
  }
  printf("\nBest: %.16f\n", b);
}
