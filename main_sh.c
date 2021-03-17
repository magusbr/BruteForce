#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char* objective = "6fc728864b65072bdcf0c09d89eb3fa8e94a82964f2818dbd87343a952fe9161";
const char* known = "VL7A";
//static const char VCHARS[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char VCHARS[] = "0123456789ABCDEFGHIJLMNOPQRSTUVZ";


char* cmd(char* try) {
  FILE *fp;
  char path[1035];
  static char ret[124] = {0};
    char cmd[128] = {0};

    sprintf(cmd, "/bin/bash hash.sh %s", try);
    //printf("command: %s\n", cmd);

  /* Open the command for reading. */
  fp = popen(cmd, "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }

  /* Read the output a line at a time - output it. */
  ret[0] = '\0';
  while (fgets(path, sizeof(path), fp) != NULL) {
    //printf("%s: %s", try, path);
    strcat(ret + strlen(ret), path);
  }

  printf("%s: %s", try, path);

  /* close */
  pclose(fp);

  return ret;
}

char* getnext() {
    static char ret[25] = {0};
    static char i = 0;
    static char j = 0;
    static char k = 0;
    static char l = 0;
    static char m = 0;
    static char n = 0;

    if (n == strlen(VCHARS)) { n = 0; m++; }
    if (m == strlen(VCHARS)) { m = 0; l++; }
    if (l == strlen(VCHARS)) { l = 0; k++; }
    if (k == strlen(VCHARS)) { k = 0; j++; }
    if (j == strlen(VCHARS)) { j = 0; i++; }
    if (i == strlen(VCHARS)) exit(1);

    sprintf(ret, "%c%c%c%c%c%c", VCHARS[i], VCHARS[j], VCHARS[k], VCHARS[l], VCHARS[m], VCHARS[n]);
    n++;
    return ret;
}

char* getnext4() {
    static char ret[25] = {0};
    static char i = 0;
    static char j = 0;
    static char k = 0;
    static char l = 0;
    
    if (l == strlen(VCHARS)) { l = 0; k++; }
    if (k == strlen(VCHARS)) { k = 0; j++; }
    if (j == strlen(VCHARS)) { j = 0; i++; }
    if (i == strlen(VCHARS)) exit(1);

    sprintf(ret, "%c%c%c%c", VCHARS[i], VCHARS[j], VCHARS[k], VCHARS[l]);
    l++;
    return ret;
}

char* getnext3() {
    static char ret[25] = {0};
    static char i = 0;
    static char j = 0;
    static char k = 0;

    if (k == strlen(VCHARS)) { k = 0; j++; }
    if (j == strlen(VCHARS)) { j = 0; i++; }
    if (i == strlen(VCHARS)) exit(1);

    sprintf(ret, "%c%c%c", VCHARS[i], VCHARS[j], VCHARS[k]);
    k++;
    return ret;
}

int main() {
    char entrada[256] = {0};// = "5BTVL7A";
    char* res;

    while(1) {
        sprintf(entrada, "%s%s", getnext(), known);
        res = cmd(entrada);

        //printf("res: %s", res);
        if (strncmp(objective, res, strlen(objective)) == 0)
        {
            printf("encontrou: %s", entrada);
            break;
        }
    }
}
