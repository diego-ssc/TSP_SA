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

#pragma once

#include "heuristic.h"

/**
 * Creates a new Database Loader.
 */
Database_loader* loader_new();

/**
 * Frees the memory used by the database loader.
 * @param loader the database loader to be freed.
 */
void loader_free(Database_loader* loader);

/**
 * Opens the database.
 * @param loader the database loader.
 */
void loader_open(Database_loader*);

/**
 * Opens the database. [Testing purpose]
 * @param loader the database loader.
 * @param path the database path.
 */
void loader_open_t(Database_loader* loader, char* path);

/**
 * Loads the database.
 * @param loader the database loader.
 */
void loader_load(Database_loader* loader);

/**
 * Returns the adjacency matrix of the loader.
 * @return the adjacency matrix of the loader.
 */
double (*loader_adj_matrix(Database_loader* loader))[1092+1];

/**
 * Returns the array of cities of the loader.
 * @return the array of cities of the loader.
 */
City** loader_cities(Database_loader* loader);
