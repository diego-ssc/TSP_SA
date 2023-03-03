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
#include <string.h>

#include "heuristic.h"
#include "path.h"

/* The path structure. */
struct _Path {
  City** cities, **r_path;
  int n;
  int* ids, *ids_r;
  double* distances;
  double cost_sum;
  double (*matrix)[CITY_NUMBER+1];
  double (*filled_matrix)[CITY_NUMBER+1];
};

/* Fills the path array. */
static void fill_path_array(Path* path) {
  int *ids = path->ids;
  int i;
  for (i = 0; i < path->n; ++i)
    *(path->r_path + i) = *(path->cities + *(ids + i));
}

/* Creates a new Path. */
Path* path_new(City** cities, int n, int* ids,
               double (*matrix)[CITY_NUMBER+1]) {
  Path* path = malloc(sizeof(struct _Path));
  path->cities = cities;
  path->r_path = city_array(n);
  path->ids = ids;
  path->n = n;
  fill_path_array(path);
  path->filled_matrix = calloc(1,sizeof(double[CITY_NUMBER+1])*(CITY_NUMBER+1));
  path->ids_r = calloc(1,sizeof(int)*n);
  memcpy(path->ids_r, path->ids, n*sizeof(int));
  path->matrix = matrix;
  path->cost_sum = path_cost_sum(path);
  path->distances = malloc(sizeof(double)*n*(n-1)/2);
  return path;
}

/* Frees the memory used by the path. */
void path_free(Path* path) {
  if (path->ids_r)
    free(path->ids_r);
  if (path->r_path)
    free(path->r_path);
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
  int* ids = path->ids_r;
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
  int* a = path->ids_r;
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
  City** r_path = path->r_path;
  int i;
  double cost = 0.0;
  int n = path->n;
  for (i = 0; i+1 < n; ++i)
    cost += path_weight_function(path, *(r_path + i),
                                 *(r_path + i+1));  
  return cost;
}

/* Computes the cost function. */
double path_cost_function(Path* path) {
  return path->cost_sum/path_normalize(path);
}

/* Randomizes the initial path. */
void path_randomize(Path* path) {
  int i, r, temp, n = path->n;
  int *ids_r = path->ids_r;
  City** cities = path->cities;

  for (i = 0; i < n; ++i) {
    r = random() % n;
    temp = *(ids_r + r);
    *(ids_r + r) = *(ids_r + i);
    *(ids_r + i) = temp;
  }
  for (i = 0; i < n; ++i)
    *(path->r_path+i) = *(cities + *(ids_r + i));
}

/* Swaps two cities in the path. */
void path_swap(Path* path) {
  City** r_path = path->r_path, *temp;
  int i = 0,j = 0, n = path->n, temp_i;
  int* ids_r = path->ids_r;
  double a = 0., b = 0., c = 0., d = 0.;

  while (i == j)
    i = random()%n, j = random()%n;

  if (*(r_path+i-1))
    a = path_weight_function(path, *(r_path+i-1),
                             *(r_path+i));
  if (*(r_path+i+1))
    b = path_weight_function(path, *(r_path+i),
                             *(r_path+i+1));

  if (*(r_path+j-1))
    c = path_weight_function(path, *(r_path+j-1),
                             *(r_path+j));
  if (*(r_path+j+1))
    d = path_weight_function(path, *(r_path+j),
                             *(r_path+j+1));
  path->cost_sum -= (a+b+c+d);

  a = 0., b = 0., c = 0., d = 0.;
  temp = *(r_path + i);
  temp_i = *(ids_r + i);
  *(r_path + i) = *(r_path + j);
  *(ids_r + i) = *(ids_r + j);

  *(r_path + j) = temp;
  *(ids_r + j) = temp_i;
  if (*(r_path+i-1))
    a = path_weight_function(path, *(r_path+i-1),
                             *(r_path+i));
  if (*(r_path+i+1))
    b = path_weight_function(path, *(r_path+i),
                             *(r_path+i+1));

  if (*(r_path+j-1))
    c = path_weight_function(path, *(r_path+j-1),
                             *(r_path+j));
  if (*(r_path+j+1))
    d = path_weight_function(path, *(r_path+j),
                             *(r_path+j+1));
  path->cost_sum += (a+b+c+d);
}

/* Returns the city array. */
City** path_array(Path* path) {
  return path->r_path;
}

/* Returns the array of ids. */
int* path_ids(Path* path) {
  return path->ids;
}

/* Returns the array of randomized ids. */
int* path_ids_r(Path* path) {
  return path->ids_r;
}
