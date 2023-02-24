#include <glib.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "city.h"

#define MAX_DISTANCE_40    4947749.059999999590218
#define MAX_DISTANCE_150   4979370.000000000000000000
#define MAX_ID             1092

/* Test environment. */
typedef struct {
  int seed;
  int *ids;
} Test_env;

/* Test environment constructor. */
static Test_env* test_env_new() {
  Test_env *test_env = malloc(sizeof(Test_env));
  if (!test_env)
    return 0;
  test_env->seed = time(0);
  srandom(test_env->seed);
  test_env->ids = malloc(sizeof(int)*12);
  int i;
  for (i = 0; i < 12; test_env->ids++)
    *test_env->ids = random() % MAX_ID;
  if (!test_env->ids) {
    free(test_env);
    return 0;
  }
  return test_env;
}

/* Test city. */
typedef struct {
  City* city_1;
  City* city_2;
} Test_city;

/* Sets up a city test case. */
static void test_city_set_up(Test_city* test_city,
                                   gconstpointer user_data) {
  /* test_city->city = city_new(); */
}

/* Tears down a city test case. */
static void test_city_tear_down(Test_city* test_city,
                                gconstpointer user_data) {
  /* g_clear_object(&test_city->city); */
}

/* Tests the distance between two cities */
static void test_city_distance(Test_city* test_city,
                               gconstpointer user_data) {
  
}

int main(int argc, char** argv) {
  Test_env *test_env = test_env_new();
  printf("Seed: %d", test_env->seed);
  
  setlocale(LC_ALL, "");
  g_test_init(&argc, &argv, NULL);

  g_test_add("/test-city/distance/", Test_city, "user-data",
             test_city_set_up, test_city_distance,
             test_city_tear_down);
  return g_test_run();
}
