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

#include <string.h>
#include <stdlib.h>

#include "heuristic.h"
#include "report.h"

/* The report structure */
struct _Report {
  char* name;
  int seed;
  City* tour;
  int normalized_cost;
};

/* Creates a new report. */
Report* report_new(char* name, int seed) {
  Report* report = malloc(sizeof(struct _Report));
  report->name = malloc(strlen(name)+1);
  strcpy(report->name, name);
  report->seed = seed;
  return report;
}

/* Frees the memory used by the report. */
void report_free(Report* report) {
  if (report->name)
    free(report->name);
  free(report);
}
