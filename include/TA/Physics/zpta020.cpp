//---------------------------------------------------------------------------------
// File Name: zpta020.cpp
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

#ifdef TA_DYNAMIC_OBJECT_GROUP_ENABLED
#include "../Physics/zpta008.h"
#ifndef TA_COLLISIONOBJECTCOMBO_H
#include "CollisionObjectCombo.h"
#endif //
namespace
TA
{
void
Dy\
na\
micObje\
ctGr\
o\
u\
p
::
Initialise
(
Dyn\
am\
icO\
bject
*
lOlllOOOllO\

)
{
Ini\
tial\
iseCo\
mmo\
n
(
)
;
lO\
OllO\
OlOlOO
=
Collis\
i\
onO\
b\
j\
ectC\
ombo
::
CreateNew
(
)
;
lOO\
ll\
OO\
lOlOO
->
Ini\
tial\
i\
se
(
)
;
lOlOl\
OlOOlO
=
lOlll\
OO\
OllO
->
lOl\
O\
lO\
l\
O\
OlO
;
lOOlO\
ll\
llOl\
O
=
lOll\
lOOOll\
O
->
lOOl\
Ol\
lll\
Ol\
O
;
lOO\
llOOlOl\
lO
=
lO\
lllOO\
OllO
->
lO\
OllO\
Ol\
OllO
;
lOOllO\
O\
lOll\
l
=
lOll\
lO\
OO\
l\
lO
->
lOO\
llOO\
lOll\
l
;
lOOl\
lOlOOOll
=
lO\
lllOOOllO
->
GetC\
enterOffset\

(
)
;
Clea\
r
(
)
;
SetFrame
(
lOl\
llO\
OO\
llO
->
Ge\
tFr\
a\
me\

(
)
)
;
lO\
lllOOOllO
->
lOO\
llOlOllll
=
this\

;
lOlOl\
llOOlOO
&
lOlO\
l\
l\
l\
OO\
lO\
l
=
lOlOlllOOll\
O\

.
lOOOl\
ll\
OO\
O
(
)
;
lOlO\
l\
llOOlOl\

.
lOOOO\
OOOO\
l
.
Clear
(
)
;
lO\
lOlllOOl\
Ol
.
lO\
lllOO\
O\
llO\

=
lO\
lll\
OOOl\
l\
O
;
}
void
Dynami\
cObje\
ct\
Group
::
Fi\
nalise
(
)
{
}
vo\
id
DynamicOb\
jec\
tGroup\

::
Upd\
a\
t\
e\

(
flo\
at
lOO\
ll\
lO\
Oll\
Ol
)
{
DynamicObject
::
Upd\
at\
e
(
lOOll\
lOO\
llOl
)
;
}
voi\
d\

Dyna\
m\
icObje\
ctGr\
oup
::
ApplyVeloc\
it\
yToN\
ext\
Fra\
me
(
floa\
t
lOOlllOOllOl\

)
{
Dy\
namicO\
b\
ject
::
App\
lyV\
eloc\
i\
t\
yToN\
e\
xtFra\
me
(
lOOlllO\
O\
llOl
)
;
fo\
r\

(
lOlOl\
llOOlll
lOlOl\
llO\
lOOO
=
lO\
lOlllOOl\
lO\

.
lOlOOOOl\
O\
O
(
)
;
!
lOlO\
lll\
Ol\
O\
OO\

.
AtEnd\

(
)
;
++
lO\
l\
Oll\
lOlOOO
)
{
lOlOlllO\
OlO\
O
&
lOlOll\
l\
OO\
lO\
l
=
*
lOl\
Oll\
l\
O\
lO\
OO\

;
TA_\
AS\
SERT\

(
lOlOll\
lO\
O\
lOl
.
lO\
ll\
lOOOll\
O
)
;
lOl\
Olll\
O\
O\
lO\
l
.
lOll\
lOO\
Ol\
l\
O
->
SetFr\
ame
(
lOlOl\
l\
lOOlOl\

.
lOO\
OOOOOOl
*
GetF\
ra\
me
(
)
)
;
lOlOll\
l\
OO\
lOl
.
lOl\
l\
lO\
OOllO\

->
Se\
t\
Angula\
rV\
e\
locit\
y
(
(
(
GetAn\
g\
u\
l\
arVeloc\
ity
(
)
/
Ge\
tF\
ram\
e
(
)
.
m3\
3Rotation
)
*
lO\
lOlllOOlOl\

.
lOOOOOO\
OO\
l
.
m33Ro\
tatio\
n
)
*
Get\
F\
rame
(
)
.
m33Rotation
)
;
lOlOlllOOlOl
.
lOlllOO\
Oll\
O
->
Se\
tLi\
nea\
rVeloc\
i\
ty
(
GetVelocityAt\
WorldPosi\
tion
(
lOlOlllOOlOl\

.
lOlllOOOllO
->
GetCente\
rOf\
M\
ass
(
)
)
)
;
lO\
l\
OlllOO\
l\
Ol
.
lOl\
llOOOllO
->
App\
lyVe\
lo\
c\
i\
tyToNextF\
rame
(
lOO\
lllO\
O\
l\
lOl\

)
;
}
}
void
Dynamic\
ObjectGroup
::
Ad\
dDyn\
amicObj\
ect
(
Dy\
namicObje\
ct
*
lOll\
l\
OOOllO
)
{
lOlO\
lll\
OOlOO
&
lOlOll\
lOOlOl
=
lOlOlll\
OOl\
lO
.
lO\
OOl\
llO\
O\
O\

(
)
;
lO\
lOl\
llO\
OlOl\

.
lO\
OOOOOO\
Ol
=
lOlllOOOllO
->
Ge\
tFrame
(
)
/
GetFram\
e
(
)
;
lOlOlllOOl\
Ol
.
lOlllOO\
OllO
=
lOl\
llOOO\
llO
;
Calculate\
Ma\
ss
(
lOl\
OlOlOOlO\

,
lO\
OllOlOOO\
ll\

,
lOOll\
OOlOllO
)
;
TA_\
ASSERT
(
lOlOlO\
lOO\
l\
O
>
0.0f
)
;
lOO\
lOlll\
lOlO\

=
1.0f
/
lOlOlOlO\
O\
lO
;
lOOl\
lOO\
l\
O\
ll\
l
=
lO\
OllOOl\
OllO
.
Get\
Inve\
rs\
e
(
)
;
lO\
lll\
OOOll\
O\

->
lOO\
llOlO\
llll
=
th\
is\

;
lO\
OlOlllOOll
=
lOOll\
OlOO\
Oll
*
lOOllO\
l\
OO\
lOl
;
lOOllOlO\
OlO\
O
=
lOOllO\
l\
O\
OO\
l\
l
*
lOOllOlOOllO
.
v3Tran\
sl\
at\
io\
n
;
}
void\

DynamicObj\
ectGroup
::
Remove\
Dy\
na\
micO\
bje\
ct
(
DynamicObject
*
lOll\
lO\
OO\
llO
)
{
lOOO\
OOOllO
(
0
,
"DynamicObjectGroup::RemoveDynamicObject: Todo")
;
}
DynamicObj\
ectGroup
*
Dyna\
micO\
bj\
e\
ctGroup
::
Crea\
teNe\
w\

(
)
{
Dy\
na\
micOb\
j\
ect\
Group
*
lOlOl\
l\
OOlll
;
lOO\
OO\
OllO\
O\
O\

(
lOl\
OllOOl\
l\
l
,
DynamicOb\
je\
ctGroup
)
;
return
lO\
lOll\
O\
Olll\

;
}
Dyna\
mic\
Obj\
ectG\
r\
o\
up\

::
Dyna\
m\
icO\
b\
jectGro\
up
(
)
:
Dy\
namicOb\
jec\
t
(
)
{
}
Dyn\
amic\
O\
b\
ject\
Group
::
~
Dynam\
icObject\
Gr\
ou\
p\

(
)
{
Finalis\
e\

(
)
;
}
void
Dyn\
amicObjectG\
rou\
p\

::
Cal\
cul\
a\
t\
eM\
ass
(
flo\
a\
t\

&
lOllOO\
O\
OlO\
O
,
Ve\
c\
3
&
lllOll\
O\
O\
OOO\

,
Mat3\
3
&
lllOl\
lOO\
O\
Ol
)
const
{
lOlOlllOl\
OOl\

lOlO\
l\
l\
l\
OlOOO
;
lll\
OllOOOO\
O\

.
Clear
(
)
;
lOll\
OOOOlOO
=
0
;
lllOllOOOOl
.
v3X
.
Clear
(
)
;
ll\
lOl\
l\
OOOO\
l
.
v3Y\

.
Clear
(
)
;
ll\
lOllOOO\
O\
l
.
v3\
Z
.
Cl\
ear
(
)
;
for
(
lOlOll\
lO\
lOOO\

=
lOlO\
lllOOllO
.
lOOOOOl\
OllO
(
)
;
!
lO\
lO\
lllOlO\
OO
.
At\
End\

(
)
;
++
lOl\
O\
lllOlOO\
O
)
{
const\

lOl\
OlllOOlOO
&
lOlOlll\
OO\
l\
O\
l
=
*
lOl\
Ol\
l\
lOlOOO
;
TA_ASS\
E\
R\
T\

(
lOlOl\
l\
lO\
OlO\
l
.
lOl\
ll\
OOOllO
)
;
lO\
ll\
OOOOlOO
+=
lOlOlllOOlOl\

.
lOlllOOOll\
O
->
GetMass
(
)
;
lllOl\
lOOOOO
+=
(
lOlOlllOO\
l\
Ol\

.
lOlllO\
OOllO
->
Get\
C\
enter\
Of\
f\
set\

(
)
*
lOlO\
lllOOlO\
l\

.
lOOO\
OOOOOl\

)
*
lOlOll\
lOOl\
O\
l
.
lOlllOOOl\
l\
O\

->
GetMa\
ss
(
)
;
}
if
(
lOl\
l\
OOOO\
l\
OO\

>
0.0f
)
lllOll\
OOOOO
/=
lOllOOOO\
lO\
O\

;
for
(
lO\
l\
OlllOlOOO\

=
lO\
l\
OlllOOl\
lO
.
lOO\
O\
O\
Ol\
O\
llO\

(
)
;
!
lOlO\
lllO\
lOO\
O
.
At\
End
(
)
;
++
lOl\
Oll\
lO\
lOO\
O
)
{
co\
n\
s\
t
lO\
lOl\
llOOlO\
O
&
lOlOll\
l\
OOl\
Ol
=
*
lOl\
Olll\
OlOOO\

;
TA_ASS\
ER\
T\

(
lOl\
O\
l\
llOOlO\
l\

.
lOlllO\
O\
OllO
)
;
Mat3\
3\

lOlOl\
llOlOlO
=
lO\
lOlllOOl\
Ol
.
lOO\
OOOOOOl
.
m33R\
o\
t\
a\
ti\
on
.
GetTransposeAsInverse\

(
)
*
lOlOl\
llO\
OlO\
l\

.
lOlllOOOl\
l\
O
->
lOOl\
l\
OOlOll\
O
*
lO\
lOlll\
OO\
lO\
l\

.
lOOO\
OO\
OOOl
.
m33Rotation\

;
float
lOlOlllOl\
Oll
=
lOl\
O\
ll\
lO\
OlOl
.
lOll\
lOO\
OllO
->
Ge\
t\
M\
ass
(
)
;
Vec3
lOlO\
lllO\
l\
lOO
=
lOlOlllOOlO\
l
.
lOlll\
OOOllO\

->
Ge\
t\
Ce\
nt\
erO\
f\
fset
(
)
*
lOl\
Ol\
llO\
OlOl\

.
lOOOO\
OOO\
Ol\

;
Vec3
lllOlOl\
OllO
=
lOlOl\
ll\
OllOO
-
ll\
lOllOOO\
OO\

;
lOl\
O\
lllOlO\
l\
O
.
M11
(
)
+=
lOl\
Oll\
lO\
lOll
*
(
lllOl\
O\
lOllO\

.
y
*
lllOlOlOl\
lO
.
y
+
lllOl\
O\
lO\
llO\

.
z
*
lllOlO\
lO\
llO\

.
z
)
;
lOlOlllOlOlO
.
M2\
2\

(
)
+=
lOlOll\
l\
OlOll
*
(
lllOlOlO\
llO
.
z
*
lllO\
lOlOll\
O\

.
z
+
lllO\
lOlOllO
.
x
*
lllO\
lO\
lOllO
.
x
)
;
lOlOlll\
OlOl\
O
.
M33
(
)
+=
lOlOlllO\
lOll
*
(
lllOlOl\
Oll\
O
.
x
*
lllOlOlOllO
.
x
+
lll\
OlOlOllO\

.
y
*
lllO\
lOlO\
llO\

.
y
)
;
lOl\
Olll\
OlO\
l\
O
.
M12\

(
)
=
lOlO\
l\
llOlO\
lO
.
M2\
1\

(
)
-=
lOlOll\
lOlO\
ll
*
lll\
Ol\
O\
lOllO\

.
x
*
ll\
lO\
l\
OlOl\
l\
O\

.
y
;
lOl\
OlllOlOlO
.
M2\
3
(
)
=
lO\
l\
OlllOlOlO
.
M32\

(
)
-=
lOlOlllOlOll
*
lllOlOlO\
llO
.
y
*
lll\
Ol\
OlOllO
.
z
;
lO\
l\
Ol\
ll\
OlO\
lO
.
M13
(
)
=
lOlOl\
llOlO\
lO\

.
M31
(
)
-=
lO\
lO\
lllO\
lOll\

*
ll\
lOlO\
lOllO\

.
z
*
ll\
lO\
lOlO\
llO
.
x
;
lllOl\
lO\
OOOl\

+=
lOl\
O\
l\
llOlOlO
;
}
}
}
#endif
