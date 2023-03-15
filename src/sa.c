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

#define T        100000//800000
#define M        12000
#define L        1200
#define EPSILON  0.002
#define PHI      0.95

#define BEST_40   0.263713276
#define BEST_150  0.149078160

/* The Batch structure. */
struct _Batch {
  double mean;
  Path* path;
};

/* The Simulated Annealing structure. */
struct _SA {
  /* The temperature */
  long double t;
  /* The epsilon. */
  double epsilon;
  /* The phi. */
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
  /* The number of cities. */
  int n;
  /* The seed. */
  unsigned int seed;
  /* The problem instance. */
  TSP* tsp;
};

/* Batch constructor. */
Batch* batch_new(Path* path) {
  Batch* batch = malloc(sizeof(struct _Batch));
  batch->path = path_copy(path);
  return batch;
}

/* Frees the memory used by the batch. */
void batch_free(Batch* batch) {
  if (batch->path)
    path_free(batch->path);
  free(batch);
}

/* Creates a new Simulated Annealing Heuristic. */
SA* sa_new(TSP* tsp, double t, int m,
           int l, double epsilon, double phi) {
  /* Heap allocation. */
  SA* sa  = malloc(sizeof(struct _SA));
  sa->sol = tsp_path(tsp);
  sa->str = malloc(sizeof(int)*tsp_city_number(tsp)*2+2);

  /* Attribute copy. */
  sa->n = tsp_city_number(tsp);
  sa->seed = tsp_seed(tsp);
  sa->tsp = tsp;

  /* Parameters. */
  sa->t       = t ? t : T;
  sa->m       = m ? m : M;
  sa->l       = l ? l : L;
  sa->epsilon = epsilon ? epsilon : EPSILON;
  sa->phi     = phi ? phi : PHI;
  return sa;
}

/* Frees the memory used by the simulated annealing heuristic. */
void sa_free(SA* sa) {
  if (sa->str)
    free(sa->str);
  free(sa);
}

/* Computes the set of solutions. */
Batch* compute_batch(SA* sa) {
  long double t = sa->t;
  Batch* batch = batch_new(sa->sol);

  int c = 0, m = sa->m;
  double r = 0.0;
  long double cost;

  while (c < sa->l && m--) {
    cost = path_cost_function(sa->sol);
    path_swap(sa->sol);
    if (path_cost_function(sa->sol) <= (cost + t)) {
      /* printf("E[%u]:%.16Lf\n", sa->seed, path_cost_function(sa->sol)); */
      c++;
      r += path_cost_function(sa->sol);
      if (path_cost_function(sa->sol) < path_cost_function(batch->path)) {
        path_free(batch->path);
        batch->path = path_copy(sa->sol);
      }
    }
    else
      path_de_swap(sa->sol);
  }
  batch->mean = r/sa->l;

  return batch;
}

/* Main routine to accept solutions. */
void threshold_accepting(SA* sa) {
  double p = 0., q;
  Batch* batch;
  Path* best = path_copy(sa->sol);
  path_randomize(sa->sol);
  while (sa->t > sa->epsilon) {
    q = DBL_MAX;

    /* printf("T: %0.16Lf\n", sa->t); */
    while (p <= q) {
      q = p;
      batch = compute_batch(sa);
      p = batch->mean;
      if (path_cost_function(batch->path) < path_cost_function(best)) {
        path_free(best);
        best = path_copy(batch->path);
      }
      batch_free(batch);
    }
    sa->t *= sa->phi;
  }
  printf("\nBest[%u]: %.16Lf\n\n\t%s\n", sa->seed, path_cost_function(best), path_to_str(best));
  path_free(best);
}

/* Computes the best neighbour of the final solution
 of the thresold accepting algorithm. */
Path* sweep(SA* sa) {
  int i, j;
  Path* copy, *path, *best, *p_best;

  path = sa->sol;
  copy = path_copy(path);
  best = path_copy(path);
  p_best = 0;

  do {
    if (p_best)
      path_free(p_best);
    p_best = path_copy(best);
    for (i = 0; i < sa->n; ++i) {
      for (j = 0; j < sa->n; ++j) {
        if (i != j)
          path_swap_indexes(copy, i, j);
        if (path_cost_function(copy) < path_cost_function(p_best)) {
          if (path_cost_function(best) > path_cost_function(copy)) {
            path_free(best);
            best = path_copy(copy);
          }
        }
        if (i != j)
          path_de_swap(copy);
      }
    }
    path_free(copy);
    copy = path_copy(best);
  } while (abs(path_cost_function(best) - path_cost_function(p_best)) > 0.00016);

  path_free(path);
  tsp_set_solution(sa->tsp, best);
  path_free(p_best);
  path_free(copy);
  return best;
}
