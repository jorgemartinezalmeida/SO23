#include <stdio.h>


int main(int argc, char *argv[]){
int i;
for (i = 0; i < 5; i++) {
    /* code */
    if (i==2) {
        continue;

        /* code */
    }
    if (i>3) {
        break;
        /* code */
    }
    printf("%d\n",i );
}
return 0;
}
