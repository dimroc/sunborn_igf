//---------------------------------------------------------------------------------
// File Name: zpta008.inl
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

fl\
o\
at
TAC_\
CALL
lOl\
O\
O\
lOOO\
OO\
O
(
flo\
at
lOO\
lllOOl\
lOl\

,
Coll\
i\
si\
on
&
llOlll\
lOlOl\

)
{
Vec\
3
lOlO\
lOl\
OOll\
l\

;
lOOll\
OOOOlOO
(
&
lOlOlO\
lOOlll
,
&
llOl\
lllOl\
Ol
.
lOO\
OOll\
lll\

,
&
ll\
OllllOl\
Ol
.
Ge\
tO\
b\
j\
ectA
(
)
->
GetL\
in\
e\
ar\
Vel\
o\
c\
it\
y\

(
)
,
&
ll\
Oll\
llOl\
Ol
.
Ge\
t\
Ob\
jec\
tA\

(
)
->
Ge\
t\
AngularVelocity
(
)
,
&
llOll\
llO\
lO\
l
.
GetObjec\
tA
(
)
->
GetC\
en\
terOfM\
a\
ss
(
)
,
&
llO\
llllOlOl
.
Get\
O\
bject\
B
(
)
->
GetLi\
nearV\
el\
ocit\
y
(
)
,
&
llOll\
l\
l\
OlOl
.
Ge\
tObjectB
(
)
->
Ge\
t\
Angu\
l\
a\
r\
Velocit\
y
(
)
,
&
llOl\
lllOl\
Ol
.
Get\
Objec\
t\
B\

(
)
->
GetCenterOfM\
ass
(
)
)
;
flo\
at
lO\
OlO\
l\
lOllOl\

=
lO\
lOl\
OlOOl\
ll\

.
Dot
(
ll\
O\
llll\
OlOl
.
lO\
OOlO\
OOOO\

)
;
if
(
ll\
Oll\
llOlOl\

.
lOlOllO\
OOl\

&
Coll\
is\
ion\

::
FL\
AG_\
USE\
_PUS\
H_OUT_\
M\
OD\
IFIER
)
lOOl\
OllO\
llO\
l
-=
llOl\
lll\
OlOl
.
lOllOOl\
OlOl
*
lOl\
O\
lllllOOl
;
el\
se\

lOOlOl\
lOllOl
-=
llOllll\
O\
l\
O\
l
.
lO\
l\
lOOlOl\
O\
l
;
lOO\
l\
OllO\
ll\
Ol\

+=
llOll\
ll\
O\
lO\
l
.
lOllO\
OlOOl\
l
;
TA_AS\
SE\
RT
(
Flo\
atIsOK
(
lOO\
lOl\
l\
Ol\
l\
Ol
)
)
;
ret\
u\
rn\

lOOl\
O\
llOl\
lOl
;
}
void\

TAC_CALL\

lOlOOOl\
llO\
ll
(
floa\
t
lOOlOl\
lO\
llOl\

,
Collis\
ion
&
llO\
ll\
llOlOl
)
{
DynamicObj\
ect
*
lO\
l\
lOOOlO\
l\
l\

=
llO\
ll\
ll\
OlO\
l\

.
Get\
O\
b\
je\
ctA
(
)
;
Dy\
n\
a\
mic\
Objec\
t
*
lO\
l\
lOOO\
llO\
O
=
ll\
OllllOlOl
.
GetOb\
jectB
(
)
;
TA_\
A\
SSERT\

(
lOllO\
OO\
lOll\

)
;
TA\
_ASSERT\

(
lOll\
OO\
OllO\
O
)
;
const
boo\
l
lO\
OlO\
l\
llOl\
ll
=
tru\
e
;
const
floa\
t
lOOlO\
lll\
OllO
=
1.0f
;
co\
ns\
t
fl\
oat
lO\
OllOOO\
l\
lOO\

=
1.0f
;
const
boo\
l
lOOlOll\
Ol\
lOO\

=
tr\
ue\

;
const
bo\
ol
lOOlOlllll\
l\
O
=
fal\
se
;
const
bo\
o\
l\

lOOlOllOl\
Ol\
l
=
fal\
se
;
co\
nst
boo\
l
lOOlOll\
Ol\
lll
=
false\

;
const\

int
lO\
l\
lOOOOll\
l\
O
=
Dyn\
am\
icObjec\
t
::
lO\
lOOlOOllOO
|
Dy\
namicO\
bjec\
t
::
lO\
O\
l\
l\
l\
OOOl\
ll
|
Dyn\
amicObje\
c\
t\

::
lOOl\
llOO\
lOOl
;
const
int
lO\
l\
l\
OOOOlll\
l\

=
Dy\
n\
a\
micObject
::
lOlOOl\
O\
OllOO
;
u32
lOOlOlllO\
lOO
=
(
(
lOllOOOllOO\

->
GetF\
l\
ags
(
)
&
lOllOO\
OOll\
lO\

)
^
lOl\
lOOOOllll
)
|
(
llOl\
ll\
lOlOl\

.
lOlOllOOO\
l
&
Col\
lision
::
FLAG_O\
B\
JECTB_SOLID\
_\
INT\
ERNA\
L
)
;
fl\
o\
at
lO\
llOOlO\
OlO
=
0.0f
;
Ve\
c4
&
lOllO\
O\
lOllO
=
llOl\
lll\
OlOl
.
lOll\
OOlOllO\

;
Vec4\

&
lOllOOl\
Olll
=
ll\
OllllOlOl
.
lOllOOlOlll
;
#define lOOlOlllOOOO
#define lOOllOOOOllO()
#define lOOllOOOOOOO
#include "../Physics/zpta005.inl"
#undef lOOllOOOOOOO
#undef lOOllOOOOllO
#undef lOOlOlllOOOO
}
void
TA\
C\
_\
CA\
LL
lOlOOOl\
lllOO\

(
fl\
oa\
t\

lOOlOll\
OllOl
,
float\

llOllOllll
,
float
lOlO\
OO\
l\
l\
llO\
l\

,
Col\
lision\

&
llOllllOlOl
)
{
Dynam\
icObject\

*
lOll\
O\
OOlO\
ll
=
llOllllOlOl
.
Ge\
t\
Objec\
tA
(
)
;
Dyn\
amicOb\
ject
*
lOllOOOllOO
=
llOllllOl\
Ol
.
Ge\
t\
O\
bj\
ec\
t\
B
(
)
;
TA_AS\
SERT
(
lO\
llOOOlOll
)
;
TA\
_\
A\
SSERT
(
lOllOOO\
ll\
OO\

)
;
cons\
t
bool\

lOO\
lOl\
ll\
Oll\
l
=
true
;
const
float
lO\
OlOlllO\
llO
=
ll\
OllOl\
lll\

;
const
float\

lOO\
llOO\
O\
l\
lOO
=
lOl\
OOOl\
ll\
lOl
;
con\
st
boo\
l
lO\
OlO\
l\
l\
OllO\
O
=
tr\
ue
;
const\

boo\
l\

lOOl\
OllllllO
=
fals\
e
;
const
bool
lOOl\
O\
llO\
lOl\
l\

=
fal\
s\
e
;
co\
n\
st
boo\
l
lOllO\
OOlOOOO
=
fals\
e
;
const\

in\
t
lOllO\
O\
OOlllO\

=
Dyn\
ami\
cObject\

::
lOlOOlOO\
l\
lOO
|
Dyn\
amicOb\
je\
ct
::
lOOlll\
OOOlll
|
Dyn\
ami\
cObjec\
t
::
lOOll\
lO\
OlOOl\

|
DynamicObje\
ct
::
lOlOOl\
O\
lO\
OOl
;
const
int
lOllOOOOl\
ll\
l
=
DynamicO\
bject
::
lOlO\
O\
lOOllOO\

;
const
u32
lOOlO\
lllOlOO\

=
(
(
lOllO\
OO\
llO\
O
->
GetF\
lags\

(
)
&
lOllOOOO\
lllO
)
^
lO\
llOOOOll\
l\
l
)
|
(
ll\
Oll\
llOlOl\

.
lOl\
O\
llOOOl
&
Coll\
ision
::
FLAG\
_OB\
JECTB\
_SO\
LID_I\
NT\
ERNAL
)
;
co\
nst
u32
lOOlO\
llOll\
ll
=
(
(
lOl\
lO\
OOlOll
->
GetFla\
g\
s
(
)
&
lOll\
O\
OO\
Ol\
l\
l\
O
)
^
lOl\
lOOOO\
llll
)
;
if
(
lOOl\
O\
llO\
llll
&&
lOOlOll\
lO\
lO\
O
)
re\
t\
u\
r\
n
;
Vec4
lOllOOlO\
llO
;
Vec4\

lOl\
l\
OOlOlll
;
fl\
oat
lOl\
lO\
O\
lO\
OlO
=
0.0f
;
#define lOOllOOOOllO()
#define lOOllOOOOOOO
#include "../Physics/zpta005.inl"
#undef lOOllOOOOOOO
#undef lOOllOOOOllO
}
bool\

TAC\
_CA\
L\
L
lOlOOOlllllO
(
float
lOllOOlO\
Ol\
O
,
Collisio\
n\

&
llOll\
llOl\
Ol
)
{
Dynam\
icObjec\
t
*
lOl\
lO\
OOlOll
=
llOl\
lllOlO\
l
.
Ge\
t\
Obj\
ectA
(
)
;
Dynam\
icO\
bj\
ect\

*
lOllO\
OOllO\
O\

=
llO\
llll\
O\
lOl
.
Get\
ObjectB\

(
)
;
TA\
_ASS\
E\
RT
(
lOllOO\
OlOll
)
;
TA_ASSE\
RT
(
lOllOOOllOO
)
;
bool
lOll\
OOOlOOOl
=
fals\
e\

;
const
bool
lOOlOl\
llOlll
=
false
;
const
flo\
a\
t
lOOl\
O\
lll\
O\
llO
=
1.0f
;
co\
n\
st
floa\
t\

lOOllOOOllOO
=
1.0f
;
const\

bool
lOO\
lOllOl\
lOO
=
tru\
e
;
cons\
t
bool
lO\
OlOlllll\
lO
=
fa\
l\
se\

;
cons\
t
bool
lOO\
l\
O\
llOlOll
=
tru\
e
;
const\

bool
lOll\
OO\
Ol\
OOOO
=
false
;
con\
s\
t
fl\
oat
lO\
Ol\
O\
ll\
OllOl
=
0.0f
;
co\
nst
int
lOllO\
OOOlllO
=
Dynami\
c\
Ob\
je\
ct
::
lOlOO\
l\
OOllOO
|
Dyn\
am\
i\
c\
O\
bject
::
lO\
OlllOO\
O\
lll
|
Dynamic\
Object\

::
lOOll\
lOOlOO\
l\

;
con\
st
int
lOllOOO\
Ol\
ll\
l
=
Dyn\
a\
m\
icObjec\
t
::
lOlOOl\
OOll\
OO
;
u3\
2
lOO\
lOl\
llOlOO
=
(
(
lOllOOOl\
lOO
->
Ge\
tF\
lags
(
)
&
lOllOOO\
Oll\
lO
)
^
lOll\
OOOOllll\

)
|
(
llOl\
lllO\
lO\
l
.
lOlOllOO\
Ol
&
Col\
li\
sion
::
FL\
AG_OBJE\
CTB_SOL\
ID_INTERNAL
)
;
boo\
l\

lOO\
l\
O\
llO\
lll\
l\

=
fa\
lse
;
lOl\
lOOlOOlO
=
-
lOllOOlOO\
lO
;
Vec\
4
lOll\
OOl\
O\
llO
;
Ve\
c\
4
lOll\
OO\
l\
Olll
;
#define lOOllOOOOllO() lOllOOOlOOOl = true;
#define lOOllOOOOOOO lOllOOOlOOOl
#include "../Physics/zpta005.inl"
#undef lOOllOOOOOOO
#undef lOOllOOOOllO
return
lOllOOOlOOOl\

;
}
vo\
id
TAC_\
CALL
lOl\
OO\
Ol\
lll\
ll\

(
flo\
at
lOOlOllOllO\
l
,
float
ll\
OllOllll
,
Col\
lisi\
on
&
llOlll\
lOlOl
)
{
TA_A\
SSE\
RT\

(
(
llO\
llllO\
l\
Ol
.
lO\
l\
O\
l\
lO\
O\
O\
l
&
Coll\
ision
::
FLAG_F\
RI\
CT\
I\
O\
N
)
==
0
)
;
TA_ASSERT\

(
ll\
Ol\
lllOlO\
l
.
lOl\
Ol\
lO\
OOl
&
Co\
ll\
ision
::
FLAG\
_F\
RICT\
I\
ON_GENERA\
TI\
NG_I\
MPULSE
)
;
DynamicObjec\
t
*
lOl\
lO\
OO\
l\
Oll
=
llO\
llllOl\
Ol
.
GetO\
b\
j\
e\
c\
t\
A
(
)
;
Dynamic\
O\
bject
*
lOllOO\
Ol\
lOO
=
llOllllOlOl
.
GetObje\
ct\
B
(
)
;
TA_ASSERT
(
lOll\
OOOlOll
)
;
TA_\
A\
S\
SE\
R\
T\

(
lOllOOOllO\
O
)
;
con\
st
bool
lOOl\
OlllOl\
l\
l
=
true
;
const
float\

lOOlOlllOllO
=
llOl\
lO\
lll\
l
;
const\

float
lOOllOO\
OllOO\

=
llOl\
lOlll\
l\

;
co\
nst
bool\

lOOl\
OllOl\
lOO\

=
fals\
e
;
const
bo\
o\
l\

lOO\
lO\
llll\
llO
=
false
;
con\
s\
t
bo\
ol\

lO\
OlOllOlOll
=
fals\
e
;
const
bo\
ol
lOll\
OOOl\
OOOO
=
fal\
s\
e
;
con\
s\
t\

int
lO\
l\
l\
OOO\
OlllO
=
Dy\
nam\
icO\
bje\
c\
t\

::
lOl\
OOlOO\
llOO
|
Dyn\
amic\
Object
::
lOOlll\
O\
OO\
ll\
l
|
Dy\
nam\
ic\
Object\

::
lO\
O\
lllO\
OlOOl\

|
Dynam\
icO\
bje\
ct
::
lO\
l\
OOlOlOO\
Ol
;
const\

int
lOll\
OOOO\
l\
lll
=
Dyna\
micOb\
j\
ect
::
lOl\
OOlO\
Oll\
OO
;
const
u3\
2
lOOlOl\
llO\
lOO
=
(
(
lO\
l\
l\
O\
OOl\
l\
OO
->
GetFlags\

(
)
&
lOl\
lOOO\
OlllO
)
^
lOll\
OOOOllll
)
|
(
llO\
lll\
l\
O\
lOl
.
lO\
l\
OllOOOl
&
Coll\
i\
sion\

::
FLA\
G_\
OB\
JECTB_SOLI\
D_INTE\
RN\
AL
)
;
const\

u32
lOOlOl\
lO\
lll\
l
=
(
(
lO\
ll\
OO\
Ol\
Ol\
l
->
Get\
Fl\
a\
gs
(
)
&
lOl\
lOOOOlllO
)
^
lOllOOOOllll
)
;
TA_\
A\
S\
S\
ERT
(
!
lOOl\
OllOl\
lll
||
!
lOOlO\
lllOl\
OO
)
;
Vec4
lOllO\
OlOllO
;
Vec\
4
lOllOO\
lO\
lll
;
float
lOllOOlO\
OlO
=
0.0f
;
#define lOOllOOOOllO()
#define lOOllOOOOOOO
#include "../Physics/zpta005.inl"
#undef lOOllOOOOOOO
#undef lOOllOOOOllO
}
