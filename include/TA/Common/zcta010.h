//---------------------------------------------------------------------------------
// File Name: zcta010.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

#ifndef lOOlOOlllOl
#define lOOlOOlllOl
#ifndef lOOlOlOlllO
#include "../Common/zcta014.h"
#endif //
#ifndef llOllllOll
#include "../Common/zcta015.h"
#endif //
#ifndef lOOOOOOllOl
#include "../Common/zcta008.h"
#endif //
#ifndef lOOlOOllllO
#endif //
#ifndef lOOlOlOllll
#endif //
#ifndef lOOlOllOOOO
#include "../Common/zcta017.h"
#endif //
name\
spa\
ce
TA
{
#ifdef lOOlOOllllO
#define lOOlOllOOOl(lOOlOllOOlO)				\
extern lOOlOlOOOOO::lOOlOlOOOOl lOOlOllOOll
#define lOOlOllOlOO(lOOlOllOOlO)				\
lOOlOlOOOOO::lOOlOlOOOOl lOOlOllOlOl(lOOlOllOllO)
#define lOOlOllOlll(lOOlOllOOlO)				\
lOOlOllOlOl.lOOlOlllOOO()
#define lOOlOlllOOl(lOOlOllOOlO)					\
lOOlOllOlOl.lOOlOlllOlO()
#define lOOlOlllOll(lOOlOllOOlO)					\
lOOlOlOOOOO::lOOlOllllOO lOOlOllllOl(lOOlOllOlOl)
#else 
#define lOOlOllOOOl(lOOlOllOOlO)
#define lOOlOllOlOO(lOOlOllOOlO)
#define lOOlOllOlll(lOOlOllOOlO)
#define lOOlOlllOOl(lOOlOllOOlO)
#define lOOlOlllOll(lOOlOllOOlO)
#endif //
#ifdef lOOlOlllllO
cla\
ss
TACO\
MM\
ON_CLASS\

lOOlOlOO\
OOO
{
lO\
O\
lOlll\
lll
(
lOOlOlOOOO\
O
)
pu\
b\
lic
:
cla\
ss
lO\
OlOlO\
OOOl
{
pub\
lic
:
lOOlOlOOOO\
l\

(
co\
ns\
t
char
*
lll\
OlOO\
OOl\

)
;
~
lOOlOlOOOOl\

(
)
;
voi\
d\

Cle\
ar
(
)
;
void
Reset\

(
)
;
con\
s\
t
llOl\
l\
lllOO
&
lO\
OllO\
OO\
O\
OO
(
)
co\
ns\
t
{
retu\
r\
n
lOOOOOOOOOl
;
}
s6\
4
GetTime
(
)
cons\
t
{
ret\
u\
r\
n\

lO\
OlOlOOOll
;
}
s6\
4\

lOOllOO\
OO\
Ol
(
)
con\
st
{
ret\
urn
lO\
O\
l\
Ol\
Ol\
OO\
O
;
}
vo\
id
lO\
Ol\
Olll\
OOO
(
)
;
void
lO\
OlOl\
llOlO\

(
)
;
private
:
s64
lOOlOlOO\
l\
ll
;
s64
lOO\
lO\
lOO\
Oll
;
in\
t
lOOl\
OlOO\
llO\

;
int\

lOOlOlOlOOO\

;
int\

lOOl\
OlOlOOl
;
llO\
l\
l\
l\
l\
lOO
lO\
OOO\
OOOOOl
;
}
;
cl\
as\
s\

lOOlO\
llllOO
{
pu\
b\
l\
ic
:
lO\
O\
lO\
ll\
ll\
O\
O
(
lOOlOlOO\
OO\
l
&
lOO\
lOlOlOl\
l\

)
;
~
lOOlO\
l\
l\
llOO\

(
)
;
privat\
e
:
lO\
O\
l\
OlOOOOl
&
lOO\
l\
lO\
OOOlO\

;
}
;
type\
d\
ef
Li\
st\

<
lOOl\
O\
lOO\
O\
O\
l\

*
>
::
lOOOOOlOOOl\

lOOOO\
OlOO\
Ol
;
vo\
id\

Re\
s\
et
(
)
;
void
lOOlOl\
OOOlO
(
lOOlOl\
OO\
O\
O\
l
*
lOOl\
OlO\
llOO\

)
;
void
lOOlOlOOlOl
(
lOOlOl\
O\
OO\
Ol
*
lOO\
l\
O\
lOllO\
O
)
;
lOOO\
OOlOOO\
l\

lOO\
OOOl\
Ol\
lO
(
)
con\
st\

{
re\
t\
urn
lOOlOlOlOlO
.
lOOOOO\
lOl\
lO\

(
)
;
}
pri\
v\
ate\

:
typed\
ef\

Li\
st\

<
lOOlOlOO\
OOl
*
>
::
Iterator
Iterat\
or
;
Lis\
t\

<
lOO\
lO\
lOO\
O\
Ol\

*
>
lOO\
lOlOlO\
lO
;
lOO\
lOlOOOO\
O\

(
)
;
~
lOOlOl\
OOO\
OO
(
)
;
void
Initi\
ali\
se
(
)
;
void
Finalise
(
)
;
}
;
#endif //
}
#ifdef lOOlOOllllO
#include "../Common/zcta007.inl"
#endif //
#endif //
