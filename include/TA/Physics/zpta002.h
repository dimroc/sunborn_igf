//---------------------------------------------------------------------------------
// File Name: zpta002.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

#ifndef llOllOOOOlO
#define llOllOOOOlO
#ifndef TA_MATHS_H
#include "../Common/Maths.h"
#endif //
#ifndef lOOllOllll
#include "../Common/zcta001.h"
#endif //
#ifndef TA_VECTOR_H
#include "../Common/Vector.h"
#endif //
#ifndef llOOlOlOllO
#include "../Physics/zpta015.h"
#endif //
na\
mes\
pa\
ce
TA\

{
str\
uct
AA\
BB
;
cl\
ass
lOl\
Ol\
l\
llOl\
O\

llOlOOllll\
l
:
publi\
c
llOOlOlOll\
l
{
public
:
llOlO\
Ol\
llll
(
)
;
~
llO\
lOO\
ll\
lll\

(
)
;
vo\
id
Ini\
tiali\
se
(
fl\
oat
ll\
Ol\
OlOlOOl\

,
int
llO\
lOlOlO\
l\
O
,
co\
ns\
t\

AAB\
B
&
lOOOO\
OOlll
,
int\

llOlOl\
Ol\
Oll
,
int
llO\
l\
OlOll\
O\
O\

)
;
void\

Fi\
nalis\
e
(
)
;
voi\
d
lOlOlOOOOl\

(
Spa\
ceDivis\
ionObjec\
t
&
lOllllllllO
)
;
void
lO\
O\
lll\
l\
l\
ll
(
SpaceDivi\
sion\
Obj\
e\
ct
&
lO\
l\
llll\
lllO
)
;
void
llOOO\
l\
O\
Ol\
O\
O
(
con\
st
AA\
B\
B
&
lOOOOOOlll
,
vo\
id\

(
TA\
C_\
CALL
*
llOOl\
Ol\
lO\
OO
)
(
Space\
D\
ivisionO\
bjec\
t\

&
lOllllllllO
,
vo\
id
*
lOO\
lll\
OOO\
O\

)
,
void
*
lOOOOllllO
)
;
void\

llOOOl\
OllOO
(
const\

Vec\
3\

&
lOOlOlOl\
ll
,
co\
nst
Vec3
&
lO\
OlOll\
O\
Ol
,
void
(
TAC\
_\
CALL
*
llO\
Ol\
Oll\
OOO\

)
(
Space\
Di\
v\
isionO\
bject
&
lOll\
lll\
lllO
,
void\

*
lOOlllOOOO\

)
,
void
*
lOOOOllllO\

)
;
in\
li\
n\
e\

int
ll\
OlO\
llOOlO
(
float
llOll\
OOOOll
)
{
retu\
rn
Fas\
t\
F\
l\
oatToIn\
t
(
(
llOl\
lOOOOll\

+
ll\
Ol\
OlOO\
lO\
O\

)
*
llOl\
Ol\
OOll\
O\

)
;
}
inline
int
llOlOllO\
Oll\

(
fl\
o\
at
ll\
OllO\
OOlO\
O
)
{
re\
tur\
n
Fast\
Fl\
oatToInt
(
(
llOllOOOlOO
+
llOlOl\
OO\
lOl
)
*
llO\
lOlOOl\
ll\

)
;
}
privat\
e
:
int
llOlOl\
OOO\
OO
;
int\

llO\
lOlO\
OO\
O\
l\

;
int
llOlO\
lO\
OOlO
;
int\

llOlO\
l\
O\
OOll
;
fl\
o\
at
ll\
OlOlO\
O\
l\
O\
O
;
fl\
oat\

llO\
lOlOOlOl\

;
float
ll\
O\
l\
OlOOll\
O
;
float
llOlOlOOl\
ll\

;
lO\
OOO\
lOll\
O
<
Sp\
aceDivision\
Objec\
t
*
,
fals\
e
>
llOlOlOll\
ll
;
SpaceDivisi\
onObject
*
ll\
OlOlOlOOO
;
Sp\
ace\
Di\
visionObject
*
*
llOlOl\
l\
Ol\
OO
(
int
ll\
OOO\
O\
OOOl
,
int
llO\
OO\
OOOlO\

)
{
retur\
n\

&
llOl\
Ol\
Ollll
[
llOOO\
OO\
OlO
*
ll\
O\
lOlOOOlO
+
llO\
OOOO\
OO\
l
]
;
}
}
;
}
;
#endif //
