// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifdef BUDDY
#include <MemoryManager.h>
#include <video.h>
#define HEADER_SIZE 8
#define MIN_ALLOC_LOG2 10
#define MIN_ALLOC ((size_t)MIN_ALLOC_LOG2)
#define MAX_ALLOC_LOG2 30
#define MAX_ALLOC ((size_t)1 << MAX_ALLOC_LOG2)
#define BUCKET_COUNT (MAX_ALLOC_LOG2 - MIN_ALLOC_LOG2 + 1)

typedef struct list_t
{
  struct list_t *prev, *next;
} list_t;

static list_t buckets[BUCKET_COUNT];

static size_t bucket_limit;

static uint8_t node_is_split[(1 << (BUCKET_COUNT - 1)) / 8];

static uint8_t *base_ptr;

static uint8_t *max_ptr;

static int update_max_ptr(uint8_t *new_value)
{
  if (new_value > max_ptr)
  {
    max_ptr = new_value;
  }
  return 1;
}

static void list_init(list_t *list)
{
  list->prev = list;
  list->next = list;
}

static void list_push(list_t *list, list_t *entry)
{
  list_t *prev = list->prev;
  entry->prev = prev;
  entry->next = list;
  prev->next = entry;
  list->prev = entry;
}

static void list_remove(list_t *entry)
{
  list_t *prev = entry->prev;
  list_t *next = entry->next;
  prev->next = next;
  next->prev = prev;
}

static list_t *list_pop(list_t *list)
{
  list_t *back = list->prev;
  if (back == list)
    return NULL;
  list_remove(back);
  return back;
}

static uint8_t *ptr_for_node(size_t index, size_t bucket)
{
  return base_ptr + ((index - (1 << bucket) + 1) << (MAX_ALLOC_LOG2 - bucket));
}

static size_t node_for_ptr(uint8_t *ptr, size_t bucket)
{
  return ((ptr - base_ptr) >> (MAX_ALLOC_LOG2 - bucket)) + (1 << bucket) - 1;
}

static int parent_is_split(size_t index)
{
  index = (index - 1) / 2;
  return (node_is_split[index / 8] >> (index % 8)) & 1;
}

static void flip_parent_is_split(size_t index)
{
  index = (index - 1) / 2;
  node_is_split[index / 8] ^= 1 << (index % 8);
}

static size_t bucket_for_request(size_t request)
{
  size_t bucket = BUCKET_COUNT - 1;
  size_t size = MIN_ALLOC;

  while (size < request)
  {
    bucket--;
    size *= 2;
  }

  return bucket;
}

static int lower_bucket_limit(size_t bucket)
{
  while (bucket < bucket_limit)
  {
    size_t root = node_for_ptr(base_ptr, bucket_limit);
    uint8_t *right_child;

    if (!parent_is_split(root))
    {
      list_remove((list_t *)base_ptr);
      list_init(&buckets[--bucket_limit]);
      list_push(&buckets[bucket_limit], (list_t *)base_ptr);
      continue;
    }

    right_child = ptr_for_node(root + 1, bucket_limit);
    if (!update_max_ptr(right_child + sizeof(list_t)))
    {
      return 0;
    }
    list_push(&buckets[bucket_limit], (list_t *)right_child);
    list_init(&buckets[--bucket_limit]);

    root = (root - 1) / 2;
    if (root != 0)
    {
      flip_parent_is_split(root);
    }
  }

  return 1;
}

void createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory)
{
  base_ptr = NULL;
}

void *allocMemory(const size_t memoryToAllocate)
{
  size_t original_bucket, bucket;

  if (memoryToAllocate + HEADER_SIZE > MAX_ALLOC)
  {
    return NULL;
  }

  if (base_ptr == NULL)
  {
    base_ptr = max_ptr = (uint8_t *)(uintptr_t)START_MEM;

    bucket_limit = BUCKET_COUNT - 1;
    update_max_ptr(base_ptr + sizeof(list_t));
    list_init(&buckets[BUCKET_COUNT - 1]);
    list_push(&buckets[BUCKET_COUNT - 1], (list_t *)base_ptr);
  }

  bucket = bucket_for_request(memoryToAllocate + HEADER_SIZE);
  original_bucket = bucket;

  while (bucket + 1 != 0)
  {
    size_t size, bytes_needed, i;
    uint8_t *ptr;

    if (!lower_bucket_limit(bucket))
    {
      return NULL;
    }

    ptr = (uint8_t *)list_pop(&buckets[bucket]);
    if (!ptr)
    {
      if (bucket != bucket_limit || bucket == 0)
      {
        bucket--;
        continue;
      }

      if (!lower_bucket_limit(bucket - 1))
      {
        return NULL;
      }
      ptr = (uint8_t *)list_pop(&buckets[bucket]);
    }

    size = (size_t)1 << (MAX_ALLOC_LOG2 - bucket);
    bytes_needed = bucket < original_bucket ? size / 2 + sizeof(list_t) : size;
    if (!update_max_ptr(ptr + bytes_needed))
    {
      list_push(&buckets[bucket], (list_t *)ptr);
      return NULL;
    }

    i = node_for_ptr(ptr, bucket);
    if (i != 0)
    {
      flip_parent_is_split(i);
    }

    while (bucket < original_bucket)
    {
      i = i * 2 + 1;
      bucket++;
      flip_parent_is_split(i);
      list_push(&buckets[bucket], (list_t *)ptr_for_node(i + 1, bucket));
    }

    *(size_t *)ptr = memoryToAllocate;
    return ptr + HEADER_SIZE;
  }

  return NULL;
}

void free(void *ptr)
{
  size_t bucket, i;

  if (!ptr)
  {
    return;
  }

  ptr = (uint8_t *)ptr - HEADER_SIZE;
  bucket = bucket_for_request(*(size_t *)ptr + HEADER_SIZE);
  i = node_for_ptr((uint8_t *)ptr, bucket);

  while (i != 0)
  {

    flip_parent_is_split(i);

    if (parent_is_split(i) || bucket == bucket_limit)
    {
      break;
    }

    list_remove((list_t *)ptr_for_node(((i - 1) ^ 1) + 1, bucket));
    i = (i - 1) / 2;
    bucket--;
  }

  list_push(&buckets[bucket], (list_t *)ptr_for_node(i, bucket));
}

void dump()
{
  size_t total_memory = MAX_ALLOC - MIN_ALLOC;
  size_t memory_used = 0;

  // Calcular la memoria utilizada
  for (int i = 0; i < BUCKET_COUNT; i++)
  {
    list_t *bucket = &buckets[i];
    list_t *current = bucket->next;
    while (current != bucket)
    {
      memory_used += current->size;
      current = current->next;
    }
  }

  // Calcular la memoria libre
  size_t memory_free = total_memory - memory_used;

  printString("Memory Status");
  printNewline();
  printString("Total Memory: ");
  printDec(total_memory);
  printString(" B (");
  printDec(total_memory / KILOBYTE);
  printString(" KB)");
  printNewline();
  printString("Memory Used: ");
  printDec(memory_used);
  printString(" B (");
  printDec(memory_used / KILOBYTE);
  printString(" KB)");
  printNewline();
  printString("Memory Free: ");
  printDec(memory_free);
  printString(" B (");
  printDec(memory_free / KILOBYTE);
  printString(" KB)");
  printNewline();
}

#endif