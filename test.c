#include <stdio.h>
#include <time.h>
#include "skiplist.h"


void skiplist_dump_all(skiplist_t* sl) {
  int32_t idx;
  skipnode_t* t;

  for (idx = -1, t = sl->root; t; t = t->socket[SKIPLIST_BOTTOM]) {
    printf("node %p :: %u/%-5d :: key=%-12d level=%-3u insert order %u\n",
      t,
      sl->count,
      idx++,
      t->key,
      t->level,
      t->order
    );
  }
}

int main(int argc, char* argv[]) {
  clock_t c;
  skiplist_t s;
  skipnode_t* t;
  uint32_t idx, e, i, k, times, level;
  char* suffix;
  if (CLOCKS_PER_SEC == 1000) {
    /*
    test.c:5:5: error: missing binary operator before token "1000000"
    #if CLOCKS_PER_SEC == 1000
    */
    suffix = "ms";
  } else if (CLOCKS_PER_SEC == 1000000) {
    suffix = "us";
  }

  if (argc == 3) {
    times = atoi(argv[1]);
    level = atoi(argv[2]);
    if (level > UINT8_MAX)
      level = 255;
  } else {
    times = 2000000;
    level = 32;
  }

  skiplist_init(&s, level);
  printf("while(%d) random hash test, skiplist level %d.\n", times, s.level);

  skiplist_insert(&s, 32, NULL);
  skiplist_insert(&s, 33, NULL);
  skiplist_insert(&s, 33, NULL);
  skiplist_insert(&s, 35, NULL);
  skiplist_insert(&s, 36, NULL);

  printf("\n[DUMP NODES]\n");
  skiplist_dump_all(&s);

  skiplist_delete(&s, 33);
  skiplist_delete(&s, 36);
  printf("\n[AFTER FREE HASH 33, 36]\n");
  if ((t = skiplist_find(&s, 35)) != NULL) {
    printf("skiplist_find(&s, 35) = found, @%p\n", t);
  }
  if ((t = skiplist_find(&s, 36)) == NULL) {
    printf("skiplist_find(&s, 36) = not found\n");
  } else {
    printf("deleted key 36 found!\n");
  }
  printf("\n[DUMP NODES]\n");
  skiplist_dump_all(&s);
  skiplist_destroy(&s);
  printf("base test finished.\n\n");

  skiplist_init(&s, level);
  idx = times, i = 0, e = 0, c = clock();
  while (idx--) {
    k = rand() * rand();
    if (!skiplist_find(&s, k)){
      skiplist_insert(&s, k, NULL);
      i ++;
    } else {
      e ++;
    }
  }
  printf("%d random hash keys inserted, %d keys exist, %ld%s\n", i, e, clock() - c, suffix);

  idx = times, i = 0, e = 0, c = clock();
  while (idx--) {
    k = rand() * rand();
    if (skiplist_find(&s, k)) {
      e ++;
    }
  }
  printf("find %d random hash keys, %ld%s\n", e, clock() - c, suffix);

  c = clock();
  skiplist_destroy(&s);
  printf("destroy skiplist, %ld%s\n", clock() - c, suffix);
  return 0;
}
