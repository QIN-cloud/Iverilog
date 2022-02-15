#ifndef __StringHeap_H
#define __StringHeap_H

/*
 * The string heap is a way to permanently allocate strings
 * efficiently. They only take up the space of the string characters
 * and the terminating nul, there is no malloc overhead.
 */
class StringHeap {

    public:
      StringHeap();
      ~StringHeap();

      const char*add(const char*);

    private:
      enum { HEAPCELL = 0x10000 };

      char*cell_base_;
      unsigned cell_ptr_;
      unsigned cell_count_;

    private: // not implemented
      StringHeap(const StringHeap&);
      StringHeap& operator= (const StringHeap&);
};

/*
 * A lexical string heap is a string heap that makes an effort to
 * return the same pointer for identical strings. This saves further
 * space by not allocating duplicate strings, so in a system with lots
 * of identifiers, this can theoretically save more space.
 */
class StringHeapLex  : private StringHeap {

    public:
      StringHeapLex();
      ~StringHeapLex();

      const char*add(const char*);

      unsigned add_count() const;
      unsigned add_hit_count() const;

    private:
      enum { HASH_SIZE = 4096 };
      const char*hash_table_[HASH_SIZE];

      unsigned add_count_;
      unsigned hit_count_;

    private: // not implemented
      StringHeapLex(const StringHeapLex&);
      StringHeapLex& operator= (const StringHeapLex&);
};

#endif
