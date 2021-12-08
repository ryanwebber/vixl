#pragma once

#ifndef NDEBUG
  #define ASSERT(Expr, Msg) RuntimeAssert(#Expr, Expr, __FILE__, __LINE__, Msg)
#else
  #define ASSERT(Expr, Msg) ;
#endif

void RuntimeAssert(const char* expr_str, bool expr, const char* file, int line, const char* msg);
