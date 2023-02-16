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

/**
 * The City opaque structure.
 */
typedef struct _City City;

/**
 * The Database Loader opaque structure.
 */
typedef struct _Database_loader Database_loader;

/**
 * The Normalizer opaque structure.
 */
typedef struct _Normalizer Normalizer;

/**
 * The Report opaque structure.
 */
typedef struct _Report Report;

/**
 * The TSP opaque structure.
 */
typedef struct _TSP TSP;

#include "city.h"
#include "database_loader.h"
#include "normalizer.h"
#include "report.h"
