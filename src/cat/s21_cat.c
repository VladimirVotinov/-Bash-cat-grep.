#include "s21_cat.h"

int main(int argc, char *argv[]) {
  if (argc > 1) {
    struct flag flags = {0};
    check_flag(argc, argv, &flags);
    process_file(argv, &flags);  //
  } else
    printf("ERROR\n");
  return 0;
}

void check_flag(int argc, char *argv[], struct flag *flags) {
  char opt;
  struct option longOptions[] = {
      {"number-nonblank", 0, 0, 'b'},
      {"number", 0, 0, 'n'},
      {"squeeze-blank", 0, 0, 's'},
      {0, 0, 0, 0},
  };
  while ((opt = getopt_long(argc, argv, "nbetsvTE", longOptions, NULL)) != -1) {
    switch (opt) {
      case 'b':
        flags->b = 1;
        break;
      case 'e':
        flags->e = 1;
        flags->v = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 't':
        flags->t = 1;
        flags->v = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'T':
        flags->t = 1;
        break;
      case 'E':
        flags->e = 1;
        break;
    }
  }
}

void process_file(char *argv[], struct flag *flags) {
  int i = optind;
  while (argv[i]) {
    FILE *file;
    if ((file = fopen(argv[i], "r")) != NULL) {
      int new_str = 1;
      int index = 0;
      char symbol;
      int empty_str = 0;
      char pre_symbol;
      while ((symbol = fgetc(file)) != EOF) {
        if (flags->n == 1 && flags->b == 0) flag_n(new_str, &index);
        if ((flags->b == 1 && flags->n == 1) ||
            (flags->b == 1 && flags->n == 0)) {
          flag_b(new_str, &index, symbol);
        }
        if (flags->s == 1) flag_s(symbol, new_str, &empty_str);
        if (flags->v == 1) flag_v(&symbol);

        if (flags->e == 1) {
          flag_e(symbol, flags, pre_symbol);
          flag_v(&symbol);
        }
        if (flags->t == 1) {
          flag_t(&symbol);
          flag_v(&symbol);
        }
        if (symbol == '\n') {
          new_str = 1;
        } else {
          new_str = 0;
        }
        if (empty_str != 2) printf("%c", symbol);
        pre_symbol = symbol;
      }
      fclose(file);
    } else
      printf("ERROR\n");
    i++;
  }
}

void flag_n(int new_str, int *index) {
  if (new_str == 1) {
    *index += 1;
    printf("%6d\t", *index);
  }
}

void flag_b(int new_str, int *index, char symbol) {
  if (new_str == 1) {
    if (symbol != '\n') {
      *index += 1;
      printf("%6d\t", *index);
    }
  }
}

void flag_e(char symbol, struct flag *flags, char pre_symbol) {
  if (flags->b && symbol == '\n' && pre_symbol == '\n') {
    printf("%6s\t$", " ");
  } else if (symbol == '\n') {
    printf("$");
  }
}

void flag_t(char *symbol) {
  if (*symbol == '\t') {
    printf("^");
    *symbol += 64;
  }
}

void flag_s(char symbol, int new_str, int *empty_str) {
  if (symbol == '\n' && new_str == 1) {
    if (*empty_str > 0) {
      *empty_str = 2;
    } else {
      *empty_str = 1;
    }
  } else {
    *empty_str = 0;
  }
}

void flag_v(char *symbol) {
  if ((*symbol >= 0 && *symbol < 9) || (*symbol > 10 && *symbol < 32)) {
    printf("^");
    *symbol += 64;
  } else if (*symbol == 127) {
    printf("^");
    *symbol = '?';
  }
}
