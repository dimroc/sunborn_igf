//---------------------------------------------------------------------------------
// File Name: zpta017.cpp
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

#define TA_PHYSICS_INTERNAL
#define lOllOllOOOO
#include "../Physics/zpta007.h"
#ifndef TA_GEOMETRY_H
#include "../Common/Geometry.h"
#endif //
#ifndef TA_COLLISIONOBJECTCACHEDPOLYDATA_H
#include "CollisionObjectCachedPolyData.h"
#endif //
nam\
e\
space
TA
{
cons\
t
fl\
oa\
t
lOO\
lOlO\
OOOll
=
Sqr\
t
(
k_\
fMax\
Float
)
;
cons\
t\

floa\
t
lOO\
lOl\
OOO\
lOO
=
k_fM\
inFloat
;
co\
nst
float\

lOOlOlOOOlOl
=
Sqrt\

(
3.0f
)
;
fl\
oa\
t\

ll\
OO\
ll\
OO\
OO\
l\

=
1.0f
;
bo\
ol
TA\
C_\
CALL
lllO\
lllOllO\

(
Collis\
io\
n\

&
llOllllOlO\
l
,
lOllO\
llOllO\

*
lOOlOlO\
OO\
llO
)
{
TA_ASSERT\

(
lOO\
lOlOOOl\
lO\

)
;
lOl\
l\
OllOll\
O\

&
lOOOOOlOO\
O\
O
=
*
(
lOll\
Ol\
lO\
ll\
O
*
)
lOOlOlO\
OOllO\

;
if
(
!
lOOOO\
OlOOO\
O
.
lOllOll\
lO\
lO
)
{
llOllllOl\
Ol
.
lOl\
lOOO\
llO\
l
=
lO\
O\
OOOlOOOO
.
lO\
llOOOl\
lOl
;
ll\
Ol\
ll\
lOlOl
.
lOllOOOlllO
=
lOOOOO\
lOO\
OO
.
lOllOOOlllO\

;
}
el\
se
{
llO\
l\
l\
l\
l\
OlOl
.
lO\
llOOOllO\
l
=
lOOOO\
OlOOOO
.
lO\
ll\
O\
O\
OlllO
;
llOllllO\
lOl\

.
lOllOOO\
lllO
=
lOOOOOlO\
OOO
.
lO\
llOO\
OllO\
l\

;
ll\
Olll\
lOlOl
.
lOO\
OOlll\
ll
*=
lO\
OOO\
OlOOOO
.
lOllOl\
lllOO
;
llO\
l\
lllOlO\
l
.
lOO\
OlOOOOO\

*=
lOO\
O\
O\
OlOOO\
O\

.
lOllOllllO\
O
.
m3\
3Rotati\
o\
n
;
llO\
llll\
OlOl
.
lOOO\
l\
OOOOO
=
-
llO\
llllOl\
Ol
.
lO\
O\
OlOO\
OOO
;
Sw\
ap\

(
llOl\
l\
llOlOl
.
lOl\
lOOOl\
l\
ll
,
llOllllOlOl
.
lOll\
O\
OlOOOO
)
;
}
TA_\
ASSE\
RT
(
lOO\
O\
O\
OlO\
OOO
.
lOllOl\
lOll\
l
)
;
re\
tur\
n\

lOOOOOlO\
OOO
.
lOllOllOll\
l
(
ll\
Oll\
llOlOl
,
lOOOOO\
lO\
OOO
.
lOllOlllOOO
)
;
}
bo\
ol
TAC\
_CALL\

lllOO\
l\
lOlO\
l
(
Coll\
i\
sion
&
llOlll\
lOlO\
l
,
lO\
llO\
l\
l\
O\
l\
l\
O\

*
lOOl\
OlOOOl\
lO
)
{
TA\
_AS\
S\
E\
RT
(
lOOlOlOOOl\
lO
)
;
lOl\
lOllOl\
l\
O\

&
lOOO\
OOl\
OOOO
=
*
(
lOllO\
llO\
l\
lO
*
)
lOOl\
O\
lOOOll\
O
;
if
(
!
lOOO\
OOlOO\
OO
.
lOll\
O\
lllOl\
O
)
{
llO\
llllO\
lOl\

.
lOllOO\
Ol\
l\
Ol
=
lO\
O\
OOOlOO\
O\
O
.
lOllOOOllO\
l\

;
llO\
ll\
llOlOl
.
lOll\
OOOlllO
=
lOOOOO\
lO\
OOO
.
lOll\
O\
OOl\
ll\
O
;
}
el\
s\
e
{
llOlll\
lOlO\
l
.
lOllO\
OOllOl
=
lOOOOOlO\
OOO
.
lOllOOOll\
lO
;
llO\
llllOl\
O\
l\

.
lO\
l\
lOOOlllO
=
lOOO\
OOl\
OOO\
O
.
lO\
ll\
O\
O\
O\
l\
l\
Ol\

;
llO\
llllOlOl
.
lOOOOll\
lll
*=
lOOOO\
Ol\
OOOO
.
lOl\
lO\
ll\
llO\
O
;
llOll\
l\
lO\
lOl
.
lOOO\
lOOOO\
O\

*=
lOO\
OOOl\
O\
OO\
O
.
lOll\
OllllOO
.
m33Rota\
tio\
n
;
llO\
l\
ll\
lOlO\
l\

.
lOOOlOOOOO
=
-
llOl\
lllOlOl
.
lOOO\
lOOO\
OO
;
Swa\
p
(
llOllllOl\
O\
l
.
lO\
llO\
OO\
l\
ll\
l\

,
llOll\
llOlOl
.
lO\
llOOlO\
OOO
)
;
}
if
(
llO\
ll\
ll\
OlOl\

.
lO\
ll\
OOOllO\
l\

->
GetC\
ollisi\
onO\
bject\
T\
ype\

(
)
==
Coll\
is\
ion\
Ob\
ject
::
TY\
P\
E_CACHED_\
POLY_DA\
TA
)
llOl\
lllOlOl
.
lOl\
lO\
OOllOl
=
(
(
Colli\
sionObjec\
t\
Cache\
dP\
o\
ly\
Data\

*
)
llOlll\
lOlO\
l
.
lOl\
lOOOllOl
)
->
ll\
l\
OlO\
OlOO\
l
(
)
;
if
(
llOllllOlOl\

.
lOllOOOlll\
O
->
Ge\
tColli\
si\
onO\
bjectTyp\
e\

(
)
==
Co\
l\
lisionObject
::
TYP\
E\
_CA\
CHED_POL\
Y\
_D\
ATA
)
llOllllOlOl
.
lO\
llOO\
OlllO
=
(
(
Colli\
s\
i\
on\
Object\
Ca\
c\
h\
edP\
olyDat\
a
*
)
llOl\
lllOl\
Ol\

.
lO\
llOO\
OlllO
)
->
lllOlO\
OlOOl\

(
)
;
TA_A\
SSERT
(
lOOO\
OOlO\
O\
OO
.
lOllOl\
lOl\
ll
)
;
return
lOO\
OO\
OlO\
OOO
.
lO\
llOllOl\
l\
l
(
llOl\
lllO\
lOl
,
lOO\
OOOlOO\
OO
.
lOll\
OlllOOO
)
;
}
inl\
ine
floa\
t
TAC_CALL
lOOl\
OlO\
OO\
lll\

(
floa\
t
lOOOOOl\
ll\
O\

)
{
return
(
lOOOOOlllO
>=
0.0f
)
?
Exp
(
Log
(
lOOOOOlllO
)
*
(
1.0f
/
3.0f
)
)
:
-
Exp
(
Lo\
g\

(
-
lO\
OOOOll\
lO
)
*
(
1.0f
/
3.0f
)
)
;
}
int
TA\
C_C\
ALL
lOO\
lOl\
OOl\
O\
OO
(
floa\
t
lll\
Oll\
lllO
,
fl\
oat
lllOl\
ll\
lOl
,
float
lOO\
O\
lll\
Oll\
O\
O\

[
2
]
)
{
fl\
oat
lOOlOlO\
OlOOl
=
lllO\
l\
l\
ll\
lO\

*
lll\
Olll\
llO
-
4.0f
*
lllOl\
lllOl
;
if
(
lOOlOl\
OOlOO\
l
<
0.0f
)
{
lOOOlllOl\
lOO
[
0
]
=
0.0f
;
lOOOlllOl\
lOO
[
1
]
=
0.0f
;
return
0
;
}
lOOl\
O\
l\
OOlO\
Ol
=
Sq\
rt
(
lO\
Ol\
OlO\
O\
lOO\
l
)
;
lOOO\
lllOll\
OO\

[
0
]
=
(
-
lll\
Oll\
lllO
-
lO\
OlOlOOlOOl\

)
*
0.5f
;
lO\
OOll\
lOl\
lOO
[
1
]
=
(
-
lllOlllllO
+
lO\
OlOl\
OOlOOl
)
*
0.5f
;
return\

2
;
}
float\

TAC\
_CALL\

lOOlOlOOlO\
l\
O
(
flo\
at\

llll\
OO\
OOO\
O
,
flo\
a\
t
lllOll\
l\
ll\
O
,
float
lllOl\
lllO\
l
)
{
if
(
Fab\
s
(
ll\
llOOO\
OOO\

)
>
lOOl\
OlOOOOll\

)
ret\
u\
r\
n
-
llllOOO\
OOO
;
if
(
Fabs\

(
ll\
lOll\
lllO
)
>
lO\
OlO\
lOOOOl\
l
)
{
if\

(
lllOll\
l\
ll\
O
>
0.0f
)
return
-
lllOllllOl
/
lllOll\
lll\
O
;
else\

return\

-
Sq\
rt
(
-
ll\
lOlll\
l\
l\
O\

)
;
}
if
(
Fa\
bs
(
lllO\
lll\
lO\
l
)
>
lOOl\
OlOO\
OOll
)
ret\
urn
-
lOOl\
OlO\
O\
Olll\

(
lllOll\
ll\
Ol\

)
;
flo\
at
lOOlOlOO\
lO\
ll
=
llllOOOO\
OO\

*
(
1.0f
/
3.0f
)
;
float
lOO\
lO\
lOO\
llO\
O
=
lOOlO\
lOO\
lOll
*
lOOlOl\
OOlOll
;
if
(
lOOlOl\
OO\
l\
lOO\

>
lOOlO\
lO\
OOO\
ll
)
retu\
rn
-
ll\
ll\
OOOOOO
;
float
lOOlO\
lOOllOl
=
lll\
OllllOl\

+
lOOlOlO\
OlOll
*
(
lO\
OlOlOO\
l\
lOO
+
lO\
OlOlOOllOO
-
ll\
lOl\
llll\
O
)
;
if
(
Fab\
s\

(
lO\
OlO\
lOOll\
Ol\

)
>
lOOlOlOOO\
Oll
)
ret\
urn
-
llllOOOOO\
O
;
fl\
oat\

lOOlOlOO\
lll\
O\

=
lll\
Oll\
ll\
lO
*
(
1.0f
/
3.0f
)
;
flo\
a\
t
lOOO\
lllO\
lO\
lO
=
lO\
OlO\
lOOlll\
O
-
lOOlOlOOllOO\

;
float
lOO\
l\
OlOOllll
=
lO\
OOlllOlOlO
+
lOO\
O\
l\
ll\
O\
lOlO
;
if
(
Fabs
(
lO\
OlOlOOlll\
l
)
>
lO\
OlOlOOOOll
)
{
if
(
lll\
l\
O\
OO\
OO\
O
!=
0.0f
)
retu\
rn\

-
lllOll\
lll\
O
/
llllOO\
OOOO
;
if
(
lllOll\
lllO\

>
0.0f
)
re\
t\
u\
rn\

-
ll\
lOllllOl\

/
lll\
OlllllO
;
el\
s\
e
ret\
urn
-
Sqrt
(
-
lllOll\
lllO
)
;
}
float
lOOlO\
l\
OlO\
OO\
O\

=
lOOl\
O\
lOOllll
*
lO\
O\
lOlO\
O\
ll\
ll
;
if
(
lOOlO\
lOlOOOO\

>
lOO\
l\
O\
lOOOO\
ll
)
{
if
(
llll\
OOO\
OOO\

!=
0.0f
)
return
-
lllOll\
l\
llO
/
llll\
O\
OO\
OOO\

;
if\

(
lllOlllllO
>
0.0f
)
retur\
n
-
lll\
O\
l\
lll\
Ol
/
lllOl\
l\
l\
l\
lO
;
el\
se
re\
turn\

-
Sqrt
(
Fabs\

(
lllOllll\
lO
)
)
;
}
float
lllOOlOlll
=
lOOlO\
lO\
lO\
OOO\

*
lOOOll\
lOlO\
l\
O
+
lOOlOl\
OOllO\
l
*
lO\
Ol\
OlO\
Oll\
Ol
;
if
(
ll\
lO\
OlOl\
ll
>=
0.0f
)
{
flo\
at
lOOlOl\
Ol\
O\
OOl
=
lO\
O\
lOlOO\
O\
ll\
l
(
lOOl\
O\
l\
OOllOl\

<=
0.0f
?
(
-
lOOlOl\
OO\
l\
lOl
+
Sqr\
t\

(
ll\
l\
OO\
lOlll\

)
)
*
0.5f
:
(
-
lOOl\
OlOOllOl
-
Sqr\
t
(
ll\
lO\
OlOlll
)
)
*
0.5f
)
;
if
(
lOOlOlOlOOOl\

!=
0.0f
)
lllOO\
lOl\
ll
=
-
lOOOl\
llO\
lOlO
/
lO\
OlOlO\
lOOOl
;
els\
e
lllOOl\
O\
lll\

=
0.0f
;
re\
tu\
r\
n\

lOOl\
OlOlOO\
Ol
+
lll\
OOlO\
lll
-
lOOlO\
l\
OOlOll
;
}
if\

(
lOOOl\
llO\
lO\
lO
>=
0.0f
)
ret\
ur\
n
lO\
OlOl\
O\
OOlll\

(
lOO\
lOlOOll\
Ol
)
-
lO\
OlOl\
O\
OlOl\
l
;
lllOOlOl\
ll
=
Sq\
r\
t
(
-
lOOOlll\
OlOlO
)
;
float
lOO\
lOlOlOOlO
=
-
lOOlOlOOllO\
l
/
(
lll\
OOlOll\
l
*
-
lOOOl\
llOl\
OlO\

*
2.0f
)
;
ClampP\
osNeg
(
lO\
O\
lO\
lOlO\
OlO\

,
1.0f
)
;
float
lOO\
OOll\
OOl\
O
=
Cos
(
AC\
os
(
lOOlOlOlOO\
lO
)
*
(
1.0f
/
3.0f
)
)
;
if
(
lOOlOlOO\
lOl\
l
<
0.0f
)
retur\
n
ll\
lOOlOlll
*
2.0f
*
lOOOOll\
O\
Ol\
O
-
lOOlO\
l\
OOlOl\
l\

;
float
lOO\
lO\
lOlO\
Oll\

=
1.0f
-
lOO\
OOllOO\
lO
*
lOOOOll\
OOlO\

;
if
(
lOOlOl\
Ol\
O\
O\
ll
<
0.0f
)
lOOlOlO\
lOO\
ll\

=
0.0f
;
retur\
n
ll\
lOOlOlll\

*
(
-
lOOOO\
llOOlO
-
lOOlOl\
OOOlO\
l
*
Sqrt\

(
lO\
O\
lOlOl\
OOll\

)
)
-
lOOlOl\
OOlOll
;
}
int
TAC\
_\
CALL
lOO\
Olll\
OlllO\

(
fl\
oat
ll\
llOOOOOO
,
float
lllOlll\
llO
,
floa\
t
lllOllllO\
l
,
flo\
a\
t\

lOOOlll\
OlOlO\

,
flo\
at\

lOOOlllOllOO
[
4
]
)
{
float
lOOlOlOlOlOO
=
llllOOOOOO\

*
llllOOOOOO
;
floa\
t
lOlOO\
O\
lO\
O\
ll
=
lOOlOl\
O\
Ol\
OlO
(
lllOllll\
lO
,
llllO\
OOOO\
O\

*
ll\
lOlll\
lOl\

-
4.0f
*
lOOOl\
l\
l\
OlOlO
,
(
lOOlOlO\
lOlOO\

-
4.0f
*
lllOll\
lllO
)
*
lOOOlllOlO\
l\
O\

+
lllOlll\
lOl\

*
lllOlll\
l\
Ol\

)
;
flo\
at\

lOOl\
O\
l\
OlOl\
Ol\

=
0.25f
*
lOO\
lO\
lOlOlOO
-
lllOlll\
l\
lO\

-
lOlOOO\
lOO\
ll
;
if
(
lO\
O\
lOlOlOl\
Ol
<
0.0f
)
return
0
;
flo\
a\
t
lOO\
l\
OlOlOllO
=
0.25f
*
lOlO\
O\
OlOOll
*
lOlOO\
OlOOl\
l\

-
lOO\
O\
lllO\
lOlO
;
if\

(
lOOl\
Ol\
Ol\
OllO
<
0.0f
)
retur\
n\

0
;
floa\
t
lO\
OlOlOlO\
lll
=
-
(
0.25f
*
llll\
OO\
OOOO\

*
lOlOO\
Ol\
O\
Oll
+
0.5f
*
lllOl\
lllOl\

)
;
flo\
at
lOOlOlOOl\
lOl
;
fl\
oa\
t
lOO\
l\
O\
lOll\
OOO
;
if
(
(
(
(
(
u32
&
)
llllOOOOO\
O
)
^
(
(
u32
&
)
lllOl\
ll\
llO
)
^
(
(
u32
&
)
lllOll\
l\
lO\
l
)
)
&
0x80000000
)
==
0
||
lll\
lOOOOOO
==
0.0f
||
lllOl\
llllO
==
0.0f
||
lll\
Ol\
lllOl
==
0.0f
)
{
if
(
lll\
Ollll\
lO
<
0.0f
&&
lOlOO\
OlOO\
ll
<
0.0f
&&
lOOlOlOlOl\
O\
l
>
0.0f
)
{
lOOlOlOOllO\
l
=
Sq\
rt
(
lOOlOlOl\
OlO\
l
)
;
lOOlOlOllOO\
O
=
lO\
Ol\
OlOlO\
lll
/
lOO\
l\
O\
l\
OO\
l\
lO\
l\

;
}
else
if
(
lOO\
O\
lll\
O\
lOlO
<
0.0f
&&
lOOlOlOlOllO
>
0.0f
)
{
lOOlOlOllOOO
=
Sqrt\

(
lOOlO\
lOlOllO
)
;
lOO\
lOlOOll\
O\
l\

=
lOOlO\
lO\
lOlll\

/
lOO\
l\
O\
lOl\
lOOO
;
}
else
{
lOOlO\
lO\
O\
llOl
=
Sq\
rt\

(
lO\
O\
lO\
lO\
lOlOl
)
;
lOOl\
OlO\
l\
lO\
OO
=
Sqrt
(
lOOlOl\
OlO\
llO
)
;
if
(
lOOlO\
lO\
l\
Olll
<
0.0f
)
lOOl\
OlOllOOO
=
-
lOOlOlOllOOO
;
}
}
els\
e
{
lOOl\
OlOOl\
lO\
l
=
Sq\
rt
(
lOO\
l\
OlOlOl\
Ol\

)
;
lOOlOl\
Oll\
OOO
=
Sqrt
(
lO\
OlO\
lO\
lOllO
)
;
if\

(
lOO\
l\
OlOl\
Ol\
l\
l
<
0.0f
)
lOOlOlOll\
O\
OO\

=
-
lOOlO\
lOllOO\
O
;
}
floa\
t
ll\
lOO\
lOl\
ll\

=
llllO\
OO\
O\
OO
*
0.5f
;
floa\
t\

lO\
OlOlOll\
OOl
=
ll\
lOOlOl\
l\
l\

-
lOOlOlOO\
l\
lO\
l
;
floa\
t\

lOO\
l\
OlO\
llOlO
=
lllOOlOll\
l
+
lOO\
lO\
l\
O\
Ol\
l\
Ol
;
if
(
(
lllO\
l\
llll\
O\

>
0.0f
&&
lO\
l\
OOOlOOll
>
0.0f
)
||
(
lllO\
lllll\
O
<
0.0f
&&
lO\
l\
OOOlOOll
<
0.0f
)
)
{
if
(
lO\
Ol\
OlOOl\
l\
Ol
!=
0.0f
)
{
if
(
lll\
lOOO\
OO\
O
>
0.0f
)
lO\
O\
lOl\
OllO\
Ol
=
(
ll\
lOlll\
ll\
O\

+
lOl\
OOOl\
O\
Oll
)
/
lOO\
lOlOllOl\
O\

;
else\

lOOlOlO\
l\
lOlO
=
(
lll\
Ol\
l\
lllO\

+
lOlOO\
OlOOl\
l\

)
/
lO\
OlOlOll\
OOl
;
}
}
floa\
t
lOOl\
O\
l\
OllOll
;
float\

lOOlOlOl\
llO\
O\

;
if
(
lOl\
OOO\
lOO\
ll
==
0.0f
&&
lOOlOlOllO\
OO
==
0.0f
)
{
lO\
OlOlOl\
lOll
=
0.0f
;
lO\
O\
lOlOll\
l\
OO
=
0.0f
;
}
else
if
(
(
lOOl\
OlOllOO\
O
>
0.0f
&&
lOlOOO\
lOOl\
l
<
0.0f
)
||
(
lOOlOl\
Ol\
lOOO
<
0.0f
&&
lOlOOOlOOll\

>
0.0f
)
)
{
lOOlOl\
OlllOO
=
-
0.5f
*
lOl\
OOOl\
OOll
+
lO\
Ol\
O\
lOllOOO
;
lOOlOlOllOll
=
lOO\
Oll\
lOlO\
lO
/
lOO\
lOlOlllO\
O
;
}
else
{
lOOlOlOll\
Oll
=
-
0.5f
*
lOlO\
OO\
lOOll
-
lOO\
l\
OlOllOO\
O
;
lOOlOl\
OlllO\
O\

=
lOO\
O\
lllO\
l\
Ol\
O
/
lO\
OlOlOllOll\

;
}
flo\
at\

lOOl\
OlO\
l\
l\
l\
Ol
[
2
]
;
float
lOOlOlO\
llllO
[
2
]
;
int
lOOOOlOO\
OOlO\

=
lO\
O\
lOlOOl\
OOO
(
lOOlOlO\
llOlO
,
lOO\
lO\
lO\
lll\
OO
,
lO\
O\
lOlOlllOl
)
;
int
lOOOOl\
OOOOl\
l\

=
lOOlO\
lO\
OlO\
OO\

(
lOOlOlO\
l\
l\
OO\
l
,
lO\
O\
lOlO\
l\
lOll
,
lOO\
l\
OlOllllO\

)
;
lO\
OOlllO\
l\
l\
OO
[
0
]
=
lOOlOlO\
lllOl\

[
0
]
;
lOOOll\
lO\
ll\
O\
O
[
1
]
=
lOO\
l\
OlOlllOl
[
1
]
;
lOOOll\
lOllOO\

[
lOO\
OOlOOOOlO
+
0
]
=
lOOlOl\
Ollll\
O
[
0
]
;
lOOOl\
ll\
O\
llOO
[
lOO\
OOl\
O\
OO\
Ol\
O\

+
1
]
=
lO\
Ol\
O\
lOllll\
O
[
1
]
;
re\
tu\
r\
n
lOO\
OOlOO\
OOl\
O
+
lOOOOlOOOOl\
l
;
}
bool
TAC_C\
ALL\

lOO\
OllO\
lOll\
l
(
con\
st
Vec3
&
lOOlOlOlllll
,
cons\
t\

Vec\
3
&
lOOl\
OllOO\
OOO
,
cons\
t
Vec\
3
&
lOOl\
Oll\
OOOOl
,
const
Ve\
c3\

&
lOOl\
OllOOOlO\

,
flo\
at\

lOOlO\
llOOOll
,
fl\
oat
lOOlOllOOlOO\

,
float\

&
llllOl\
O\
O\
O\
l
,
Vec3
&
lO\
ll\
llOO\
Ol
,
Vec3
&
lOllll\
OOlO
)
{
con\
s\
t
float
lOl\
lOl\
OlOl
=
0.00001f
;
{
Vec3
llOlOOlO\
lll
=
lOOlOllOOOOO\

-
lOO\
lOlOlllll\

;
Vec3
lOOOl\
OlOOlOl\

=
Geom\
etry\

::
Fin\
dClosest\
P\
o\
intO\
n\
Line
(
lOO\
l\
Ol\
lOOO\
Ol
,
lO\
OlO\
lOllll\
l
,
lOOl\
Oll\
OO\
OO\
O
)
;
Vec\
3\

lOO\
lOllOOlO\
l
=
llOlOOl\
Olll
.
Cr\
oss\

(
lOOlOl\
l\
OOOOl
-
lO\
OOlOlOOlOl
)
.
Cross
(
lOOlOllO\
O\
OlO
)
;
float\

lll\
OOlOlOl
=
lOOlOl\
lOOlOl
.
GetM\
a\
gni\
tu\
d\
e\
Sq\
rd
(
)
;
if
(
lll\
OO\
l\
OlO\
l
<
lOl\
lO\
lOlOl
*
lO\
llO\
l\
O\
lOl
*
lOl\
lO\
l\
Ol\
Ol
)
re\
tur\
n
fal\
se
;
lO\
OlO\
ll\
O\
O\
l\
Ol
/=
Sq\
r\
t
(
lllO\
OlOl\
Ol
)
;
Ve\
c3
lOO\
lOll\
OOllO
=
lOOl\
OllOOOOl
+
lOOlO\
llOOlOl\

*
lOOl\
OllO\
OOll
;
float\

lOOlOllOOlll
=
Geom\
etry
::
Find\
Sho\
rtestDi\
stanceToLi\
neS\
q\
rd\

(
lOOlOllO\
O\
llO
,
lOO\
lO\
lO\
l\
l\
lll
,
lOOlO\
ll\
OO\
OO\
O
)
;
if
(
lOOlOllOOlll
>
lOO\
lO\
l\
lOO\
l\
OO\

*
lOOlOl\
l\
OO\
lOO
)
return
false
;
}
Ma\
t33
lOO\
OO\
O\
OOl\
l
;
lOOO\
OOO\
O\
ll
.
Set\
ToL\
oo\
k\
Do\
wnVector
(
lO\
O\
lOllOO\
OlO
)
;
Mat\
33\

lOOO\
lllO\
Oll\
O
=
lOOOOOOOll\

;
lOO\
O\
l\
ll\
O\
OllO
.
TransposeAs\
Inve\
rt
(
)
;
Vec3
llll\
l\
l\
l\
ll
=
(
lOOlOlOlllll\

-
lO\
O\
lOllO\
OOOl
)
*
lOOOll\
lOOllO
;
Vec3
llll\
OO\
OOOOO
=
(
(
lOOlO\
llOOOOO\

-
lOOlOlOl\
llll
)
*
lO\
OOlll\
OOllO
)
;
flo\
at
lOOOll\
lOOl\
ll
=
llllOOOO\
OOO
.
Ge\
tMagn\
itud\
e\
S\
q\
rd
(
)
;
if
(
lO\
OOlll\
OOlll
<
lOllO\
lOlOl\

)
return\

false
;
const
floa\
t
lOO\
lO\
ll\
OlOOO
=
lll\
lOO\
OOOO\
O
.
Dot\

(
llll\
l\
ll\
ll
)
/
lOO\
Ol\
llOOl\
ll\

;
lllll\
ll\
ll
-=
llllOOO\
OO\
O\
O\

*
lO\
O\
lOl\
lO\
lOO\
O
;
float
lOOOl\
llOlOOO
=
2.0f
*
lllll\
l\
lll
.
Dot\

(
llllOOOOOOO
)
;
float
lOOOlllOlO\
Ol\

=
lllll\
llll
.
Ge\
tMa\
g\
n\
itude\
S\
qr\
d
(
)
-
Pow\
2
(
lOOlOl\
lOOOll\

)
-
Pow2
(
lOOlO\
llO\
OlO\
O\

)
;
co\
n\
s\
t
flo\
at
ll\
lOll\
llll
=
Po\
w\
2
(
lOOOlllOOlll\

)
;
const\

float
llll\
O\
OOOOO
=
2.0f
*
lO\
OO\
lll\
OO\
lll
*
lOOOlllOl\
OOO
;
co\
nst\

float
lllOlllllO\

=
2.0f
*
lOOOlll\
O\
Oll\
l
*
lOO\
OlllOlO\
Ol\

+
Pow2
(
lOOOlllO\
l\
O\
OO
)
+
4.0f
*
Pow2
(
lOO\
l\
OllOOOll
*
lll\
lOOOOOOO
.
z
)
;
const
float
ll\
lOlll\
lOl\

=
2.0f
*
lO\
O\
OlllOlOOO
*
lOOO\
l\
ll\
OlO\
Ol
+
8.0f
*
Pow2
(
lO\
O\
l\
Ol\
lO\
OOl\
l\

)
*
llllllll\
l\

.
z
*
llllOO\
OO\
O\
O\
O
.
z
;
co\
ns\
t\

floa\
t
lOOOlllOl\
O\
lO
=
Po\
w\
2\

(
lOOOlllOlO\
O\
l\

)
+
4.0f
*
Pow2
(
lOOl\
OllOO\
Ol\
l\

)
*
(
Pow2
(
lll\
lll\
lll
.
z
)
-
Pow2\

(
lO\
OlOllOOlOO\

)
)
;
co\
nst
flo\
a\
t
lO\
OO\
lll\
Ol\
O\
ll
=
1.0f
/
lllOlll\
l\
l\
l\

;
float
lO\
OO\
lllOll\
OO
[
4
]
;
int
lOO\
Ol\
llOll\
Ol
=
lO\
OOll\
lOl\
llO
(
llllOOO\
OOO
*
lOOOl\
ll\
OlOll
,
lllOllll\
l\
O
*
lOO\
Ol\
llO\
lO\
l\
l
,
lllOllllOl
*
lOO\
OlllOl\
Oll\

,
lOOOl\
llOl\
OlO
*
lO\
O\
Olll\
Ol\
O\
l\
l\

,
lO\
O\
O\
lllOll\
OO
)
;
llllOlOO\
Ol
=
1.0f
;
bool
lO\
OOOlO\
llOOl
=
false
;
fo\
r
(
in\
t\

lOO\
OllOOOl
=
0
;
lO\
OOllOOO\
l\

<
lOOOlllO\
l\
lOl
;
lO\
OOllOO\
Ol
++
)
{
floa\
t
lOOlO\
ll\
OlO\
O\
l
=
lO\
OO\
l\
l\
lOll\
OO\

[
lOO\
Ol\
lOOOl
]
-
lOOlOllOlO\
O\
O
;
if\

(
lOO\
lOllO\
lOO\
l\

<
0.0f
||
lO\
OlOll\
OlO\
Ol
>
llll\
OlO\
OOl
)
contin\
ue
;
Vec3\

lOOOlllOl\
l\
l\
l
=
lllllll\
ll
+
lll\
lO\
O\
OO\
OOO
*
lOOOlllO\
llOO
[
lOOOllOOOl
]
;
if
(
lOOO\
lllOllll\

.
z
<
0.0f
)
conti\
nu\
e
;
Vec3
llOOll\
llO\
O\

=
(
lOOOlllO\
llll
)
*
lOOOOO\
OOl\
l
+
lOO\
lOl\
lOO\
OOl
;
Vec3\

llOll\
OOlOO\

=
lOOOlllO\
lll\
l
;
llOll\
OO\
lOO
.
z
=
0.0f
;
float
lO\
l\
ll\
lOOll
=
llOl\
lO\
Ol\
OO
.
GetM\
agnitude
(
)
;
if
(
lOl\
lll\
OOll
<=
lOOl\
Oll\
OOO\
ll
)
cont\
in\
ue
;
llOllO\
Ol\
OO
*=
lO\
OlO\
l\
lOOOl\
l
/
lOllllOO\
ll
;
Vec3
lOO\
Ol\
OO\
OO\
O
=
lOOOlllOllll
-
ll\
Ol\
lOO\
lOO\

;
lO\
l\
lllO\
Oll\

=
lO\
OOlOOOO\
O\

.
GetMa\
g\
nitude
(
)
;
TA\
_ASS\
E\
RT
(
lOll\
l\
lOOl\
l\

>
lOllOlOlO\
l
)
;
lOOOlOO\
OOO
*=
lOOOO\
OOO\
l\
l
;
lOOOlOOO\
O\
O
/=
lOllllOOll
;
llllOlOOOl
=
lOOl\
O\
ll\
O\
l\
OOl
;
lOl\
l\
l\
lOOOl
=
llO\
O\
ll\
llOO
;
lOllllO\
Ol\
O\

=
lO\
O\
O\
lOOO\
OO\

;
lOOOOl\
O\
llOOl
=
tru\
e
;
}
re\
tur\
n
lO\
OOO\
lOllOOl
;
}
}