#include "s21_cat.h"

int main(int argc, char **argv) {
  if (argc > 0) {
    struct s_flags catFlags = {0};
    unsigned int i = 1;
    if (findFlags(argv, argc, &catFlags, &i) == 0) {
      fprintf(stderr, "Flag error.");
    } else {
      if (findFiles(argv, argc, &catFlags, &i) == 0) {
        fprintf(stderr, "File error.");
      }
    }
  }
  return 0;
}

int findFlags(char **argv, int argc, struct s_flags *catFlags,
              unsigned int *i) {
  int flag = 1;
  while ((*i < (unsigned int)argc) && (flag == 1) && (argv[*i])) {
    unsigned int j = 0;
    if (strcmp(argv[*i], "--number-nonblank") == 0) {
      catFlags->catFlag_b = 1;
    } else if (strcmp(argv[*i], "--number") == 0) {
      catFlags->catFlag_n = 1;
    } else if (strcmp(argv[*i], "--squeeze-blank") == 0) {
      catFlags->catFlag_s = 1;
    } else if (argv[*i][j] == '-') {
      j++;
      if (combineFlags(argv, *i, j, &(*catFlags)) == 0) {
        flag = 0;
      }
    } else {
      break;
    }
    (*i)++;
  }
  return flag;
}

int combineFlags(char **argv, unsigned int i, int j, struct s_flags *catFlags) {
  int flag = 1;
  while (argv[i][j] && flag == 1) {
    if (argv[i][j] == 'b') {
      catFlags->catFlag_b = 1;
    } else if (argv[i][j] == 'v') {
      catFlags->catFlag_v = 1;
    } else if (argv[i][j] == 'e') {
      catFlags->catFlag_e = 1;
      catFlags->catFlag_v = 1;
    } else if (argv[i][j] == 'E') {
      catFlags->catFlag_E = 1;
    } else if (argv[i][j] == 'n') {
      catFlags->catFlag_n = 1;
    } else if (argv[i][j] == 's') {
      catFlags->catFlag_s = 1;
    } else if (argv[i][j] == 't') {
      catFlags->catFlag_t = 1;
      catFlags->catFlag_v = 1;
    } else if (argv[i][j] == 'T') {
      catFlags->catFlag_T = 1;
    } else {
      flag = 0;
    }
    j++;
  }
  if (argv[i][1] == 0) {
    flag = 0;
  }
  return flag;
}

int findFiles(char **argv, int argc, struct s_flags *catFlags,
              unsigned int *i) {
  int flag = 1;
  // определяем файлы и выполняем задачу
  if (argv[*i]) {
    while (argv[*i] && *i < (unsigned int)argc) {
      char *fileName = argv[*i];
      flagScript(fileName, catFlags);
      (*i)++;
    }
  } else {
    flag = 0;
  }
  return flag;
}

// flags manager
void flagScript(char *fileName, struct s_flags *catFlags) {
  int countString_b = 0;
  int countString_n = 0;
  int toggle_b = catFlags->catFlag_b;
  int countFlag_s = 0;
  unsigned char previous_n = 0;
  FILE *f = NULL;
  char c = 0;
  if ((f = fopen(fileName, "r"))) {
    while ((c = fgetc(f)) != EOF) {
      unsigned char sym = c;
      handlerFlag_s(sym, &(*catFlags), &countFlag_s);  // flag -s
      // end of string
      if (sym == '\n') {
        toggle_b = catFlags->catFlag_b;  // for flag b
      }
      handlerFlag_b_n(sym, &(*catFlags), countFlag_s, &toggle_b, &countString_b,
                      &countString_n, &previous_n);     // flag -b or flag -n
      handlerFlag_e_E(sym, &(*catFlags), countFlag_s);  // flag e E
      sym = handlerFlag_v(sym, &(*catFlags));           // flag -v
      sym = handlerFlag_t(sym, &(*catFlags));           // flag t
      output(sym, countFlag_s);                         // output
    }
    fclose(f);
  } else {
    fprintf(stderr, "File error.");
  }
}

// flag -s
void handlerFlag_s(unsigned char sym, struct s_flags *catFlags,
                   int *countFlag_s) {
  if (catFlags->catFlag_s == 1) {
    if (sym == '\n') {
      (*countFlag_s)++;
    } else {
      (*countFlag_s) = 0;
    }
  }
}

// flag -b and flag -n
void handlerFlag_b_n(unsigned char sym, struct s_flags *catFlags,
                     int countFlag_s, int *toggle_b, int *countString_b,
                     int *countString_n, unsigned char *previous_n) {
  if (countFlag_s <= 2) {
    if (catFlags->catFlag_b && *toggle_b) {
      if (sym != '\n') {
        (*countString_b)++;
        printf("%6d\t", *countString_b);
        *toggle_b = 0;
      }
    } else if (catFlags->catFlag_n && (catFlags->catFlag_b == 0)) {
      if (*previous_n == 0) {
        (*countString_n)++;
        printf("%6d\t", *countString_n);
      }
      if (*previous_n == '\n') {
        (*countString_n)++;
        printf("%6d\t", *countString_n);
      }
      *previous_n = sym;
    }
  }
}
// flags e E
void handlerFlag_e_E(unsigned char sym, struct s_flags *catFlags,
                     int countFlag_s) {
  if (countFlag_s <= 2) {
    if ((catFlags->catFlag_e == 1 || catFlags->catFlag_E == 1) && sym == '\n') {
      printf("$");
    }
  }
}

// flag v
unsigned char handlerFlag_v(unsigned char sym, struct s_flags *catFlags) {
  if (sym != '\n' && catFlags->catFlag_v) {
    if (sym < 32 && sym != 9 && sym != 10) {
      printf("^");
      sym += 64;
    } else if (sym > 127 && sym < 160) {
      printf("M-^");
      sym -= 64;
    } else if (sym == 127) {
      printf("^");
      sym -= 64;
    }
  }
  return sym;
}

// flag t
unsigned char handlerFlag_t(unsigned char sym, struct s_flags *catFlags) {
  if ((catFlags->catFlag_t || catFlags->catFlag_T) && sym == '\t') {
    printf("^");
    sym = 'I';
  }
  return sym;
}

// output
void output(unsigned char sym, int countFlag_s) {
  if (countFlag_s <= 2) {
    printf("%c", sym);
  }
}