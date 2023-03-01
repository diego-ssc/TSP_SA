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
  int* city_n;
  int* ids;
  int* s;
  double* distances;
  double (*matrix)[CITY_NUMBER+1];
};

/* Creates a new Normalizer. */
Normalizer* normalizer_new(int city_n, int* ids,
                           double(*matrix)[CITY_NUMBER+1]) {
  Normalizer* normalizer = malloc(sizeof(struct _Normalizer));
  normalizer->city_n = calloc(1, sizeof(int));
  normalizer->distances = malloc(sizeof(double)*city_n);
  normalizer->ids = ids;
  normalizer->matrix = matrix;
  *(normalizer->city_n) += city_n;
  return normalizer;
}

/* Frees the memory used by the normalizer. */
void normalizer_free(Normalizer* normalizer) {
  if (normalizer->city_n)
    free(normalizer->city_n);
  if (normalizer->distances)
    free(normalizer->distances);
  free(normalizer);
}

static int fequal(const void* n, const void* m) {
  return fabs(*(double*)n-*(double*)m) < 0.00016;
}

/* Normalizes the provided number. */
double normalizer_normalize(Normalizer* normalizer) {
  int i;
  double sum = 0.0;
  for (i = 0; i+1< *normalizer->city_n; i++) {
    *(normalizer->distances + i) =
      *(*(normalizer->matrix+ *(normalizer->ids + i)) + *(normalizer->ids + i+1));
    printf("In distances: %.16f\n", *(normalizer->distances + i));
  } 
    
  qsort(normalizer->distances, *normalizer->city_n, sizeof(double), fequal);

  for (i = *normalizer->city_n -1; i > 0; i--)
    sum += *(normalizer->distances + i);

  return sum;
}
