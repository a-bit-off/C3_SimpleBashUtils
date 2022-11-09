#include "s21_grep.h"

int main(int argc, char **argv) {
  struct flags Flags = {0};
  struct data Data = {0};
  int countPattern = 0;
  int countName = 0;
  int countAllFileName = 0;

  parsingScript(&Flags, &Data, &countPattern, &countName, argc, argv,
                &countAllFileName);
  grepFunction(&Flags, &Data, &countPattern, &countName, &countAllFileName);
}

void parsingScript(struct flags *Flags, struct data *Data, int *countPattern,
                   int *countName, int argc, char **argv,
                   int *countAllFileName) {
  int patternError = 0;
  if (argc > 2) {
    unsigned int i = 1;
    while (argv[i] && i < (unsigned int)argc) {
      // FLAG
      if (findFlags(argv, argc, &(*Flags), &i) != 0) {
        // PATTERN
        if (findPatterns(&(*Flags), &(*Data), argv, argc, &i,
                         &(*countPattern)) == 0) {
          if (Flags->e == 1) {
            if (Flags->s == 0) {
              patternError = 1;
            }
          }
        }
        // FILE
        if (patternError == 0) {
          findFiles(*Flags, &(*Data), argv, argc, &i, &(*countName),
                    &(*countAllFileName));
        }
      } else {
        if (Flags->s == 0) {
          fprintf(stderr, "Flag error.");
        }
      }
    }
  }
  if (*countPattern == 0 || *countName == 0 || patternError == 1) {
    if (Flags->s == 0) {
      fprintf(stderr, "Pattern or file error.");
    }
    exit(1);
  }
}

// вернет 0 если не будет флага или 1 если будет
int findFlags(char **argv, int argc, struct flags *Flags, unsigned int *i) {
  unsigned int j = 0;
  int flag = 1;
  int count = 0;
  while (argv[*i] && flag == 1 && *i < (unsigned int)argc) {
    j = 0;
    if (argv[*i][j] == '-') {
      j = 1;
      if (count == 0) {
        count++;
        if (argv[*i][j] == 'e') {
          Flags->e = 1;
        } else if (argv[*i][j] == 'i') {
          Flags->i = 1;
        } else if (argv[*i][j] == 'v') {
          Flags->v = 1;
        } else if (argv[*i][j] == 'c') {
          Flags->c = 1;
        } else if (argv[*i][j] == 'l') {
          Flags->l = 1;
        } else if (argv[*i][j] == 'n') {
          Flags->n = 1;
        } else if (argv[*i][j] == 'h') {
          Flags->h = 1;
        } else if (argv[*i][j] == 's') {
          Flags->s = 1;
        } else if (argv[*i][j] == 'f') {
          Flags->f = 1;
        } else if (argv[*i][j] == 'o') {
          Flags->o = 1;
        } else {
          flag = 0;
        }
      }
    } else {
      break;
    }
    (*i)++;
  }
  return flag;
}

int findPatterns(struct flags *Flags, struct data *Data, char **argv, int argc,
                 unsigned int *i, int *countPattern) {
  char *pattern = NULL;
  int is_find = 0;
  if (argv[*i] && *i < (unsigned int)argc) {
    pattern = argv[*i];
    (*i)++;
    if (Flags->f == 1 && pattern != NULL) {
      is_find = flag_f_handler(&(*Flags), pattern, &(*Data), &(*countPattern));
    } else if (pattern != NULL) {
      strcpy(Data->patternsArray[*countPattern], pattern);
      (*countPattern)++;
      is_find = 1;
    }
  }
  return is_find;
}

void findFiles(struct flags Flags, struct data *Data, char **argv, int argc,
               unsigned int *i, int *countName, int *countAllFileName) {
  char *fileName = NULL;
  FILE *f;
  if (argv[*i] && *i < (unsigned int)argc) {
    while (argv[*i] && argv[*i][0] != '-') {
      (*countAllFileName)++;
      fileName = argv[*i];
      (*i)++;
      if ((f = fopen(fileName, "r")) != NULL) {
        strcpy(Data->fileNamesArray[*countName], fileName);
        (*countName)++;
        fclose(f);
      } else {
        if (Flags.s == 0) {
          fprintf(stderr, "File error.");
        }
      }
    }
  }
}

void grepFunction(struct flags *Flags, struct data *Data, int *countPattern,
                  int *countName, int *countAllFileName) {
  FILE *f = NULL;
  // цикл файлов
  for (int i = 0; i < *countName; i++) {
    int countFlag_c = 0;
    int countFlag_l = 0;
    char *fileName = Data->fileNamesArray[i];
    int countFlag_n = 0;
    if ((f = fopen(fileName, "r")) != NULL) {
      int length = 0;
      char c = 0;
      char endOfFile = 0;
      char stringFile[1024];
      // цикл по строкам файла
      while ((c = fgetc(f)) != EOF) {
        endOfFile = c;
        length++;
        stringFile[length - 1] = c;
        if (c == '\n') {
          grepPatternCycle(*countPattern, *Flags, &(*Data), &(*stringFile),
                           &countFlag_c, &countFlag_l, &countFlag_n, fileName,
                           *countAllFileName, c, &endOfFile, &length);
        }
      }
      if (c == EOF) {
        if (endOfFile != '\n') {
          grepPatternCycle(*countPattern, *Flags, &(*Data), &(*stringFile),
                           &countFlag_c, &countFlag_l, &countFlag_n, fileName,
                           *countAllFileName, c, &endOfFile, &length);
        }
      }
      fclose(f);
    }
    grepFlag_c_l(&(*Flags), &(*countAllFileName), fileName, countFlag_c,
                 countFlag_l);
  }
}

void grepPatternCycle(int countPattern, struct flags Flags, struct data *Data,
                      char *stringFile, int *countFlag_c, int *countFlag_l,
                      int *countFlag_n, char *fileName, int countAllFileName,
                      char c, char *endOfFile, int *length) {
  if (c == EOF) {
    (*countFlag_n)++;
    if (*endOfFile != '\n') {
      (*length)++;
      stringFile[*length - 1] = '\n';
      stringFile[*length] = '\0';
    }
  } else if (c == '\n') {
    (*length)++;
    (*countFlag_n)++;
    stringFile[*length - 1] = '\0';
  }
  for (int j = 0; j < countPattern; j++) {
    char *pattern = Data->patternsArray[j];
    reegexFunction(pattern, Flags, stringFile, &(*countFlag_c), &(*countFlag_l),
                   *countFlag_n, fileName, countAllFileName);
  }
  *length = 0;
}

void grepFlag_c_l(struct flags *Flags, int *countAllFileName, char *fileName,
                  int countFlag_c, int countFlag_l) {
  // flag c
  if (Flags->c == 1) {
    if (*countAllFileName > 1) {
      printf("%s:%d\n", fileName, countFlag_c);
    } else {
      printf("%d\n", countFlag_c);
    }
  } else if (Flags->l == 1 && countFlag_l > 0) {  // flag l
    printf("%s\n", fileName);
  }
}

void reegexFunction(char *pattern, struct flags Flags, char *stringFile,
                    int *countFlag_c, int *countFlag_l, int countFlag_n,
                    char *fileName, int countAllFileName) {
  regex_t reegex = {0};
  regmatch_t pmatch[1];
  if (Flags.i == 1) {
    regcomp(&reegex, pattern, REG_ICASE | REG_EXTENDED);
  } else {
    regcomp(&reegex, pattern, REG_EXTENDED);
  }
  if (regexec(&reegex, stringFile, 1, pmatch, 0) == Flags.v) {
    if (countAllFileName > 1 && Flags.h == 0 && Flags.l == 0 && Flags.c == 0) {
      printf("%s:", fileName);
    }
    if (Flags.c == 1) {
      if (stringFile[0] != '\n') {
        (*countFlag_c)++;
      }
    } else if (Flags.l == 1) {
      (*countFlag_l)++;
    } else if (Flags.n == 1) {
      printf("%d:%s", countFlag_n, stringFile);
    } else if (Flags.h == 1) {
      printf("%s", stringFile);
    } else if (Flags.o == 1) {
      flag_o_handler(stringFile, reegex, pmatch);
    } else {
      printf("%s", stringFile);
    }
  }
  regfree(&reegex);
}

void flag_o_handler(char *text, regex_t reg, regmatch_t pmatch[1]) {
  char *start_pos = text;
  int count = 0;
  while (!count && pmatch[0].rm_eo != pmatch[0].rm_so) {
    printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
           start_pos + pmatch[0].rm_so);
    start_pos += pmatch[0].rm_eo;
    count = regexec(&reg, start_pos, 1, pmatch, REG_NOTBOL);
  }
}

int flag_f_handler(struct flags *Flags, char *pattern, struct data *Data,
                   int *countPattern) {
  FILE *f;
  int flag = 1;
  int is_find = 0;
  //посимвольное считывание паттрена в файле
  if (((f = fopen(pattern, "r")) != NULL) && Flags->f) {
    int length = 0;
    char c = 0;
    char patternInFile[1024];
    if (patternInFile != NULL) {
      int firstPattern = 0;
      int identic = 0;
      int endOfFile = 0;
      while ((c = fgetc(f)) != EOF) {
        endOfFile = c;
        length++;
        patternInFile[length - 1] = c;
        if (c == '\n') {
          flag_f_identic(&length, &identic, &(*patternInFile), &(*countPattern),
                         &(*Data), &firstPattern, c);
        }
      }
      if (c == EOF) {
        if (endOfFile != '\n') {
          flag_f_identic(&length, &identic, &(*patternInFile), &(*countPattern),
                         &(*Data), &firstPattern, c);
          is_find = 1;
        }
      }
    } else {
      free(patternInFile);
      flag = 0;
    }
    fclose(f);
  } else {
    flag = 0;
  }
  if (flag == 0) {
    exit(1);
  }
  return is_find;
}

void flag_f_identic(int *length, int *identic, char *patternInFile,
                    int *countPattern, struct data *Data, int *firstPattern,
                    char c) {
  (*length)++;
  *identic = 0;
  if (c == '\n') {
    patternInFile[*length - 1] = '\0';
  } else if (c == EOF) {
    patternInFile[*length - 1] = '\n';
    patternInFile[*length] = '\0';
  }
  for (int i = 0; i < *countPattern; i++) {
    if (strcmp(Data->patternsArray[i], patternInFile) == 0) {
      *identic = 1;
    }
  }
  if (*firstPattern == 0) {
    *firstPattern = 1;
    strcpy(Data->patternsArray[*countPattern], patternInFile);
    (*countPattern)++;
    *length = 0;
  } else if (identic == 0) {
    strcpy(Data->patternsArray[*countPattern], patternInFile);
    (*countPattern)++;
    *length = 0;
  } else {
    *identic = 0;
  }
}