#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("expected usage: combine <filename>\n");
        return -1;
    }
    char jsFile[1024];
    strcpy(jsFile, argv[1]);
    jsFile[strlen(jsFile) - 2] = '\0';
    jsFile[strlen(jsFile) - 2] = 'j';
    jsFile[strlen(jsFile) - 1] = 's';
    int filenameLen = strlen(argv[1]);
    char command[4096];
    sprintf(command, "du -b %s | cut -f -1", argv[1]);
    FILE *filesizefp = popen(command, "r");
    int filesize;
    fgets(command, 4096, filesizefp);
    sscanf(command, "%d", &filesize);
    fclose(filesizefp);
    sprintf(command, "truncate -s %d %s", filesize - filenameLen - 27, argv[1]);
    system(command);
    sprintf(command, "printf \">\\n\" >> %s", argv[1]);
    system(command);
    sprintf(command, "cat %s %s >> %sTEMP", argv[1], jsFile, argv[1]);
    system(command);
    sprintf(command, "printf \"</script></body></html>\" >> %sTEMP", argv[1]);
    system(command);
    sprintf(command, "cp %sTEMP %s", argv[1], argv[1]);
    system(command);
    sprintf(command, "rm %sTEMP", argv[1]);
    system(command);
    sprintf(command, "rm %s", jsFile);
    system(command);
}