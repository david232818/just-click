#ifndef __UTILS_H__
#define __UTILS_H__

#define BUILD_MACRONAME3(prefix, middle, postfix) prefix##_##middle##_##postfix

/* check whether n is in domain [start, end] */
#define IS_IN_DOMAIN(n, start, end) \
    ((n) >= (start) && (n) <= (end))

#endif
