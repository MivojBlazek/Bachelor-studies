#include "hashtable.h"
#include "test_util.h"
#include <stdio.h>
#include <stdlib.h>

#define INSERT_TEST_DATA(TABLE)                                                \
  ht_insert_many(TABLE, TEST_DATA, sizeof(TEST_DATA) / sizeof(TEST_DATA[0]));

const ht_item_t TEST_DATA[15] = {
    {"Bitcoin", 53247.71}, {"Ethereum", 3208.67}, {"Binance Coin", 409.15},
    {"Cardano", 1.82},     {"Tether", 0.86},      {"XRP", 0.93},
    {"Solana", 134.50},    {"Polkadot", 34.99},   {"Dogecoin", 0.22},
    {"USD Coin", 0.86},    {"Uniswap", 21.68},    {"Terra", 30.67},
    {"Litecoin", 156.87},  {"Avalanche", 47.03},  {"Chainlink", 21.90}};

void init_test() {
  printf("Hash Table - testing script\n");
  printf("---------------------------\n");
  HT_SIZE = 13;
  printf("\nSetting HT_SIZE to prime number (%i)\n", HT_SIZE);
  printf("\n");
}

TEST(test_table_init, "Initialize the table")
ht_init(test_table);
ENDTEST

TEST(test_search_nonexist, "Search for a non-existing item")
ht_init(test_table);
ht_item_t *searchin = ht_search(test_table, "Ethereum");
if (searchin != NULL)
{
  ht_print_item(searchin);
}
ENDTEST

TEST(test_insert_simple, "Insert a new item")
ht_init(test_table);
ht_insert(test_table, "Ethereum", 3208.67);
ENDTEST

TEST(test_search_exist, "Search for an existing item")
ht_init(test_table);
ht_insert(test_table, "Ethereum", 3208.67);
ht_item_t *searchin = ht_search(test_table, "Ethereum");
if (searchin != NULL)
{
  ht_print_item(searchin);
}
ENDTEST

TEST(test_insert_many, "Insert many new items")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ENDTEST

TEST(test_search_collision, "Search for an item with colliding hash")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ht_item_t *searchin = ht_search(test_table, "Terra");
if (searchin != NULL)
{
  ht_print_item(searchin);
}
ENDTEST

TEST(test_insert_update, "Update an item")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ht_insert(test_table, "Ethereum", 12.34);
ENDTEST

TEST(test_get, "Get an item's value")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
float *testFloat = ht_get(test_table, "Ethereum");
if (testFloat != NULL)
{
  printf("value of %s: %f\n", "Ethereum", *testFloat);
}
else
{
  printf("value of %s: NULL\n", "Ethereum");
}
ENDTEST

TEST(test_delete, "Delete an item")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ht_delete(test_table, "Terra");
ENDTEST

TEST(test_delete_all, "Delete all the items")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ht_delete_all(test_table);
ENDTEST

//advance tests

TEST(advance_test1, "Advance test1")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ht_delete(test_table, "Cardano");
ENDTEST

TEST(advance_test2, "Advance test2")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ht_delete(test_table, "Solana");
ht_delete(test_table, "Binance Coin");
ENDTEST

TEST(advance_test3, "Advance test3")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ht_delete(test_table, "Cardano");
ht_delete(test_table, "Cardano");
ht_delete(test_table, "Cardano");
ht_delete(test_table, "Cardano");
ht_delete(test_table, "Cardano");
ENDTEST

TEST(advance_test4, "Advance test4")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ht_insert(test_table, "ethereum", 1234567.89);
ENDTEST

TEST(advance_test5, "Advance test5")
ht_init(test_table);
ht_insert(test_table, "", 12.89);
ENDTEST

TEST(advance_test6, "Advance test6")
ht_init(test_table);
ht_insert(test_table, "", 12.89);
ht_delete(test_table, "");
ENDTEST

TEST(advance_test7, "Advance test7")
ht_init(test_table);
ht_init(test_table);
ht_init(test_table);
ENDTEST

TEST(advance_test8, "Advance test8")
ht_init(test_table);
ht_insert(test_table, "", 12.89);
ht_delete_all(test_table);
ht_delete_all(test_table);
ht_delete_all(test_table);
ENDTEST

int main(int argc, char *argv[]) {
  init_uninitialized_item();
  init_test();

  test_table_init();
  test_search_nonexist();
  test_insert_simple();
  test_search_exist();
  test_insert_many();
  test_search_collision();
  test_insert_update();
  test_get();
  test_delete();
  test_delete_all();

  //advance tests
  advance_test1();
  advance_test2();
  advance_test3();
  advance_test4();
  advance_test5();
  advance_test6();
  advance_test7();
  advance_test8();

  free(uninitialized_item);
}
