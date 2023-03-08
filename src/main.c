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

#include <stdio.h>

#include "heuristic.h"

int main(int argc, char** argv) {
  int inst[] = {1,2,3,4,5,6,7,54,163,164,165,168,172,186,327,329,331,332,
                333,483,489,490,491,492,493,496,653,654,656,657,815,816,
                817,820,978,979,980,981,982,984};
  TSP* tsp = tsp_new(40, inst, 4444);
  /* SA* sa = sa_new(tsp, 0, 0., 0, 0., 0.); */
  Path* path = path_new(loader_cities(tsp_database_loader(tsp)), 40, inst,
                        loader_adj_matrix(tsp_database_loader(tsp)));
  printf("Path cost function: %.16f\n", path_cost_function(path));
  printf("%s\n", path_to_str(path));
  path_swap(path);
  printf("%s\n", path_to_str(path));
  path_de_swap(path);
  printf("%s\n", path_to_str(path));
  path_free(path);
  /* threshold_accepting(sa); */
  /* sa_free(sa); */
  tsp_free(tsp);
  return 0;
}
