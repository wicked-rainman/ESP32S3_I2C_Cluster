#include <string.h>
void CharReplace(char *target, char FromChar, char ToChar) {
    int k;
    int len;
    len=strlen(target);
    for(k=0;k<len;k++) {
        if(target[k] == FromChar) target[k]=ToChar;
    }
}
