/**
 * @file op_string.h
 * general purpose C string handling declarations.
 *
 * @remark Copyright 2003 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef OP_STRING_H
#define OP_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param  s: input string
 * @param len: len char to copy
 *
 * Allocate and copy len character from s to a newly allocated buffer then
 * append a '\0' terminator. Return the newly allocated string
 */
char * op_xstrndup(char const * s, size_t len);

/**
 * @param str: string to test
 * @param prefix: prefix string
 *
 * return non zero if prefix parameters is a prefix of str
 */
int strisprefix(char const * str, char const * prefix);

/**
 * @param c: input string
 *
 * return a pointer to the first location in c whiwh is not a blank space
 * where blank space are in " \t\n"
 */
char const * skip_ws(char const * c);

/**
 * @param c: input string
 *
 * return a pointer to the first location in c whiwh is a blank space
 * where blank space are in " \t\n"
 */
char const * skip_nonws(char const * c);

/**
 * @param c: input string
 *
 * return non zero is c string contains only blank space
 * where blank space are in " \t\n"
 */
int empty_line(char const * c);

/**
 * @param c: input string
 *
 * return non zero is c string is a comment. Comment are line with optionnal
 * blank space at left then a '#' character. Blank space are in " \t\n"
 */
int comment_line(char const * c);

#ifdef __cplusplus
}
#endif

#endif /* !OP_STRING_H */