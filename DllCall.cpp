#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "DllCall.h"
#include <QDebug>

typedef int(*dll_Init_def)(unsigned char**);
dll_Init_def dll_Init_p = NULL;

typedef int(*dll_Run_def)(unsigned char*, unsigned char*, int);
dll_Run_def dll_Run_p = NULL;

typedef int(*dll_DeInit_def)(unsigned char**);
dll_DeInit_def dll_DeInit_p = NULL;

HINSTANCE hdll = NULL;

int closeLibrary()
{
    if (hdll) FreeLibrary(hdll);
    dll_Init_p = NULL;
    dll_Run_p = NULL;
    dll_DeInit_p = NULL;

    return 0;
}

int loadLibrary()
{
    hdll = LoadLibrary(L"DllLib.dll");

    if (hdll)
    {
        // Bind functions
        dll_Init_p = (dll_Init_def)GetProcAddress(hdll, "dll_Init");
        dll_Run_p = (dll_Run_def)GetProcAddress(hdll, "dll_Run");
        dll_DeInit_p = (dll_DeInit_def)GetProcAddress(hdll, "dll_DeInit");

        if (dll_Init_p == NULL ||
            dll_Run_p == NULL ||
            dll_DeInit_p == NULL)
        {
            printf("can not load func addr!!!\n");
            FreeLibrary(hdll);
            return 1;
        }

        return 0;
    }
    else
    {
        printf("loadLibrary fail \n");
        return 1;
    }

    return 0;
}

int dllCall(void)
{
    if(loadLibrary()==0)
    {
        unsigned char* ucHandle = NULL;
        int test_length = 64;
        unsigned char* testStr = (unsigned char *)malloc(test_length);
        strcpy((char*)testStr, "handle test");
        dll_Init_p(&ucHandle);
        dll_Run_p(ucHandle, testStr, test_length);
        qDebug()<<"ucHandle =" << ucHandle;
        qDebug()<<"ucHandle content = " << (char*)ucHandle;
        dll_DeInit_p(&ucHandle);
        qDebug()<<"ucHandle =" << ucHandle;
        closeLibrary();
        free(testStr);
    }
    return 0;
}
