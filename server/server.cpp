#include "stdafx.h"
#include <stdio.h>
#include "../cas/cas.h"

void __cdecl main(int c,char **v)
{
 if(c<3)
  puts("server protseq endpoint");
 else
 {
  casStartA(v[1],v[2],2,1,0);
 }
}
