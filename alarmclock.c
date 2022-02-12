#include <stdio.h>
#include <time.h>

//Forward declare all functions and variables
//variables and structs
typedef struct Alarm AllAlarms;

//functions
void setTime(time_t t);
void f_s();
void f_l();
void f_c();
void f_x();
int selectFunction(char c);

//Definitions

//Struct definitions

typedef struct {
    char functionChar;
    char functionDescription[20];
} FunctionDescriptor;

typedef struct Alarm {
    int random;
} alarm;

typedef struct AllAlarms{
    int testing;
    int testingList[12];
    alarm alarms[50];

 } allAlarms;

//Function Definitions
void f_s() {
    printf("This is the schedule function");
}

int selectFunction(char c) {
    switch( c )
{
	case 's':
        f_s();
        break;
	case 'l':
        printf("l not implemented");
        break;
	case 'c':
        printf("c not implemented");
        break;
    case 'x':
        printf("x not implemented");
        break;
    default:
        printf("invalid selection");
        return 0;
}
    return 1;
}
//Data init




int main() {

    time_t result = time(NULL);
    
    printf("Welcome to the alarm clock! It is currently %s \nPlease enter \"s\" (schedule), \"l\" (list), \"c\" (cancel), \"x\" (exit)" , ctime(&result));
    while (1) {
        char selectedFunction = scanf(3);
        int sf = selectFunction(selectedFunction);
    }
    return 0;
}