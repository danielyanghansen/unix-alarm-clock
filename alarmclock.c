#include <stdio.h>
#include <time.h>
#include <termios.h>


//Global static variables

//Forward declare all functions and variables
//variables and structs
typedef struct Alarm;

//functions
void setTime(time_t t);
void f_s();
void f_l();
void f_c();
void f_x();
int scheduleAlarm(char *targetTime);
int selectFunction(char c);

//Definitions

//Struct definitions

typedef struct {
    char functionChar;
    char functionDescription[20];
} FunctionDescriptor;

typedef struct Alarm {
    time_t secondsLeft;
    struct tm targetTime;
    char alarmDescription[40];
} alarm; //alarm is an alias for "struct Alarm"

alarm _gAlarmList[10];

//Data init

//Function Definitions
void f_s() {
    printf("This is the schedule function");
    //int second, minute, hour, day, month, year;
    struct tm alarmTm;
    char timeString[20] = {0};
    char *arrValue;

    printf("Enter time in format: YYYY:MM:DD:HH:MM:SS\n");
    int invalidTime = 1;
    while (invalidTime) {
        //scanf("%04d:%02d:%02d:%02d:%02d:%02d", &year, &month, &day, &hour, &minute, &second); //aNNOYING
        scanf("%19c", &timeString);
        arrValue = strptime(timeString, "%Y:%m:%d:%H:%M:%S", &alarmTm);
        //error catch: Invalid format, try again
        invalidTime = 0;

    }
    
    printf("Entered time is %s\n", timeString);
    // loop(take target time input |> parse input, repeat if invalid)
    // |> loop(take target text input |> verify char array 40 or smaller)
    // |> fork,
    // child: |> create Alarm struct on previous info |> loop (counting down alarm) |> alarm goes off!
    // parent: |> register child PID
};

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

int scheduleAlarm(char *targetTime) {
    //
}

//getch() / getche() code

static struct termios old, current;

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  current = old; /* make new settings same as old settings */
  current.c_lflag &= ~ICANON; /* disable buffered i/o */
  if (echo) {
      current.c_lflag |= ECHO; /* set echo mode */
  } else {
      current.c_lflag &= ~ECHO; /* set no echo mode */
  }
  tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
  return getch_(1);
}


int main() {

    time_t result = time(NULL);
    
    printf("Welcome to the alarm clock! It is currently %s \nPlease enter \"s\" (schedule), \"l\" (list), \"c\" (cancel), \"x\" (exit)" , ctime(&result));
    int run;
    run = 1;
    while (run) {
        char selectedFunction;
        selectedFunction = getch(); //TODO: try to use scanf() maybe? RN we can enter a char[], which is bad. 
        int sf = selectFunction(selectedFunction);
        run = sf;
    }
    return 0;
}




//note to self: int scanf(const char *format, ...)
//=> *variable means overloadable