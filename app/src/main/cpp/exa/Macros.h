//
// Created by sunzhijun on 2018/1/11.
//

#ifndef TESTJNI_MACROS_H
#define TESTJNI_MACROS_H
#define SAFE_FREE(p)             do { if(p) { free(p); (p) = NULL; } } while(0)
#endif //TESTJNI_MACROS_H
