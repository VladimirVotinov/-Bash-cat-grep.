#ifndef CAT
#define CAT

#include <getopt.h>
#include <stdio.h>

struct flag {
  int n, b, e, t, s, v;  //
};

void check_flag(int argc, char *argv[], struct flag *flags);
void process_file(char *argv[], struct flag *flags);
void flag_n(int new_str, int *index);
void flag_b(int new_str, int *index, char symbol);
void flag_e(char symbol, struct flag *flags, char pre_symbol);  // int* index,
void flag_t(char *symbol);
void flag_s(char symbol, int new_str, int *empty_str);
void flag_v(char *symbol);
#endif
