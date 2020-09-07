#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "animal.h"
#include "big_animal.h"

// Inject object, so that calling function can simulate RAII
void use_animal(struct animal *a) {
  printf("%s %s\n", a->typeid_name(), a->say(a));
}

// RAII simulated by calling a "use" function
// between calls of constructor and destructor
void animal_raii() {
  // Constructor exception simulated by error parameter
  bool animal_alloc_error = false;

  // Constructor called before "use" function
  struct animal *a = animal_alloc("eek", &animal_alloc_error);
  if (animal_alloc_error)
    exit(EXIT_FAILURE);

  // printf("Animal\n");
  use_animal(a);

  // Destructor called after "use" function
  animal_free(&a);
}

// Polymorphic call to derived functions from base object pointer
void big_animal_polymorphic() {
  bool animal_alloc_error = false;
  struct big_animal *b_a = big_animal_alloc("moo", 2ul, &animal_alloc_error);
  if (animal_alloc_error)
    exit(EXIT_FAILURE);

  // printf("Big animal\n");
  // printf("Big animal length: %lu\n", b_a->length(b_a));

  // Since VMTs overlap (fist functions have the same semantics)
  // casting the derived object to the base object will work
  // if only functions are called, and no private data is accessed
  struct animal *base = (struct animal *)b_a;

  // Call function using the base class pointer,
  // calling the virtual function in the derived class, not the base class
  use_animal(base);

  // Free using the base class pointer,
  // calling the virtual destructor for the derived class, not the base class
  animal_free((struct animal **)&b_a);
}

int main() {
  animal_raii();
  big_animal_polymorphic();
  exit(EXIT_SUCCESS);
  return 0;
}
