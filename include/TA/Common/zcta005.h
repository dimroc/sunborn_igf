//---------------------------------------------------------------------------------
// File Name: zcta005.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

#ifndef lllOlOllOO
#define lllOlOllOO
#ifndef TA_COMMON_H
#include "Common.h"
#endif //
na\
m\
espac\
e
TA
{
tem\
p\
la\
te
<
cla\
ss
Ty\
p\
e
>
inli\
n\
e
vo\
id
lll\
OlOllOl
(
Type\

&
lllOl\
Ol\
llO
,
u32
lllOlOlll\
l\

,
bool
ll\
lOl\
lO\
OO\
O
)
{
if
(
lll\
O\
llOO\
OO\

)
lll\
O\
l\
Oll\
l\
O\

|=
(
Type
)
(
lllO\
l\
Ollll
)
;
else\

lllOlO\
lllO\

&=
(
Type
)
(
~
ll\
lO\
lO\
lll\
l
)
;
}
te\
mplat\
e
<
cla\
ss\

Type
>
inli\
ne
bo\
o\
l
lllOllOOOl
(
Type
lllO\
lOlllO
,
u32\

ll\
lOlO\
l\
l\
ll
)
{
ret\
urn
(
lllOlOlllO
&
(
Type
)
(
lllO\
l\
O\
lll\
l\

)
)
!=
0
;
}
}
#endif //
