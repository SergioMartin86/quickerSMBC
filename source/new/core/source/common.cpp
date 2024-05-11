#include <common.hpp>

thread_local bool c;                      /**< Carry flag. */
thread_local bool z;                      /**< Zero flag. */
thread_local bool n;                      /**< Negative flag. */
thread_local bool _storePPUEnabled = true;
thread_local bool _storeDataStorageEnabled = true;