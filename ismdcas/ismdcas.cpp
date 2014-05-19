#include "stdafx.h"
#include "../cas/cas.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
 return TRUE;
}

void WINAPI part2(void *,WCHAR *arg[])
{
 casStartW(arg[0],arg[1],2,1,0);
}
