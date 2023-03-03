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

#include "heuristic.h"
#include "path.h"

/* The path structure. */
struct _Path {
  City** cities;
  City** p_path;
  int n;
  int* ids;
  double* distances;
  double cost_sum;
  double (*matrix)[CITY_NUMBER+1];
  double (*filled_matrix)[CITY_NUMBER+1];
};

/* Creates a new Path. */
Path* path_new(City** cities, int n, int* ids,
               double (*matrix)[CITY_NUMBER+1]) {
  Path* path = malloc(sizeof(struct _Path));
  path->cities = cities;
  path->p_path = city_array(n);
  path->filled_matrix = calloc(1,sizeof(double[CITY_NUMBER+1])*(CITY_NUMBER+1));
  path->n = n;
  path->ids = ids;
  path->matrix = matrix;
  path->cost_sum = path_cost_sum(path);
  path->distances = malloc(sizeof(double)*n*(n-1)/2);
  return path;
}

/* Frees the memory used by the path. */
void path_free(Path* path) {
  if (path->p_path)
    free(path->p_path);
  if (path->filled_matrix)
    free(path->filled_matrix);
  if (path->distances)
    free(path->distances);
  free(path);
}

/* Tells if the edge in the graph exists. */
static int edge_exists(Path* path, City* c_1, City* c_2) {
  return (*(*(path->matrix + city_id(c_1))+city_id(c_2))) != 0.0;
}

/* Determines the order of double numbers. */
static int fequal(const void* n, const void* m) {
  return *(double*)n-*(double*)m < 0;
}

/* Normalizes the provided number. */
double path_normalize(Path* path) {
  int i,j,k=0,n=path->n;
  int* ids = path->ids;
  City** cities = path->cities;
  double* distances = path->distances;
  double sum = 0.0;

  for (i = 0; i < n; ++i)
    for (j = i+1; j < n; ++j)
      if (edge_exists(path, *(cities + *(ids+i)), *(cities + *(ids+j)))) {
        *(distances + k) =  *(*(path->matrix + *(ids+i))+ *(ids+j));
        ++k;
      }

  qsort(distances, n * (n-1)/2, sizeof(double), fequal);
  for (i = 0; i < n-1; i++)
    sum += *(distances + i);

  return sum;
}

/* Computes the weight of an edge between two cities. */
double path_weight_function(Path* path, City* c_1, City* c_2) {
  return edge_exists(path, c_1, c_2) ?
    *(*(path->matrix + city_id(c_1))+city_id(c_2))
    : city_distance(c_1, c_2) * path_max_distance(path);
}

/* Computes the maximum distance of a problem instance. */
double path_max_distance(Path* path) {
  int* a = path->ids;
  double (*m)[CITY_NUMBER+1] = path->matrix;
  int i,j;
  double max = 0.0;
  int n = path->n;
  for (i = 0; i < n; ++i)
    for (j = i+1; j < n; ++j)
      max = max < *(*(m+ *(a+i)) + *(a+j)) ? *(*(m+ *(a+i)) + *(a+j)) : max;

  return max;
}

/* Computes the sum of the costs. */
double path_cost_sum(Path* path) {
  int* a = path->ids;
  City** cities = path->cities;
  int i;
  double cost = 0.0;
  int n = path->n;
  for (i = 0; i+1 < n; ++i)
    cost += path_weight_function(path, *(cities + *(a+i)),
                                 *(cities + *(a+i+1)));
  return cost;
}

/* Computes the cost function. */
double path_cost_function(Path* path) {
  return path->cost_sum/path_normalize(path);
}
