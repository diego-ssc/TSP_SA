#include <glib.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

#include "city.h"

#define MAX_DISTANCE_40    4947749.059999999590218
#define MAX_DISTANCE_150   4979370.000000000000000000
#define MAX_ID 1000 //Check

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
  City* city;
} Test_city;

/* Sets up a city test case. */
static Test_city* test_city_set_up(Test_city* test_city,
                                   gconstpointer user_data) {
  /* test_city->city = city_new(); */
  return 0;
}

/* Tears down a city test case. */
static void test_city_tear_down(Test_city* test_city,
                                gconstpointer user_data) {
  /* g_clear_object(&test_city->city); */
}

int main(int argc, char** argv) {
  setlocale(LC_ALL, "");
  g_test_init(&argc, &argv, 0);

  /* g_test_add("/test-city/test-distance/", Test_city, ""); */
  return g_test_run();
}
