#include <common.hpp>

thread_local bool c = false;                      /**< Carry flag. */
thread_local bool z = false;                      /**< Zero flag. */
thread_local bool n = false;                      /**< Negative flag. */
thread_local bool _storePPUEnabled = true;
thread_local bool _storeDataStorageEnabled = true;
thread_local bool _storeStackEnabled = true;