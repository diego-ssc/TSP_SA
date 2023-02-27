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
#include "normalizer.h"

/* The normalizer structure */
struct _Normalizer {
  /* The normalized number */
  int n;
};

/* Creates a new Normalizer. */
Normalizer* normalizer_new() {
  Normalizer* normalizer = malloc(sizeof(struct _Normalizer));
  return normalizer;
}

/* Frees the memory used by the normalizer. */
void normalizer_free(Normalizer* normalizer) {
  free(normalizer);
}

/* Normalizes the provided number. */
float normalizer_normalize(Normalizer* normalizer, float n) {
  return 0.0;
}
