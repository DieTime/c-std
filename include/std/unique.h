#ifndef STD_UNIQUE_H
#define STD_UNIQUE_H

#include "std/unused.h"

#define std_unique_ptr(type) \
    __attribute__((cleanup(__std_cleanup_func_for_##type))) type *

#define declare_std_unique_ptr_for_type(type, safe_cleanup_func)     \
    static std_unused void __std_cleanup_func_for_##type(void *pp) { \
        type **data_ptr = (type **) pp;                              \
                                                                     \
        if (!data_ptr || !(*data_ptr))                               \
            return;                                                  \
                                                                     \
        safe_cleanup_func(data_ptr);                                 \
    }

#endif /* STD_UNIQUE_H */
