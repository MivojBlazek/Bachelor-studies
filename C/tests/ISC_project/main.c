#include <stdio.h>

typedef struct date
{
    int day;
    int month;
    int year;
} Tdate;

typedef struct student
{
    char login[100];
    char name[100];
    char surname[100];
} Tstudent;

typedef struct subject
{
    char name[100];
    unsigned code;
} Tsubject;

typedef struct timetable
{
    Tstudent stud_name;
    Tdate stud_date;
    Tsubject stud_subject;
} Ttimetable;

int are_dates_same(Tdate date1, Tdate date2)
{
    if (date1.year < date2.year)
        return -1;
    else if (date1.year == date2.year)
    {
        if (date1.month < date2.month)
            return -1;
        else if (date1.month == date2.month)
        {
            if (date1.day < date2.day)
                return -1;
            else if (date1.day == date2.day)
                return 0;
        }
    }
    return 1;
}

#define TRUE 1
#define FALSE 0

int has_student_subject(Ttimetable timetable[2], Tsubject *subject)
{
    for (int i = 0; i < 2; i++)
    {
        if (subject->code == timetable[i].stud_subject.code)
        {
            printf("%s\n", timetable[i].stud_name.name);
            return TRUE;
        }
    }
    return FALSE;
}

int days_between(Tdate date1, Tdate date2)
{
    if (date1.year != date2.year || date1.month != date2.month)
        return -1;
    if (date1.day < date2.day)
        return date2.day - date1.day;
    return date1.day - date2.day;
}

int main()
{
    Tdate date1 = {.day = 20, .month = 10, .year = 2008};
    Tdate date2 = {.day = 21, .month = 10, .year = 2008};


    Ttimetable timetable1[2] = {
        {.stud_name.login = "xxxxx00", .stud_name.name = "Josef", .stud_name.surname = "Novak",
         .stud_date.day = 5, .stud_date.month = 8, .stud_date.year = 2002,
         .stud_subject.name = "IZP", .stud_subject.code = 106},
        {.stud_name.login = "xblaze38", .stud_name.name = "Michal", .stud_name.surname = "Blazek",
         .stud_date.day = 3, .stud_date.month = 11, .stud_date.year = 2022,
         .stud_subject.name = "ISC", .stud_subject.code = 105}
                            };
    Tsubject subject1 = {.name = "ISC", .code = 105};




    int a = are_dates_same(date1, date2);
    if (a == 1)
        printf("Prvni datum je vetsi\n");
    else if (a == -1)
        printf("Druhy datum je vetsi\n");
    else
        printf("Data jsou stejna\n");


    int b = days_between(date1, date2);
    if (b == -1)
        printf("Roky nebo mesice se neshoduji\n");
    else
        printf("Pocet dni mezi daty je %d\n", b);


    int c = has_student_subject(timetable1, &subject1);


    return 0;
}
