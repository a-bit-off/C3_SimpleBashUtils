#ifndef S21_CAT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct s_flags {
  int catFlag_b;
  int catFlag_e;
  int catFlag_E;
  int catFlag_n;
  int catFlag_s;
  int catFlag_t;
  int catFlag_T;
  int catFlag_v;
};

void flagScript(char *fileName, struct s_flags *catFlags);
void handlerFlag_s(unsigned char sym, struct s_flags *catFlags,
                   int *countFlag_s);
void handlerFlag_b_n(unsigned char sym, struct s_flags *catFlags,
                     int countFlag_s, int *toggle_b, int *countString_b,
                     int *countString_n, unsigned char *previous_n);
void handlerFlag_e_E(unsigned char sym, struct s_flags *catFlags,
                     int countFlag_s);
unsigned char handlerFlag_v(unsigned char sym, struct s_flags *catFlags);
unsigned char handlerFlag_t(unsigned char sym, struct s_flags *catFlags);
void output(unsigned char sym, int countFlag_s);

int findFlags(char **argv, int argc, struct s_flags *catFlags, unsigned int *i);
int combineFlags(char **argv, unsigned int i, int j, struct s_flags *catFlags);
int findFiles(char **argv, int argc, struct s_flags *catFlags, unsigned int *i);

#endif  //  S21_CAT_H