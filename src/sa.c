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
#include <float.h>

#include "heuristic.h"
#include "sa.h"

/* #define T        8 */
#define T        14
#define M        122000
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
  long double t;
  double epsilon;
  double phi;
  /* The current solution */
  Path* sol;
  /* Maximum iterations of `compute_batch`
     if no better solution is found.       */
  int m;
  /* Accepted solutions average and last
     accepted solution. */
  Batch* batch;
  /* The number of iterations the computing of
     a batch should take.*/
  int l;
  /* The string representation of the best path. */
  char* str;
};


/* Creates a new Simulated Annealing Heuristic. */
SA* sa_new(TSP* tsp, double t, int m,
           int l, double epsilon, double phi) {
  SA* sa = malloc(sizeof(struct _SA));
  sa->batch = malloc(sizeof(struct _Batch));
  sa->sol = path_new(loader_cities(tsp_database_loader(tsp)), tsp_city_number(tsp),
                     tsp_ids(tsp), tsp_adj_matrix(tsp));
  sa->t = t ? t : T;
  sa->m = m ? m : M;
  sa->l = l ? l : L;
  sa->epsilon = epsilon ? epsilon : EPSILON;
  sa->phi = phi ? phi : PHI;
  sa->str = malloc(sizeof(int)*tsp_city_number(tsp)*2+2);
  return sa;
}

/* Frees the memory used by the simulated annealing heuristic. */
void sa_free(SA* sa) {
  if (sa->str)
    free(sa->str);
  if (sa->batch)
    free(sa->batch);
  if (sa->sol)
    path_free(sa->sol);
  free(sa);
}

/* Computes the set of solutions. */
Batch* compute_batch(SA* sa) {
  long double t = sa->t;
  Batch* batch = sa->batch;

  int c = 0, m = sa->m;
  double r = 0.0;
  long double cost;

  while (c < sa->l && m--) {
    cost = path_cost_function(sa->sol);
    path_swap(sa->sol);
    if (path_cost_function(sa->sol) <= cost + t) {
      c++;
      r += path_cost_function(sa->sol);
    } else
      path_de_swap(sa->sol);
  }
  batch->mean = r/sa->l;
  batch->path = sa->sol;

  return batch;
}

void threshold_accepting(SA* sa) {
  double p = 0., q;
  Batch* batch;
  long double cost;
  long double b = 1000000000;
  path_randomize(sa->sol);
  while (sa->t > sa->epsilon) {
    q = DBL_MAX;

    printf("T: %0.16Lf\n", sa->t);
    /* fflush(stdout); */
    while (p <= q) {
      q = p;
      batch = compute_batch(sa);
      p = batch->mean;
      cost = path_cost_function(batch->path);
      printf("Accepted: %.16Lf\n", cost);
      
      if (b > cost) {
        sprintf(sa->str, "%s", path_to_str(batch->path));
        b = cost;
      }
    }
    sa->t *= sa->phi;
  }
  printf("\nBest: %.16Lf\n", b);
  printf("%s\n", sa->str);
}
