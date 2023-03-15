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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "heuristic.h"

/* The Data structure. */
typedef struct {
  /* The number of cities. */
  int n;
  /* The array of seeds. */
  unsigned int seed;
  /* The array of ids. */
  int *ids;
} Data;

/**
 * Creates a new Data.
 * @param n the number of cities.
 * @param ids the ids.
 * @param seeds the seeds.
 */
static Data* data_new(int n, int* ids, unsigned int seed) {
  /* Heap allocation. */
  Data* data = malloc(sizeof(Data));
  data->ids  = calloc(1, sizeof(int)*n);

  /* Value copy. */
  data->n    = n;
  data->seed = seed;

  /* Heap initialization. */
  memcpy(data->ids, ids, sizeof(int)*n);
  return data;
}

/**
 * Frees the memory used by the Data.
 * @param data the data.
 */
static void data_free(Data* data) {
  if (data->ids)
    free(data->ids);
  free(data);
}

/**
 * Executes the heuristic with the tsp instance.
 * @param data the user data.
 */
static void* heuristic(void* v_data) {
  Data* data = (Data*)v_data;
  printf("Seed: %d\n", data->seed);
  TSP* tsp = tsp_new(data->n, data->ids, data->seed);
  SA* sa = sa_new(tsp, 0, 0., 0, 0., 0.);
  threshold_accepting(sa);
  sa_free(sa);
  tsp_free(tsp);
  data_free(v_data);
  return  0;
}

/* Prints the execution instructions of the program. */
static void usage() {
  fprintf(stderr, "Usage:\n\t./TSP_SA [tsp-parameters]"
          " [cities-ids] [options]\n\n");
  fprintf(stderr, "Parameters:\n"
          "\t-t --temperature\n"
          "\t\tSets the desired temperature for the execution.\n\n"
          "\t-m --maxiter\n"
          "\t\tSets the desired maximum number of iterations to find"
          " a proper batch.\n\n"
          "\t-l --batchsize\n"
          "\t\tSets the desired batch size.\n\n"
          "\t-e --epsilon\n"
          "\t\tSets the desired epsilon value.\n\n"
          "\t-p --phi\n"
          "\t\tSets the desired phi value.\n\n");
  fprintf(stderr, "Options:\n"
          "\t-s --seeds\n"
          "\t\tSets the number of seeds the program will use.\n\n"
          "\t-v --verbose\n"
          "\t\tPrints the evaluation for each path the program finds.\n\n"
          "\t-r --report\n"
          "\t\tGenerates a report of the results.\n\n");
  exit(1);
}

/**
 * Creates the requested number of threads
 * to execute the heuristic.
 * @param n the number of threads.
 * @param s the initial seed.
 * @inst the TSP instance.
 */
static void create_threads(int n, int s, int* inst) {
  int i;
  pthread_t th[n];

  for (i = 0; i < n; ++i) {
    Data* data = data_new(150, inst, i+s);
    if (pthread_create(th+i, NULL, heuristic, data)) {
      fprintf(stderr, "Thread could not be created.");
      exit(1);
    }
  }

  for (i = 0; i < n; ++i) {
    if(pthread_join(*(th+i), NULL)) {
      fprintf(stderr, "Thread could not be joined.");
      exit(1);
    }
  }
}

/* Parses the arguments passed to the program. */
void parse_arguments(int argc, char** argv) {
  if (argc < 3)
    usage();
}

/* Executes the main thread of the program. */
int main(int argc, char** argv) {
  int inst[] = {1,2,3,4,5,6,7,54,163,164,165,168,172,186,327,329,331,332,
                333,483,489,490,491,492,493,496,653,654,656,657,815,816,
                817,820,978,979,980,981,982,984};

  /* int inst_1[] = {483,979,186,980,327,164,331,984,491,492,489, */
  /*                 4,817,978,6,5,165,3,333,981,820,332,982,816, */
  /*                 7,654,490,653,2,656,1,168,657,815,496,172,163, */
  /*                 329,493,54}; */

  /* /\* int inst_1[] = {54,483,186,980,327,164,331,984,491,492,489,817,978,5,6,981, *\/ */
  /* /\*                 333,3,165,4,979,493,329,163,172,496,815,656,2,653,490,654, *\/ */
  /* /\*                 820,332,982,816,7,1,168, 657}; *\/ */
  /* TSP* tsp = tsp_new(40, inst_1, 2902); */
  /* SA* sa = sa_new(tsp, 0, 0., 0, 0., 0.); */
  /* printf("Original cost function: %0.16Lf\n%s\n\n", path_cost_function(tsp_path(tsp)), */
  /*        path_to_str(tsp_path(tsp))); */
  /* Path* best = sweep(sa); */
  /* printf("%s\nCost Function: %0.16Lf\n\n", path_to_str(best), */
  /*        path_cost_function(best)); */
  /* sa_free(sa); */
  /* tsp_free(tsp); */
  
  /* int inst_2[] = {483,54,186,980,327,164,331,984,491,492,489,4,817,978,6,5,165,3,333,981,820,332,982,816,7,654,490,653,2,656,1,168,657,815,496,172,163,329,493,979}; */
  /* TSP* tsp = tsp_new(40, inst_2, 2902); */

  /* Path* path = path_new(loader_cities(tsp_database_loader(tsp)), 40, inst_2, tsp_seed(tsp), */
  /*                       loader_adj_matrix(tsp_database_loader(tsp))); */
  /* printf("Norm: %.16f\n", path_normalize(path)); */
  /* printf("Sum: %.16Lf\n", path_sum(path)); */
  /* printf("Cost function: %.16Lf\n", path_cost_function(path)); */
  /* printf("S:    %s\n\n", path_to_str(path)); */
  /* tsp_free(tsp); */
  /* path_free(path); */



  
  /* int inst[] = {1,2,3,4,5,6,7,8,9,11,12,14,16,17,19,20,22,23,25,26,54,74,75, */
  /*     77,151,163,164,165,166,167,168,169,171,172,173,174,176,179,181, */
  /*     182,183,184,185,186,187,190,191,297,326,327,328,329,330,331,332, */
  /*     333,334,336,339,340,343,344,345,346,347,349,350,351,352,444,483, */
  /*     489,490,491,492,493,494,495,496,499,500,501,502,504,505,507,508, */
  /*     509,510,511,512,520,652,653,654,655,656,657,658,660,661,662,663, */
  /*     665,666,667,668,670,671,673,674,675,676,678,680,815,816,817,818, */
  /*     819,820,821,822,823,825,826,828,829,832,837,840,978,979,980,981, */
  /*     982,984,985,986,988,990,991,995,999,1001,1003,1037,1038,1073,1075}; */

  TSP* tsp = tsp_new(40, inst, 2902);
  SA* sa = sa_new(tsp, 0, 0., 0, 0., 0.);

  threshold_accepting(sa);
  sa_free(sa);
  tsp_free(tsp);
  return 0;
}
