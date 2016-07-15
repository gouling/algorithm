#ifndef PHP_ALGORITHM_H
#define PHP_ALGORITHM_H

#define PHP_ALGORITHM_EXTNAME "algorithm"
#define PHP_ALGORITHM_EXTVER "1.0"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "calendar.c"

extern zend_module_entry algorithm_module_entry;
#define phpext_algorithm_ptr &algorithm_module_entry
#endif