#include <glib.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "heuristic.h"

/* Instance values. */
#define NUM_CITIES_1       40
#define NUM_CITIES_2       150
#define MAX_ID             1092
#define EVAL_40            4129508.339517763
#define EVAL_150           6249022.603226478
#define NORMALIZER_40      180088219.480000019073486
#define NORMALIZER_150     721914154.580000042915344
#define MAX_DISTANCE_40    4947749.059999999590218
#define MAX_DISTANCE_150   4979370.000000000000000000

/* Predefined instances. */
static int instance[2][150] = {
  {1,2,3,4,5,6,7,54,163,164,165,168,172,186,327,329,331,332,
   333,483,489,490,491,492,493,496,653,654,656,657,815,816,
   817,820,978,979,980,981,982,984},
  {1,2,3,4,5,6,7,8,9,11,12,14,16,17,19,20,22,23,25,26,54,74,75,
   77,151,163,164,165,166,167,168,169,171,172,173,174,176,179,181,
   182,183,184,185,186,187,190,191,297,326,327,328,329,330,331,332,
   333,334,336,339,340,343,344,345,346,347,349,350,351,352,444,483,
   489,490,491,492,493,494,495,496,499,500,501,502,504,505,507,508,
   509,510,511,512,520,652,653,654,655,656,657,658,660,661,662,663,
   665,666,667,668,670,671,673,674,675,676,678,680,815,816,817,818,
   819,820,821,822,823,825,826,828,829,832,837,840,978,979,980,981,
   982,984,985,986,988,990,991,995,999,1001,1003,1037,1038,1073,1075}
};

/* Test environment. */
typedef struct {
  int seed;
  Database_loader* loader;
} Test_env;

/* Test environment constructor. */
static Test_env* test_env_new() {
  Test_env *test_env = malloc(sizeof(Test_env));
  test_env->loader = loader_new();
  loader_open(test_env->loader);
  loader_load(test_env->loader);
  test_env->seed = time(0);//4
  srandom(test_env->seed);
  return test_env;
}

/* Test path. */
typedef struct {
  /* Database_loader* loader; */
  Path* path_40;
  Path* path_150;
} Test_path;

/* Sets up a city test case. */
static void test_path_set_up(Test_path* test_path,
                             gconstpointer data) {
  Database_loader* loader = ((Test_env*)data)->loader;
  test_path->path_40 = path_new(loader_cities(loader),
                                NUM_CITIES_1, (int*)instance,
                                loader_adj_matrix(loader));
  test_path->path_150 = path_new(loader_cities(loader),
                                 NUM_CITIES_2, (int*)instance[1],
                                 loader_adj_matrix(loader));
}

/* Tears down a city test case. */
static void test_path_tear_down(Test_path* test_path,
                                gconstpointer data) {
  if (test_path->path_40)
    path_free(test_path->path_40);
  if (test_path->path_150)
    path_free(test_path->path_150);
}

/* Tests the max distance in a TSP instance */
static void test_path_max_distance(Test_path* test_path,
                                   gconstpointer data) {
  g_assert_cmpfloat_with_epsilon(path_max_distance(test_path->path_40),
                                 MAX_DISTANCE_40,0.00016);
  g_assert_cmpfloat_with_epsilon(path_max_distance(test_path->path_150),
                                 MAX_DISTANCE_150,0.00016);
}

/* Tests the normalized cost of a tour. */
static void test_path_normalizer(Test_path* test_path,
                                 gconstpointer data) {
  g_assert_cmpfloat_with_epsilon(path_normalize(test_path->path_40),
                                 NORMALIZER_40, 0.00016);
  g_assert_cmpfloat_with_epsilon(path_normalize(test_path->path_150),
                                 NORMALIZER_150, 0.00016);
}

/* Tests the distance between two cities. */
static void test_path_cost_function(Test_path* test_path,
                                    gconstpointer data) {
  g_assert_cmpfloat_with_epsilon(path_cost_function(test_path->path_40),
                                 EVAL_40,0.00016);
  g_assert_cmpfloat_with_epsilon(path_cost_function(test_path->path_150),
                                 EVAL_150,0.00016);
}

/* The the path swapping. */
static void test_path_swap(Test_path* test_path,
                           gconstpointer data) {
  int n = NUM_CITIES_1, m = NUM_CITIES_2;
  path_randomize(test_path->path_40);
  g_assert_cmpfloat_with_epsilon(path_cost_function(test_path->path_40),
                                 path_cost_sum(test_path->path_40)/
                                 path_normalize(test_path->path_40),
                                 0.00016);
  while (n--) {
    path_swap(test_path->path_40);
    g_assert_cmpfloat_with_epsilon(path_cost_function(test_path->path_40),
                                   path_cost_sum(test_path->path_40)/
                                   path_normalize(test_path->path_40),
                                   0.00016);
  }
  path_randomize(test_path->path_150);
  g_assert_cmpfloat_with_epsilon(path_cost_function(test_path->path_150),
                                 path_cost_sum(test_path->path_150)/
                                 path_normalize(test_path->path_150),
                                 0.00016);
  while (m--) {
    path_swap(test_path->path_150);
    g_assert_cmpfloat_with_epsilon(path_cost_function(test_path->path_150),
                                   path_cost_sum(test_path->path_150)/
                                   path_normalize(test_path->path_150),
                                   0.00016);
  }
}

static void test_path_de_swap(Test_path* test_path,
                              gconstpointer data) {
  int n = NUM_CITIES_1, m = NUM_CITIES_2;
  Path* copy;
  path_randomize(test_path->path_40);
  while (n--) {
    copy = path_copy(test_path->path_40);
    path_swap(test_path->path_40);
    path_de_swap(test_path->path_40);
    g_assert(path_cmp(test_path->path_40, copy));
    path_free(copy);
  }

  path_randomize(test_path->path_150);
  while (m--) {
    copy = path_copy(test_path->path_150);
    path_swap(test_path->path_150);
    path_de_swap(test_path->path_150);
    g_assert(path_cmp(test_path->path_150, copy));
    path_free(copy);
  }
}

int main(int argc, char** argv) {
  setlocale(LC_ALL, "");
  g_test_init(&argc, &argv, NULL);

  Test_env* test_env = test_env_new();

  g_test_add("/path/test_path_max_distance", Test_path, test_env,
             test_path_set_up,
             test_path_max_distance,
             test_path_tear_down);
  g_test_add("/path/test_path_normalizer", Test_path, test_env,
             test_path_set_up,
             test_path_normalizer,
             test_path_tear_down);
  g_test_add("/path/test_path_cost_function", Test_path, test_env,
             test_path_set_up,
             test_path_cost_function,
             test_path_tear_down);
  g_test_add("/path/test_path_swap", Test_path, test_env,
             test_path_set_up,
             test_path_swap,
             test_path_tear_down);
  g_test_add("/path/test_path_de_swap", Test_path, test_env,
             test_path_set_up,
             test_path_de_swap,
             test_path_tear_down);
  return g_test_run();
}
