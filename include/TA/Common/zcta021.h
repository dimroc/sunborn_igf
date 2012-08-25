//---------------------------------------------------------------------------------
// File Name: zcta021.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

#ifndef lOOOOlOlOO
#define lOOOOlOlOO
#ifndef TA_DEBUG_H
#include "Debug.h"
#endif //
#ifndef TA_COMMON_H
#include "Common.h"
#endif //
na\
me\
space
TA\

{
str\
uct
TA\
C\
OMMON_CL\
ASS\

Vers\
i\
on
{
Version
(
)
{
}
Versi\
on
(
u16\

lOlOOll\
llll\

,
u1\
6
lOlOl\
OOOOOO\

)
{
th\
is\

->
lOl\
OOllllll\

=
lOlO\
O\
lll\
l\
ll
;
th\
is
->
lOlO\
lOOO\
OOO
=
lO\
l\
Ol\
O\
OOOO\
O
;
}
u16\

lOl\
OOllllll\

;
u16
lOlO\
lO\
OOO\
OO
;
bool\

operat\
or
<=
(
co\
nst\

Versio\
n
&
lO\
O\
Oll\
ll\
llO
)
co\
nst
{
if
(
lOlO\
O\
ll\
ll\
l\
l
<=
lOOO\
lll\
lllO
.
lOl\
O\
O\
llllll
)
{
TA\
_ASSERT
(
lOlOlOOOO\
OO
<
10
)
;
TA\
_AS\
SERT\

(
lO\
O\
Oll\
llll\
O
.
lOlOlOO\
OOO\
O
<
10
)
;
if
(
lOl\
O\
lOOOOO\
O\

<=
lOO\
Oll\
l\
lllO
.
lOlOlOOOO\
OO\

)
return
tr\
ue
;
}
retur\
n\

fal\
se
;
}
bo\
o\
l
op\
er\
a\
tor
>=
(
co\
n\
st
Version
&
lOO\
Ol\
llll\
lO\

)
const
{
if
(
lOlOOlllll\
l
>=
lOO\
OllllllO
.
lOlOOllllll
)
{
TA_ASS\
ER\
T
(
lO\
lOlOOOOOO\

<
10
)
;
TA_AS\
SE\
RT\

(
lO\
O\
Ollllll\
O
.
lOlOlOO\
OO\
OO
<
10
)
;
if\

(
lOl\
Ol\
O\
OOO\
O\
O
>=
lO\
OOllll\
llO
.
lOl\
OlO\
O\
O\
OOO\

)
re\
t\
ur\
n\

true
;
}
return\

fa\
l\
se\

;
}
bool
opera\
to\
r
<
(
cons\
t\

Vers\
io\
n\

&
lOOOl\
lllll\
O
)
const
{
if
(
lOlO\
Oll\
ll\
ll
<
lOOOllll\
llO
.
lO\
l\
OOl\
llll\
l
)
{
TA_\
A\
SSERT
(
lOlOl\
OOO\
O\
OO
<
10
)
;
TA_ASSE\
R\
T\

(
lOO\
Oll\
llllO\

.
lOlOlOOOOOO\

<
10
)
;
if\

(
lOlOlOOO\
OOO
<
lOOOl\
lllllO
.
lOlOlOOOO\
OO
)
ret\
urn
tr\
ue
;
}
re\
t\
urn\

false
;
}
bo\
ol\

operat\
o\
r
>
(
co\
n\
st
Versi\
on
&
lOOOll\
llllO
)
const
{
if
(
lOlOOllllll
>
lOOO\
lll\
lllO
.
lO\
lOO\
lllll\
l
)
{
TA_AS\
SERT\

(
lO\
lO\
l\
OOOOOO
<
10
)
;
TA_A\
SSE\
RT
(
lOOOllllllO
.
lOlOlOOOO\
OO\

<
10
)
;
if
(
lO\
lO\
l\
OO\
OOO\
O
>
lOOOllllllO
.
lOlOl\
O\
O\
OOOO\

)
retur\
n
true
;
}
ret\
urn
fa\
lse\

;
}
}
;
}
;
#endif //
