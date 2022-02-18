#include <stdio.h>
#include <time.h>
#include <termios.h>
#include <math.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>


//Global static variables
time_t current_time;
//Forward declare all functions and variables
//variables and structs
typedef struct Alarm;


//functions
void setTime(time_t t);
void f_s(void);
void f_l(void);
void f_c(void);
void f_x(void);
int scheduleAlarm(char *targetTime);
int selectFunction(char c);
void ring(void);
void printAllAlarms(void);
int findFirstAvailableSpot(void);

//Definitions

//Struct definitions

typedef struct {
    char functionChar;
    char functionDescription[40];
} FunctionDescriptor;

typedef struct Alarm {
    struct tm targetTime;
    char alarmDescription[42];
    volatile pid_t PID; //we're just assuming this is "safe enough"
} blarm; //alarm is an alias for "struct Alarm" (for some reason "alarm" was a taken symbol def.)


blarm _gAlarmList[10] = {NULL}; //TODO: GLOBAL ACCESS and fill with relevant Alarms by PID from var or tmp
int firstAvailableSpot;



//Data init

//Function Definitions

int findFirstAvailableSpot() {
    for (int i = 0; i < 10 ; i++) {
            if (&_gAlarmList[i] == 0) {
                return i;
                //
                break;
            } else if (i >= 9 && &_gAlarmList != 0) {
                printf("No spots available");
                return 10;
            };
        };
}

void ring() {
    printf("RING!");
}

int outputAlarm(struct Alarm alarm) {
    char outDateString[40] = {'\0'};
    time_t secondsLeftFromStart = mktime(&alarm.targetTime) - time(NULL);

    strftime(outDateString, 39, "%a %m. %h %Y: %H:%M:%S", &alarm.targetTime);
    printf("Alarm target time: %s\n", outDateString);
    printf("Remaining time in seconds:%i \n",(int) secondsLeftFromStart);
    printf("Alarm Description: %s\n", alarm.alarmDescription);
    printf("\n");

    return 1;
}   //TODO: Check if %lu needs to be replaced with %ld

int scheduleAlarm(char *targetTime) {
    //
}

void f_s() {
    printf("This is the schedule function");

    firstAvailableSpot = findFirstAvailableSpot();
    if (findFirstAvailableSpot >= 10) {
        return;
    }
    printf("First available spot: %i", firstAvailableSpot);
    int selectedSpot = firstAvailableSpot;
    //int second, minute, hour, day, month, year;
    struct tm alarmTm;
    char timeString[40] = {0};
    char *arrValue;

    printf("Enter time in format: YYYY:MM:DD:HH:MM:SS\n");
    int invalidTime = 1;
    while (invalidTime) {
        //scanf("%04d:%02d:%02d:%02d:%02d:%02d", &year, &month, &day, &hour, &minute, &second); //aNNOYING
        scanf("%19c", &timeString);
        strptime(timeString, "%Y:%m:%d:%H:%M:%S", &alarmTm);
        //error catch: Invalid format, try again

        invalidTime = 0;
    }
    printf("Entered time is %s \n", timeString);
    char descString[42] = {0};

    printf("Enter a description (max 40 characters): \n");
    scanf(" %40[^\n]", &descString); 
    printf("Desc: %s\n", descString);
    
    
    blarm freshAlarm = {
        alarmTm,
        descString,
        0
    };

    pid_t PID = fork();

    if (PID == 0) {
        time_t secondCount = (mktime(&alarmTm)) - time(NULL);
        printf("Seconds Left: %lu \n", secondCount);
        //create alarm struct

        //TODO: Log in var.
        printf("Now running alarm: \n");
        int output = outputAlarm(freshAlarm);

        printf("");

        //sysout something
        //begin countdown
        clock_t begin;
        double time_spent;
        unsigned int i;
        //begin = clock();
        for (i=0;1;i++) {
            //time_spent = (double)(clock() - begin) / CLOCKS_PER_SEC;
            time_t secondsLeft = mktime(&freshAlarm.targetTime) - time(NULL);
            //printf("Time spent: %f",time_spent);
            if ((long) secondsLeft <= (long) 0) {
                ring();

                break;
            };
        };
        _gAlarmList[selectedSpot] = (blarm) {0};
        kill(getpid(),1);
    } else if (PID) {
        freshAlarm.PID = PID;
        _gAlarmList[selectedSpot] = freshAlarm;

        printf("This is the parent with ID %d, the child ID is %d", getpid(), PID);
        printf("");

    } // if parent
    //register PID in var or tmp
    

};

void f_l() {
    printf("This is the list function");
}

void f_c() {
    printf("This is the cancel function");

    pid_t pidTest = getpid();
    kill(pidTest, 1);

}

void f_x() {
    printf("Goodbye :)");
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
            f_c();
            break;
        case 'x':
            f_x();
            return 0;
        case '\n':
            break;
        default:
            printf("\ninvalid selection\n");
            return 1;
    }
        return 1;
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
    
    printf("Welcome to the alarm clock!");
    int run;
    run = 1;
    while (run) {
        printf("It is currently %s \nPlease press \"s\" (schedule), \"l\" (list), \"c\" (cancel), \"x\" (exit). Do not press enter: \n" , ctime(&result));
        char selectedFunction = getche(); //Getch returns the first given character, and does not wait for enter.
        int sf = selectFunction(selectedFunction);
        run = sf;
    }
    return 0;
}




//note to self: int scanf(const char *format, ...)
//=> *variable means overloadable

