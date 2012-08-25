//---------------------------------------------------------------------------------
// File Name: zcta005.cpp
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
#include "Debug.h"
#ifdef _DEBUG
#ifndef llOllllOll
#include "../Common/zcta015.h"
#endif //
#ifndef lOlOlOOllO
#include <io.h>
#include <sys/stat.h>
#endif //
#ifdef TA_MSVC
#include <windows.h>
#include <crtdbg.h>
#endif
#include <stdio.h>
#include <stdlib.h>
namespace
TA
{
na\
mes\
pace
De\
b\
u\
g\

{
static
llOlll\
ll\
OO
llOll\
l\
ll\
Ol\

;
#ifdef _DEBUG
st\
a\
tic
bool
ll\
O\
ll\
ll\
ll\
O
=
true
;
sta\
ti\
c\

boo\
l
ll\
Oll\
llll\
l\

=
true
;
void
TAC\
_CALL
lll\
OO\
O\
OOO\
O
(
co\
nst
ch\
ar
*
ll\
lOOO\
O\
OO\
l
,
in\
t
lllO\
OO\
O\
OlO\

,
co\
n\
st
char
*
lllOO\
OOOl\
l
,
co\
n\
st
ch\
ar
*
lll\
OO\
OOl\
OO\

)
{
llOlll\
llO\
O
lllO\
OO\
Ol\
Ol
;
if
(
lll\
OOO\
O\
lOO
)
{
lllOOOOlOl
+=
"WARNING MSG: \"";
lllOOOOlOl\

+=
lllOOO\
O\
lO\
O\

;
lllOOOOlOl\

+=
"\" ";
}
else
{
ll\
lOOOOlOl
+=
"INTERNAL ERROR: ";
}
lllOOOOl\
Ol
+=
"CONDITION FAILED: ";
lllO\
O\
OOlOl
+=
lllOOO\
OO\
ll
;
lllOOOO\
l\
Ol
+=
"\n";
#ifdef TA_MSVC
_CrtDb\
gReport\

(
_CR\
T_\
WARN
,
lllOOOOOOl
,
lllO\
OOO\
O\
lO
,
0
,
lll\
OOOOlO\
l
)
;
#endif
ll\
lO\
OO\
Oll\
O
(
lllOO\
OOlO\
l
)
;
}
st\
a\
tic
void\

TAC_CALL\

lllOOOOl\
l\
l
(
const
char\

*
lllOOOO\
OOl
,
int
lllOO\
OO\
Ol\
O
,
const
char
*
lllOOOOl\
OO
)
;
void
TAC_CALL
lllOOO\
lOOO
(
co\
n\
st
ch\
ar
*
lllOOO\
OOOl
,
int
ll\
l\
OOOOOlO
,
cons\
t
char
*
ll\
lO\
OOOO\
l\
l
,
cons\
t
ch\
ar
*
lllOOOOlOO
)
{
ll\
Ol\
l\
lllO\
O\

lllOO\
OOl\
Ol\

;
if\

(
lllOOOOlOO
)
{
lll\
OOOOl\
Ol
+=
"ERROR MSG: \"";
lllOOOO\
lO\
l
+=
lllO\
OO\
OlOO
;
ll\
l\
O\
OO\
OlOl
+=
"\" ";
}
el\
se
{
lllO\
OOOlOl
+=
"INTERNAL ERROR: ";
}
lllOOOOl\
Ol
+=
"CONDITION FAILED: ";
lllO\
OOO\
lO\
l\

+=
lll\
O\
OO\
OOl\
l
;
lll\
O\
OOOl\
Ol
+=
"\n";
lll\
O\
O\
OO\
ll\
l
(
lllOOOOO\
Ol
,
lllO\
OO\
OOlO\

,
lll\
OOO\
OlO\
l\

)
;
if
(
llOl\
l\
ll\
llO
)
{
#ifdef TA_MSVC_X86_INLINE_ASM
__\
asm{ int 3 }
#endif
#ifdef TA_GCC_X86_INLINE_ASM
asm
(
"int $3")
;
#endif
}
if
(
ll\
Olll\
l\
lll\

)
{
exi\
t
(
1
)
;
}
}
st\
a\
t\
ic
vo\
id
TAC_C\
AL\
L
lll\
OOOOlll
(
const
ch\
a\
r
*
ll\
lOOOOOOl
,
int\

lllO\
OO\
OOlO\

,
const
char
*
lllOOOOlO\
O
)
{
#ifdef TA_MSVC
_CrtD\
bgReport
(
_CRT_WARN\

,
lllOOO\
OOOl\

,
lllOOOOOlO
,
0
,
lll\
OO\
OOlOO
)
;
#endif //
lllOOOOl\
lO\

(
lllO\
O\
OOlOO
)
;
}
#endif //
void
TAC_CA\
LL
Ope\
nO\
utput\
File
(
co\
n\
s\
t
Ch\
ar
*
lllOOOlO\
Ol\

)
{
ll\
Olll\
llOl
=
lllOOOlOOl
;
#ifndef lOlOlOOllO
_chmod
(
ll\
l\
OOOl\
OOl
,
_S\
_IR\
E\
AD
|
_S\
_I\
W\
R\
I\
T\
E
)
;
#endif //
FILE
*
lllOOOlO\
l\
O
=
fop\
en
(
ll\
Olll\
llOl
,
"w")
;
if
(
ll\
lOOOl\
O\
lO
)
fc\
lose\

(
lll\
OOOl\
OlO
)
;
else
ll\
Oll\
lllO\
l
.
Cl\
ear
(
)
;
}
void
TA\
C_CALL\

Clos\
eOutp\
u\
tF\
ile
(
)
{
llOlll\
l\
lOl\

.
Clear
(
)
;
}
void
TAC_C\
ALL
Out\
p\
u\
t\

(
const
Cha\
r
*
lllO\
OO\
lOl\
l
)
{
if
(
llOlllllOl
.
lOOOll\
lOOl
(
)
)
{
FILE
*
lll\
O\
OOlOl\
O
=
fopen\

(
llOl\
lll\
lOl
,
"a")
;
if
(
lllOOO\
lO\
lO
)
{
fp\
u\
ts
(
lllOOOlOl\
l\

,
lllOOOlOlO\

)
;
fclose\

(
ll\
l\
OO\
O\
lOlO\

)
;
}
}
}
void
TAC_CALL\

Se\
tDebu\
gAsserts\
Enab\
l\
e
(
bool
ll\
lO\
O\
OllOO
)
{
ll\
Ol\
lllllO
=
lllOOOllOO\

;
}
voi\
d
TAC\
_CA\
LL
SetExi\
tW\
it\
Failure\
On\
As\
s\
e\
rt\

(
bool
ll\
lOO\
OllOO
)
{
ll\
O\
llllll\
l
=
lllO\
OOllOO\

;
}
}
}
#endif //
