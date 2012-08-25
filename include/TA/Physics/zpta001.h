//---------------------------------------------------------------------------------
// File Name: zpta001.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

#ifndef llOOlOlllll
#define llOOlOlllll
#ifndef TA_VECTOR_H
#include "../Common/Vector.h"
#endif //
na\
mes\
pace
TA
{
co\
nst
in\
t\

lO\
l\
llO\
O\
lO\
O\
l\

=
31
;
cons\
t
float\

llOOll\
O\
OOOO\

=
0.01f
;
exte\
rn\

fl\
o\
a\
t
llOOl\
lOO\
OOl\

;
name\
space
llOOl\
l\
OOO\
lO
{
inli\
n\
e
bo\
ol
IsZero
(
cons\
t
Vec3\

&
ll\
l\
lllll\
l\

)
{
ret\
u\
rn
lll\
l\
ll\
lll
.
Ge\
tMag\
ni\
t\
ude\

(
)
<
0.01f
*
0.01f
;
}
in\
li\
n\
e\

bool
llOO\
l\
lOOO\
ll\

(
const
Vec3
&
lll\
ll\
l\
l\
l\
l\

,
const
Vec3
&
lO\
OOO\
O\
OOOO
)
{
return\

IsZero\

(
lOOOO\
O\
OOOO
-
ll\
lll\
ll\
ll
)
;
}
}
}
#endif //
