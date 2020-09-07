#include <stdio.h>
#include <string.h>

#include "big_animal.h"

char const *big_animal_typeid_name() { return CLASS_NAME(big_animal); }

void big_animal_release(struct big_animal *self) {
  // printf("big_animal_release\n");
  self->length = NULL;
  self->release = NULL;
  self->repeat_count = NULL;
  self->say = NULL;
  self->typeid_name = NULL;
  if (self->private.big_sound)
    free(self->private.big_sound);

  self->private.big_sound = NULL;
  self->private.big_sound_length = 0;
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

size_t big_animal_repeat_count(struct big_animal const *self) {
  return self->private.repeat_count;
}

void big_animal_make(struct big_animal *self, char const *sound,
                     size_t repeat_count, bool *has_error) {
  // printf("big_animal_make\n");
  animal_make(&self->base, sound, has_error);
  if (has_error && *has_error)
    return;

  self->private.repeat_count = repeat_count;
  size_t l = 1;
  size_t sound_length = self->base.length(&self->base);
  if (repeat_count > 0)
    l = 1 + (repeat_count - 1) + repeat_count * sound_length;

  self->private.big_sound = (char *)calloc(l, sizeof(char));
  char const *s = self->base.say(&self->base);
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
  self->length = big_animal_length;
  self->release = big_animal_release;
  self->repeat_count = big_animal_repeat_count;
  self->say = big_animal_say;
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
