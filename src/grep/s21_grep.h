#ifndef GREP
#define GREP

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

struct flag {
  int e, i, v, c, l, n, h, s, f, o;
  int mult_file_flag;
};

void check_flag(int argc, char *argv[], char *findStr, struct flag *flags);
void process_grep(int argc, char *argv[], struct flag *flags, char *findStr);
int openFile(char *pattern, char *fileName);
void Search(struct flag *flags, char *pattern, char *fileName);
#endif