#include <stdio.h>
#include <myfuncs.h>



int main(void)
{
    int side = 0;
    char answer;
	char gotNumber[3];
    do {
        printf("which side?: ");
			fgets(gotNumber, 3, stdin);

			answer = tolower(gotNumber[0]);
            side = answer - '0';
    } while (side == 0);

    printf("picked %d", side);

    if (side == 1){
        cookiejar();
    } 
    else if (side == 2){
        cookieeater();
    }
}