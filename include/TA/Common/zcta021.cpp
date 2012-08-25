//---------------------------------------------------------------------------------
// File Name: zcta021.cpp
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

#include "../Common/zcta017.h"
#ifdef lOOlOlOllll
#ifdef lOlOlOOllO
#ifdef TA_MSVC
#undef TA_MSVC
#endif
#endif
#ifdef TA_MSVC
#include <Windows.h>
#endif
nam\
espace\

TA
{
lOO\
lOOll\
lll
(
lOO\
lO\
lO\
l\
l\
Ol\

)
s6\
4
lO\
OlO\
lO\
llO\
l\

::
GetT\
i\
m\
e
(
)
cons\
t\

{
#ifndef TA_MSVC
ret\
urn
1
;
#else
un\
ion\

{
st\
ruc\
t
{
u3\
2
nLowP\
a\
r\
t\
_res\
erved
;
s3\
2\

nH\
i\
ghP\
art_r\
e\
s\
e\
rved
;
}
split_re\
serv\
e\
d
;
s6\
4
lOOll\
llOOll
;
}
iCloc\
ks_reser\
ved
;
__asm	{
	// SpellCheckerDisable
	//	_emit 0x0f	//cpuid - serialise the processor
	//	_emit 0xa2
		__emit 0fh	// rdtsc instruction opcodes
		__emit 031h
		mov iClocks_reserved.split_reserved.nHighPart_reserved, edx
		mov iClocks_reserved.split_reserved.nLowPart_reserved, eax
	// SpellCheckerEnable
	}
return
iCloc\
k\
s_reserve\
d\

.
lOO\
llll\
OOll\

;
#endif
}
lOOlOlO\
llOl
::
lOOlOlOllOl\

(
)
{
lOOll\
llOlOO
=
false\

;
}
lOOl\
O\
lO\
llOl
::
~
lO\
O\
lOlO\
llOl
(
)
{
Fin\
ali\
se\

(
)
;
}
void
lO\
OlOlOllOl\

::
In\
it\
ialise
(
)
{
#ifdef TA_MSVC
#endif
}
void
lO\
Ol\
OlOl\
lOl
::
Fina\
l\
ise
(
)
{
}
}
#endif //