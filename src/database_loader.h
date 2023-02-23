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
 * Loads the database cities into a two dimensional
 * array.
 * @param loader the database loader.
 */
void loader_load_cities(Database_loader* loader);

/**
 * Loads the database connections into a two dimensional
 * array.
 * @param loader the database loader.
 */
void loader_load_connections(Database_loader* loader);

/**
 * Returns the two dimensional array of the loader.
 * @return the two dimensional array of the loader.
 */
City** loader_matrix(Database_loader* loader);
