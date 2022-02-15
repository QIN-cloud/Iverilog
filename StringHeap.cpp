#include "StringHeap.h"
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <assert.h>

StringHeap::StringHeap()
{
      cell_base_ = 0;
      cell_ptr_ = HEAPCELL;
      cell_count_ = 0;
}

StringHeap::~StringHeap()
{
	// This is a planned memory leak. The string heap is intended
	// to hold permanently-allocated strings.
}

const char* StringHeap::add(const char*text)
{
      unsigned len = strlen(text);
      assert((len+1) <= HEAPCELL);

      unsigned rem = HEAPCELL - cell_ptr_;
      if (rem < (len+1)) {
	    cell_base_ = (char*)malloc(HEAPCELL);
	    cell_ptr_ = 0;
	    cell_count_ += 1;
	    assert(cell_base_ != 0);
      }

      char*res = cell_base_ + cell_ptr_;
      memcpy(res, text, len);
      cell_ptr_ += len;
      cell_base_[cell_ptr_++] = 0;

      assert(cell_ptr_ <= HEAPCELL);

      return res;
}


StringHeapLex::StringHeapLex()
{
      hit_count_ = 0;
      add_count_ = 0;

      for (unsigned idx = 0 ;  idx < HASH_SIZE ;  idx += 1)
	    hash_table_[idx] = 0;
}

StringHeapLex::~StringHeapLex()
{
}

unsigned StringHeapLex::add_hit_count() const
{
      return hit_count_;
}

unsigned StringHeapLex::add_count() const
{
      return add_count_;
}

static unsigned hash_string(const char*text)
{
      unsigned h = 0;

      while (*text) {
	    h = (h << 4) ^ (h >> 28) ^ *text;
	    text += 1;
      }
      return h;
}

const char* StringHeapLex::add(const char*text)
{
      unsigned hash_value = hash_string(text) % HASH_SIZE;

	/* If we easily find the string in the hash table, then return
	   that and be done. */
      if (hash_table_[hash_value]
	  && (strcmp(hash_table_[hash_value], text) == 0)) {
	    hit_count_ += 1;
	    return hash_table_[hash_value];
      }

	/* The existing hash entry is not a match. Replace it with the
	   newly allocated value, and return the new pointer as the
	   result to the add. */
      const char*res = StringHeap::add(text);
      hash_table_[hash_value] = res;
      add_count_ += 1;

      return res;
}