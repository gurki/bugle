- [1] https://www.w3schools.com/charsets/ref_utf_box.asp

```
//  base
00:34:11.896 [35c7]  main opened ...  #envelope  [main, main.cpp:65]
00:34:11.896 [35c7]  ..hallo  #info #debug  [main, main.cpp:67]
00:34:11.896 [35c7]  ··sync opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.002 [35c7]  ····go deeper  #info  [main::lambda, main.cpp:79]
00:34:12.002 [35c7]  ··sync closed (105 ms)  #envelope  [main::lambda, main.cpp:77]
00:34:12.002 [030e]  async opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.207 [030e]  ··go deeper  #info  [main::lambda, main.cpp:79]
00:34:12.207 [030e]  async closed (204 ms)  #envelope  [main::lambda, main.cpp:77]
00:34:12.315 [35c7]  main closed (312 ms)  #envelope  [main main.cpp:65]

//  light dirtree
00:34:11.896 [35c7]  ├ main opened ...  #envelope  [main, main.cpp:65]
00:34:11.896 [35c7]  │  ├ hallo  #info #debug  [main, main.cpp:67]
00:34:11.896 [35c7]  │  ├ sync opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.002 [35c7]  │  │  ├ go deeper  #info  [main::lambda, main.cpp:79]
00:34:12.002 [35c7]  │  ├ sync closed (105 ms)  #envelope  [main::lambda, main.cpp:77]
00:34:12.002 [030e]  ├ async opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.207 [030e]  │  ├ go deeper  #info  [main::lambda, main.cpp:79]
00:34:12.207 [030e]  ├ async closed (204 ms)  #envelope  [main::lambda, main.cpp:77]
00:34:12.315 [35c7]  ├ main closed (312 ms)  #envelope  [main main.cpp:65]

//  heavy dirtree
00:34:11.896 [35c7]  ┣ main opened ...  #envelope  [main, main.cpp:65]
00:34:11.896 [35c7]  ┃  ┣ hallo  #info #debug  [main, main.cpp:67]
00:34:11.896 [35c7]  ┃  ┣ sync opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.002 [35c7]  ┃  ┃  ┣ go deeper  #info  [main::lambda, main.cpp:79]
00:34:12.002 [35c7]  ┃  ┣ sync closed (105 ms)  #envelope  [main::lambda, main.cpp:77]
00:34:12.002 [030e]  ┣ async opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.207 [030e]  ┃  ┣ go deeper  #info  [main::lambda, main.cpp:79]
00:34:12.207 [030e]  ┣ async closed (204 ms)  #envelope  [main::lambda, main.cpp:77]
00:34:12.315 [35c7]  ┣ main closed (312 ms)  #envelope  [main main.cpp:65]

//  light dirtree dense
00:34:11.896 [35c7]  ├ main opened ...  #envelope  [main, main.cpp:65]
00:34:11.896 [35c7]  │ ├ hallo  #info #debug  [main, main.cpp:67]
00:34:11.896 [35c7]  │ ├ sync opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.002 [35c7]  │ │ ├ go deeper  #info  [main::lambda, main.cpp:79]
00:34:12.002 [35c7]  │ ├ sync closed (105 ms)  #envelope  [main::lambda, main.cpp:77]
00:34:12.002 [030e]  ├ async opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.207 [030e]  │ ├ go deeper  #info  [main::lambda, main.cpp:79]
00:34:12.207 [030e]  ├ async closed (204 ms)  #envelope  [main::lambda, main.cpp:77]
00:34:12.315 [35c7]  ├ main closed (312 ms)  #envelope  [main main.cpp:65]

//  ┬  start thread
00:34:11.896 [35c7]  ┬ main opened ...  #envelope  [main, main.cpp:65]
00:34:11.896 [35c7]  │  ├ hallo  #info #debug  [main, main.cpp:67]
00:34:11.896 [35c7]  │  ├ sync opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.002 [35c7]  │  │  ├ go deeper  #info  [main::lambda, main.cpp:79]
00:34:12.002 [35c7]  │  ├ sync closed (105 ms)  #envelope  [main::lambda, main.cpp:77]
00:34:12.002 [030e]  ┬ async opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.207 [030e]  │  ├ go deeper  #info  [main::lambda, main.cpp:79]
00:34:12.207 [030e]  ├ async closed (204 ms)  #envelope  [main::lambda, main.cpp:77]
00:34:12.315 [35c7]  ┬ main closed (312 ms)  #envelope  [main main.cpp:65]

//  └ close scope
00:34:11.896 [35c7]  ┬ main opened ...  #envelope  [main, main.cpp:65]
00:34:11.896 [35c7]  │  ├ hallo  #info #debug  [main, main.cpp:67]
00:34:11.896 [35c7]  │  ├ sync opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.002 [35c7]  │  │  ├ go deeper  #info  [main::lambda, main.cpp:79]
00:34:12.002 [35c7]  │  └ sync closed (105 ms)  #envelope  [main::lambda, main.cpp:77]
00:34:12.002 [030e]  ┬ async opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.207 [030e]  │  ├ go deeper  #info  [main::lambda, main.cpp:79]
00:34:12.207 [030e]  └ async closed (204 ms)  #envelope  [main::lambda, main.cpp:77]
00:34:12.315 [35c7]  └ main closed (312 ms)  #envelope  [main main.cpp:65]

//  └ close scope
00:34:11.896 [35c7]  ╤ main opened ...  #envelope  [main, main.cpp:65]
00:34:11.896 [35c7]  │  ├ hallo  #info #debug  [main, main.cpp:67]
00:34:11.896 [35c7]  │  ├ sync opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.002 [35c7]  │  │  ├ go deeper  #info  [main::lambda, main.cpp:79]
00:34:12.002 [35c7]  │  └ sync closed (105 ms)  #envelope  [main::lambda, main.cpp:77]
00:34:12.002 [030e]  ╤ async opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.207 [030e]  │  ├ go deeper  #info  [main::lambda, main.cpp:79]
00:34:12.207 [030e]  └ async closed (204 ms)  #envelope  [main::lambda, main.cpp:77]
00:34:12.315 [35c7]  ═ main closed (312 ms)  #envelope  [main main.cpp:65]

//  ┌ start scope (!)
00:34:11.896 [35c7]  ┬ main opened ...  #envelope  [main, main.cpp:65]
00:34:11.896 [35c7]  │  ├ hallo  #info #debug  [main, main.cpp:67]
00:34:11.896 [35c7]  │  ┌ sync opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.002 [35c7]  │  │  ├ go deeper  #info  [main::lambda, main.cpp:79]
00:34:12.002 [35c7]  │  └ sync closed (105 ms)  #envelope  [main::lambda, main.cpp:77]
00:34:12.002 [030e]  ┬ async opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.207 [030e]  │  ├ go deeper  #info  [main::lambda, main.cpp:79]
00:34:12.207 [030e]  └ async closed (204 ms)  #envelope  [main::lambda, main.cpp:77]
00:34:12.315 [35c7]  └ main closed (312 ms)  #envelope  [main main.cpp:65]

//  ─ single line
00:34:11.896 [35c7]  ┬ main opened ...  #envelope  [main, main.cpp:65]
00:34:11.896 [35c7]  │  ─ hallo  #info #debug  [main, main.cpp:67]
00:34:11.896 [35c7]  │  ┌ sync opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.002 [35c7]  │  │  ─ go deeper  #info  [main::lambda, main.cpp:79]
00:34:12.002 [35c7]  │  └ sync closed (105 ms)  #envelope  [main::lambda, main.cpp:77]
00:34:12.002 [030e]  ┬ async opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.207 [030e]  │  ─ go deeper  #info  [main::lambda, main.cpp:79]
00:34:12.207 [030e]  └ async closed (204 ms)  #envelope  [main::lambda, main.cpp:77]
00:34:12.315 [35c7]  └ main closed (312 ms)  #envelope  [main main.cpp:65]

//  no close scope
00:34:11.896 [35c7]  ┬ main opened ...  #envelope  [main, main.cpp:65]
00:34:11.896 [35c7]  │  ├ hallo  #info #debug  [main, main.cpp:67]
00:34:11.896 [35c7]  │  ┌ sync opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.002 [35c7]  │  │  ├ go deeper  #info  [main::lambda, main.cpp:79]
00:34:12.002 [030e]  ┬ async opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.207 [030e]  │  ├ go deeper  #info  [main::lambda, main.cpp:79]

//  ─ single line
00:34:11.896 [35c7]  ┬ main opened ...  #envelope  [main, main.cpp:65]
00:34:11.896 [35c7]  │  ─ hallo  #info #debug  [main, main.cpp:67]
00:34:11.896 [35c7]  │  ┌ sync opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.002 [35c7]  │  │  ─ go deeper  #info  [main::lambda, main.cpp:79]
00:34:12.002 [030e]  ┬ async opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.207 [030e]  │  ─ go deeper  #info  [main::lambda, main.cpp:79]

//  shift
00:34:11.896 [35c7]  main opened ...  #envelope  [main, main.cpp:65]
00:34:11.896 [35c7]   ├ hallo  #info #debug  [main, main.cpp:67]
00:34:11.896 [35c7]   ┌ sync opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.002 [35c7]   │  ├ go deeper  #info  [main::lambda, main.cpp:79]
00:34:12.002 [35c7]   └ sync closed (105 ms)  #envelope  [main::lambda, main.cpp:77]
00:34:12.002 [030e]  async opened ...  #envelope  [main::lambda, main.cpp:77]
00:34:12.207 [030e]   ├ go deeper  #info  [main::lambda, main.cpp:79]
00:34:12.207 [030e]  async closed (204 ms)  #envelope  [main::lambda, main.cpp:77]
00:34:12.315 [35c7]  main closed (312 ms)  #envelope  [main main.cpp:65]
```

```
02:12:47.255 [ace6]  ┌ main opened ...         [ #envelope    ] [ main, main.cpp:66         ]
02:12:47.255 [ace6]  │ ├ hallo                 [ #info #debug ] [ main, main.cpp:68         ]
02:12:47.255 [ace6]  │ ┌ sync opened ...       [ #envelope    ] [ main::lambda, main.cpp:78 ]
02:12:47.368 [ace6]  │ │ ├ go deeper           [ #info        ] [ main::lambda, main.cpp:80 ]
02:12:47.368 [ace6]  │ └ sync closed (112 ms)  [ #envelope    ] [ main::lambda, main.cpp:78 ]
02:12:47.369 [97ef]  ┌ async opened ...        [ #envelope    ] [ main::lambda, main.cpp:78 ]
02:12:47.573 [97ef]  │ ├ go deeper             [ #info        ] [ main::lambda, main.cpp:80 ]
02:12:47.573 [97ef]  └ async closed (204 ms)   [ #envelope    ] [ main::lambda, main.cpp:78 ]
02:12:47.573 [ace6]  └ main closed (317 ms)    [ #envelope    ] [ main, main.cpp:66         ]

02:12:47.255 [ace6]  ┌ main opened ...         [#envelope   ] [main, main.cpp:66        ]
02:12:47.255 [ace6]  │ ├ hallo                 [#info #debug] [main, main.cpp:68        ]
02:12:47.255 [ace6]  │ ┌ sync opened ...       [#envelope   ] [main::lambda, main.cpp:78]
02:12:47.368 [ace6]  │ │ ├ go deeper           [#info       ] [main::lambda, main.cpp:80]
02:12:47.368 [ace6]  │ └ sync closed (112 ms)  [#envelope   ] [main::lambda, main.cpp:78]
02:12:47.369 [97ef]  ┌ async opened ...        [#envelope   ] [main::lambda, main.cpp:78]
02:12:47.573 [97ef]  │ ├ go deeper             [#info       ] [main::lambda, main.cpp:80]
02:12:47.573 [97ef]  └ async closed (204 ms)   [#envelope   ] [main::lambda, main.cpp:78]
02:12:47.573 [ace6]  └ main closed (317 ms)    [#envelope   ] [main, main.cpp:66        ]

02:12:47.255 [ace6]  ┌ main opened ...  [ #envelope ] [ main, main.cpp:66 ]
02:12:47.255 [ace6]  │ ├ hallo  [ #info #debug ] [ main, main.cpp:68 ]
02:12:47.255 [ace6]  │ ┌ sync opened ...  [ #envelope ] [ main::lambda, main.cpp:78 ]
02:12:47.368 [ace6]  │ │ ├ go deeper  [ #info ] [ main::lambda, main.cpp:80 ]
02:12:47.368 [ace6]  │ └ sync closed (112 ms)  [ #envelope ] [ main::lambda, main.cpp:78 ]
02:12:47.369 [97ef]  ┌ async opened ...  [ #envelope ] [ main::lambda, main.cpp:78 ]
02:12:47.573 [97ef]  │ ├ go deeper  [ #info ] [ main::lambda, main.cpp:80 ]
02:12:47.573 [97ef]  └ async closed (204 ms)  [ #envelope ] [ main::lambda, main.cpp:78 ]
02:12:47.573 [ace6]  └ main closed (317 ms)  [ #envelope ] [ main, main.cpp:66 ]

02:12:47.255 [ace6]  ┌ main opened ...  [ #envelope ]           [ main, main.cpp:66 ]
02:12:47.255 [ace6]  │ ├ hallo  [ #info #debug ]                [ main, main.cpp:68 ]
02:12:47.255 [ace6]  │ ┌ sync opened ...  [ #envelope ]         [ main::lambda, main.cpp:78 ]
02:12:47.368 [ace6]  │ │ ├ go deeper  [ #info ]                 [ main::lambda, main.cpp:80 ]
02:12:47.368 [ace6]  │ └ sync closed (112 ms)  [ #envelope ]    [ main::lambda, main.cpp:78 ]
02:12:47.369 [97ef]  ┌ async opened ...  [ #envelope ]          [ main::lambda, main.cpp:78 ]
02:12:47.573 [97ef]  │ ├ go deeper  [ #info ]                   [ main::lambda, main.cpp:80 ]
02:12:47.573 [97ef]  └ async closed (204 ms)  [ #envelope ]     [ main::lambda, main.cpp:78 ]
02:12:47.573 [ace6]  └ main closed (317 ms)  [ #envelope ]      [ main, main.cpp:66 ]

02:12:47.255 [ace6]  ┌ main opened ...  [#envelope]         [main, main.cpp:66]
02:12:47.255 [ace6]  │ ├ hallo  [#info #debug]              [main, main.cpp:68]
02:12:47.255 [ace6]  │ ┌ sync opened ...  [#envelope]       [main::lambda, main.cpp:78]
02:12:47.368 [ace6]  │ │ ├ go deeper  [#info]               [main::lambda, main.cpp:80]
02:12:47.368 [ace6]  │ └ sync closed (112 ms)  [#envelope]  [main::lambda, main.cpp:78]
02:12:47.369 [97ef]  ┌ async opened ...  [#envelope]        [main::lambda, main.cpp:78]
02:12:47.573 [97ef]  │ ├ go deeper  [#info]                 [main::lambda, main.cpp:80]
02:12:47.573 [97ef]  └ async closed (204 ms)  [#envelope]   [main::lambda, main.cpp:78]
02:12:47.573 [ace6]  └ main closed (317 ms)  [#envelope]    [main, main.cpp:66]
```