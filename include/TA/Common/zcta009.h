//---------------------------------------------------------------------------------
// File Name: zcta009.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

#ifndef lOlOlOlOlO
#define lOlOlOlOlO
#ifndef TA_DEBUG_H
#include "Debug.h"
#endif //
#ifndef TA_COMMON_H
#include "Common.h"
#endif //
#ifndef TA_TYPES_H
#include "Types.h"
#endif //
na\
mespace
TA
{
tem\
plate\

<
cl\
ass\

Ty\
pe
,
bo\
o\
l
lOO\
Oll\
l\
llOl
=
false
>
clas\
s\

TA\
COMM\
ON_CLASS
lOl\
OlllOO\
O
{
pri\
v\
ate
:
struct\

lOOllOOOOO\

;
pu\
blic\

:
cl\
a\
s\
s\

lOlO\
lll\
OOl
{
pub\
lic
:
cl\
as\
s
Iterat\
or
{
pub\
lic\

:
Ite\
rator
(
)
{
lOOOOO\
Ol\
l\
l\
O
=
0
;
}
~
Iter\
at\
or
(
)
{
}
Itera\
tor
(
Type\

*
lOOlllOOOl\

)
{
In\
it\
i\
a\
l\
ise\

(
(
lOOl\
lOO\
O\
OO
*
)
lOOlllOOOl
)
;
}
Iter\
at\
or\

(
co\
ns\
t\

It\
erat\
or\

&
lOOOl\
lll\
ll\
O
)
{
lOOOOOOlllO
=
lOOOllllllO\

.
lO\
OOOOOlllO
;
}
void
Finalise
(
)
{
lOO\
O\
O\
OOlll\
O
=
0
;
}
voi\
d
op\
erat\
or
++
(
)
{
if
(
lOO\
OOOOl\
llO
)
lOOO\
OOOlll\
O
=
lOOOOOOl\
l\
lO\

->
lOOOOO\
Ollll\

;
}
voi\
d
operator
--
(
)
{
lO\
O\
llOOOOO
*
lO\
O\
Ol\
llllll\

=
(
lOOllO\
OOOO
*
)
(
(
u3\
2
)
(
u8\

*
)
lOOOO\
OOll\
lO
->
lOOlOOOOO\
O\
O
-
(
(
u32
)
(
u8
*
)
&
lO\
OOO\
OOl\
l\
lO
->
lOOOO\
OOllll
-
(
u32
)
(
u8\

*
)
lO\
OOOO\
Ol\
ll\
O
)
)
;
TA_\
ASSERT\

(
lOO\
Oll\
lllll
->
lOO\
OOO\
O\
ll\
ll
==
lOO\
OOOOlllO
)
;
lO\
OO\
O\
OOlllO\

=
lOOOl\
l\
lllll
;
}
Ite\
ra\
to\
r
&
ope\
r\
at\
or\

=
(
const
Iter\
a\
t\
o\
r
&
lOOO\
llllll\
O
)
{
lO\
O\
OOOO\
lllO
=
lOOOlllll\
lO
.
lOO\
OOO\
O\
ll\
lO
;
re\
turn
*
this
;
}
bo\
ol
operator\

==
(
con\
st\

Itera\
t\
or\

&
lOOO\
llllll\
O
)
con\
s\
t\

{
return\

lO\
O\
OOOO\
lllO
==
lOOOl\
ll\
l\
llO
.
lOOO\
OOO\
ll\
lO
;
}
Typ\
e
&
ope\
rat\
or
*
(
)
{
return
lO\
O\
OO\
OOl\
ll\
O
->
lOOO\
O\
OlOO\
O\
O
;
}
boo\
l
AtE\
nd
(
)
{
retu\
rn
lOO\
O\
OO\
Ol\
ll\
O\

==
0
;
}
private
:
fr\
ien\
d\

cla\
ss\

lOl\
OlllO\
Ol
;
Ite\
rator
(
lO\
OllOOOOO
*
lOOlll\
OOOl
)
{
Initi\
alise
(
lOOlllO\
OOl
)
;
}
vo\
id
Init\
ia\
lise\

(
lOOllOO\
OOO
*
lO\
Ol\
ll\
O\
OO\
l
)
{
lOOOO\
OOlllO
=
lO\
Oll\
lO\
O\
Ol
;
}
void\

lOOl\
OO\
OOOOl
(
Type
*
lOOll\
lOO\
O\
l
)
{
lOOO\
OOO\
llO\

(
lOOOOO\
Olll\
O
,
"Pool::Iterator::AddAfter. The iterator is off the end of the array")
;
lOOllO\
OOOO
*
lOO\
lOOOOOl\
O
=
(
lO\
OllOO\
O\
O\
O
*
)
(
lOO\
lll\
OO\
Ol
)
;
TA_\
ASS\
ERT
(
lOO\
lOOOO\
OlO
)
;
TA\
_ASS\
ERT\

(
lOO\
lOOO\
OOlO
->
lO\
O\
OOOOllll\

==
0
)
;
TA_\
ASSE\
RT
(
lOOlOOOOOlO
->
lOO\
lO\
O\
OOO\
OO\

==
0
)
;
lOOlOOOOOl\
O
->
lOO\
O\
OOOllll\

=
lO\
OO\
O\
OO\
lll\
O
->
lOO\
OOO\
O\
llll
;
if
(
lO\
Ol\
OOOOOlO
->
lOOOOO\
O\
l\
l\
ll
)
lOOlOOOO\
OlO\

->
lO\
OOOOOll\
ll\

->
lO\
Ol\
OOOOOO\
O
=
&
lO\
O\
lO\
OOOOl\
O
->
lOOOOOO\
l\
l\
ll
;
lO\
OOOOOl\
l\
l\
O
->
lOOOOOOl\
lll\

=
lOO\
lOOOOOlO
;
lOOlOOO\
OOlO
->
lO\
OlOOOO\
OOO
=
&
lOOOOOOl\
llO
->
lO\
OOO\
OO\
llll
;
}
voi\
d
lOOlOOOOOll\

(
Type
*
lOOl\
l\
lOOOl\

)
{
lO\
OOOOO\
llO
(
lOO\
OOO\
Oll\
l\
O
,
"Pool::Iterator::AddBefore. The iterator is off the end of the array")
;
lO\
OllOOOOO
*
lOO\
lOO\
OOO\
lO
=
(
lOOllO\
O\
OOO\

*
)
(
lOOll\
lOOOl
)
;
TA_ASSERT\

(
lO\
Ol\
OOOO\
OlO
)
;
TA_ASSE\
RT
(
lOO\
l\
OO\
OO\
Ol\
O
->
lO\
O\
OO\
OO\
llll
==
0
)
;
TA_ASSER\
T
(
lOO\
lOOOOOlO
->
lOOlOOO\
OO\
OO
==
0
)
;
lO\
OlO\
O\
O\
OOl\
O
->
lOOOOOO\
l\
lll\

=
lOO\
O\
O\
OOlllO
;
lOOlOOOOOlO
->
lOOlO\
OOO\
O\
O\
O
=
lOOOOOOlllO
->
lOOlOO\
OO\
O\
O\
O
;
*
lOOO\
OO\
Ol\
llO
->
lOO\
lOOO\
OOOO
=
lO\
O\
lOOOOOl\
O
;
lO\
OOO\
OOll\
lO\

->
lOOl\
OO\
OOO\
O\
O
=
&
lOO\
lOOOOOlO
->
lOOOOOO\
l\
l\
l\
l
;
}
lOO\
llO\
OOOO
*
lO\
OOOOOlllO
;
}
;
lOlO\
lllOOl\

(
)
{
lOOOOOl\
OlOl\

=
0
;
}
void
Fi\
nalise
(
)
{
#ifdef _DEBUG
wh\
i\
le
(
lO\
OO\
OO\
lOlOl\

)
{
lOOllOOOOO
*
lOOlll\
OOOl
=
lOOOOOlOlOl
;
lOO\
ll\
l\
OOOl
->
lOOOO\
OOllll
=
0
;
lO\
OlllO\
O\
Ol
->
lOO\
l\
O\
O\
O\
OOOO
=
0
;
lOOOOOlOlOl
=
lOOOOOlOlO\
l
->
lOOOOOOll\
ll
;
}
#else //
lOOOO\
OlO\
l\
Ol
=
0
;
#endif //
}
Iterator
lOlO\
OOOlOO
(
)
{
re\
turn\

Iterat\
o\
r\

(
lOO\
OOOl\
O\
l\
O\
l
)
;
}
vo\
id\

lOlOlOOOO\
l
(
Type\

*
lOOlllOOOl
)
{
lOOllOOOO\
O
*
lOO\
lOO\
OO\
OlO\

=
(
lOO\
llOOOOO
*
)
(
lO\
Oll\
lOOOl
)
;
TA_ASSER\
T
(
lOO\
l\
OO\
OOOlO
)
;
TA\
_A\
SSE\
RT
(
lOOlOOO\
OOl\
O
->
lO\
OOOO\
Olll\
l
==
0
)
;
TA_A\
SS\
ERT
(
lO\
OlO\
O\
OOOlO
->
lOOlO\
OOOO\
OO
==
0
)
;
lOO\
lO\
OOOOl\
O
->
lOOOO\
OO\
ll\
ll
=
lOOOOOlOl\
Ol
;
if
(
lO\
OlO\
O\
OOOlO
->
lOO\
O\
O\
OOlll\
l
)
lOO\
lO\
OOOOlO
->
lOOOO\
OO\
lll\
l\

->
lOOlOOOO\
O\
O\
O
=
&
lOO\
l\
OOOOOlO
->
lOOOOOOlll\
l
;
lOOOOO\
lO\
l\
Ol
=
lOO\
lOOO\
O\
OlO\

;
lOOlO\
OOOOl\
O\

->
lOO\
lOOOOOOO\

=
&
lOOOOOl\
Ol\
Ol
;
}
void
lOOlO\
OOOlOO
(
Type
*
lOOlllO\
OOl
)
{
lO\
Oll\
O\
O\
OOO
*
lOOlO\
OO\
O\
OlO\

=
(
lO\
Ol\
l\
OO\
OOO
*
)
(
lOOlllOO\
Ol
)
;
TA_ASSE\
RT
(
lOO\
l\
O\
OOOOlO\

)
;
TA\
_A\
S\
S\
ERT
(
lOOlO\
O\
OO\
Ol\
O
->
lOOOO\
OOl\
lll
==
0
)
;
TA\
_\
A\
SSERT
(
lOOlO\
O\
O\
O\
Ol\
O
->
lO\
OlOOOOO\
OO
==
0
)
;
lO\
Ol\
lO\
OOOO
*
lOO\
l\
OOOO\
lOl
=
lOO\
OOOlOlOl
;
lOOllOO\
OO\
O
*
*
lOO\
lOOOOOOO
=
&
lO\
OOOOl\
OlOl
;
while\

(
lO\
O\
lOO\
OOlO\
l
)
{
lOOl\
O\
OO\
O\
OOO
=
&
lO\
Ol\
OOOOlOl
->
lOOOOOOllll
;
lOO\
lOOOO\
l\
Ol
=
lOOlOO\
OOl\
Ol
->
lOOO\
OOOllll\

;
}
TA_\
ASSE\
R\
T
(
(
*
lOOlOOOOO\
OO
)
==
0
)
;
(
*
lOOlO\
OO\
O\
OOO\

)
=
lO\
OlOOO\
OOlO
;
lOOlOOOOOlO\

->
lOOlOO\
O\
OO\
OO
=
lOOl\
OO\
OO\
OO\
O
;
lO\
O\
lOOOO\
OlO
->
lOOOO\
OOll\
ll
=
0
;
}
void\

lO\
OlO\
OO\
O\
OOl
(
Typ\
e
*
lOO\
lllOOO\
l
,
Iterator\

lO\
lOOl\
O\
OlO
)
{
lOlOOlO\
O\
lO
.
lOOlO\
OOOO\
Ol
(
lOO\
l\
llOOOl
)
;
}
void
lOO\
lOOOOOl\
l
(
Type\

*
lOOl\
llOOO\
l
,
Ite\
r\
ator
lO\
lOO\
l\
OO\
l\
O
)
{
lOlO\
OlO\
O\
lO
.
lOOlOO\
OOO\
ll\

(
lOOll\
lO\
O\
Ol
)
;
}
voi\
d\

lOOlllll\
l\
l
(
Type
*
lOOl\
llOOOl
)
{
lO\
Ol\
lOO\
OO\
O
*
lOOlOOOO\
OlO
=
(
lO\
Oll\
O\
O\
OO\
O
*
)
(
lOOll\
lOOO\
l
)
;
TA_AS\
S\
ERT
(
lOOlOOOO\
O\
l\
O
)
;
TA_ASSERT
(
lOOl\
OOOOO\
lO
->
lO\
OlOOOOOOO\

)
;
#ifdef _DEBUG
if\

(
lOO\
lOOO\
llO\

)
{
bool
lOO\
lOO\
OOllO\

=
fal\
se
;
lOOl\
lOOO\
OO
*
lOOlOOO\
Olll
=
lO\
OOOOl\
OlOl
;
while
(
lO\
OlOOO\
Olll
)
{
if\

(
lO\
OlOO\
OOlll
==
lOOlOO\
OOOlO\

)
{
lOOl\
OOOOllO
=
tru\
e\

;
break
;
}
lOOlOOO\
O\
l\
ll\

=
lOOl\
OOOO\
lll
->
lO\
O\
O\
OO\
O\
llll\

;
}
TA_ASS\
ERT
(
lOOlOOOOl\
l\
O\

)
;
}
#endif //
*
lOOlOOOOOlO
->
lOOlOOOO\
OOO
=
lOO\
l\
O\
O\
OO\
O\
lO
->
lO\
OOOO\
Ollll
;
if\

(
lOOl\
O\
OO\
O\
OlO
->
lOOO\
OOOllll
)
lOOlO\
OOOO\
lO
->
lO\
OOOOOl\
ll\
l
->
lOOlOOOOOOO
=
lO\
OlO\
OOO\
Ol\
O
->
lOOlOO\
OOOO\
O
;
#ifdef _DEBUG
lOOlOO\
OO\
O\
lO
->
lO\
OOOOO\
llll
=
0
;
lOO\
lO\
OO\
O\
OlO
->
lOO\
lOOO\
OOOO
=
0
;
#endif //
}
void
llO\
ll\
OlO\
l\
O\

(
lOlOlllOOl
&
lOOlOOOlOO\
O
)
{
lOOllO\
O\
OOO
*
lOOlOO\
OlO\
Ol
=
lOOl\
O\
OOlO\
O\
O
.
lO\
OOO\
OlOlO\
l
;
lOOlOOOlOO\
O
.
lOOO\
OOlOlOl\

=
lO\
OOOOlOlO\
l\

;
lOOO\
OOl\
OlOl
=
lOO\
lO\
OO\
l\
OOl\

;
if
(
lOOOO\
OlOl\
Ol
)
lOOOOOlO\
lOl
->
lO\
OlOO\
OO\
OO\
O\

=
&
lO\
OOOOlOlOl
;
if
(
lOOlOOO\
lOOO\

.
lOOOOOlOlOl
)
lOOlOOOlOOO
.
lO\
OOOOlOlOl
->
lOOlOOOO\
OO\
O
=
&
lO\
OlOO\
Ol\
O\
OO\

.
lOO\
OOOl\
OlO\
l\

;
}
voi\
d
lOO\
lOOOlOlO
(
)
{
#ifdef _DEBUG
lOO\
ll\
O\
OO\
O\
O
*
lOOl\
O\
OOOlll
=
lOOOOOl\
O\
lO\
l\

;
wh\
ile
(
lOOlOOOOl\
ll
)
{
if
(
lOOl\
OOOOlll\

->
lOO\
OOOOl\
lll
)
TA\
_ASSE\
RT\

(
lOOl\
OOOO\
lll
->
lO\
O\
O\
OOOll\
ll
->
lOOlOOO\
OOOO
==
&
lOOlO\
OO\
Ol\
ll\

->
lOOO\
OOOlll\
l
)
;
TA_\
ASS\
ERT
(
(
*
lO\
OlOOOOlll
->
lO\
O\
lOO\
OOOOO
)
==
lOOlOO\
OOl\
ll\

)
;
lOOlOO\
OOl\
ll\

=
lO\
OlOOOOlll
->
lO\
OOO\
OOllll\

;
}
#endif //
}
bool
lO\
O\
l\
OOO\
lO\
ll\

(
con\
st
Ty\
pe\

*
lOOlllO\
OOl
)
{
const
lOOllOOOO\
O
*
lOOlOO\
OOOl\
O
=
(
lOOll\
OOOOO
*
)
(
lOOl\
llO\
OO\
l\

)
;
lOO\
llOO\
OOO\

*
lOOlO\
OO\
Ol\
ll\

=
lOO\
O\
OOl\
O\
l\
Ol\

;
whil\
e
(
lO\
O\
l\
OOO\
O\
lll
)
{
if\

(
lOO\
lOOO\
Olll
==
lOOlOO\
OOO\
lO
)
{
return
tr\
ue
;
}
lOOlOOOOlll\

=
lOOlOOOOll\
l
->
lOOOO\
OO\
llll
;
}
retur\
n
fa\
lse
;
}
sta\
tic
void
TAC_\
C\
AL\
L
lO\
OlOO\
O\
llO\
O
(
Type\

*
lO\
O\
lll\
O\
OOl
)
{
lO\
OllOOOO\
O\

*
lOOl\
OOOO\
OlO\

=
(
lOOllOOO\
OO
*
)
(
lO\
O\
l\
llOOO\
l
)
;
TA_\
ASSER\
T\

(
lOO\
lO\
OOOOlO
)
;
TA\
_\
ASSERT\

(
lOO\
lOOOO\
OlO
->
lOOlOOO\
O\
O\
OO
)
;
*
lOOlOOO\
OOl\
O\

->
lO\
OlO\
OOOOOO
=
lOO\
lOOOOOlO
->
lOOOOO\
Ollll\

;
if\

(
lOOl\
OOO\
O\
O\
lO
->
lO\
OO\
O\
OOllll\

)
lOO\
lO\
OOOOlO
->
lOOOOOOllll
->
lOOlO\
O\
OOO\
OO
=
lOOlOOOOOlO
->
lOO\
l\
OO\
OOOOO
;
#ifdef _DEBUG
lO\
OlOOOO\
O\
lO\

->
lOO\
O\
OOOll\
l\
l
=
0
;
lOOl\
O\
O\
OOOlO
->
lOOlOO\
O\
O\
OOO\

=
0
;
#endif //
}
int
lOO\
Oll\
lOOl\

(
)
{
in\
t
lOOlllOllO
=
0
;
for
(
It\
erator
lOl\
OOl\
O\
OlO
=
lOlOOOO\
lOO
(
)
;
!
lOlOOlOO\
lO
.
AtE\
nd
(
)
;
++
lOlO\
O\
lO\
Ol\
O
)
lO\
OlllOllO\

++
;
ret\
ur\
n
lO\
Oll\
lOllO\

;
}
void
llOllO\
Oll\
O
(
lOlOlll\
OO\
l\

&
lOOlOOOlOOO
)
{
if
(
lOOOOOlOl\
O\
l
)
{
lOOll\
OO\
OO\
O\

*
lO\
O\
lOO\
O\
ll\
Ol\

=
lOOO\
OO\
lOlOl
;
while
(
lOO\
l\
O\
OOllOl\

->
lOOOOOOl\
l\
ll\

)
lO\
O\
l\
OOO\
ll\
Ol\

=
lOOlOOO\
ll\
Ol
->
lOOOO\
OO\
lll\
l\

;
lOOl\
O\
OOllOl\

->
lOOOOOOl\
lll
=
lOOl\
O\
OOlOO\
O\

.
lOOOO\
OlOlO\
l
;
lOO\
l\
OO\
OllOl
->
lOOOOOOl\
ll\
l\

->
lOOl\
OOO\
OOO\
O
=
&
lO\
OlOOO\
l\
l\
Ol\

->
lO\
OOOOOllll\

;
}
el\
s\
e\

{
lOO\
OO\
OlOlOl
=
lOOlOOOlOOO\

.
lOO\
OOOlOlOl
;
if
(
lO\
OOOOlO\
lOl
&&
lOO\
OO\
OlOlOl\

->
lOOO\
OO\
O\
l\
lll
)
lOOOOOlOl\
Ol
->
lOOOOO\
Ollll
->
lOOlOOOO\
OOO\

=
&
lOOOOOlOlO\
l\

;
}
lOOlO\
OOlOO\
O
.
lOOOOOl\
OlO\
l
=
0
;
}
lO\
OllOOOO\
O
*
lOOlOO\
Olll\
O
(
)
{
ret\
u\
rn
lOOOO\
O\
l\
OlOl
;
}
bool\

lOOOO\
OlOOlO
(
)
{
retu\
rn
lOOOOOlO\
lOl\

==
0
;
}
priv\
ate\

:
lOO\
llO\
O\
OOO
*
lO\
OOOOlOlOl\

;
}
;
cla\
ss\

lOO\
lO\
OOll\
ll
{
pu\
bli\
c
:
cla\
ss
Ite\
ra\
t\
or\

{
pub\
lic
:
It\
erator
(
)
{
lOOOO\
OO\
lllO
=
0
;
}
~
Iterator\

(
)
{
}
It\
era\
t\
or
(
Type
*
lOO\
ll\
lOOOl
)
{
In\
it\
ialise
(
(
lOOll\
O\
OOOO\

*
)
lOO\
lll\
OOOl
)
;
}
Iter\
a\
to\
r
(
const
Ite\
r\
ator
&
lOOOllll\
ll\
O
)
{
lOOO\
OOO\
lllO
=
lOOOl\
llll\
lO
.
lOOOOOOll\
lO
;
}
void
Finalis\
e
(
)
{
lOOOO\
O\
OlllO
=
0
;
}
vo\
i\
d
opera\
t\
or\

++
(
)
{
if\

(
lOOOOOOlllO
)
lOO\
OOOOl\
ll\
O
=
lOOOOOOlllO
->
lOOO\
O\
OOll\
l\
l
;
}
void\

opera\
to\
r\

--
(
)
{
lO\
Ol\
lOO\
OOO\

*
lOO\
Olll\
llll\

=
(
lO\
Ol\
lOOO\
O\
O
*
)
(
(
u32
)
(
u8
*
)
lOOOOOO\
l\
ll\
O
->
lOOlOO\
OOOOO
-
(
(
u32
)
(
u8
*
)
&
lOOOO\
OO\
lllO
->
lOOOO\
OO\
llll
-
(
u32
)
(
u8\

*
)
lO\
OOO\
OOl\
llO
)
)
;
TA_ASSE\
R\
T
(
lO\
OOl\
l\
lllll
->
lOOOOOO\
lll\
l\

==
lOO\
OOOO\
ll\
lO
)
;
lOOOO\
OOlllO
=
lOOOllll\
ll\
l\

;
}
Iterator\

&
operator\

=
(
const\

Iterato\
r
&
lOO\
Olll\
lllO\

)
{
lOOO\
O\
O\
Ol\
llO
=
lO\
OOl\
ll\
ll\
lO\

.
lOOOOOOlllO
;
re\
turn
*
this
;
}
boo\
l
operator
==
(
const
It\
er\
ator
&
lO\
OOl\
lll\
llO
)
cons\
t
{
retu\
rn
lOOO\
OO\
OlllO
==
lOOOl\
lll\
l\
lO
.
lOOOOOO\
ll\
lO
;
}
Typ\
e
&
operator
*
(
)
{
ret\
urn
lO\
OOOOOl\
llO\

->
lOOOOOl\
OOO\
O
;
}
boo\
l
AtEnd
(
)
{
return
lOOOO\
OOl\
l\
lO
==
0
;
}
pr\
iv\
at\
e
:
frien\
d
class
lO\
OlOO\
Ol\
l\
l\
l
;
It\
e\
r\
ator
(
lOOllO\
OO\
OO\

*
lOO\
lllOOOl
)
{
In\
itialise
(
lOOll\
lOOOl
)
;
}
voi\
d
In\
itia\
lis\
e\

(
lOOllO\
OOOO
*
lOOlll\
OOOl
)
{
lO\
OOO\
OOl\
l\
lO
=
lOO\
lllOOOl
;
}
void\

lOOlOOOO\
OOl
(
Typ\
e
*
lOOlll\
OOOl
)
{
lOOO\
O\
OO\
llO\

(
lO\
OOOO\
Oll\
lO
,
"Pool::Iterator::AddAfter. The iterator is off the end of the array")
;
lOOllOOO\
O\
O
*
lOOlOO\
OOOlO
=
(
lOO\
ll\
OOOOO\

*
)
(
lO\
O\
l\
l\
l\
OOOl\

)
;
TA\
_A\
SSER\
T
(
lO\
OlOO\
OO\
O\
lO
)
;
TA_\
A\
S\
S\
E\
RT
(
lOOl\
O\
OOO\
OlO
->
lOOOOOO\
l\
ll\
l\

==
0
)
;
TA_ASSE\
R\
T
(
lOOl\
OOOOO\
l\
O
->
lOOlOO\
O\
OOOO
==
0
)
;
lO\
OlO\
O\
OOO\
lO
->
lO\
OOOO\
Oll\
ll
=
lOOOO\
OOll\
lO\

->
lOO\
O\
OO\
O\
l\
l\
ll
;
if
(
lO\
O\
lOOOOOlO
->
lO\
OOOOOllll
)
lOO\
lOOOOOlO
->
lO\
O\
OOOOllll
->
lOO\
l\
OOOOO\
O\
O
=
&
lO\
OlOOO\
OOlO\

->
lOOOOO\
Ollll\

;
lOO\
OOOOlllO
->
lO\
OOOO\
Ollll\

=
lO\
OlOO\
OO\
Ol\
O
;
lO\
OlOO\
OO\
OlO
->
lOOlOOO\
OOO\
O
=
&
lOOOOOOl\
l\
lO
->
lOOOOOOll\
ll\

;
}
vo\
id
lOO\
l\
O\
OO\
OOll
(
Ty\
p\
e
*
lO\
OlllOO\
Ol
)
{
lO\
OOOOOllO
(
lOO\
O\
O\
O\
OlllO
,
"Pool::Iterator::AddBefore. The iterator is off the end of the array")
;
lOOllOOO\
OO
*
lOOlOO\
OOOlO
=
(
lO\
OllOOOOO
*
)
(
lOOll\
l\
O\
OO\
l
)
;
TA_AS\
S\
ERT\

(
lO\
O\
lOOOOO\
lO
)
;
TA\
_ASSE\
R\
T
(
lOOlOOO\
OOl\
O\

->
lOOOO\
OOl\
lll\

==
0
)
;
TA_A\
SS\
ERT
(
lOO\
lOOO\
O\
OlO
->
lO\
OlOOOOOO\
O\

==
0
)
;
lOO\
lOOOOOlO
->
lO\
OO\
OO\
O\
ll\
ll
=
lO\
O\
OOO\
Ol\
llO
;
lO\
OlOOOOOl\
O\

->
lOOlO\
OOOOO\
O
=
lOO\
OOOO\
ll\
l\
O
->
lOOlOOOOO\
O\
O\

;
*
lOOOOO\
OlllO
->
lOOl\
OOOOOOO
=
lO\
OlOO\
OOO\
lO
;
lOOO\
OO\
O\
lllO\

->
lOOlOOO\
OOOO
=
&
lOOlOOOOOl\
O
->
lOO\
O\
O\
OOl\
lll
;
}
lOOllO\
OO\
OO\

*
lOO\
OO\
OOlllO
;
}
;
lOO\
lOOO\
lll\
l\

(
)
{
lOOOO\
O\
l\
OlOl\

=
0
;
lOOOOOlO\
ll\
l
=
0
;
}
void
Finalise
(
)
{
#ifdef _DEBUG
while
(
lOOO\
OOl\
OlOl
)
{
lOOllOOOOO
*
lOO\
lllOOOl\

=
lOOOO\
Ol\
O\
l\
Ol
;
lOO\
lllOO\
Ol
->
lOO\
OOO\
O\
llll
=
0
;
lOOl\
llO\
OO\
l
->
lOOlOOOOOOO
=
0
;
lO\
OOOOlOlO\
l
=
lOOOOOlOlOl
->
lOO\
OO\
OOllll
;
}
#else //
lOOOOOlOlO\
l
=
0
;
#endif //
lOOOOOlOll\
l
=
0
;
}
Itera\
t\
or
lO\
lOOO\
O\
lOO
(
)
{
re\
t\
ur\
n
Itera\
tor\

(
lOO\
O\
OOl\
O\
l\
Ol
)
;
}
void\

lOl\
OlOOO\
Ol\

(
Ty\
pe
*
lOOlll\
O\
OOl
)
{
lOOll\
OOOOO
*
lOOl\
OOOOOlO
=
(
lOO\
llOOOOO
*
)
(
lOO\
lllOO\
Ol
)
;
TA_ASSERT\

(
lOO\
lOOO\
O\
Ol\
O
)
;
TA_ASSER\
T\

(
lOOl\
OO\
O\
OO\
lO
->
lO\
OOO\
O\
Ollll
==
0
)
;
TA_ASSERT
(
lOOlOOOOO\
l\
O
->
lO\
OlO\
OOOOOO
==
0
)
;
lOOlOOOOOlO
->
lOO\
OOOO\
llll\

=
lOOOOOl\
OlOl\

;
if
(
lOO\
lOOO\
O\
Ol\
O\

->
lO\
OO\
O\
O\
Ollll
)
lOOlOOO\
OOlO
->
lOOO\
OOOl\
ll\
l
->
lOOlOOOO\
O\
O\
O\

=
&
lOOlOOO\
OOl\
O\

->
lOOOOOOllll
;
lOO\
OO\
OlO\
lOl
=
lOOlOO\
OOOlO
;
lOOlOOOO\
Ol\
O\

->
lO\
OlOOOOOOO\

=
&
lO\
OO\
OO\
l\
OlO\
l\

;
if
(
lOOO\
O\
OlOl\
ll
==
0
)
lOOOOOl\
O\
l\
ll
=
lOOlO\
OOO\
OlO\

;
TA\
_\
AS\
SE\
RT
(
(
lO\
O\
OO\
OlO\
ll\
l\

&&
lOO\
OOO\
l\
OlOl
)
||
(
!
lOOOOOl\
Ol\
ll
&&
!
lOOOOOl\
Ol\
Ol
)
)
;
}
vo\
id\

lOO\
l\
OOO\
OlO\
O
(
Type
*
lOOlllO\
OOl
)
{
lOOllOOO\
OO
*
lOOl\
OOOOOlO\

=
(
lOO\
llOOOOO\

*
)
(
lO\
Olll\
OO\
Ol
)
;
TA\
_AS\
SE\
R\
T\

(
lOOlOO\
OOOl\
O
)
;
TA_ASSE\
R\
T
(
lO\
O\
l\
O\
OOO\
OlO
->
lOOOOOOllll
==
0
)
;
TA_ASSERT
(
lO\
OlO\
OOOOlO
->
lOO\
l\
O\
O\
OOOOO
==
0
)
;
if\

(
lO\
OO\
OOlOlll
)
{
lOOlOOOOOlO\

->
lOOOOOOlll\
l
=
lOOOOOlOlll
->
lO\
OO\
O\
OOl\
l\
l\
l\

;
if
(
lO\
OlOO\
OOOlO
->
lO\
OO\
OOOllll
)
lOOl\
O\
OOOOlO
->
lO\
O\
O\
OOOllll
->
lOOlOO\
OOO\
OO
=
&
lOOlOOOOO\
lO
->
lOOO\
O\
OOll\
ll
;
lOOOOOl\
Ol\
ll
->
lO\
O\
OOO\
Ollll\

=
lOOl\
OO\
OOO\
lO
;
lO\
OlOO\
OO\
O\
l\
O
->
lOOlOOOOOO\
O
=
&
lOOO\
OOlOl\
ll\

->
lOOO\
OO\
Olll\
l
;
lOO\
OO\
O\
lOl\
ll
=
lOOlO\
OOOO\
lO
;
}
else
{
TA_A\
SSERT\

(
lOOO\
OOlOlO\
l
==
0
)
;
lOOOO\
OlOlOl
=
lO\
O\
l\
OO\
O\
O\
OlO\

;
lO\
OlOO\
OOO\
lO
->
lOOOOO\
Ol\
l\
ll
=
0
;
lOOlOOOOOlO
->
lOOlO\
OOO\
O\
OO
=
&
lO\
O\
O\
OOlOlO\
l\

;
lO\
OO\
OOlOlll
=
lO\
OlO\
O\
OOOlO
;
}
TA_ASSER\
T
(
(
lOOOOOlOl\
ll\

&&
lOOOOO\
lOlO\
l
)
||
(
!
lOOOO\
OlOll\
l
&&
!
lO\
O\
OOOlOlOl
)
)
;
}
void
lOO\
l\
OOOOO\
Ol
(
Typ\
e
*
lO\
Oll\
lOO\
O\
l\

,
Iterat\
o\
r
lOl\
OOlOOl\
O
)
{
if
(
lOOO\
O\
O\
lOlll
==
lO\
lO\
O\
lOOlO
.
lOO\
OOOOlllO
)
lOO\
OOOlOll\
l
=
(
lOOllOO\
O\
OO
*
)
(
lOOlllO\
OOl
)
;
lO\
lOOlOOlO
.
lOOlOOOOOO\
l
(
lOO\
lll\
OOOl
)
;
TA_AS\
S\
ERT
(
(
lOOOOOlO\
ll\
l\

&&
lO\
O\
O\
O\
OlOlOl
)
||
(
!
lOOOOO\
lOl\
ll
&&
!
lOOOO\
O\
lOlOl
)
)
;
}
vo\
id
lOOlOOOOOl\
l
(
Ty\
p\
e\

*
lOO\
lllOOOl
,
Ite\
r\
a\
t\
or
lOlO\
O\
lOOl\
O\

)
{
lOlO\
O\
lOO\
lO
.
lOOlOO\
OO\
Oll\

(
lOO\
lllOOOl
)
;
TA_A\
S\
SERT\

(
(
lOOOOO\
lOlll\

&&
lOO\
OOO\
lO\
lO\
l
)
||
(
!
lO\
OOOOl\
Olll
&&
!
lOOOOOlOl\
Ol
)
)
;
}
vo\
id\

lOOlllllll\

(
Type
*
lOOlllOO\
Ol
)
{
TA_A\
SSERT
(
(
lOOOOO\
lOlll\

&&
lOO\
O\
OOlOlOl
)
||
(
!
lOOOOOl\
Olll
&&
!
lOO\
OOOlO\
lOl
)
)
;
lO\
OllO\
OOOO
*
lO\
O\
lOOOOOlO
=
(
lOOl\
lO\
O\
OOO\

*
)
(
lO\
Ol\
llO\
OOl\

)
;
TA_ASSERT
(
lOOlOOOOOlO
)
;
TA_ASSERT
(
lOOlOO\
OOO\
lO\

->
lOO\
l\
OOOO\
OO\
O\

)
;
#ifdef _DEBUG
if
(
lOO\
lOOOllO
)
{
bool
lOO\
lOO\
O\
Oll\
O
=
fa\
l\
se
;
lOO\
llOOOO\
O\

*
lOOlOOOOl\
ll
=
lOOOOOlOlOl\

;
while\

(
lOOlO\
O\
OOl\
ll
)
{
if
(
lOOlO\
OOOlll
==
lOO\
l\
OO\
O\
OO\
lO
)
{
lOOlOOOOllO
=
true
;
break
;
}
lOO\
lOO\
OOll\
l
=
lOOlOO\
OOlll
->
lOO\
OOO\
Ollll
;
}
TA_\
ASSE\
RT\

(
lO\
Ol\
OOOOllO
)
;
}
#endif //
if
(
lO\
OOOOlOlll
==
lOOlOOO\
OOlO
)
{
if
(
lOOO\
OOlOlOl
==
lOO\
l\
OO\
OOOlO
)
{
lOO\
OO\
OlOl\
l\
l\

=
0
;
}
else
{
lOO\
l\
l\
OO\
OOO
*
lOOOllllll\
l
=
(
lOOll\
OOO\
O\
O\

*
)
(
(
uSize
)
(
u8
*
)
lOO\
O\
OOlOlll\

->
lOOl\
OOOOOOO\

-
(
(
uSi\
ze
)
(
u8\

*
)
&
lOOOO\
O\
lOll\
l
->
lO\
OO\
OOO\
llll
-
(
uSize
)
(
u8\

*
)
lOO\
OOOlOl\
l\
l
)
)
;
TA_\
A\
SS\
E\
RT
(
lOOOll\
lllll
->
lOOO\
OO\
Ollll
==
lOOOOOlOlll
)
;
lOOOOO\
lOlll
=
lOOOl\
l\
l\
l\
l\
ll
;
}
}
*
lOO\
lOOOOOl\
O
->
lOOlOOOOOOO
=
lO\
Ol\
OOO\
OO\
l\
O
->
lOO\
OOO\
O\
llll
;
if
(
lO\
Ol\
OO\
OO\
OlO
->
lOO\
OO\
O\
O\
llll
)
lO\
OlOO\
OOOl\
O
->
lOOOOOOl\
lll\

->
lO\
O\
l\
OOOOOOO\

=
lOO\
lO\
OOO\
OlO
->
lOOl\
O\
OOOOOO\

;
#ifdef _DEBUG
lO\
O\
lOOOO\
O\
lO
->
lOO\
OOO\
Ol\
lll
=
0
;
lOOlOOO\
OOlO
->
lOOlOOOOOOO\

=
0
;
#endif //
TA_A\
S\
SERT
(
(
lOO\
O\
OO\
lO\
lll
&&
lO\
OOO\
OlOlO\
l
)
||
(
!
lO\
OOOOlOl\
ll\

&&
!
lO\
OOOOlO\
lOl
)
)
;
}
void
llOllOlO\
lO
(
lO\
Ol\
OOO\
l\
lll\

&
lOOlOOOl\
OOO\

)
{
lOOl\
lOOOO\
O
*
lOOlOOO\
lOOl
=
lO\
OlOOOl\
OOO
.
lOOO\
OOlOlOl
;
lO\
OlOOO\
lOOO
.
lOOOOOlOlOl
=
lOOOOOl\
OlOl
;
lO\
OOOOlOlOl\

=
lOOlOOOlOO\
l
;
if
(
lOO\
OOOlOlOl
)
lOOO\
OO\
lO\
lOl
->
lO\
OlOOOOOOO
=
&
lOOOOO\
l\
OlOl
;
if
(
lOOl\
OOOl\
OO\
O
.
lOOOOOlOlOl
)
lOOl\
OOOlOOO
.
lOOO\
OOl\
O\
lOl\

->
lOO\
l\
OOOO\
OOO\

=
&
lO\
OlOOOl\
OO\
O
.
lOO\
OOOlOlOl\

;
lOOlOOOlOOl
=
lOOlO\
O\
OlOO\
O
.
lOOOO\
OlOlll\

;
lOOl\
OOOl\
OO\
O
.
lOOOOO\
lOlll
=
lO\
OO\
OO\
lOll\
l
;
lOOOOOlOl\
ll
=
lOOlOOOlOOl\

;
TA_ASSERT\

(
(
lOOOOOlOlll
&&
lOO\
O\
OOlO\
l\
Ol
)
||
(
!
lO\
OO\
OOl\
O\
lll
&&
!
lOOOOO\
lO\
l\
Ol\

)
)
;
TA_ASS\
ERT\

(
(
lOOlOOOlO\
O\
O\

.
lO\
O\
OOOlOlll
&&
lO\
OlO\
OOl\
O\
OO
.
lOOO\
OO\
l\
OlO\
l
)
||
(
!
lOO\
lOOOlOO\
O
.
lOOOO\
OlO\
lll
&&
!
lOO\
lOOO\
lOOO
.
lO\
OOOOlOl\
Ol\

)
)
;
}
vo\
id
lOOlOOOlO\
lO
(
)
{
#ifdef _DEBUG
lOOl\
lOOOOO
*
lOOlOOOO\
lll
=
lO\
OOOOlOlOl\

;
whil\
e\

(
lOOlOO\
OOlll
)
{
if
(
lO\
OlO\
OOOl\
ll\

->
lO\
OOOO\
Ollll
)
TA_\
ASSERT
(
lOOlOOOOl\
l\
l
->
lOOOOOOllll
->
lOO\
lOOOOOO\
O\

==
&
lOO\
l\
O\
OOOll\
l\

->
lOOOO\
OOll\
ll
)
;
TA\
_\
AS\
SERT
(
(
*
lOOlO\
OOOl\
ll
->
lOO\
l\
OO\
OOO\
OO
)
==
lO\
Ol\
O\
O\
OO\
l\
ll
)
;
lOOlOO\
OOl\
ll
=
lO\
Ol\
OOOO\
l\
ll
->
lOOOOOOl\
l\
ll\

;
}
#endif //
}
boo\
l
lOOlOOOl\
O\
l\
l
(
Typ\
e
*
lOO\
lll\
OOOl\

)
{
lOOll\
OOO\
OO\

*
lOOl\
OOOOO\
lO
=
(
lO\
OllO\
OOOO
*
)
(
lO\
Oll\
lOO\
Ol
)
;
lOO\
l\
l\
OOOOO
*
lOOl\
OOOOlll
=
lOOOOOlOlOl\

;
while
(
lOOl\
O\
O\
OOlll
)
{
if
(
lOOl\
O\
OOOlll
==
lO\
OlOOOO\
O\
lO
)
{
return
tru\
e
;
}
lOOl\
OOO\
Olll
=
lOOlO\
OOOlll
->
lO\
OOOO\
Oll\
ll
;
}
re\
t\
urn\

fa\
l\
se
;
}
in\
t
lO\
OOlllOO\
l
(
)
{
int
lOOlllOl\
l\
O
=
0
;
fo\
r
(
It\
erator
lO\
l\
OOlOOlO
=
lOl\
OOOO\
lO\
O\

(
)
;
!
lOlOO\
l\
OOlO
.
AtE\
nd\

(
)
;
++
lO\
lOO\
lOOlO
)
lO\
O\
l\
llOllO
++
;
return
lOOl\
l\
lOllO
;
}
lOOllOOO\
OO\

*
lOOlOOOl\
llO\

(
)
{
retu\
rn
lOO\
O\
OOl\
OlOl
;
}
lOOl\
lOOOOO
*
lOOl\
OOlOO\
OO\

(
)
{
return
lOOOOOl\
Ol\
ll
;
}
bool
lOOOO\
Ol\
OO\
lO
(
)
{
return
lOOOOOlOl\
Ol
==
0
;
}
privat\
e\

:
lOOllOOOOO
*
lO\
OO\
OOlOl\
Ol
;
lOO\
llOO\
OOO
*
lO\
OO\
O\
OlOl\
ll
;
}
;
lOlOlllOOO
(
)
;
~
lO\
lO\
lllO\
OO
(
)
;
void\

Initi\
alise
(
int\

lOO\
lllOllO\

)
;
void
Finalise
(
)
;
bo\
o\
l
lOOlOOlOOO\
l
(
)
;
Type
*
Alloc
(
)
;
vo\
id\

Fr\
ee
(
Type\

*
lOOl\
llOOO\
l\

)
;
vo\
id
llOlOlO\
OOl
(
lOlO\
l\
ll\
OOl
&
lOO\
lOOOlO\
OO\

)
;
in\
t
lO\
OlOOlOO\
lO
(
)
;
bool
lOO\
lOOlOOll\

(
)
;
in\
t
lOOOl\
ll\
OO\
l
(
)
;
pr\
iv\
at\
e
:
frie\
n\
d
cl\
as\
s
lOl\
OlllOOl
;
struct
lO\
Oll\
OOOOO
{
Type
lOO\
OOOlO\
O\
OO
;
lOOllOOOOO
*
lO\
OO\
OOOl\
lll
;
lOOllO\
O\
OO\
O\

*
*
lOOlOO\
OOO\
OO
;
}
;
struct
lO\
OlOOlOlOO\

{
lO\
Oll\
OOO\
OO
*
lO\
Ol\
OOlOlOl
;
lO\
Ol\
OOlO\
lOO
*
lOOOOOOllll
;
}
;
in\
t
lO\
lO\
OOl\
OOl
;
lO\
OlOO\
lOlOO
lO\
OlO\
OlOl\
lO
;
lO\
l\
Ol\
llO\
O\
l
lOOl\
OOlOl\
l\
l
;
}
;
}
#include "../Common/zcta006.inl"
#endif //
