#ifndef COMPILER_H
#define COMPILER_H


#if __STDC_VERSION__ < 199901L
    #define inline
#endif


#if __STDC_VERSION__ <= 201112L
    #define alignof(type) offsetof(struct { char c; type d; }, d)
#endif


#endif