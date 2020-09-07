#pragma once

// Base class
struct animal {
  // VMT
  struct {
    // RTTI typeid name (no this/self pointer)
    // This is virtual, unlike in C++
    // Can only be called from an instance, so not really a class function
    char const *(*typeid_name)();

    // Virtual destructor
    void (*release)(struct animal *self);

    // Virtual function: return the sound that the animal makes
    char const *(*say)(struct animal const *self);
  };

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

// Protected constructor (initialisation only, no allocation)
extern void animal_make(struct animal *self, char const *sound,
                        bool *has_error);
