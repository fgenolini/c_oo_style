#include <stdio.h>
#include <string.h>

#include "animal.h"
#include "std.h"

static char const *animal_typeid_name(void) { return CLASS_NAME(animal); }

void animal_release(struct animal *self) {
  // printf("animal_release\n");
  if (!IS_SAME(animal, self)) {
    self->release(self);
    return;
  }

  self->length = NULL;
  self->release = NULL;
  self->say = NULL;
  self->typeid_name = NULL;
  self->private.sound = NULL;
  self->private.sound_length = 0;
  // printf("animal released\n");
}

void animal_free(struct animal **self) {
  // printf("animal_free\n");
  struct animal *s = *self;

  // This could be an overriden function
  s->release(s);

  free(s);
  self = NULL;
}

static char const *animal_say(struct animal const *self) {
  // printf("animal_say\n");
  return self->private.sound;
}

static size_t animal_length(struct animal const *self) {
  // printf("animal_length\n");
  return self->private.sound_length;
}

void animal_make(struct animal *self, char const *sound, bool *has_error) {
  // printf("animal_make\n");
  if (!sound) {
    THROW(has_error);
    return;
  }

  self->private.sound = sound;
  self->private.sound_length = strlen(sound);
  self->length = animal_length;
  self->release = animal_release;
  self->say = animal_say;
  self->typeid_name = animal_typeid_name;
}

struct animal *animal_alloc(char const *sound, bool *has_error) {
  // printf("animal_alloc\n");
  struct animal *new_animal = (struct animal *)calloc(1, sizeof(struct animal));
  if (!new_animal) {
    THROW(has_error);
    return NULL;
  }

  animal_make(new_animal, sound, has_error);
  if (CATCH(has_error)) {
    free(new_animal);
    return NULL;
  }

  return new_animal;
}
