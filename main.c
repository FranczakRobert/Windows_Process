// SO IN1 20C LAB06
// Robert Franczak
// fr47434@zut.edu.pl
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <errno.h>
#include <windows.h>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("Błędna ilośc argumentów \n");
        return 1;
    }

    int arg_length = strlen(argv[1]);

    if (arg_length > 20)
    {
        printf("Argument ma za dużo znaków \n");
        return 1;
    }

    if (arg_length == 1)
        return argv[1][0];

    int first_half = arg_length / 2;
    int second_half = arg_length - first_half;

    char* argStr[2] = {
        malloc(sizeof(char) * (first_half + 1)),
        malloc(sizeof(char) * (second_half + 1))
    };

    memset(argStr[0],0,1 + first_half);
    memset(argStr[1],0,1 + second_half);
    for(int i = 0; i < first_half; i++)
    {
        argStr[0][i] = argv[1][i];
    }
    
    for(int i = first_half; i < arg_length; i++)
    {
        argStr[1][i - first_half] = argv[1][i];
    }

    PROCESS_INFORMATION processInfo[2];
    STARTUPINFOA startupInfo;

    memset(&startupInfo, 0, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);

    for (int i = 0; i < 2; ++i)
    {
        memset(&processInfo[i],0,sizeof(processInfo[0]));

        char strArg[100];
        sprintf_s(strArg, sizeof strArg,"%s %s",argv[0],argStr[i]);

             CreateProcessA(
             NULL,                       //nazwa programu do uruchomienia jako kod nowego procesu (wezmie sobie z 2 argumentu i tak)
             strArg,                     //łańcuch zawierający argumenty dla uruchamianego programu
             NULL,                       //SECURITY_ATTRIBUTES
             NULL,                       //SECURITY_ATTRIBUTES
             0,                          //!0 - odziedziczy wszystkie uchwyty utworzone przez rodzica 0 nie dziedziczy
             0,                          //ustawia priorytet dla nowego procesu
             NULL,                       //wskaźnik na środowisko, które otrzyma nowy proces albo NULL jeśli nowy proces ma odziedziczyć środowisko rodzica
             NULL,                       //łańcuch określający katalog bieżący dla nowego procesu albo NULL, jeśli proces ma pracować w tym samym katalogu bieżącym co rodzic
             &startupInfo,               //STARTUPINFOA
             &processInfo[i]             //PROCESS_INFORMATION
             );
    }

    int max = 0;
    for (int i = 0; i < 2; ++i)
    {
        long unsigned int result;
        WaitForSingleObject(processInfo[i].hProcess,INFINITE);
        GetExitCodeProcess(processInfo[i].hProcess,&result);
        printf("\n%6d %6d %10s %c",GetCurrentProcessId(),processInfo[i].dwProcessId,argStr[i],result);
        CloseHandle(processInfo[i].hProcess);
        CloseHandle(processInfo[i].hThread);

        if(result > max)
            max = result;
    }

    return max;
}


