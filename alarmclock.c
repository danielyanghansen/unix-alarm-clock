#include <stdio.h>
#include <time.h>
#include <termios.h>
#include <math.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


//Global static variables
time_t current_time;
//Forward declare all functions and variables
//variables and structs
typedef struct Alarm;


//functions
void setTime(time_t t);
int f_s(void);
void f_l(void);
void f_c(void);
void f_x(void);
int scheduleAlarm(char *targetTime);
int selectFunction(char c);
void ring(int);
void printAllAlarms(void);

//int insert_alarm(struct tm);
//-
//-

//Definitions

int NUM_ALARMS = 10;

//Struct definitions

typedef struct {
    char functionChar;
    char functionDescription[40];
} FunctionDescriptor;

typedef struct Alarm {
    struct tm targetTime;
//    char alarmDescription[42];
    pid_t PID; //we're just assuming this is "safe enough"
} alarmStruct; //alarm is an alias for "struct Alarm" (for some reason "alarm" was a taken symbol def.)


alarmStruct* all_alarms;



//Data init

//Function Definitions

//============================================FUNCTIONS USED "GLOBALLY"=======================================================
//============================================GETCH========================================
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

//===========================================ALARM RELATED CODE=============================================
void ring(int soundIndex) {
    #ifdef __linux
        execlp("mpg123","mpg123","-q", "sonic-rings-drop.mp3" ,0);
    #elif __APPLE__
        execlp("afplay", "afplay", "sonic-rings-drop.mp3", 0); //Note: None of us have a mac, so this is hard to test
    #else
        printf("RING!\n");
    #endif
}

int outputAlarm(struct Alarm alarmStruct) {
    char outDateString[40] = {'\0'};
    time_t secondsLeftFromStart = mktime(&alarmStruct.targetTime) - time(NULL);

    strftime(outDateString, 39, "%a %d. %B %Y: %H:%M:%S", &alarmStruct.targetTime);
    printf("Alarm target time: %s\n", outDateString);
    printf("Remaining time in seconds:%i \n",(int) secondsLeftFromStart);
    printf("\n");

    return 1;
}   //TODO: Check if %lu needs to be replaced with %ld

void start_alarm_process(struct tm *newTargetTime) {
    time_t secondCount = (mktime(newTargetTime)) - time(NULL);
    printf("Seconds Left: %lu \n", secondCount);
    printf("");
    //=================START ALARM PROCESS==========
    while (1) {
        time_t secondsLeft = mktime(newTargetTime) - time(NULL);
        if ((long) secondsLeft <= (long) 0) {
            ring(1);

            break;
        };
        sleep(0.2f); //Save a few resources.
    };

    //Terminate process and drop out of list
    
    //kill(getpid(),1);
    exit(0);
    return;
}

pid_t add_alarm_process(struct tm newTargetTime, int alarmIndex) {

    pid_t pid = fork();
    if (pid == -1) {
        printf("\nError initializing child process\n");
        return -1;
    }
    if( pid != 0) {
        printf("\nDette er alarmIndex\n");

        all_alarms[alarmIndex] = (alarmStruct){newTargetTime, pid};
        outputAlarm(all_alarms[alarmIndex]);
    };
    if (pid == 0) {
        start_alarm_process(&newTargetTime);
    };
    return getpid();
}

int insert_alarm(struct tm newTargetTime) {
    for (int i = 0; i <= NUM_ALARMS - 1; i++) {
        if (all_alarms[i].PID == 0) {
            add_alarm_process(newTargetTime, i);
            return i;
        };     
    };
    return NUM_ALARMS;
};

void remove_alarm(int index) {
    if (all_alarms[index].PID==0) {
        printf("Already deleted");
        return;
    }
    kill(all_alarms[index].PID, 1);
    all_alarms[index] = (alarmStruct){0,0};
}

void list_alarms() {
    for (int i = 0; i < NUM_ALARMS; i++) {
            if (all_alarms[i].PID != 0) {
            printf("Alarm #%i:\n", i+1);
            outputAlarm(all_alarms[i]);
        }
    }
    return;
}

void process_cleanup() {
    for (int i = 0; i <= NUM_ALARMS - 1; i++) {
        if (all_alarms[i].PID == 0) continue;

        int wstatus;
        pid_t pid = waitpid(all_alarms[i].PID, &wstatus, WNOHANG | WUNTRACED | WCONTINUED);

        if (pid > 0) remove_alarm(i);
    }
}

//==============================================LOOP SELECTED FUNCTIONS========================================================

int f_s() {
    printf("\nThis is the schedule function\n");

    //check available spots
    for (int i = 0; i <= NUM_ALARMS -1; i++ ) {
        if (all_alarms[i].PID == 0) {
            break;
        }
        if (i == NUM_ALARMS -1 && all_alarms[i].PID != 0) {
            printf("No spot available, cancelling alarm planner\n");
            return 1;
        }
    }

    struct tm alarmTm;
    memset(&alarmTm, 0, sizeof(struct tm));
    time_t currentTime = time(NULL);
    struct tm* localTm = localtime(&currentTime);

    //Use time zone settings from local timezone
    alarmTm.tm_isdst = localTm->tm_isdst;
    alarmTm.tm_zone = localTm->tm_zone;
    alarmTm.tm_gmtoff = localTm->tm_gmtoff;
    
    char timeString[40] = {0};

    printf("Enter time in format: YYYY:MM:DD:HH:MM:SS\n");
    int invalidTime = 1;
    while (invalidTime) {
        scanf("%19c", &timeString);
        strptime(timeString, "%Y:%m:%d:%H:%M:%S", &alarmTm);
        //TODO: error catch: Invalid format, try again

        invalidTime = 0;
    }
    char outDateString[40] = {0};

    strftime(outDateString, 39, "%a %d. %B %Y: %H:%M:%S", &alarmTm);
    printf("Alarm target time: %s\n", outDateString);

    int usedIndex = insert_alarm(alarmTm);
    printf("Alarm made at index %i\n", usedIndex);
    
    
};

void f_l() {
    printf("\nThis is the list function\n");
    list_alarms();
}

void f_c() {
    printf("\nThis is the cancel function\nSelect an alarm to cancel by entering its index");
    list_alarms();
    while (1) {
        int inputIndexNumber;
        scanf("%d", &inputIndexNumber);
        if (inputIndexNumber > NUM_ALARMS -1 || inputIndexNumber < 0) {
            printf("Index out of bounds, try again\n");
            continue;
        }else{
            remove_alarm(inputIndexNumber -1);
            printf("\nAlarm deleted.\n");
            break;
        }
    }

}

void f_x() {
    printf("\nCancelling all remaining alarms\n");
    for (int i = 0; i <= NUM_ALARMS; i++) {
        if (all_alarms[i].PID != 0) {
            remove_alarm(i);
        }
    }
    printf("\nGoodbye :)\n");
}

//=====================================================CORE LOOP===================================================================

int selectFunction(char c) {
    switch( c )
    {
        case 's':
            return f_s();
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


int main() {
    // setup
    all_alarms = (alarmStruct*) malloc(NUM_ALARMS * sizeof(alarmStruct));

    memset(all_alarms, 0, NUM_ALARMS * sizeof(alarmStruct));

    //welcome message
    time_t result = time(NULL);
    printf("Welcome to the alarm clock!\n");
    int run;
    run = 1;
    while (run) {
        printf("It is currently %s \nPlease press \"s\" (schedule), \"l\" (list), \"c\" (cancel), \"x\" (exit). Do not press enter: \n> " , ctime(&result));
        char selectedFunction = getche(); //Getch returns the first given character, and does not wait for enter.
    

        process_cleanup();

        int sf = selectFunction(selectedFunction);
        run = sf;
        printf("\n");
    }
    return 0;
}

