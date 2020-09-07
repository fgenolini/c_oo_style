#pragma once

#define CLASS_NAME(name) #name

#define IS_SAME(some_type, expression) \
  (strcmp(expression->typeid_name(), CLASS_NAME(some_type)) == 0)

#define DYNAMIC_CAST(new_type, expression) \
  (IS_SAME(new_type, expression)) ? \
    (struct new_type const *)a : \
    NULL
