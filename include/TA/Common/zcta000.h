//---------------------------------------------------------------------------------
// File Name: zcta000.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

#ifndef lOOllOlllO
#define lOOllOlllO
#ifndef TA_AABB_H
#include "AABB.h"
#endif //
#ifndef lOOllOllll
#include "../Common/zcta001.h"
#endif //
name\
spa\
ce
TA
{
cla\
ss
Serialis\
er
;
class
TACO\
M\
M\
O\
N\
_CLA\
SS
lOO\
OOll\
Ol\
l
{
publ\
ic\

:
struc\
t\

TA\
CO\
MMO\
N\
_CLASS
lO\
O\
ll\
O\
OOOO
{
void\

*
Ge\
tData
(
)
{
return
lOOlllOOOO
;
}
priva\
t\
e\

:
frien\
d
cl\
a\
ss\

lO\
O\
OOllOl\
l
;
voi\
d
*
lOOlll\
OOOO\

;
}
;
typede\
f
void
(
TAC_CALL
*
lOOOO\
lllOO\

)
(
lOOllO\
OOOO\

*
lOOlllOOOl
,
vo\
id
*
lOOlll\
OOO\
O\

)
;
lOOOOllOl\
l
(
)
;
~
lOOO\
O\
l\
lOll
(
)
;
void
In\
iti\
ali\
se\

(
int
lOO\
Ol\
OlOOl\

)
;
voi\
d
Fina\
l\
ise\

(
)
;
void\

Ser\
ialise
(
Seriali\
s\
e\
r
&
lO\
OOlOllOl
,
vo\
i\
d
*
lOO\
O\
lO\
ll\
lO
)
;
void
lO\
O\
Oll\
OO\
ll
(
Se\
riali\
ser
&
lOOOlOllO\
l
,
void
*
lOO\
O\
lOl\
l\
lO\

)
;
vo\
id\

lOOOllO\
llO
(
void
*
lOOOllO\
lll\

,
const
AAB\
B
&
lOOOOOOl\
l\
l
)
;
vo\
id
lO\
O\
O\
llOlO\
l
(
)
;
voi\
d
lO\
OlO\
lO\
OlO\

(
const
AAB\
B\

&
lOO\
OOO\
Ol\
ll\

,
lOOO\
O\
lllOO\

lOOlOlOOl\
l\

,
void
*
lO\
O\
OOl\
ll\
lO
)
co\
nst
;
void\

lOOlOlO\
OlO
(
const\

Ve\
c\
3
&
lOOlOl\
Olll\

,
cons\
t
Vec\
3
&
lOOOlOOOOO
,
floa\
t\

lOOlO\
l\
l\
OOO
,
lOOO\
Oll\
lOO\

lOOlO\
lOOll
,
voi\
d
*
lOOOOlll\
lO
)
const
;
cons\
t
AAB\
B
&
GetAABB\

(
)
const
;
pri\
vat\
e
:
int
lOOOl\
OlOlO
;
int\

lO\
OOlOlOOO
;
lOO\
O\
Ol\
Ol\
lO
<
AABB
,
fals\
e
>
lOOO\
lOlOl\
l
;
lOOOOlOl\
lO
<
void
*
,
fal\
se\

>
lOOOlOl\
lOO
;
void\

lOO\
lOOOOl\
l
(
in\
t
lOO\
l\
OO\
lOOO
,
int\

lO\
O\
lOO\
lOOl
)
;
vo\
id
lOOlOOOl\
l\
l
(
int\

lOOl\
O\
OlOO\
O
,
in\
t
lOOlOOlOOl
,
int\

lOOOlO\
Olll\

,
co\
nst\

AA\
BB
&
lOO\
lO\
llO\
ll
)
con\
st
;
void
lO\
O\
lO\
l\
Ol\
lO\

(
int\

lO\
O\
lOO\
lOOO
,
int
lOOlO\
OlOO\
l\

,
int
lOOOl\
O\
O\
l\
l\
l\

)
con\
st\

;
void
lOOlOlOllO
(
int
lO\
O\
lOOl\
OOO\

,
int
lOOlOOlO\
O\
l\

,
in\
t\

lO\
OOlOOlll
,
float
lOOll\
OOOO\
l
,
fl\
oat
lOOl\
lO\
OOlO\

)
const
;
bool\

lOOllO\
O\
lOl\

(
float
&
lOOl\
l\
OOOOl
,
fl\
o\
a\
t
&
lOO\
l\
lOOO\
l\
O
,
co\
ns\
t
AA\
B\
B
&
lOOOOOOlll
)
const\

;
}
;
}
#include "../Common/zcta001.inl"
#endif //
