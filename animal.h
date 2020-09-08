#pragma once

#include <stdbool.h>
#include <stdlib.h>

#include "std.h"

struct animal;

// De-initialiser for animal objects, does not de-allocate
typedef void (*animal_release_t)(struct animal *self);

// Base class
struct animal {
  struct BASE_VMT {
    // RTTI typeid name (no this/self pointer)
    // This is virtual, unlike in C++
    // Can only be called from an instance, so not really a class function
    typeid_name_t typeid_name;

    // Virtual destructor
    animal_release_t release;

    // Virtual function: text for the sound that the animal makes
    char const *(*say)(struct animal const *self);

    // Virtual function: length of the text for the sound that the animal makes
    size_t (*length)(struct animal const *self);
  } VMT;

  // Private data, do not use
  struct private_animal {
    char const *sound;
    size_t sound_length;
  } private;
};

// Creates a new instance of the animal class on the heap
extern struct animal *animal_alloc(char const *sound, bool *has_error);

// Frees an animal instance from the heap, and sets the pointer to NULL.
// This is a virtual destructor
extern void animal_free(struct animal **self);

// Constructor; initialisation only, no allocation
extern void animal_make(struct animal *self, char const *sound,
                        bool *has_error);

// Destructor; de-initialisation only, no de-allocation
extern void animal_release(struct animal *self);
