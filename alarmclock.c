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

void f_l() {
    printf("This is the list function");
}

void f_c() {
    printf("This is the cancel function");
}

void f_x() {
    printf("This is the exit function");
}

int selectFunction(char c) {
    switch( c )
{
	case 's':
        f_s();
        break;
	case 'l':
        f_l();
        break;
	case 'c':
        f_x();
        break;
    case 'x':
        f_x();
        break;
    case '\n':
        break;
    default:
        printf("invalid selection");
        return -1;
}
    return 0;
}
//Data init




int main() {

    time_t result = time(NULL);
    
    printf("Welcome to the alarm clock! It is currently %s \nPlease enter \"s\" (schedule), \"l\" (list), \"c\" (cancel), \"x\" (exit)" , ctime(&result));
    int run;
    run = 1;
    while (run) {
        char selectedFunction;
        selectedFunction = getchar() ;
        int sf = selectFunction(selectedFunction);
        run = sf;
        if (run == -1) {
            printf("Error");
        };
    }
    return 0;
}




//note to self: int scanf(const char *format, ...)
//=> *variable means overloadable