//---------------------------------------------------------------------------------
// File Name: zcta016.cpp
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

#ifndef TA_RANDFUNC_H
#include "RandFunc.h"
#endif //
na\
mespace\

TA
{
stati\
c\

const
u8
lO\
O\
llOO\
lOl\
O\

[
256
]
=
{
217
,
199
,
71
,
164
,
219
,
144
,
55
,
177
,
91
,
1
,
179
,
85
,
53
,
115
,
156
,
102
,
56
,
248
,
245
,
4
,
62
,
158
,
239
,
247
,
146
,
159
,
55
,
203
,
232
,
205
,
111
,
104
,
104
,
190
,
118
,
81
,
207
,
24
,
135
,
137
,
53
,
22
,
227
,
63
,
110
,
211
,
101
,
155
,
236
,
50
,
74
,
145
,
199
,
188
,
178
,
212
,
213
,
90
,
98
,
253
,
88
,
6
,
102
,
37
,
91
,
185
,
149
,
234
,
255
,
209
,
118
,
58
,
13
,
242
,
219
,
5
,
58
,
238
,
116
,
204
,
115
,
145
,
203
,
56
,
148
,
146
,
148
,
40
,
191
,
16
,
238
,
159
,
141
,
57
,
156
,
224
,
22
,
237
,
248
,
5
,
36
,
46
,
116
,
55
,
211
,
99
,
166
,
49
,
5
,
143
,
55
,
29
,
168
,
146
,
119
,
223
,
226
,
126
,
165
,
17
,
151
,
185
,
168
,
247
,
29
,
98
,
144
,
142
,
178
,
239
,
2
,
237
,
48
,
135
,
55
,
144
,
158
,
11
,
137
,
48
,
82
,
238
,
97
,
13
,
238
,
175
,
54
,
98
,
218
,
88
,
21
,
34
,
16
,
167
,
156
,
162
,
195
,
103
,
81
,
164
,
11
,
29
,
134
,
239
,
251
,
36
,
128
,
52
,
226
,
199
,
191
,
8
,
139
,
139
,
173
,
123
,
63
,
207
,
47
,
212
,
87
,
190
,
47
,
89
,
248
,
157
,
161
,
195
,
157
,
141
,
13
,
180
,
102
,
141
,
68
,
133
,
183
,
101
,
184
,
204
,
165
,
8
,
194
,
101
,
229
,
49
,
161
,
39
,
134
,
146
,
194
,
215
,
20
,
30
,
172
,
87
,
166
,
57
,
59
,
72
,
39
,
225
,
145
,
197
,
176
,
191
,
107
,
143
,
200
,
47
,
172
,
70
,
37
,
142
,
255
,
74
,
8
,
80
,
2
,
3
,
212
,
155
,
48
,
236
,
71
,
208
,
117
,
55
,
244
,
250
,
233
,
108
,
248
,
99
,
27
,
165
,
}
;
sta\
ti\
c
u32
lOOl\
lO\
OlOl\
l
=
0
;
st\
ati\
c
u32
lOOllOOllOO
=
0
;
vo\
id
TA\
C_CALL
Ran\
dFunc
::
TaRand\
Seed\

(
u32
lOO\
llOO\
llOl
)
{
lOOllO\
OlO\
l\
l
=
lO\
OllO\
OllOl
;
lOOllOO\
llOO
=
lOO\
ll\
OOllOl
;
}
u3\
2
TAC_\
C\
AL\
L
RandFun\
c
::
Ta\
R\
andInt\

(
)
{
u32\

lOO\
ll\
OOl\
ll\
O
=
(
(
u32
*
)
lOOll\
OOlOlO\

)
[
lOOl\
lOOl\
Oll\

&
63
]
+
lO\
O\
llOOlOll
+
(
(
u32
*
)
lO\
Oll\
OOlOlO
)
[
lOOllOOllOO\

&
63
]
+
lOOllOOllO\
O
;
lOOllOOlOll
+=
(
2
*
3
*
5
*
7
*
11
*
13
*
17
*
19
*
21
+
1
)
;
lOOll\
O\
Ol\
lOO
+=
(
2
*
3
*
5
*
7
*
11
*
13
+
1
)
;
retu\
rn
lOOll\
O\
OlllO
;
}
}
