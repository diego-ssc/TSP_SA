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

#include "database_loader.h"
#include "heuristic.h"

int main(int argc, char** argv) {
  /* Database_loader* loader = loader_new(); */
  /* loader_open(loader);  */
  /* /\* loader_load_connections(loader); *\/ */
  /* loader_load_cities(loader); */
  /* loader_free(loader); */

  City** cities = city_array(3);
  City * c1 = city_new(1, "hello", "H", 5, 4);
  City * c2 = city_new(2, "there", "T", 7, 45);
  City * c3 = city_new(3, "friend", "F", 74, 4);
  city_array_set_element(&cities, &c1, 0);
  city_array_set_element(&cities, &c2, 1);
  city_array_set_element(&cities, &c3, 2);
  city_array_free(&cities, 3);
}
