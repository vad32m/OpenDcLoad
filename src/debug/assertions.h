#ifndef _ASSERTIONS_H__
#define _ASSERTIONS_H__

#ifdef DEBUG

#include <stdbool.h>

bool
_assert_is_ptr_valid(void* ptr);

void
_assert_die(const char* file, const char* line);

#define ASSERT_STR_HELPER(x) #x
#define ASSERT_STR(x) ASSERT_STR_HELPER(x)

#define ASSERT_PTR_VALID(PTR) if (!(_assert_is_ptr_valid(PTR))) {_assert_die(__FILE__, ASSERT_STR(__LINE__));}

#define ASSERT_NULL(NUMBER)  if ((NUMBER) != 0) {_assert_die(__FILE__, ASSERT_STR(__LINE__));}

#define ASSERT_NON_NULL(PTR) if ((PTR) == 0) {_assert_die(__FILE__, ASSERT_STR(__LINE__));}

#define ASSERT_TRUE(EXPRESSION) if (!(EXPRESSION)) {_assert_die(__FILE__, ASSERT_STR(__LINE__));}

#else //DEBUG

#define ASSERT_PTR_VALID(PTR) ((void)0)

#define ASSERT_NON_NULL(PTR) ((void)0)

#define ASSERT_NULL(NUMBER) ((void)0)

#define ASSERT_TRUE(NUMBER) ((void)0)

#endif //DEBUG
#endif
