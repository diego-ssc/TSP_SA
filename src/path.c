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
#include <stdio.h>

#include "heuristic.h"
#include "path.h"

/* The path structure. */
struct _Path {
  /* The arrays of cities. */
  City** cities, **r_path;
  /* The number of cities. */
  int n;
  /* The ids of the city. */
  int* ids;
  /* The distances between every city in
     the path. */
  double* distances;
  /* The sum of the costs of the cities. */
  long double cost_sum;
  /* The maximum distance. */
  double max_distance;
  /* The normalizer. */
  double normalized_v;
  /* The adjacency matrix. */
  double (*matrix)[CITY_NUMBER+1];
  /* The indexes with which a swap has been made*/
  int i,j;
  /* The string representation. */
  char* str;
  /* The seed. */
  unsigned int seed;
};

/* Fills the path array. */
static void fill_path_array(Path*);

/* Computes the maximum distance of the path. */
static double c_path_max_distance(Path*);

/* Tells if the edge in the graph exists. */
static int edge_exists(Path*, City*, City*);

/* Determines the order of double numbers. */
static int fequal(const void*, const void*);

/* Computes the normalized path weights. */
static double c_path_normalize(Path*);

/* Computes the random indexes used by swap function. */
static void random_indexes(Path*);

/* Computes the path swapping. */
static void c_path_swap(Path*);

/* Copies the ids of one path to another. */
static void copy_ids(Path*, int*);

/* Creates a new Path. */
Path* path_new(City** cities, int n, int* ids,
               unsigned int seed, double (*matrix)[CITY_NUMBER+1]) {
  /* Heap allocated. */
  Path* path      = malloc(sizeof(struct _Path));
  path->distances = calloc(1,sizeof(double)*n*(n-1)/2);
  path->str       = malloc(sizeof(int)*n*2+2);
  path->r_path    = city_array(n);
  path->ids       = calloc(1, sizeof(int)*n);

  /* Pointer copy. */
  path->cities = cities;
  path->n      = n;
  path->matrix = matrix;
  path->seed   = seed;


  /* Heap memory intialization. */
  copy_ids(path, ids);
  fill_path_array(path);

  /* Linear operations. */
  path->max_distance = c_path_max_distance(path);
  path->cost_sum     = path_cost_sum(path);
  path->normalized_v = c_path_normalize(path);

  return path;
}

/* Frees the memory used by the path. */
void path_free(Path* path) {
  if (path->r_path)
    free(path->r_path);
  if (path->distances)
    free(path->distances);
  if (path->str)
    free(path->str);
  if (path->ids)
    free(path->ids);
  free(path);
}

/* Computes the weight of an edge between two cities. */
double path_weight_function(Path* path, City* c_1, City* c_2) {
  return edge_exists(path, c_1, c_2) ?
    *(*(path->matrix + city_id(c_1))+city_id(c_2))
    : city_distance(c_1, c_2) * path_max_distance(path);
}

/* Computes the sum of the costs. */
double path_cost_sum(Path* path) {
  int* ids = path->ids;
  City** cities = path->cities;
  int i;
  double cost = 0.0;
  int n = path->n;
  for (i = 0; i+1 < n; ++i)
    cost += path_weight_function(path, *(cities + *(ids+i)),
                                 *(cities + *(ids+i+1)));
  return cost;
}

/* Normalizes the path weights. */
double path_normalize(Path* path) {
  return path->normalized_v;
}

/* Computes the cost function. */
long double path_cost_function(Path* path) {
  return path->cost_sum/path->normalized_v;
}

/* Randomizes the initial path. */
void path_randomize(Path* path) {
  int i, temp, r, n = path->n;
  int *ids_r = path->ids;
  City** cities = path->cities;

  for (i = 0; i < n; ++i) {
    r = rand_r(&path->seed) % n;
    temp = *(ids_r + r);
    *(ids_r + r) = *(ids_r + i);
    *(ids_r + i) = temp;
  }
  for (i = 0; i < n; ++i)
    *(path->r_path+i) = *(cities + *(ids_r + i));

  path->cost_sum = path_cost_sum(path);
}

/* Swaps two cities in the path. */
void path_swap(Path* path) {
  random_indexes(path);
  c_path_swap(path);
}

/* Swaps two cities in the path. */
void path_de_swap(Path* path) {
  c_path_swap(path);
}

/* Swaps the desired indexes in the path. */
void path_swap_indexes(Path* path, int i, int j) {
  path->i = i < j ? i : j;
  path->j = j > i ? j : i;
  c_path_swap(path);
}

/* Computes the path swapping. */
static void c_path_swap(Path* path) {
  City** r_path = path->r_path, *temp;
  int i = path->i;
  int j = path->j;
  int temp_i, n = path->n;
  int* ids_r = path->ids;
  long double a = 0., b = 0., c = 0., d = 0.;

  if (i-1 >= 0)
    a = path_weight_function(path, *(r_path+i-1),
                             *(r_path+i));
  if (i+1 < n)
    b = path_weight_function(path, *(r_path+i),
                             *(r_path+i+1));

  if (j-1 >= 0)
    c = path_weight_function(path, *(r_path+j-1),
                             *(r_path+j));
  if (j+1 < n)
    d = path_weight_function(path, *(r_path+j),
                             *(r_path+j+1));

  if (abs(i-j) == 1)
    b = 0.;

  path->cost_sum -= (a+b+c+d);

  a = 0., b = 0., c = 0., d = 0.;
  temp = *(r_path + i);
  temp_i = *(ids_r + i);
  *(r_path + i) = *(r_path + j);
  *(ids_r + i) = *(ids_r + j);

  *(r_path + j) = temp;
  *(ids_r + j) = temp_i;
  if (i-1 >= 0)
    a = path_weight_function(path, *(r_path+i-1),
                             *(r_path+i));
  if (i+1 < n)
    b = path_weight_function(path, *(r_path+i),
                             *(r_path+i+1));

  if (j-1 >= 0)
    c = path_weight_function(path, *(r_path+j-1),
                             *(r_path+j));
  if (j+1 < n)
    d = path_weight_function(path, *(r_path+j),
                             *(r_path+j+1));

  if (abs(i-j) == 1)
    b = 0.;

  path->cost_sum += (a+b+c+d);
}

/* Computes the random indexes used by swap function. */
static void random_indexes(Path* path) {
  path->i = 0;
  path->j = 0;
  while (path->i == path->j)
    path->i = rand_r(&path->seed)%(path->n), path->j = rand_r(&path->seed)%(path->n);
  path->i = path->i < path->j ? path->i : path->j;
  path->j = path->i > path->j ? path->i : path->j;
}

/* Returns the city array. */
City** path_array(Path* path) {
  return path->r_path;
}

/* Returns the sum of the costs of the cities. */
long double path_sum(Path* path) {
  return path->cost_sum;
}

/* Returns the array of ids. */
int* path_ids(Path* path) {
  return path->ids;
}

/* Returns the maximum distance of the path. */
double path_max_distance(Path* path) {
  return path->max_distance;
}

/* Fills the path array. */
static void fill_path_array(Path* path) {
  int *ids = path->ids;
  int i;
  for (i = 0; i < path->n; ++i)
    *(path->r_path + i) = *(path->cities + *(ids + i));
}

/* Tells if the edge in the graph exists. */
static int edge_exists(Path* path, City* c_1, City* c_2) {
  return (*(*(path->matrix + city_id(c_1))+city_id(c_2))) != 0.0;
}

/* Computes the maximum distance of the path. */
static double c_path_max_distance(Path* path) {
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

/* Determines the order of double numbers. */
static int fequal(const void* n, const void* m) {
  return *(double*)n-*(double*)m < 0;
}

/* Computes the normalized path weights. */
static double c_path_normalize(Path* path) {
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

/* Returns the number of cities in the path. */
int path_n(Path* path) {
  return path->n;
}

/* Returns a neighbour of the path. */
Path* path_neighbour(Path* path) {
  path_swap(path);
  return path;
}

/* Returns a copy of the path. */
Path* path_copy(Path* path) {
  Path* copy      = malloc(sizeof(struct _Path));
  copy->distances = calloc(1,sizeof(double)*path->n*(path->n-1)/2);
  copy->str       = malloc(sizeof(int)*path->n*2+2);
  copy->r_path    = city_array(path->n);
  copy->ids       = calloc(1, sizeof(int)*path->n);

  /* Pointer copy. */
  copy->cities = path->cities;
  copy->n      = path->n;
  copy->seed   = path->seed;
  copy->matrix = path->matrix;

  /* Value copy. */
  copy->max_distance = path->max_distance;
  copy->cost_sum     = path->cost_sum;
  copy->normalized_v = path->normalized_v;

  /* Heap memory intialization. */
  copy_ids(copy, path->ids);
  fill_path_array(copy);

  return copy;
}

/* Copies the ids of one path to another. */
static void copy_ids(Path* path, int* ids) {
  for (int i = 0; i < path->n; ++i)
    *(path->ids + i) = *(ids + i);
}

/* Returns if the paths are equal. */
int path_cmp(Path* p_1, Path* p_2) {
  if (!p_1 || !p_2)
    return 0;
  if (p_1->n != p_2->n)
    return 0;
  if (abs(p_1->cost_sum - p_2->cost_sum) >= 0.00016)
    return 0;
  if (abs(p_1->max_distance - p_2->max_distance) >= 0.00016)
    return 0;
  if (abs(p_1->normalized_v - p_2->normalized_v) >= 0.00016)
    return 0;
  int i;
  for(i = 0; i < p_1->n; ++i) {
    if (*(p_1->ids+i) != *(p_2->ids+i))
      return 0;
    if (!city_cmp(*(p_1->r_path+i),
                  *(p_2->r_path+i)))
      return 0;
  }
  return 1;
}

/* Returns the string representation of the path */
char* path_to_str(Path* path) {
  int i;
  char* str = path->str;
  City** cities = path->r_path;
  sprintf(str, "%s", "[");
  for (i = 0; i < path->n; ++i) {
    sprintf(&str[strlen(str)], "%d", city_id(*(cities+i)));
    if (i+1 < path->n)
      sprintf(&str[strlen(str)], "%s", ",");
  }
  strcat(str, "]");
  return str;
}
