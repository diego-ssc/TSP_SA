#include <glib.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "city.h"

#define MAX_DISTANCE_40    4947749.059999999590218
#define MAX_DISTANCE_150   4979370.000000000000000000
#define MAX_ID             1092
#define EVAL_40            4129508.339517763
#define EVAL_150           6249022.603226478
#define NORMALIZER_40      180088219.480000019073486
#define NORMALIZER_150     721914154.580000042915344

static int instance_1[1][40] = {
  {1,2,3,4,5,6,7,54,163,164,165,168,172,186,327,329,331,332,
      333,483,489,490,491,492,493,496,653,654,656,657,815,816,
      817,820,978,979,980,981,982,984}
};

static int instance_2[1][150] = {
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
  City* city_1;
  City* city_2;
} Test_city;

/* Sets up a city test case. */
static void test_city_set_up(Test_city* test_city,
                             gconstpointer data) {
  /* Test_env* test_env = (Test_env*)data; */
  /* test_city->city = city_new(); */
}

/* Tears down a city test case. */
static void test_city_tear_down(Test_city* test_city,
                                gconstpointer data) {
  /* g_clear_object(&test_city->city); */
}

/* Tests the distance between two cities */
static void test_city_distance(Test_city* test_city,
                               gconstpointer data) {

}

/* Tests the distance between two cities */
static void test_city_cost(Test_city* test_city,
                           gconstpointer data) {

}

int main(int argc, char** argv) {
  setlocale(LC_ALL, "");
  g_test_init(&argc, &argv, NULL);

  Test_env *test_env = test_env_new();
  printf("Seed: %d", test_env->seed);

  g_test_add("/test-city/distance/", Test_city, test_env,
             test_city_set_up,
             test_city_distance,
             test_city_tear_down);
  g_test_add("/test-city/cost/", Test_city, test_env,
             test_city_set_up,
             test_city_cost,
             test_city_tear_down);
  return g_test_run();
}
