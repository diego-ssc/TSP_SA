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

#include "city.h"

/* The city structure */
struct _City {
  int id;
  int x;
  int y;
  char* country;
  char* name;
};

/* Creates a new City. */
City* city_new(int id, char* name,
               char* country,
               int x, int y
               ) {
  City* city = malloc(sizeof(struct _City));
  if (!city)
    return 0;
  city->id = id;
  city->x = x;
  city->y = y;
  city->country = country;
  if (!city->country) {
    free(city);
    return 0;
  }
  city->name = name;
  if (!city->name) {
    free(city);
    return 0;
  }
  return city;
}

/* Frees the memory used by the city. */
void city_free(City* city) {
  if (city->country)
    free(city->country);
  if(city->name)
    free(city->name);
  free(city);
}

/* Computes the distance between two cities. */
float city_distance(City* c_1, City* c_2) {
  return 0.0;
}

/* Returns an array of cities. */
City* city_array(int n) {
  return malloc(n * sizeof(struct _City));;
}

/* Frees the memory used by the array of cities. */
void city_array_free(City* city, int n) {
  while(int a = n-- + 1) {
    printf("n: %d\n", n);
    printf("na: %d\n", n%a);
    city_free(city + n%a);
  }
  
}
