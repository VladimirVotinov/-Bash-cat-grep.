#include "s21_grep.h"
int main(int argc, char *argv[]) {
  if (argc >= 3) {
    struct flag flags = {0};
    char findStr[1024] = {0};
    check_flag(argc, argv, findStr, &flags);
    process_grep(argc, argv, &flags, findStr);
  } else {
    printf("ERROR\n");
  }
  return 0;
}

void check_flag(int argc, char *argv[], char *findStr, struct flag *flags) {
  int option;
  while ((option = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (option) {
      case 'e':
        flags->e = 1;
        snprintf(findStr, 1024, "%s", optarg);
        break;
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'f':
        flags->f = 1;
        snprintf(findStr, 1024, "%s", optarg);
        break;
      case 'o':
        flags->o = 1;
        break;
      default:
        printf("ERROR\n");
        break;
    }
  }
}

void process_grep(int argc, char *argv[], struct flag *flags, char *findStr) {
  int filePatt = 0;
  char pattern[1024] = {0};
  if (!flags->f && !flags->e) snprintf(pattern, 1024, "%s", argv[optind++]);
  if (flags->f) filePatt = openFile(pattern, findStr);
  if (!flags->f && flags->e) snprintf(pattern, 1024, "%s", findStr);
  if (filePatt != -1) {
    int fileCount = 0;
    if (argc - optind > 1) fileCount = 1;
    for (int i = optind; i < argc; i++) {
      if (fileCount && !flags->h && !flags->l) printf("%s:", argv[i]);
      Search(flags, pattern, argv[i]);
    }
  }
}

int openFile(char *pattern, char *fileName) {
  FILE *fp;
  fp = fopen(fileName, "r");
  int i = 0;
  if (fp != NULL) {
    int c;
    while ((c = fgetc(fp)) != EOF) {
      if (c == 13 || c == 10) pattern[i++] = '|';
      if (c != 13 && c != 10) pattern[i++] = c;
    }
  } else {
    printf("ERROR\n");
    i = -1;
  }
  if (pattern[i - 1] == '|') pattern[i - 1] = '\0';
  fclose(fp);
  return i;
}

void Search(struct flag *flags, char *pattern, char *fileName) {
  int cflags = REG_EXTENDED;
  regex_t regular;
  FILE *fp;
  fp = fopen(fileName, "r");
  if (flags->i) cflags = REG_ICASE;
  if (fp != NULL) {
    regcomp(&regular, pattern, cflags);
    char textTest[1024] = {0};
    regmatch_t pmatch[1];
    int lineMatchCount = 0;
    int lineNum = 1;
    int maxMatch = 5;
    while (fgets(textTest, 1024, fp) != NULL) {
      char *current = textTest;
      int match = 0;
      int success = regexec(&regular, textTest, 1, pmatch, 0);
      if (strchr(textTest, '\n') == NULL) strcat(textTest, "\n");
      if (success == 0 && !flags->v) match = 1;
      if (success == REG_NOMATCH && flags->v) match = 1;
      if (match && !flags->l && !flags->c && flags->n) printf("%d:", lineNum);
      if (match && !flags->l && !flags->c && !flags->o) printf("%s", textTest);
      if (match && flags->o) {
        for (int i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++) {
          printf("%c", textTest[i]);
        }
        printf("\n");
        int offset = pmatch[0].rm_eo;
        current += offset;
        for (int m = 0; m < maxMatch; m++) {
          if (regexec(&regular, current, 1, pmatch, 0)) break;
          for (int i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++) {
            printf("%c", current[i]);
          }
          printf("\n");
          offset = pmatch[0].rm_eo;
          current += offset;
        }
      }
      lineMatchCount += match;
      lineNum++;
    }
    if (flags->l && lineMatchCount > 0) printf("%s\n", fileName);
    if (flags->c && !flags->l) printf("%d\n", lineMatchCount);
    regfree(&regular);
    fclose(fp);
  } else {
    if (!flags->s) perror(fileName);
  }
}
