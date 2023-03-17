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
#include <sys/sysinfo.h>
#include <math.h>
#include <time.h>

#include "heuristic.h"

/* The Data structure. */
typedef struct {
  /* The number of cities. */
  int n;
  /* The array of seeds. */
  unsigned int seed;
  /* The array of ids. */
  int *ids;
  /* The maximum number of exeuctions
     of a batch. */
  int m;
  /* The batch size. */
  int l;
  /* The temperature. */
  long double t;
  /* The epsilon. */
  double e;
  /* The phi. */
  double phi;
  /* The average. */
  double a;
  /* The verbose option. */
  int v;
} Data;

/**
 * Creates a new Data.
 * @param n the number of cities.
 * @param ids the ids.
 * @param seed the seed.
 * @param m The maximum number of exeuctions
 * of a batch.
 * @param l The batch size.
 * @param t The temperature.
 * @param e The epsilon.
 * @param phi The phi.
 * @param a The average.
 */
static Data* data_new(int n, int* ids, unsigned int seed,
                      int m, int l, long double t, double e,
                      double phi, double a, int v) {
  /* Heap allocation. */
  Data* data = malloc(sizeof(Data));
  data->ids  = calloc(1, sizeof(int)*n);

  /* Value copy. */
  data->n    = n;
  data->seed = seed;
  data->t    = t;
  data->m    = m;
  data->l    = l;
  data->e    = e;
  data->phi  = phi;
  data->a    = a;
  data->v    = v;

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
  TSP* tsp = tsp_new(data->n, data->ids, data->seed);
  SA* sa = sa_new(tsp, data->t, data->m, data->l,
                  data->e, data->phi, data->a);
  sa_set_temperature(sa, initial_temperature(sa));
  threshold_accepting(sa);
  sa_free(sa);
  tsp_free(tsp);
  data_free(v_data);
  return 0;
}

/* Prints the execution instructions of the program. */
static void usage() {
  fprintf(stderr, "Usage:\n\t./TSP_SA [tsp-parameters]"
          " [cities-ids] [options]\n\n");
  fprintf(stderr, "Parameters:\n"
          "\t-t\n"
          "\t\tSets the desired temperature for the execution.\n\n"
          "\t-m\n"
          "\t\tSets the desired maximum number of iterations to find"
          " a proper batch.\n\n"
          "\t-l\n"
          "\t\tSets the desired batch size.\n\n"
          "\t-e\n"
          "\t\tSets the desired epsilon value.\n\n"
          "\t-p\n"
          "\t\tSets the desired phi value.\n\n"
          "\t-a\n"
          "\t\tSets the desired average of accepted cities.\n\n");
  fprintf(stderr, "Options:\n"
          "\t-s\n"
          "\t\tSets the number of seeds the program will use.\n\n"
          "\t-v\n"
          "\t\tPrints the evaluation for each path the program finds.\n\n"
          "\t-c\n"
          "\t\tSets the ids of the desired instance. It can be a file or"
          " a list of ids.\n\n");
  exit(1);
}

/**
 * Creates the requested number of threads
 * to execute the heuristic.
 * @param n the number of threads.
 * @param s the initial seed.
 * @param inst the TSP instance.
 * @param c the number of cities.
 * @param m The maximum number of exeuctions
 * of a batch.
 * @param l The batch size.
 * @param t The temperature.
 * @param e The epsilon.
 * @param phi The phi.
 * @param a The average.
 * @param v The verbose option.
 */
static void create_threads(int n, int s, int* inst, int c,
                           int m, int l, long double t, double e,
                           double phi, double a, int v) {
  int i;
  pthread_t th[n];

  for (i = 0; i < n; ++i) {
    Data* data = data_new(c, inst, i+s, m, l, t, e, phi, a, v);
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

int* parse_file(const char* file_name, int* size) {
  int* ids, id, i = 0;
  char c;

  FILE* file = fopen(file_name, "r");
  if (!file) {
    perror("TSP_SA");
    exit(1);
  }


  while ((c = getc(file)) != EOF)
    if (c == ',')
      ++*size;

  if (*size)
    ++*size;

  ids = calloc(1, sizeof(int)*(*size));
  fclose(file);
  file = fopen(file_name, "r");
  while (EOF != fscanf(file, "%d%c", &id, &c)) {
    *(ids + i) = id;
    i++;
  }
  fclose(file);

  return ids;
}

int* parse_city_args(int argc, char **argv, int *size) {
  int i;
  for (i = 1; i < argc; i++, ++*size)
    if (*(argv + i)[0] == '-')
      break;

  int *ids = calloc(1, sizeof(int) * (*size));

  for (i = 0; i+1 < argc; i++) {
    if (*(argv + i)[0] == '-')
      break;
    *(ids + i) = atoi(*(argv + i+1));
  }

  return ids;
}

int* parse_cities(int argc, char **argv, int *size, int* status) {
  if ((argc - 1) > 0 && *(argv + 1)[0] != '-') {
    *status = 1;
    if ((argc - 2) > 0 && *(argv + 2)[0] != '-') {
      return parse_city_args(argc, argv, size);
    } else {
      return parse_file(*(argv+1), size);
    }
  }
  return 0;
}

/* Parses the arguments passed to the program. */
void parse_arguments(int argc, char** argv) {
  if (argc < 3)
    usage();
  int c, m = 0, l = 0, size = 0, s = 1, v = 0,
    * ids = 0, cities = 0;
  long double t = 0.;
  double e = 0., phi = 0., a = 0.;


  printf("First arg: %s\n", *argv);
  
  while (--argc > 0)
    if ((*++argv)[0] == '-')
      while ((c = *++argv[0]))
        switch (c) {
        case 't':
          t = argc - 1 ? atof(*(argv + 1)) : t;
          break;
        case 'm':
          m = argc - 1 ? atoi(*(argv + 1)) : m;
          break;
        case 'l':
          l = argc - 1 ? atoi(*(argv + 1)) : l;
          break;
        case 'e':
          e = argc - 1 ? atof(*(argv + 1)) : e;
          break;
        case 'p':
          phi = argc - 1 ? atof(*(argv + 1)) : phi;
          break;
        case 'a':
          a = argc - 1 ? atof(*(argv + 1)) : a;
          break;
        case 's':
          printf("Hello: %s\n", *(argv + 1));
          s = argc - 1 ? atoi(*(argv + 1)) : s;
          break;
        case 'v':
          v = 1;
          break;
        case 'c':
          ids = parse_cities(argc, argv, &size, &cities);
          break;
        default:
          printf("TSP_SA: illegal option %c\n", c);
          argc = 0;
          break;
        }

  if (!cities)
    usage();

  int procs = get_nprocs();
  printf("procs: %d\n", procs);
  printf("s: %d\n", s);
  int n = s < procs ? s : procs;
  int x = ceil((double)s/n);

  for(int i =0 ; i <size; ++i)
    if ((*(ids+i) <= 0) || (*(ids+i) > 1092)) {
      fprintf(stderr, "TSP_SA: Invalid file format\n");
      exit(1);
    }

  printf("THread number: %d\n", n);
  srand(time(0));
  while (x--)
    create_threads(n, random(), ids, size, m, l, t, e, phi, a, v);
  if (ids)
    free(ids);
}

/* Executes the main thread of the program. */
int main(int argc, char** argv) {
  /* int inst[] = {1,2,3,4,5,6,7,54,163,164,165,168,172,186,327,329,331,332, */
  /*               333,483,489,490,491,492,493,496,653,654,656,657,815,816, */
  /*               817,820,978,979,980,981,982,984}; */

  /* int inst[] = {1,2,3,4,5,6,7,8,9,11,12,14,16,17,19,20,22,23,25,26,54,74,75, */
  /*     77,151,163,164,165,166,167,168,169,171,172,173,174,176,179,181, */
  /*     182,183,184,185,186,187,190,191,297,326,327,328,329,330,331,332, */
  /*     333,334,336,339,340,343,344,345,346,347,349,350,351,352,444,483, */
  /*     489,490,491,492,493,494,495,496,499,500,501,502,504,505,507,508, */
  /*     509,510,511,512,520,652,653,654,655,656,657,658,660,661,662,663, */
  /*     665,666,667,668,670,671,673,674,675,676,678,680,815,816,817,818, */
  /*     819,820,821,822,823,825,826,828,829,832,837,840,978,979,980,981, */
  /*     982,984,985,986,988,990,991,995,999,1001,1003,1037,1038,1073,1075}; */

  //0.1528316180717023 256-512
/*   #define T        8 */
/* #define M        260000 */
/* #define L        12000//9000//1800 */
/* #define EPSILON  0.000016 */
/* #define PHI      0.98 */
/* #define P        0.98//0.98 */
/* #define N        1000 */
  /* 0.1524116513820390 256-512 */
/*   #define T        8 */
/* #define M        260000 */
/* #define L        12000//9000//1800 */
/* #define EPSILON  0.000016 */
/* #define PHI      0.98 */
/* #define P        0.98//0.98 */
/* #define N        900 */

  /* int s = 256; */
  /* int n = 8; */
  /* int t = 32; */
  /* while (n--) { */
  /*   create_threads(t, s, inst, 150); */
  /*   s += t; */
  /* } */

  parse_arguments(argc, argv);

  return 0;
}
