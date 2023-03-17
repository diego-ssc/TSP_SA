#include <glib.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "heuristic.h"

/* Test environment. */
typedef struct {
  int seed;
} Test_env;

/* Test environment constructor. */
static Test_env* test_env_new() {
  Test_env *test_env = malloc(sizeof(Test_env));
  if (!test_env)
    return 0;
  test_env->seed = time(0);
  srandom(test_env->seed);
  return test_env;
}

/* Test city. */
typedef struct {
  Database_loader* loader;
} Test_city;

/* Sets up a city test case. */
static void test_city_set_up(Test_city* test_city,
                             gconstpointer data) {
  test_city->loader = loader_new();
  loader_open(test_city->loader);
  loader_load(test_city->loader);
}

/* Tears down a city test case. */
static void test_city_tear_down(Test_city* test_city,
                                gconstpointer data) {
  if (test_city->loader)
    loader_free(test_city->loader);
}

/* Tests the natural distance between two cities. */
static void test_city_distance(Test_city* test_city,
                               gconstpointer data) {
  g_assert_cmpfloat_with_epsilon(city_distance(*(loader_cities(test_city->loader) + 1),
                                               *(loader_cities(test_city->loader) + 7)),
                                 *(*(loader_adj_matrix(test_city->loader)+ 1) + 7),0.016);
  g_assert_cmpfloat_with_epsilon(city_distance(*(loader_cities(test_city->loader) + 17),
                                               *(loader_cities(test_city->loader) + 559)),
                                 *(*(loader_adj_matrix(test_city->loader)+ 17) + 559),0.016);
  g_assert_cmpfloat_with_epsilon(city_distance(*(loader_cities(test_city->loader) + 1090),
                                               *(loader_cities(test_city->loader) + 1092)),
                                 *(*(loader_adj_matrix(test_city->loader)+ 1090) + 1092),0.016);
}

int main(int argc, char** argv) {
  setlocale(LC_ALL, "");
  g_test_init(&argc, &argv, NULL);

  Test_env *test_env = test_env_new();
  printf("Seed: %d", test_env->seed);

  g_test_add("/city/test_city_distance", Test_city, test_env,
             test_city_set_up,
             test_city_distance,
             test_city_tear_down);
  return g_test_run();
}
