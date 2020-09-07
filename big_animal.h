#pragma once

#include "animal.h"

// Derived from animal
struct big_animal {
  // Overriden VMT, must start the same as animal.VMT
  struct {
    // RTTI typeid name (no this/self pointer)
    // This is an overriden virtual class function, unlike in C++
    char const *(*typeid_name)();

    // Overriden virtual destructor
    void (*release)(struct big_animal *self);

    // Overriden virtual function: return the sound that the big_animal makes
    char const *(*say)(struct big_animal const *self);

    // Overriden virtual function: length of the text for the sound that the
    // big_animal makes
    size_t (*length)(struct big_animal const *self);

    // Virtual function: number of times that the base sound is repeated
    size_t (*repeat_count)(struct big_animal const *self);
  };

  struct animal base;

  // Private data, do not use
  struct private_big_animal {
    char *big_sound;
    size_t repeat_count;
    size_t big_sound_length;
  } private;
};

// Creates a new instance of the big_animal class on the heap
extern struct big_animal *
big_animal_alloc(char const *sound, size_t repeat_count, bool *has_error);

// Frees a big_animal instance from the heap, and sets the pointer to NULL
extern void big_animal_free(struct big_animal **self);

// Constructor; initialisation only, no allocation
extern void big_animal_make(struct big_animal *self, char const *sound,
                            size_t repeat_count, bool *has_error);

// Destructor; de-initialisation only, no de-allocation
extern void big_animal_release(struct big_animal *self);
