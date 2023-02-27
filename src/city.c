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
#include <math.h>

#include "city.h"

#define EARTH_RADIUS 6373000
#define _USE_MATH_DEFINES

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
  city->id = id;
  city->x = x;
  city->y = y;
  city->country = malloc(strlen(country)+1);
  strcpy(city->country, country);
  city->name = malloc(strlen(name)+1);
  strcpy(city->name, name);
  return city;
}

/* Frees the memory used by the city. */
void city_free(City* city) {
  if (city->name)
    free(city->name);
  if (city->country)
    free(city->country);
  free(city);
}

/* Returns the id of the city. */
int city_id(City* city) {
  return city->id;
}

/* Returns the x coordinate of the city. */
int city_x_coordinate(City* city) {
  return city->x;
}

/* Returns the y coordinate of the city. */
int city_y_coordinate(City* city) {
  return city->y;
}

/* Returns the name of the city. */
char* city_country(City* city) {
  return city->country;
}

/* Returns the name of the city. */
char* city_name(City* city) {
  return city->name;
}

/* Computes the distance between two cities. */
float city_distance(City* c_1, City* c_2) {
  float x_1 = (c_1->x)*M_PI/180;
  float x_2 = (c_2->x)*M_PI/180;
  float y_1 = (c_1->y)*M_PI/180;
  float y_2 = (c_2->y)*M_PI/180;

  float a = pow(sin((y_2 - y_1)/2),2)
    + cos(y_1) * cos(y_2)
    * pow(sin((x_2 - x_1)/2),2);
  float c = 2 * atan2(sqrt(a), sqrt(1-a));

  return EARTH_RADIUS * c;
}

/* Returns an array of cities. */
City** city_array(int n) {
  return malloc(n * sizeof(City*));
}

/* Frees the memory used by the array of cities. */
void city_array_free(City*** cities, int n) {
  int a = n;
  while ((n--))
    city_free(*((*cities) + a-(n+1)));
  free(*cities);
}

/* Sets the i-th element of a cities array. */
void city_array_set_element(City*** city_1, City** city_2, int i) {
  *((*city_1) + i) = *city_2;
}
