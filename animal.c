#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "animal.h"

char const *animal_typeid_name() { return "animal"; }

void animal_release(struct animal *self) {
  // printf("animal_release\n");
  self->release = NULL;
  self->say = NULL;
  self->private.sound = NULL;
  self->private.sound_length = 0;
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
