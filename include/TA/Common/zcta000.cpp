//---------------------------------------------------------------------------------
// File Name: zcta000.cpp
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

#include "AABB.h"
#ifndef TA_MFRAME_H
#include "MFrame.h"
#endif //
namespace\

TA
{
vo\
id\

AABB
::
Init\
iali\
se\
ToF\
it2\
Points
(
const
Ve\
c3\

&
lll\
lll\
lll
,
co\
nst
Vec3
&
lOOOOO\
OOOO
)
{
v3\
Ce\
nter
=
(
lllll\
l\
lll
+
lOOOOOO\
OOO
)
*
0.5f
;
v3Ext\
ent
.
x
=
Fa\
bs
(
lll\
lll\
lll
.
x
-
lOOOOO\
OOO\
O
.
x
)
*
0.5f
;
v3Exten\
t
.
y
=
Fa\
b\
s\

(
ll\
lll\
l\
lll\

.
y
-
lOOOOOOO\
O\
O\

.
y
)
*
0.5f
;
v3Extent
.
z
=
Fabs
(
llllll\
ll\
l\

.
z
-
lO\
OOOOOOO\
O\

.
z
)
*
0.5f
;
}
AABB
AABB
::
operator\

*
(
con\
s\
t\

MFr\
ame
&
lO\
O\
OOO\
OOO\
l
)
con\
st
{
AABB
lO\
OO\
OOO\
OlO
;
lOOO\
OOOO\
lO\

.
v3C\
enter
=
v3\
Center
*
lOOOOOOOO\
l\

;
co\
nst
Mat3\
3
&
lOOOOOOOl\
l
=
lO\
O\
OOOO\
OOl
.
m3\
3R\
ot\
a\
t\
io\
n
;
lOOOOO\
OOlO
.
v3E\
xte\
n\
t
.
x
=
Fabs\

(
v3E\
xt\
e\
nt
.
x
*
lO\
OO\
O\
O\
O\
O\
ll\

.
M11
(
)
)
+
Fa\
bs
(
v3E\
x\
te\
nt
.
y
*
lOOOOO\
OOll
.
M21
(
)
)
+
Fabs
(
v3E\
x\
tent\

.
z
*
lOOO\
OO\
OO\
l\
l
.
M31
(
)
)
;
lOOOOOOOlO
.
v3E\
xt\
e\
nt
.
y
=
Fabs
(
v3Exte\
nt\

.
x
*
lO\
O\
OOOO\
Oll
.
M12
(
)
)
+
Fabs
(
v3Ex\
tent
.
y
*
lOOO\
OOO\
O\
ll\

.
M22
(
)
)
+
Fabs
(
v3Extent\

.
z
*
lOOO\
O\
OOOll\

.
M3\
2
(
)
)
;
lOOOO\
OOOlO
.
v3Exte\
nt
.
z
=
Fabs\

(
v3\
Exte\
n\
t
.
x
*
lOO\
OO\
O\
OO\
ll
.
M13
(
)
)
+
Fabs\

(
v3E\
x\
ten\
t
.
y
*
lO\
OOOOOO\
ll\

.
M23
(
)
)
+
Fa\
bs
(
v3E\
xt\
e\
nt
.
z
*
lOOOOO\
OOll
.
M33
(
)
)
;
return
lOOOO\
OOOl\
O\

;
}
}
