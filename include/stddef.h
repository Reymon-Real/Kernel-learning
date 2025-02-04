#ifndef __STDDEF__H__
#define __STDDEF__H__
#define NULL ((void*)0)

typedef unsigned int size_t;
typedef unsigned int wchar_t;
typedef int ptrdiff_t;

typedef struct {
    long long max_align_member;
} max_align_t;

#endif