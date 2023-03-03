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
#include <math.h>

#include <stdio.h>

#include "heuristic.h"
#include "normalizer.h"

#define CITY_NUMBER 1092

/* The normalizer structure */
struct _Normalizer {
  int n;
  int* ids;
  double* distances;
  Database_loader* loader;
};

/* Creates a new Normalizer. */
Normalizer* normalizer_new(int n, int* ids,
                           Database_loader* loader) {
  Normalizer* normalizer = malloc(sizeof(struct _Normalizer));
  normalizer->distances = malloc(sizeof(double)*n*(n-1)/2);
  normalizer->ids = ids;
  normalizer->loader = loader;
  return normalizer;
}

/* Frees the memory used by the normalizer. */
void normalizer_free(Normalizer* normalizer) {
  if (normalizer->distances)
    free(normalizer->distances);
  free(normalizer);
}

/* /\* Determines the order of double numbers. *\/ */
/* static int fequal(const void* n, const void* m) { */
/*   return *(double*)n-*(double*)m < 0; */
/* } */

/* Normalizes the provided number. */
double normalizer_normalize(Normalizer* normalizer) {
  /* int i,j,k=0,n=normalizer->n; */
  /* int* ids = normalizer->ids; */
  /* City** cities = loader_cities(normalizer_loader(normalizer)); */
  /* double (*m)[CITY_NUMBER + 1] = loader_adj_matrix */
  /*   (normalizer_loader(normalizer)); */
  /* double sum = 0.0; */

  /* for (i = 0; i < n; ++i) */
  /*   for (j = i+1; j < n; ++j) */
  /*     if (edge_exists(tsp, *(cities + *(ids+i)), *(cities + *(ids+j)))) { */
  /*       *(tsp->distances + k) =  *(*(m + *(ids+i))+ *(ids+j)); */
  /*       ++k; */
  /*     } */

  /* qsort(tsp->distances, tsp->n * (tsp->n-1)/2, sizeof(double), fequal); */
  /* for (i = 0; i < tsp->n-1; i++) */
  /*   sum += *(tsp->distances + i); */

  /* return sum; */
  return 0.0;
}
