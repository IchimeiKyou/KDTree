To use this system, Qt (v5.9.0) and graphviz(v3.0.0) are necessary.<br>
You may download graphviz via https://graphviz.org/download/ and Qt via https://download.qt.io/archive/qt/ 

```code
KDTree/
├── myQt/
     ├── build-experiment1-Desktop_Qt_5_9_0_MinGW_32bit-Debug/
     └── experiment1/
            ├── main.cpp    // Program entry, initializes Qt application and shows main window
            ├── tree.h      // k-d tree data structure and algorithms (insert, delete, search, visualize)
            ├── widget.h    // Qt main window class declaration, UI components and slots
            ├── widget.cpp  // Implementation of main window logic and interaction with Tree
            ├── experiment1.pro
            └── graph/       // .dot and .png files of KD-Tree
```
## 用例
- 初期化3次元のK-D木
![初期化](./Image/初期化.png "初期化3次元のK-D木")
- 検索(20, 24, 56)
![検索](./Image/検索.png "検索(20, 24, 56)")
- 挿入(3, 20, 25)
![挿入](./Image/挿入.png "挿入(3, 20, 25)")
- 削除(20, 24, 56)
![削除](./Image/削除.png "削除(20, 24, 56)")
- 上限下限検索(10, 10, 10) ~ (30, 30, 30)
![上限下限検索](./Image/上限下限検索.png "上限下限検索(10, 10, 10) ~ (30, 30, 30)")
