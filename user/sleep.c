#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char **argv) {
    int i;
    if(argc != 2){
        fprintf(2, "Usage: sleep needs 1 parameter, gets %d parameter(s) instead.\n", argc-1);
        exit(1);
    }
    i = atoi(argv[1]);
    if(i <= 0)
        exit(1);
    sleep(i);
    exit(0);
}