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
  City** matrix;
  sqlite3 *db;
  char *path;
  char *zErrMsg;
  char *sql;
  int rc;
};

/* Creates a new Database Loader. */
Database_loader* loader_new() {
  Database_loader* loader = malloc(sizeof(struct _Database_loader));
  if (!loader)
    return 0;
  return loader;
}

/* Frees the memory used by the database loader. */
void loader_free(Database_loader* loader) {
  if (loader->matrix)
    free(loader->matrix);
  if (loader->zErrMsg)
    sqlite3_free(loader->zErrMsg);
  if (loader->db)
    sqlite3_free(loader->db);
  free(loader);
}

/* The callback function of the sql queries */
static int callback(void *loader, int numCol,
                    char **colData, char **colName) {
  int i;
  for (i = 0; i < numCol; i++) {
    /* callback_str += colData[i]; */
    /* callback_str += "\n"; */
    printf("%s\n", *(colData + i));
  }
  /* ((Database_loader*)loader)->matrix = colData; */
  return 0;
}

/* Opens the database. */
void loader_open(Database_loader* loader) {
  loader->path = realpath("./data/tsp.db", 0);

  if (access(loader->path, F_OK) == 0) {
    loader->rc = sqlite3_open(loader->path, &(loader->db));
    if (loader->rc) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(loader->db));
      exit(1);
    }
  } else {
    fprintf(stderr, "Database not found\n");
  }
}

/* Loads the database cities into a two dimensional array. */
void loader_load_cities(Database_loader* loader) {
  loader->sql = "SELECT * FROM cities;";
  loader->rc = sqlite3_exec(loader->db, loader->sql, callback, loader, &(loader->zErrMsg));
  if (loader->rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", loader->zErrMsg);
    sqlite3_free(loader->zErrMsg);
  }
}

/* Loads the database connections into a two dimensional array. */
void loader_load_connections(Database_loader* loader) {
  loader->sql = "SELECT * FROM connections;";
  loader->rc = sqlite3_exec(loader->db, loader->sql, callback, loader, &(loader->zErrMsg));
  if (loader->rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", loader->zErrMsg);
    sqlite3_free(loader->zErrMsg);
  }
}
