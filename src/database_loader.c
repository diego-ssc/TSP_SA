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

/* The database loader structure. */
struct _Database_loader {
  City** cities;
  /* int connections[123403][123403]; */
  int connections[12][12];
  sqlite3 *db;
  char *path;
  char *zErrMsg;
  char *sql;
  int rc;
};

/* The callback data structure. */
typedef struct _Data {
  Database_loader* loader;
  void (*f)(Database_loader*,
            int*, int*,char**);
  int* j;
} Data;

/* Creates a new Database Loader. */
Database_loader* loader_new() {
  Database_loader* loader = malloc(sizeof(struct _Database_loader));
  loader->cities = city_array(1092);
  if (!loader)
    return 0;
  return loader;
}

/* Frees the memory used by the database loader. */
void loader_free(Database_loader* loader) {
  if (loader->zErrMsg)
    sqlite3_free(loader->zErrMsg);
  if (loader->db)
    sqlite3_free(loader->db);
  if (loader->cities)
    city_array_free(&(loader->cities), 1092);
  free(loader);
}

/* The callback function of the sql queries */
static int callback(void *data, int numCol,
                    char **colData, char **colName) {
  int i,*j;

  Database_loader* loader = ((Data*)data)->loader;
  printf("Dirección: %p\n", loader);
  void(*f)(Database_loader*, int*, int*, char**) = ((Data*)data)->f;
  j = ((Data*)data)->j;
  i = 0;
  while (i < numCol) {
    printf("%s : index = %d, %d\n", *(colData + i), i, *j);
    (*f)(loader, &i, j, colData);
    printf("A.%s : index = %d, %d\n", *(colData + i), i, *j);
  }
  return 0;
}

/* Fills up the database loader cities array. */
static void fill_cities(Database_loader* loader,
                        int* i, int* j, char** data) {
  printf("(%d, %s, %s, %d, %d)\n\n",
         atoi(*(data+*i)), *(data+*i+1), *(data+*i+2),
         atoi(*(data+*i+4)), atoi(*(data+*i+5)));
  City* city = city_new(atoi(*(data+*i)), *(data+*i+1), *(data+*i+2),
                        atoi(*(data+*i+4)), atoi(*(data+*i+5)));
  city_array_set_element(&loader->cities,&city, *j);
  printf("guardado en índice: %d; a la ciudad: %s; dentro del arreglo: %s\n",
         *j, city_name(city), city_name(*(loader->cities+*j)));
  (*i) += 6;
  (*j)++;
}

/* Fills up the database loader connections array. */
static void fill_connections(Database_loader* loader,
                             int* i, int* j, char** data) {
  *(*(loader->connections + atoi(*(data+*i))) + atoi(*(data+*i+1)))
    = atoi(*(data+*i+1));
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
void loader_load_cities(Database_loader* loader) {
  Data *data = malloc(sizeof(struct _Data));
  data->loader = loader;
  data->f = fill_cities;
  int n = 0;
  data->j = &n;
  loader->sql = "SELECT * FROM cities;";
  loader->rc = sqlite3_exec(loader->db, loader->sql, callback,
                            data, &(loader->zErrMsg));
  if (loader->rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", loader->zErrMsg);
    sqlite3_free(loader->zErrMsg);
  }
  /*   printf("name: %s\n",city_name(*(loader->cities+i))); */
  printf("Dirección final: %p\n", loader);
  for (int i = 0; i < 5; ++i)
    printf("guardado en índice: %d; a la ciudad: %s\n",
           i, city_name(*(loader->cities+i)));
  free(data);
}

/* Loads the database connections into a two dimensional array. */
void loader_load_connections(Database_loader* loader) {
  Data *data = malloc(sizeof(struct _Data));
  data->loader = loader;
  data->f = fill_connections;
  int n = 0;
  data->j = &n;
  loader->sql = "SELECT * FROM connections;";
  loader->rc = sqlite3_exec(loader->db, loader->sql, callback,
                            data, &(loader->zErrMsg));
  if (loader->rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", loader->zErrMsg);
    sqlite3_free(loader->zErrMsg);
  }
  free(data);
}
