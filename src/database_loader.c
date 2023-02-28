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

#include <sqlite3.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "database_loader.h"

#define CITY_NUMBER 1092

/* The database loader structure. */
struct _Database_loader {
  City** cities;
  double (*connections)[CITY_NUMBER+1];
  sqlite3 *db;
  char *path;
  char *zErrMsg;
  char *sql;
  int rc;
  int *n;
  double *max_distance;
};

/* The callback data structure. */
typedef struct _Data {
  Database_loader* loader;
  void (*f)(Database_loader*,
            int*, char**);
} Data;

/* Creates a new Database Loader. */
Database_loader* loader_new() {
  Database_loader* loader = malloc(sizeof(struct _Database_loader));
  loader->cities = city_array(CITY_NUMBER+1);
  loader->connections = calloc(1, (CITY_NUMBER+1)*sizeof(double[CITY_NUMBER+1]));
  loader->n = calloc(1, sizeof(int));
  return loader;
}

/* Frees the memory used by the database loader. */
void loader_free(Database_loader* loader) {
  if (loader->cities)
    city_array_free(&(loader->cities), CITY_NUMBER+1);
  if (loader->connections)
    free(loader->connections);
  if (loader->path)
    free(loader->path);
  if (loader->n)
    free(loader->n);
  if (loader->zErrMsg)
    sqlite3_free(loader->zErrMsg);
  if (loader->db)
    sqlite3_close(loader->db);
  free(loader);
}

/* The callback function of the sql queries */
static int callback(void *data, int numCol,
                    char **colData, char **colName) {
  int i;

  Database_loader* loader = ((Data*)data)->loader;
  void(*f)(Database_loader*, int*, char**) = ((Data*)data)->f;
  i = 0;
  while (i < numCol)
    (*f)(loader, &i, colData);
  return 0;
}

/* Fills up the database loader cities array. */
static void fill_cities(Database_loader* loader,
                        int* i, char** data) {
  City* city = city_new(atoi(*(data+*i)), *(data+*i+1), *(data+*i+2),
                        atoi(*(data+*i+4)), atoi(*(data+*i+5)));
  city_array_set_element(&(loader->cities),&city, *loader->n + 1);
  *i += 6;
  ++*loader->n;
}

/* Fills up the database loader connections array. */
static void fill_connections(Database_loader* loader,
                             int* i, char** data) {
  *(*(loader->connections + atoi(*(data+*i))) + atoi(*(data+*i+1)))
    = atof(*(data+*i+2));
  *i += 3;
}

/* Opens the database. */
void loader_open(Database_loader* loader) {
  loader->path = realpath("./data/tsp.db", 0);

  if (access(loader->path, F_OK) == 0) {
    loader->rc = sqlite3_open(loader->path, &(loader->db));
    if (loader->rc) {
      fprintf(stderr, "Can't open database: %s\n",
              sqlite3_errmsg(loader->db));
      exit(1);
    }
  } else {
    fprintf(stderr, "Database not found\n");
  }
}

/* Loads the database cities into a two dimensional array. */
static void loader_load_cities(Database_loader* loader) {
  Data *data = malloc(sizeof(struct _Data));
  data->loader = loader;
  data->f = fill_cities;
  loader->sql = "SELECT * FROM cities;";
  loader->rc = sqlite3_exec(loader->db, loader->sql, callback,
                            data, &(loader->zErrMsg));
  if (loader->rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", loader->zErrMsg);
    sqlite3_free(loader->zErrMsg);
  }
  free(data);
}

/* Loads the database connections into a one dimensional array. */
static void loader_load_connections(Database_loader* loader) {
  Data *data = malloc(sizeof(struct _Data));
  data->loader = loader;
  data->f = fill_connections;
  loader->sql = "SELECT * FROM connections;";
  loader->rc = sqlite3_exec(loader->db, loader->sql, callback,
                            data, &(loader->zErrMsg));
  if (loader->rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", loader->zErrMsg);
    sqlite3_free(loader->zErrMsg);
  }
  free(data);
}

/* Loads the database. */
void loader_load(Database_loader* loader) {
  loader_load_cities(loader);
  *loader->n -= *loader->n;
  loader_load_connections(loader);
}

/* Returns the adjacency matrix of the loader. */
double (*loader_adj_matrix(Database_loader* loader))[CITY_NUMBER+1] {
  return loader->connections;
}

/* Returns the array of cities of the loader. */
City** loader_cities(Database_loader* loader) {
  return loader->cities;
}
