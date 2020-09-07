#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Base class
struct animal {
  // VMT
  struct {
    // Typeid name (no this/self pointer)
    // This is virtual, unlike in C++
    // Can only be called from an instance, so not really a class function
    char const *(*typeid_name)();

    // Virtual destructor
    void (*release)(struct animal *self);

    // Virtual function
    char const *(*say)(struct animal const *self);
  };

  // Private data, do not use
  struct private_animal {
    char const *sound;
    size_t sound_length;
  } private;
};

// Derived from animal
struct big_animal {
  // Overriden VMT, must start the same as animal.VMT
  struct {
    // Typeid name (no this/self pointer)
    // This is an overriden virtual class function, unlike in C++
    char const *(*typeid_name)();

    // Overriden virtual destructor
    void (*release)(struct big_animal *self);

    // Overriden virtual function
    char const *(*say)(struct big_animal const *self);

    // Virtual function
    size_t (*length)(struct big_animal const *self);
  };

  struct animal base;

  // Private data, do not use
  struct private_big_animal {
    char *big_sound;
    size_t repeat_count;
    size_t big_sound_length;
  } private;
};

char const *animal_typeid_name() { return "animal"; }

void animal_release(struct animal *self) {
  // printf("animal_release\n");
  self->release = NULL;
  self->say = NULL;
  self->private.sound = NULL;
}

void animal_free(struct animal **self) {
  // printf("animal_free\n");
  struct animal *s = *self;
  s->release(s);
  free(s);
  self = NULL;
}

char const *animal_say(struct animal const *self) {
  // printf("animal_say\n");
  return self->private.sound;
}

void animal_make(struct animal *self, char const *sound, bool *has_error) {
  // printf("animal_make\n");
  if (!sound) {
    if (has_error)
      *has_error = true;

    return;
  }

  self->private.sound = sound;
  self->private.sound_length = strlen(sound);
  self->release = animal_release;
  self->say = animal_say;
  self->typeid_name = animal_typeid_name;
}

struct animal *animal_alloc(char const *sound, bool *has_error) {
  // printf("animal_alloc\n");
  struct animal *new_animal = (struct animal *)calloc(1, sizeof(struct animal));
  if (!new_animal) {
    if (has_error)
      *has_error = true;

    return NULL;
  }

  animal_make(new_animal, sound, has_error);
  if (has_error && *has_error) {
    free(new_animal);
    return NULL;
  }

  return new_animal;
}

char const *big_animal_typeid_name() { return "big_animal"; }

void big_animal_release(struct big_animal *self) {
  // printf("big_animal_release\n");
  self->release = NULL;
  self->say = NULL;
  self->private.repeat_count = 0;
  self->base.release(&self->base);
}

void big_animal_free(struct big_animal **self) {
  // printf("big_animal_free\n");
  struct big_animal *s = *self;
  s->release(s);
  free(s);
  self = NULL;
}

char const *big_animal_say(struct big_animal const *self) {
  // printf("big_animal_say\n");
  return self->private.big_sound;
}

size_t big_animal_length(struct big_animal const *self) {
  return self->private.big_sound_length;
}

void big_animal_make(struct big_animal *self, char const *sound,
                     size_t repeat_count, bool *has_error) {
  // printf("big_animal_make\n");
  animal_make(&self->base, sound, has_error);
  if (has_error && *has_error)
    return;

  self->private.repeat_count = repeat_count;
  size_t l = 1;
  size_t sound_length = self->base.private.sound_length;
  if (repeat_count > 0)
    l = 1 + (repeat_count - 1) + repeat_count * sound_length;

  self->private.big_sound = (char *)calloc(l, sizeof(char));
  char const *s = self->base.private.sound;
  char *c = self->private.big_sound;
  for (size_t i = 0; i < repeat_count; ++i) {
    if (i > 0) {
      *c = ' ';
      ++c;
    }

    for (size_t j = 0; j < sound_length; ++j) {
      *c = s[j];
      ++c;
    }
  }

  self->private.big_sound_length = strlen(self->private.big_sound);
  self->release = big_animal_release;
  self->say = big_animal_say;
  self->length = big_animal_length;
  self->typeid_name = big_animal_typeid_name;
}

struct big_animal *big_animal_alloc(char const *sound, size_t repeat_count,
                                    bool *has_error) {
  // printf("big_animal_alloc\n");
  struct big_animal *new_animal =
      (struct big_animal *)calloc(1, sizeof(struct big_animal));
  if (!new_animal) {
    if (has_error)
      *has_error = true;

    return NULL;
  }

  big_animal_make(new_animal, sound, repeat_count, has_error);
  if (has_error && *has_error) {
    free(new_animal);
    return NULL;
  }

  return new_animal;
}

void use_animal(struct animal *a) {
  printf("%s %s\n", a->typeid_name(), a->say(a));
}

void animal_section() {
  bool animal_alloc_error = false;
  struct animal *a = animal_alloc("eek", &animal_alloc_error);
  if (animal_alloc_error)
    exit(EXIT_FAILURE);

  // printf("Animal\n");
  use_animal(a);
  animal_free(&a);
}

void big_animal_section() {
  bool animal_alloc_error = false;
  struct big_animal *b_a = big_animal_alloc("moo", 2ul, &animal_alloc_error);
  if (animal_alloc_error)
    exit(EXIT_FAILURE);

  // printf("Big animal\n");
  // printf("Big animal length: %lu\n", b_a->length(b_a));

  struct animal *base = (struct animal *)b_a;

  // Call function using the base class pointer,
  // calling the virtual function in the derived class, not the base class
  use_animal(base);

  // Free using the base class pointer,
  // calling the virtual destructor for the derived class, not the base class
  animal_free((struct animal **)&b_a);
}

int main() {
  animal_section();
  big_animal_section();
  exit(EXIT_SUCCESS);
  return 0;
}
