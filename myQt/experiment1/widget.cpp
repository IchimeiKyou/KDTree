#include "widget.h"
#include "QVBoxLayout"  // 排版的垂直类
#include "QGridLayout"  // 排版的网格类

Widget::Widget(QWidget *parent): QWidget(parent)        // 初始化图形界面
{
    /* 清空graph文件夹 */
    system("del C:\\Users\\<username>\\Desktop\\myQt\\experiment1\\graph /q *.*");

    picNum = 0, currentNum = 0; // 初始化图像编号

    this->setWindowState(Qt::WindowMaximized);      // 设置屏幕全屏

    /*最左侧图像*/
    picLabel = new QLabel();
    picLabel->setScaledContents(false);
    picLabel->setAlignment(Qt::AlignCenter);
    //picLabel->setPixmap(QPixmap("C:\\Users\\<username>\\Desktop\\graph\\graph0.png"));
    // picLabel->setScaledContents(true);
    picLabel->setFixedSize(900, 900);       // 标准大小(375, 905)

    QFont font; font.setPointSize(16);      // 设置字体

    /*初始化标签及宽高度*/
    chooseKLabel = new QLabel("データ次元選択");
    chooseKLabel->setFixedHeight(60); chooseKLabel->setFixedWidth(200); chooseKLabel->setAlignment(Qt::AlignCenter); chooseKLabel->setFont(font);
    findLabel = new QLabel("各次元の値");
    findLabel->setFixedHeight(60); findLabel->setFixedWidth(200); findLabel->setAlignment(Qt::AlignCenter); findLabel->setFont(font);
    insertLabel = new QLabel("各次元の値");
    insertLabel->setFixedHeight(60); insertLabel->setFixedWidth(200); insertLabel->setAlignment(Qt::AlignCenter); insertLabel->setFont(font);
    initializeLabel = new QLabel("ファイルを読み込んでk-d木初期化（txt）");
    initializeLabel->setFixedHeight(60); initializeLabel->setFixedWidth(600); initializeLabel->setAlignment(Qt::AlignCenter); initializeLabel->setFont(font);
    eraseLabel = new QLabel("各次元の値");
    eraseLabel->setFixedHeight(60); eraseLabel->setFixedWidth(200); eraseLabel->setAlignment(Qt::AlignCenter); eraseLabel->setFont(font);
    searchLabel1 = new QLabel("各次元の下限");
    searchLabel1->setFixedHeight(60); searchLabel1->setFixedWidth(200); searchLabel1->setAlignment(Qt::AlignCenter); searchLabel1->setFont(font);
    searchLabel2 = new QLabel("各次元の上限");
    searchLabel2->setFixedHeight(60); searchLabel2->setFixedWidth(200); searchLabel2->setAlignment(Qt::AlignCenter); searchLabel2->setFont(font);

    /*初始化文本框及宽高度*/
    chooseKLine = new QLineEdit();
    chooseKLine->setFixedHeight(60); chooseKLine->setFixedWidth(400); chooseKLine->setAlignment(Qt::AlignCenter); chooseKLine->setFont(font);
    findLine = new QLineEdit();
    findLine->setFixedHeight(60); findLine->setFixedWidth(400); findLine->setAlignment(Qt::AlignCenter); findLine->setFont(font);
    insertLine = new QLineEdit();
    insertLine->setFixedHeight(60); insertLine->setFixedWidth(400); insertLine->setAlignment(Qt::AlignCenter); insertLine->setFont(font);
    eraseLine = new QLineEdit();
    eraseLine->setFixedHeight(60); eraseLine->setFixedWidth(400); eraseLine->setAlignment(Qt::AlignCenter); eraseLine->setFont(font);
    searchLine1 = new QLineEdit();
    searchLine1->setFixedHeight(60); searchLine1->setFixedWidth(400); searchLine1->setAlignment(Qt::AlignCenter); searchLine1->setFont(font);
    searchLine2 = new QLineEdit();
    searchLine2->setFixedHeight(60); searchLine2->setFixedWidth(400); searchLine2->setAlignment(Qt::AlignCenter); searchLine2->setFont(font);

    /*初始化按钮*/
    chooseKButton = new QPushButton("決定");       // 选定维度
    chooseKButton->setFixedHeight(60); chooseKButton->setFixedWidth(200); chooseKButton->setFont(font);
    findButton = new QPushButton("検索");          // 查询按钮
    findButton->setFixedHeight(60); findButton->setFixedWidth(200); findButton->setFont(font);
    insertButton = new QPushButton("挿入");        // 插入按钮
    insertButton->setFixedHeight(60); insertButton->setFixedWidth(200); insertButton->setFont(font);
    initializeButton = new QPushButton("初期化");  // 初始化按钮
    initializeButton->setFixedHeight(60); initializeButton->setFixedWidth(200); initializeButton->setFont(font);
    eraseButton = new QPushButton("削除");         // 删除按钮
    eraseButton->setFixedHeight(60); eraseButton->setFixedWidth(200); eraseButton->setFont(font);
    searchButton = new QPushButton("探索");        // 查询按钮
    searchButton->setFixedHeight(60); searchButton->setFixedWidth(200); searchButton->setFont(font);
    leftButton = new QPushButton("<<");          // 向左按钮
    leftButton->setFixedHeight(60); leftButton->setFixedWidth(200); leftButton->setFont(font);
    rightButton = new QPushButton(">>");         // 向右按钮
    rightButton->setFixedHeight(60); rightButton->setFixedWidth(200); rightButton->setFont(font);
    clearButton = new QPushButton("全削除");         // 清空按钮
    clearButton->setFixedHeight(60); clearButton->setFixedWidth(200); clearButton->setFont(font);
    quitButton = new QPushButton("終了");          // 退出按钮
    quitButton->setFixedHeight(60); quitButton->setFixedWidth(200); quitButton->setFont(font);


    /*创建各层布局*/
    QGridLayout *behindBackground = new QGridLayout;       // 设置最底层网格布局
    QGridLayout *optionBackground = new QGridLayout;       // 设置各功能板布局
    QGridLayout *chooseKBackground = new QGridLayout;      // 选定维度功能板
    QGridLayout *findBackground = new QGridLayout;         // 查询功能板
    QGridLayout *insertBackground = new QGridLayout;       // 插入功能板
    QGridLayout *initializeBackground = new QGridLayout;   // 初始化功能板
    QGridLayout *eraseBackground = new QGridLayout;        // 删除功能板
    QGridLayout *searchBackground = new QGridLayout;       // 搜索功能板
    QGridLayout *toolBackground = new QGridLayout;         // 刷新/撤回/退出功能板

    behindBackground->addLayout(optionBackground, 0, 1);   // 操作的网络布局层

    /*配置组件*/
    behindBackground->addWidget(picLabel, 0, 0);           // 将图片放入最底层网格布局(第0行, 第0列)
    optionBackground->addLayout(chooseKBackground, 0, 0);
    optionBackground->addLayout(findBackground, 1, 0);
    optionBackground->addLayout(insertBackground, 2, 0);
    optionBackground->addLayout(initializeBackground, 3, 0);
    optionBackground->addLayout(eraseBackground, 4, 0);
    optionBackground->addLayout(searchBackground, 5, 0);
    optionBackground->addLayout(toolBackground, 6, 0);

    chooseKBackground->addWidget(chooseKLabel, 0, 0);
    chooseKBackground->addWidget(chooseKLine, 0, 1);
    chooseKBackground->addWidget(chooseKButton, 0, 4);

    findBackground->addWidget(findLabel, 0, 0);
    findBackground->addWidget(findLine, 0, 1);                // 查询文本
    findBackground->addWidget(findButton, 0, 4);               // 查询按钮(第0行, 第1列)

    insertBackground->addWidget(insertLabel, 0, 0);
    insertBackground->addWidget(insertLine, 0, 1);            // 插入文本
    insertBackground->addWidget(insertButton, 0, 4);           // 插入按钮

    initializeBackground->addWidget(initializeLabel, 0, 0);
    initializeBackground->addWidget(initializeButton, 0, 1);   // 初始化按钮

    eraseBackground->addWidget(eraseLabel, 0, 0);
    eraseBackground->addWidget(eraseLine, 0, 1);              // 删除文本
    eraseBackground->addWidget(eraseButton, 0, 4);             // 删除按钮

    searchBackground->addWidget(searchLabel1, 0, 0);           // "第一维度下限"
    searchBackground->addWidget(searchLine1, 0, 1);            // 查询-第一维度下限
    searchBackground->addWidget(searchLabel2, 1, 0);           // "第二维度上限"
    searchBackground->addWidget(searchLine2, 1, 1);            // 查询-第一维度上限
    searchBackground->addWidget(searchButton, 2, 3);           // 查询按钮

    toolBackground->addWidget(leftButton, 0, 0);
    toolBackground->addWidget(rightButton, 0, 1);
    toolBackground->addWidget(clearButton, 0, 2);
    toolBackground->addWidget(quitButton, 0, 3);

    /*设置确认按钮*/
    setLayout(behindBackground);
    connect(chooseKButton, SIGNAL(clicked()), this, SLOT(choose()));
    connect(findButton, SIGNAL(clicked()), this, SLOT(find()));         // 点击"查询"查询数据并更新图像
    connect(insertButton, SIGNAL(clicked()), this, SLOT(insert()));     // 点击"插入"插入数据并更新图像
    connect(initializeButton, SIGNAL(clicked()), this, SLOT(initialize()));// 点击"初始化"初始化
    connect(eraseButton, SIGNAL(clicked()), this, SLOT(erase()));       // 点击"删除"删除数据并更细图像
    connect(searchButton, SIGNAL(clicked()), this, SLOT(search()));     // 点击"查询"进行范围查询
    connect(leftButton, SIGNAL(clicked()), this, SLOT(left()));         // 点击"撤回"回退程序
    connect(rightButton, SIGNAL(clicked()), this, SLOT(right()));       // 点击"恢复"恢复程序
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clear()));         // 点击"清空"清空2-d树
    connect(quitButton, SIGNAL(clicked()), this, SLOT(quit()));         // 点击"退出"退出程序
}

Widget::~Widget()
{

}
