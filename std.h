#pragma once

#include <string.h>

// Gets the textual representation of the name of a class (without the struct
// before it)
#define CLASS_NAME(name) #name

// Checks if the type from a base class pointer is equal to the type of a
// derived class pointer
#define IS_SAME(some_type, expression)                                         \
  (strcmp(expression->typeid_name(), CLASS_NAME(some_type)) == 0)

// Checks if a base class pointer can be cast to a derived class pointer
#define DYNAMIC_CAST(new_type, expression)                                     \
  (IS_SAME(new_type, expression)) ? (struct new_type const *)expression : NULL

// Simulates throwing an exception by setting a bool error value
#define THROW(error_pointer)                                                   \
  if (error_pointer)                                                           \
    *error_pointer = true;

// Simulates catching an exception by checking a bool error value
#define CATCH(error_pointer) (error_pointer && *error_pointer)

// This is a derived class, based on the specified class
#define BASED_ON(base_class)

// Virtual method table
#define VMT

// Virtual method table for a base class
#define BASE_VMT

// Virtual method table for a derived class, should start with the base VMT
#define DERIVED_VMT(base_vmt)

// Function that gets the RTTI typeid name
typedef char const *(*typeid_name_t)(void);
