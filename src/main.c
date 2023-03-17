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
  /* The temperature batch size. */
  int n_t;
} Data;

/**
 * Creates a new Data.
 * @param n the number of cities.
 * @param ids the ids.
 * @param seed the seed.
 * @param m The maximum number of executions
 * of a batch.
 * @param l The batch size.
 * @param t The temperature.
 * @param e The epsilon.
 * @param phi The phi.
 * @param a The temperature batch size.
 */
static Data* data_new(int n, int* ids, unsigned int seed,
                      int m, int l, long double t, double e,
                      double phi, double a, int v, int n_t) {
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
  data->n_t  = n_t;

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
                  data->e, data->phi, data->a, data->n_t);
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
          "\t\tSets the desired average of accepted cities.\n\n"
          "\t-k\n"
          "\t\tSets the desired temperature batch size.\n\n");
  fprintf(stderr, "Options:\n"
          "\t-s\n"
          "\t\tSets the initial seed the program will use.\n\n"
          "\t-n\n"
          "\t\tSets the number of seeds the program will use.\n\n"
          "\t-v\n"
          "\t\tPrints the evaluation for each path the program finds.\n\n"
          "\t-c\n"
          "\t\tSets the ids of the desired instance. It can be a file or"
          " a list of ids.\n\n"
          "\t-f\n"
          "\t\tParses a file which contains the parameters.\n\n");
  exit(1);
}

/**
 * Creates the requested number of threads
 * to execute the heuristic.
 * @param n the number of threads.
 * @param s the initial seed.
 * @param inst the TSP instance.
 * @param c the number of cities.
 * @param m The maximum number of exeuctions of a batch.
 * @param l The batch size.
 * @param t The temperature.
 * @param e The epsilon.
 * @param phi The phi.
 * @param a The average.
 * @param v The verbose option.
 * @param n_t the the temperature batch size.
 */
static void create_threads(int n, int s, int* inst, int c,
                           int m, int l, long double t, double e,
                           double phi, double a, int v, int n_t) {
  int i;
  pthread_t th[n];

  for (i = 0; i < n; ++i) {
    Data* data = data_new(c, inst, i+s, m, l, t, e, phi, a, v, n_t);
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

/* Parses the file that contains the cities. */
static int* parse_file(const char* file_name, int* size) {
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
  while (EOF != fscanf(file, "%d%*c", &id)) {
    *(ids + i) = id;
    i++;
  }
  fclose(file);

  return ids;
}

/* Parses the parameters written on the file. */
static void parse_parameters(const char* file_name, long double* t,
                             int* l, int* m, double* e, double* p,
                             double* a, int* s, int* n_t) {
  FILE* file = fopen(file_name, "r");
  if (!file) {
    perror("TSP_SA");
    exit(1);
  }

  char param[256];
  long double n;
  while(EOF != fscanf(file, "%s : %Lf", param, &n)) {
    switch (*param) {
    case 'T':
      *t = n;
      break;
    case 'L':
      *l = (int)n;
      break;
    case 'M':
      *m = (int)n;
      break;
    case 'E':
      *e = (double)n;
      break;
    case 'p':
      *p = (double)n;
      break;
    case 'P':
      *a = (double)n;
      break;
    case 'N':
      *n_t = (int)n;
      break;
    case 'S':
      *s = (int)n;
      break;
    default:
      break;
    }
  }

  fclose(file);
}


/* Parses the ids passed as arguments to the program. */
static int* parse_city_args(int argc, char **argv, int *size) {
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

/**
 * Parses the cities passed to the program.
 * @param argc the number of arguments.
 * @param argv the arguments.
 * @param size the number of cities.
 * @return the id array.
 */
static int* parse_cities(int argc, char **argv, int *size, int* status) {
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
  int c, m = 0, l = 0, size = 0, s = 0, v = 0,
    * ids = 0, cities = 0, n = 1, n_t = 0;
  long double t = 0.;
  double e = 0., phi = 0., a = 0.;
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
        case 'n':
          n = argc - 1 ? atoi(*(argv + 1)) : n;
          break;
        case 's':
          s = argc - 1 ? atoi(*(argv + 1)) : s;
          break;
        case 'v':
          v = 1;
          break;
        case 'c':
          ids = parse_cities(argc, argv, &size, &cities);
          break;
        case 'k':
          n_t = argc - 1 ? atoi(*(argv + 1)) : n_t;
          break;
        case 'f':
          parse_parameters(*(argv+1), &t, &l, &m, &e, &phi, &a, &s, &n_t);
          break;
        default:
          fprintf(stderr, "TSP_SA: illegal option %c\n", c);
          argc = 0;
          break;
        }
  if (!cities)
    usage();

  int procs = get_nprocs();
  int lower = n < procs ? n : procs;
  int x = ceil((double)n/lower);

  for(int i =0 ; i <size; ++i)
    if ((*(ids+i) <= 0) || (*(ids+i) > 1092)) {
      fprintf(stderr, "TSP_SA: Invalid path\n");
      exit(1);
    }

  while (x--)
    create_threads(lower, s, ids, size, m, l, t, e, phi, a, v, n_t);
  if (ids)
    free(ids);
}

/* Executes the main thread of the program. */
int main(int argc, char** argv) {
  parse_arguments(argc, argv);
  return 0;
}
