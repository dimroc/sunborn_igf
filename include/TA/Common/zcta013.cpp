//---------------------------------------------------------------------------------
// File Name: zcta013.cpp
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

#define lOOOlOlllll
#include "MemoryMgr.h"
#ifndef TA_DEBUG_H
#include "Debug.h"
#endif //
#ifndef TA_TYPES_H
#include "Types.h"
#endif //
#ifdef TA_MSVC
#include <malloc.h>
#endif //
#ifdef TA_GCC
#include <malloc.h>
#endif //
#include <stdlib.h>
na\
mes\
pa\
ce
TA
{
#ifndef DOXYGEN
#ifdef _DEBUG
int
lOOO\
lOll\
l\
OO\

=
0
;
char
*
lOO\
Ol\
Oll\
lOl
=
0
;
#endif //
static\

Me\
m\
ory\
M\
gr
::
Fn\
A\
l\
l\
ocCallBa\
ck
lOO\
O\
llOOOll\

=
0
;
st\
at\
ic
MemoryM\
gr
::
FnF\
reeCa\
l\
lB\
ack\

lO\
OOllOO\
lOO
=
0
;
sta\
tic
MemoryM\
gr
::
FnDbgAllocCa\
llBack
lOOOllOOlOl
=
0
;
static
Mem\
oryM\
gr
::
FnD\
bgFreeCallBack
lOOOllO\
OllO
=
0
;
static
in\
t
lOOO\
llOOl\
l\
l\

=
0
;
static\

int
lO\
O\
Oll\
OlO\
OO
=
0
;
#endif //
bool\

TAC\
_CALL\

Me\
mo\
r\
yM\
gr
::
SetAl\
l\
o\
c\

(
FnAll\
oc\
Cal\
lBack\

lO\
OO\
l\
l\
OlOOl\

)
{
if
(
lOO\
Ol\
lOOlll\

!=
0
&&
lO\
OO\
l\
l\
OlOOO
!=
0
)
ret\
ur\
n
false
;
lOOOllOOOll
=
lOOOllOl\
OOl\

;
return
true\

;
}
bool
TAC\
_CALL
Memo\
ryM\
g\
r
::
Set\
F\
ree
(
FnFr\
eeCall\
B\
a\
ck
lO\
OOllOlOlO\

)
{
if
(
lOOOllOOlll
!=
0
&&
lOO\
Ol\
l\
OlOO\
O\

!=
0
)
return
fal\
s\
e
;
lOOO\
llOOlOO\

=
lO\
OO\
llOl\
OlO
;
return
true\

;
}
bo\
o\
l\

TAC\
_\
C\
ALL\

MemoryM\
g\
r\

::
Se\
tDbgAll\
oc
(
FnDbgAl\
l\
oc\
C\
al\
lBack
lOOOllOlOl\
l
)
{
if
(
lOO\
Ol\
lOOlll
!=
0
&&
lOOO\
ll\
OlOOO\

!=
0
)
return
false
;
lOO\
O\
ll\
OO\
l\
Ol\

=
lOOOllOlOl\
l
;
re\
turn
true
;
}
bool\

TAC_CA\
LL
Mem\
oryMg\
r\

::
SetDbg\
Fre\
e\

(
FnDbgFree\
CallBa\
ck\

lOOO\
l\
lOllOO
)
{
if
(
lOOOllOOll\
l
!=
0
&&
lOOOll\
OlO\
O\
O
!=
0
)
return
false
;
lOOO\
ll\
OOllO
=
lOOOllOllOO\

;
retu\
rn
tru\
e
;
}
voi\
d\

TA\
C_\
C\
AL\
L
MemoryMg\
r
::
lO\
OOll\
OOOOO
(
)
{
lOOOllOO\
l\
ll\

++
;
}
void\

TAC_CALL
MemoryMg\
r
::
lOO\
OllOO\
O\
Ol
(
)
{
lOOOllOOlll
--
;
TA_ASSE\
RT
(
lOOOll\
O\
Ol\
ll
>=
0
)
;
}
void\

*
TA\
C_\
CALL
Me\
mo\
ryMgr
::
Db\
g\
Alloc\

(
unsig\
n\
e\
d
in\
t
lOOl\
l\
lOllO
,
un\
signed\

int
lOOOllOll\
O\
l
,
const
char
*
lllOOOl\
OO\
l
,
int
lllOO\
OOOlO
)
{
lOOOll\
OlOOO
++
;
void\

*
lO\
OlllO\
OO\
O
;
if
(
TA\

::
lOOOll\
OOlO\
l\

)
lOO\
lllOOOO
=
TA
::
lO\
O\
O\
llOOlOl
(
lOOl\
l\
lOl\
l\
O
,
lO\
OOll\
Ol\
lOl
,
ll\
lO\
OOlOOl
,
lllOOOOOlO
)
;
else
#if defined(lOlOlOOllO)
re\
tu\
r\
n
me\
mal\
ign
(
lOOO\
llOll\
O\
l\

,
lOOlllOl\
lO
)
;
#elif defined(TA_GCC)
re\
tur\
n\

__mingw_\
aligned_ma\
lloc
(
lOO\
l\
llOllO
,
lOOOllO\
llOl
)
;
#else
ret\
ur\
n
_al\
igned_\
mallo\
c
(
lO\
Oll\
l\
OllO
,
lOO\
Ol\
lOllOl
)
;
#endif
lOOOOOOl\
lO
(
(
(
uS\
iz\
e
)
lO\
OlllOOOO
&
(
lO\
OO\
l\
l\
O\
llOl\

-
1
)
)
==
0
,
"MemoryMgr::DbgAlloc: memory not aligned.")
;
retu\
r\
n
lO\
Oll\
lO\
OOO
;
}
vo\
id\

TAC_CALL\

MemoryMg\
r
::
DbgFree\

(
void
*
lOOOl\
OllOl\
O\

,
con\
st
char\

*
lllOOO\
lOOl
,
int
ll\
lO\
OOOOl\
O
)
{
lOOOl\
l\
Ol\
OOO
--
;
lOOOOOO\
ll\
O
(
lOO\
Oll\
OlOOO
>=
0
,
"MemoryMgr::Free: More memory deallocated then allocated")
;
if\

(
TA
::
lO\
O\
O\
l\
lOOllO
)
TA
::
lO\
OO\
llO\
O\
llO
(
lO\
OOlOl\
l\
O\
lO
,
lllO\
OOlOO\
l
,
lllOOOOOlO
)
;
els\
e\

#if defined(lOlOlOOllO)
fr\
e\
e\

(
lOO\
OlOl\
l\
OlO
)
;
#elif defined(TA_GCC)
__mi\
ngw_aligned\
_\
fr\
e\
e\

(
lO\
OOlOllOlO
)
;
#else
_ali\
gned\
_f\
ree
(
lOOO\
l\
OllO\
lO\

)
;
#endif
}
voi\
d
*
TA\
C_CALL
MemoryMg\
r\

::
Alloc
(
uns\
ig\
ned
int
lOOlllO\
ll\
O
,
unsig\
ned
int
lOOO\
llO\
ll\
Ol
)
{
lO\
OO\
ll\
Ol\
OO\
O
++
;
if\

(
TA
::
lOOOl\
lOOOll
)
retu\
rn
TA
::
lOOOllOOOll
(
lOO\
l\
llOllO\

,
lO\
O\
OllOl\
lOl
)
;
else
#if defined(lOlOlOOllO)
ret\
u\
rn
memali\
gn
(
lOOOllOll\
Ol
,
lOOl\
l\
lOllO
)
;
#elif defined(TA_GCC)
retu\
rn
__m\
i\
n\
g\
w_a\
l\
i\
gned\
_ma\
lloc
(
lO\
OlllO\
llO
,
lO\
OO\
l\
l\
OllOl
)
;
#else
retur\
n
_a\
l\
igne\
d_\
m\
a\
lloc
(
lOOl\
l\
l\
Oll\
O\

,
lOO\
Oll\
OllO\
l
)
;
#endif
}
void
TA\
C_CA\
L\
L
Mem\
o\
ryMgr
::
Fre\
e
(
void
*
lOOO\
lOll\
Ol\
O
)
{
lOOOllOlO\
OO
--
;
lO\
OOOOOllO
(
lOOOllO\
lOO\
O
>=
0
,
"MemoryMgr::Free: More memory deallocated then allocated")
;
if
(
TA
::
lOOO\
llOOl\
O\
O
)
TA
::
lOOO\
llOOl\
OO
(
lOO\
OlO\
llOl\
O
)
;
else\

#if defined(lOlOlOOllO)
free\

(
lOOOlOllOlO
)
;
#elif defined(TA_GCC)
__mi\
n\
gw_al\
igned_\
free
(
lOO\
OlOllOlO\

)
;
#else
_alig\
ne\
d\
_free
(
lOOOlOll\
OlO\

)
;
#endif
}
int
TA\
C_CALL
Mem\
oryM\
gr
::
GetNumA\
l\
lo\
cations\
Un\
Fr\
eed
(
)
{
retur\
n
lOO\
OllOl\
OOO
;
}
}