#include <glib.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "heuristic.h"

#define MAX_ID             1092
#define EVAL_40            4129508.339517763
#define EVAL_150           6249022.603226478
#define NORMALIZER_40      180088219.480000019073486
#define NORMALIZER_150     721914154.580000042915344
#define MAX_DISTANCE_40    4947749.059999999590218
#define MAX_DISTANCE_150   4979370.000000000000000000

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
  Normalizer* normalizer;
  TSP* tsp_40;
  TSP* tsp_150;
} Test_city;

/* Sets up a city test case. */
static void test_city_set_up(Test_city* test_city,
                             gconstpointer data) {
  test_city->normalizer = normalizer_new();
  test_city->tsp_40 = tsp_new(40, (int*)instance);
  test_city->tsp_150 = tsp_new(150, (int*)instance[1]);
  test_city->loader = loader_new();
  loader_open(test_city->loader);
  loader_load(test_city->loader);
}

/* Tears down a city test case. */
static void test_city_tear_down(Test_city* test_city,
                                gconstpointer data) {
  if (test_city->loader)
    loader_free(test_city->loader);
  if (test_city->normalizer)
    free(test_city->normalizer);
  if (test_city->tsp_40)
    free(test_city->tsp_40);
  if (test_city->tsp_150)
    free(test_city->tsp_150);
}

/* Tests the max distance between in a TSP instance */
static void test_city_max_distance(Test_city* test_city,
                               gconstpointer data) {
  g_assert_cmpfloat_with_epsilon(tsp_max_distance(test_city->tsp_40),
                                 MAX_DISTANCE_40,0.00016);
  g_assert_cmpfloat_with_epsilon(tsp_max_distance(test_city->tsp_150),
                                 MAX_DISTANCE_150,0.00016);
}

/* Tests the distance between two cities */
static void test_city_cost(Test_city* test_city,
                           gconstpointer data) {
  City** cities = loader_cities(test_city->loader);
  City** cities_1 = city_array(40);
  City** cities_2 = city_array(150);
  TSP* tsp_40 = test_city->tsp_40;
  TSP* tsp_150 = test_city->tsp_150;
  int i;
  for (i = 0; i < 40; ++i)
    *(cities_1 + i) = *(cities + (instance[0][i])-1);

  g_assert_cmpfloat_with_epsilon(tsp_tour_cost(tsp_40, cities_1),
                                 EVAL_40,0.00016);
  for (i = 0; i < 150; ++i)
    *(cities_2 + i) = *(cities + (instance[1][i])-1);

  g_assert_cmpfloat_with_epsilon(tsp_tour_cost(tsp_150, cities_2),
                                 EVAL_150,0.00016);
  city_array_free(&cities_1,40);
  city_array_free(&cities_2,150);
}

/* Tests the normalized cost of a tour. */
static void test_city_normalizer(Test_city* test_city,
                                 gconstpointer data) {
  Normalizer* normalizer = test_city->normalizer;
  TSP* tsp_40 = test_city->tsp_40;
  TSP* tsp_150 = test_city->tsp_150;
  City** cities = loader_cities(test_city->loader);
  City** cities_1 = city_array(40);
  City** cities_2 = city_array(150);
  int i;
  for (i = 0; i < 40; ++i)
    *(cities_1 + i) = *(cities + (instance[0][i])-1);
  for (i = 0; i < 150; ++i)
    *(cities_2 + i) = *(cities + (instance[1][i])-1);
  float n = normalizer_normalize(normalizer, tsp_tour_cost(tsp_40, cities_1));

  g_assert_cmpfloat_with_epsilon(n, NORMALIZER_40, 0.00016);

  n = normalizer_normalize(normalizer, tsp_tour_cost(tsp_150, cities_1));

  g_assert_cmpfloat_with_epsilon(n, NORMALIZER_150, 0.00016);

  city_array_free(&cities_1,40);
  city_array_free(&cities_2,150);
}

int main(int argc, char** argv) {
  setlocale(LC_ALL, "");
  g_test_init(&argc, &argv, NULL);

  Test_env *test_env = test_env_new();
  printf("Seed: %d", test_env->seed);

  g_test_add("/city/test_city_max_distance", Test_city, test_env,
             test_city_set_up,
             test_city_max_distance,
             test_city_tear_down);
  g_test_add("/city/test_city_cost", Test_city, test_env,
             test_city_set_up,
             test_city_cost,
             test_city_tear_down);
  g_test_add("/city/test_city_normalizer", Test_city, test_env,
             test_city_set_up,
             test_city_normalizer,
             test_city_tear_down);
  return g_test_run();
}
