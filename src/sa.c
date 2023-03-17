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
#include <math.h>

#include "heuristic.h"
#include "sa.h"

#define T        8
#define M        260000
#define L        12000//9000//1800
#define EPSILON  0.000016
#define PHI      0.98
#define P        0.98//0.98
#define N        9000

#define T_EPSILON 0.00016

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
  /* Percentage of accepted solutions. */
  double p;
  /* The seed. */
  unsigned int seed;
  /* The problem instance. */
  TSP* tsp;
};

/* Returns the percentage of accepted neighbours. */
static double accepted_percentage(SA*);

/* Computes the intial temperature */
static long double binary_search(SA*, double, double);

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
SA* sa_new(TSP* tsp, double t, int m, int l,
           double epsilon, double phi, double p) {
  /* Heap allocation. */
  SA* sa  = malloc(sizeof(struct _SA));
  sa->sol = tsp_path(tsp);
  sa->str = malloc(sizeof(int)*tsp_city_number(tsp)*2+2);

  /* Attribute copy. */
  sa->n    = tsp_city_number(tsp);
  sa->seed = tsp_seed(tsp);
  sa->tsp  = tsp;

  /* Parameters. */
  sa->t       = t ? t : T;
  sa->m       = m ? m : M;
  sa->l       = l ? l : L;
  sa->epsilon = epsilon ? epsilon : EPSILON;
  sa->phi     = phi ? phi : PHI;
  sa->p       = p ? p : P;

  /* Path randomization. */
  path_randomize(sa->sol);

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
  printf("T[%u]: %0.16Lf\n", sa->seed, sa->t);
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
  printf("\nBest[%u]:%.16Lf\n\n\t%s\n", sa->seed, path_cost_function(best), path_to_str(best));
  path_free(sa->sol);
  tsp_set_solution(sa->tsp, best);
  sweep(sa);
  printf("\nBest[%u][Sweep]:%.16Lf\n\n\t%s\n", sa->seed, path_cost_function(tsp_path(sa->tsp)),
         path_to_str(tsp_path(sa->tsp)));
}

/* Computes the best neighbour of the final solution
 of the thresold accepting algorithm. */
Path* sweep(SA* sa) {
  int i, j;
  Path* copy, *path, *best, *p_best;

  path = tsp_path(sa->tsp);
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
  } while (fabs(path_cost_function(best) - path_cost_function(p_best)) > T_EPSILON);

  path_free(path);
  tsp_set_solution(sa->tsp, best);
  path_free(p_best);
  path_free(copy);
  return best;
}

/* Computes the initial temperature. */
long double initial_temperature(SA* sa) {
  double p = accepted_percentage(sa);
  long double t_1, t_2;
  if (fabs(sa->p - p) <= T_EPSILON)
    return sa->t;
  if (p < sa->p) {
    while (p < sa->p) {
      sa->t *= 2.;
      p = accepted_percentage(sa);
    }
    t_1 = sa->t/2;
    t_2 = sa->t;
  } else {
    while (p > sa->p) {
      sa->t /= 2.;
      p = accepted_percentage(sa);
    }
    t_1 = sa->t;
    t_2 = sa->t*2.;
  }
  return binary_search(sa, t_1, t_2);
}

/* Returns the percentage of accepted neighbours. */
static double accepted_percentage(SA* sa) {
  int c = 0, i;
  Path* n;
  for (i = 0; i < N; ++i) {
    n = path_copy(sa->sol);
    path_swap(sa->sol);
    if (path_cost_function(sa->sol) <= (path_cost_function(n) + sa->t))
      c++;
    else
      path_de_swap(sa->sol);
    path_free(n);
  }
  return (double)c/N;
}

/* Computes the intial temperature */
static long double binary_search(SA* sa, double t_1, double t_2) {
  long double t_m = (t_1 + t_2)/2.;
  if ((t_2 - t_1) < T_EPSILON)
    return t_m;
  double p = accepted_percentage(sa);
  if (fabs(sa->p - p) < T_EPSILON)
    return t_m;
  if (p > sa->p)
    return binary_search(sa, t_1, t_m);
  else
    return binary_search(sa, t_m, t_2);
}

/* Returns the temperature of the heuristic. */
long double sa_temperature(SA* sa) {
  return sa->t;
}

/* Sets the temperature of the heuristic. */
void sa_set_temperature(SA* sa, long double t) {
  sa->t = t;
}
