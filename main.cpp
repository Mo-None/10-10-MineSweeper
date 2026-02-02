#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<easyx.h>
#include<graphics.h>
#define row 10
#define col 10  //定义行列的常量
#define MineNum 10 //定义雷的数量
#define ImgSize 25 //图片的长宽尺寸

IMAGE imgs[15];
void loadResource()
{
    for (int i = 0; i < 15;i++)
    {
        char imgPath[15] = { 0 };
        sprintf(imgPath,"./image/%d.png",i);
        loadimage(&imgs[i],imgPath,ImgSize,ImgSize);
    }
}

void show(int map[][col])
{
    for (int i = 0; i < row; i++)
    {
        for (int k = 0; k < col; k++)
        {
            printf("%2d ",map[i][k]);
        }
        printf("\n");
    }
}
void init(int map[][col])
{
    loadResource();

    //设置随机数种子
    srand(time(NULL));

    //把map全部初始化为0
    memset(map,0, sizeof(int)*row*col);

    //随机设置雷 用-1表示
    for (int i = 0 ; i < MineNum;)
    {
        int r = rand() % row;
        int c = rand() % col;
        if (map[r][c] == 0)
        {
            map[r][c] = -1;

            i++;
        }
    }

    for (int i = 0; i < row; i++)
    {
        for (int k = 0; k < col; k++)
        {
            if (map[i][k]==-1)
            {
                for (int x = i - 1;x <= i+1 ;x++)
                {
                    for (int y = k-1; y <= k+1 ;y++)
                    {
                        if ((x > -1 && x < row && y > -1 && y <col)&&map[x][y]!=-1)
                        {
                            ++map[x][y];
                        }
                    }
                }
            }
        }
    }

    //加密格子
    for (int i = 0; i < row; i++)
    {
        for (int k = 0; k < col; k++)
        {
            map[i][k]+=10;
        }
    }
}//初始化数据
void draw(int map[][col])
{
    //贴图
    for (int i = 0; i < row; i++)
    {
        for (int k = 0; k < col; k++)
        {
            if (map[i][k]>=0&&map[i][k]<=8)
            {
                putimage(k*ImgSize,i*ImgSize,&imgs[map[i][k]]);
            }
            else if (map[i][k] == -1)
            {
                putimage(k*ImgSize,i*ImgSize,&imgs[13]);
            }
            else if (map[i][k] >= 9 && map[i][k]<=18)
            {
                putimage(k*ImgSize,i*ImgSize,&imgs[9]);
            }
            else if (map[i][k] >= 19 && map[i][k]<=28)
            {
                putimage(k*ImgSize,i*ImgSize,&imgs[10]);
            }
            else if (map[i][k] >= 29 && map[i][k]<=38)
            {
                putimage(k*ImgSize,i*ImgSize,&imgs[12]);
            }
        }
    }
}//绘制地图
void boomBlank(int map[][col],int R,int C)
{
    if (map[R][C] == 0)
    {
        for (int x = R - 1;x <= R+1 ;x++)
        {
            for (int y = C-1; y <= C+1 ;y++)
            {
                if ((x > -1 && x < row && y > -1 && y <col)&&map[x][y]>=9&&map[x][y]<=18)
                {
                    map[x][y]-=10;
                    boomBlank(map,x,y);
                }
            }
        }
    }
}//点击空白格子，连锁爆开空白格子和数字
void mouseMsg(ExMessage* msg, int map[][col])
{
    int c = msg->x / ImgSize;
    int r = msg->y / ImgSize;
    //左键打开格子
    if (msg->message == WM_LBUTTONDOWN)
    {
        if (map[r][c]>=9 && map[r][c]<=18)
        {
            map[r][c] -= 10;
            boomBlank(map,r,c);
        }
    }
    else if (msg->message == WM_RBUTTONDOWN)
    {
        if (map[r][c]>=9 && map[r][c]<=18)
        {
            map[r][c] += 10;
        }
        else if (map[r][c]>=19 && map[r][c]<=28)
        {
            map[r][c] += 10;
        }
        else
        {
            map[r][c] -= 20;
        }
    }
}//鼠标操作数据
int judge(int map[][col],int R ,int C)
{
    //雷炸了
    if (map[R][C] == -1)
    {
        return -1;
    }

    int cnt = 0 ;
    for (int i = 0 ; i < row; i++)
    {
        for (int k = 0 ; k < col ; k++)
            if (map[i][k]>=0&&map[i][k]<=8)
            {
                ++cnt;
            }
    }
    if (row*col - MineNum ==cnt)
    {
        return 1;
    }

    return 0;
}//游戏结束判断

int main()
{
    //创造窗口
    initgraph(ImgSize*row,ImgSize*col);

    int J = 0;
    int map[row][col] = {0};
    //扫雷地图变量命名
    init(map);
    draw(map);

    //游戏主循环
    while (J==0)
    {
        ExMessage msg{};
        while (peekmessage(&msg,EM_MOUSE))
        {
            switch (msg.message)
            {
                case WM_LBUTTONDOWN:
                case WM_RBUTTONDOWN:
                    mouseMsg(&msg,map);
                    draw(map);
                    J=judge(map,msg.y/ImgSize,msg.x/ImgSize);
                    if (J == -1)
                    {
                        int select = MessageBox(GetHWnd(),"你这么牛，怎么输了捏？敢再来一把吗？","low B!",MB_OKCANCEL);
                        if (select == IDOK)
                        {
                            init(map);
                        }
                        else
                        {
                            exit(0);
                        }
                    }
                    else if (J == 1)
                    {
                        int select = MessageBox(GetHWnd(),"你太牛了，还想再来一局吗？","NB!",MB_OKCANCEL);
                        if (select == IDOK)
                        {
                            init(map);
                        }
                        else
                        {
                            exit(0);
                        }
                    }
                    break;
            }
        }
    }
    getchar();

    return 0;
}