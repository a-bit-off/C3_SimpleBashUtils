#ifndef S21_GREP_H
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct flags {
  int e, i, v, c, l, n, h, s, f, o;
};

struct data {
  char patternsArray[128][1024];
  char fileNamesArray[128][1024];
};

void parsingScript(struct flags *Flags, struct data *Data, int *countPattern,
                   int *countName, int argc, char **argv,
                   int *countAllFileName);
int findFlags(char **argv, int argc, struct flags *Flags, unsigned int *I);
int findPatterns(struct flags *Flags, struct data *Data, char **argv, int argc,
                 unsigned int *i, int *countPattern);
void findFiles(struct flags Flags, struct data *Data, char **argv, int argc,
               unsigned int *i, int *countName, int *countAllFileName);
void grepFunction(struct flags *Flags, struct data *Data, int *countPattern,
                  int *countName, int *countAllFileName);
void grepPatternCycle(int countPattern, struct flags Flags, struct data *Data,
                      char *stringFile, int *countFlag_c, int *countFlag_l,
                      int *countFlag_n, char *fileName, int countAllFileName,
                      char c, char *endOfFile, int *length);
void grepFlag_c_l(struct flags *Flags, int *countAllFileName, char *fileName,
                  int countFlag_c, int countFlag_l);
void reegexFunction(char *pattern, struct flags Flags, char *stringFile,
                    int *countFlag_c, int *countFlag_l, int countFlag_n,
                    char *fileName, int countName);
void flag_o_handler(char *text, regex_t reg, regmatch_t pmatch[1]);
int flag_f_handler(struct flags *Flags, char *pattern, struct data *Data,
                   int *countPattern);
void flag_f_identic(int *length, int *identic, char *patternInFile,
                    int *countPattern, struct data *Data, int *firstPattern,
                    char c);

#endif
