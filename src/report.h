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
 * Creates a new report.
 * @param name the name.
 * @param seed the seed.
 */
Report* report_new(char* name, int seed);

/**
 * Frees the memory used by the report.
 * @param report the report.
 */
void report_free(Report* report);

/**
 * Returns the name of the report.
 * @param report the report.
 * @return the name of the report.
 */
char* report_name(Report* report);

/**
 * Returns the seed of the report.
 * @param report the report.
 * @return the seed of the report.
 */
int report_seed(Report* report);

/**
 * Returns the path of the report.
 * @param report the report.
 * @return the path of the report.
 */
City** report_path(Report* report);

/**
 * Returns the cost of the path's
 * report.
 * @param report the report.
 * @return the cost of the path's
 * report.
 */
int report_cost(Report* report);

/**
 * Sets the path of a report.
 * @param report the report.
 * @param path the path.
 */
void report_set_path(Report* report, City** path);

/**
 * Sets the normalized cost of the report.
 * @param report the report.
 * @param normalized_cost the normalized cost.
 */
void report_set_normalized_cost(Report* report, int normalized_cost);

/**
 * Prints the report's data.
 * @param report the report.
 */
void report_print(Report* report);
