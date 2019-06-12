#include <stdio.h>
#include <stdlib.h> //for cmd : system("CLS")
#include <conio.h> //for getch(); 1_at the end 2_AI move
#include <windows.h> //for setColor (downloaded from internet)
#include <time.h> // for random seed for some AI actions

#define MAX 500 //for storing data in arrays cell and map
#define P2 1 //blue
#define P1 4 //red
#define EMPTY 7 //white
#define BORDER 8 //gray
#define UI 14 //yellow

void changeXY(int, int*, int*); //change ball position in every move
void print(int, int, int, int, int, int, int); //print the map based on map array
int reaction(int); // every move makes a line passed, this line is mutual to both cells (this function is to store cell.edge mutual)
int checkWin(int, int, int); //if the move results in winning
int mapWidth(int); //change from cell[] basis to map[] basis for X
int mapHeight(int); //change from cell[] basis to map[] basis for Y
int checkCell(int ,int, int, int); //
void linePassed(int, int, int, int, int, int); //makes a line colored based on move for map data
void setColor(int); //set color for printing
void initializeMap(int, int, int, int); //sets first cells data
int numpad(int); //when numpad mode is on, change numpad keys to normal command number
int aiMove(int, int, int, int, int); //move AI based on cell[][] and where ball is and width and height and its goal side

struct prop
{
    int flag; //has the ball been on this cell before?
    int edge[8]; //has the 0-8 side move been passed before?
};
struct prop cell[MAX][MAX]; //storing cell data for calculating
int map[MAX][MAX]; //storing map data for printing
/*
map guide numbers :
0 : empty
1 : p1 line
2 : p2 line
*/



int main() {
    srand(time(NULL));
    int width, height, i, j, numpad_mode, ai_mode, ai_turn, ai_eq_p1Turn = 0, clearHis, useRec;
    int x, y; // current x and y
    setColor(UI);
    printf("Map Width: ");
    scanf("%d",&width);
    while(width < 3 || width % 2 == 0)
    {
        system("CLS");
        printf("Enter an odd number!\nMap Width: ");
        scanf("%d",&width);
    }
    printf("Map Height: ");
    scanf("%d",&height);
    while(height < 3 || height % 2 == 0)
    {
        system("CLS");
        printf("Enter an odd number!\nMap Height: ");
        scanf("%d",&height);
    }
    system("CLS");
    //use recommended setting question
    printf("Select an option\n1) Use recommended settings\n2) Manually choose options\n");
    scanf("%d",&useRec);
    while(useRec != 1 && useRec != 2)
    {
        system("CLS");
        printf("Wrong number!\nSelect an option\n1) Use recommended settings\n2) Manually choose options\n");
        scanf("%d",&useRec);
    }
    useRec--;
    system("CLS");
    if(!useRec) //useRec works vice versa :)
    {
        numpad_mode = 1;
        ai_mode = 1;
        ai_turn = 0;
        ai_eq_p1Turn = 0;
        clearHis = 1;
    }
    else
    {
        printf("Which set of keys do you want to use?\n1) 1-8 keys\n2) Numpad keys (1-9 except 5) (Recommended)\n");
        scanf("%d",&numpad_mode);
        while(numpad_mode != 1 && numpad_mode != 2)
        {
            system("CLS");
            printf("Wrong number!\nWhich set of keys do you want to use?\n1) 1-8 keys\n2) Numpad keys (1-9 except 5) (Recommended)\n");
            scanf("%d",&numpad_mode);
        }
        numpad_mode--;
        system("CLS");
        printf("Select an option:\n1)Play with Human\n2)Play with Computer\n");
        scanf("%d",&ai_mode);
        while(ai_mode != 1 && ai_mode != 2)
        {
            system("CLS");
            printf("Wrong number!\nSelect an option:\n1)Play with Human\n2)Play with Computer\n");
            scanf("%d",&ai_mode);
        }
        ai_mode--;

        if(ai_mode)
        {
            system("CLS");
            printf("Select an option:\n1)Play as Player 1\n2)Play as Player 2\n");
            scanf("%d",&ai_turn);
            while(ai_turn != 1 && ai_turn != 2)
            {
                system("CLS");
                printf("Wrong number!\nSelect an option:\n1)Play as Player 1\n2)Play as Player 2\n");
                scanf("%d",&ai_turn);
            }
            ai_turn = ai_turn == 1 ? 0 : 1;
            ai_eq_p1Turn = ai_turn;
        }
        system("CLS");
        printf("Do you want to record match history?\n1) Yes\n2) No (Recommended)\n");
        scanf("%d",&clearHis);
        while(clearHis != 1 && clearHis != 2)
        {
            system("CLS");
            printf("Wrong number!\nDo you want to record match history?\n1) Yes\n2) No (Recommended)\n");
            scanf("%d",&clearHis);
        }
        clearHis--;
        system("CLS");
    }
    setColor(EMPTY);


    //initializing map
    x = width / 2;
    y = height / 2;
    initializeMap(width, height, x, y);
    //game starts:
    int p1Turn = 1, mov_err = 0;
    while(1)
    {
        int er_access = 0;
        int n, win = 0;

        print(x, y, 0, width, height, p1Turn, clearHis);
        printf("\n");
        if(mov_err)
        {
            setColor(UI);
            printf("Can't move there!\n");
            setColor(EMPTY);
            mov_err = 0;
        }
        //if all sides are blocked, round will go for the person in turn!
        int draw = 0;
        for(i = 0; i < 8; i++)
        {
            if(cell[x][y].edge[i])
                draw++;
        }
        if(draw == 8)
        {
            win =  p1Turn ? 2 : 1;
            print(x, y, 0, width, height, p1Turn, clearHis);
            setColor(UI);
            printf("Nowhere to move! Player ");
            win == 1 ? setColor(P1) : setColor(P2);
            printf("%d ", win);
            setColor(UI);
            printf("won!\n");
            setColor(EMPTY);
            break;
        }
        printf("Player ");
        p1Turn? setColor(P1) : setColor(P2);
        printf("%d ", p1Turn? 1 : 2);
        setColor(EMPTY);
        printf("turn\n", p1Turn? 1 : 2);

        //input :
        if(ai_mode && ai_turn)
        {
            n = aiMove(x,y,width,height,ai_eq_p1Turn);
            printf("Press Enter to see computer next move...\n");
            getchar();
        }
        else if(numpad_mode == 0)
        {
            printf("Enter a number between 1 and 8(0 to exit): ");
            scanf("%d",&n);
            while(n < 0 || n > 8){
                printf("Wrong input! Enter a number between 1 and 8(0 to exit): ");
                scanf("%d",&n);
            }
        }
        else
        {
            printf("Enter a number between 1 and 9(0 to exit)(and not 5): ");
            scanf("%d",&n);
            while(n < 0 || n > 9 || n == 5){
                printf("Wrong input! Enter a number between 1 and 8(0 to exit)(and not 5): ");
                scanf("%d",&n);
            }
            n = numpad(n);
        }

        //calculating move:
        if(n == 0)
            break;
        n--;
        if(!cell[x][y].edge[n])
        {
            //check if ball will go into a goal
            win = checkWin(n, y, height);
            if(win != 0)
            {
                print(x, y, win, width, height, p1Turn, clearHis);
                setColor(UI);
                printf("Player ");
                win == 1 ? setColor(P1) : setColor(P2);
                printf("%d ", win == 1 ? 1 : 2);
                setColor(UI);
                printf("Won!\n", win == 1 ? 1 : 2);
                setColor(EMPTY);
                break;
            }
            cell[x][y].edge[n] = 1;
            int oldX = x, oldY = y;
            //increasing or decreasing of x and y based on n
            changeXY(n,&x,&y);
            linePassed(mapWidth(oldX),mapHeight(oldY),mapWidth(x),mapHeight(y),p1Turn,n);
            cell[x][y].edge[reaction(n)] = 1;
            if(cell[x][y].flag)
                p1Turn = !p1Turn;
            else
                cell[x][y].flag = 1;
            p1Turn = !p1Turn;
            if(ai_eq_p1Turn)
                ai_turn = p1Turn;
            else
                ai_turn = !p1Turn;
        }
        else
            mov_err = 1;
    }
    printf("See you next time!\nPress any key to quit...\n");
    getch();
    return 0;
}

int reaction(int n)
{
    return (n + 4) % 8;
}

void changeXY(int n, int *x, int *y)
{
    if(n == 6 || n == 7 || n == 0)
        (*x)--;
    if(n >= 2 && n <=4)
        (*x)++;
    if(n <= 2)
        (*y)--;
    if(n >= 4 && n <=6)
        (*y)++;
}
int checkWin(int n, int y, int height)
{
    if(n <= 2 && y == 0)
        return 1;
    if(n >= 4 && n <= 6 && y == height - 1)
        return 2;
    return 0;
}

int numpad(int n)
{
    switch(n)
    {
    case 1:
        return 7;
        break;
    case 2:
        return 6;
        break;
    case 3:
        return 5;
        break;
    case 4:
        return 8;
        break;
    case 6:
        return 4;
        break;
    case 7:
        return 1;
        break;
    case 8:
        return 2;
        break;
    case 9:
        return 3;
        break;
    default:
        return 0;
        break;
    }
    return 0;
}

void linePassed(int x1, int y1, int x2, int y2, int p1Turn, int n)
{
    if (!p1Turn)
        p1Turn = 2;
    int i;
    if (x1 == x2)
    {
        if(y1>y2){
            int tmp = y2;
            y2 = y1;
            y1 = tmp;
        }
        for(i = 1; i < y2 - y1; i++)
        {
            map[x1][y1 + i] = p1Turn;
        }
    }
    else if (y1 == y2)
    {
        if(x1>x2){
            int tmp = x2;
            x2 = x1;
            x1 = tmp;
        }
        for(i = 1; i < x2 - x1; i++)
        {
            map[x1 + i][y1] = p1Turn;
        }
    }
    else
    {
        if(n == 0 || n == 6)
        {
            int tmp = x2;
            x2 = x1;
            x1 = tmp;

            tmp = y2;
            y2 = y1;
            y1 = tmp;
        }

        if(n == 0 || n == 4)
        {
            map[x1 + 2][y1 + 1] = p1Turn;
            map[x1 + 4][y1 + 2] = p1Turn;
            map[x1 + 6][y1 + 3] = p1Turn;
        }

        if(n == 2 || n == 6)
        {
            map[x1 + 1][y1 - 1] = p1Turn;
            map[x1 + 3][y1 - 2] = p1Turn;
            map[x1 + 5][y1 - 3] = p1Turn;
        }

    }
}

int mapWidth(int width)
{
    //index based
    return width * 7;
}
int mapHeight(int height)
{
    //index based
    return height * 4;
}

int checkCell(int x,int y,int width,int height)
{
    int i, j;
    for(j = 0; j < height; j++)
    {
        for(i = 0; i < width; i++)
        {
            if(mapWidth(i) == x && mapHeight(j) == y)
                return 1;
        }
    }
    return 0;


}

void initializeMap(int width, int height, int x, int y)
{
    int i, j;
    cell[x][y].flag = 1;
    for(i = 0; i < width; i++)
    {
        if(i != width / 2)
        {
            cell[i][0].flag = 1;//top_flag
            cell[i][height-1].flag = 1;//bottom_flag
        }
        for(j = 0; j < 4; j++) //top
            cell[i][0].edge[j] = 1;
        cell[i][0].edge[7] = 1;
        for(j = 3; j < 8; j++) //bottom
            cell[i][height - 1].edge[j] = 1;
    }
    for(i = 0; i < height; i++)
    {
        cell[0][i].flag = 1;//left_flag
        cell[width-1][i].flag = 1;//right_flag
        for(j = 6; j < 8; j++) //left
            cell[0][i].edge[j] = 1;
        cell[0][i].edge[1] = 1;
        cell[0][i].edge[5] = 1;
        cell[0][i].edge[0] = 1;
        for(j = 2; j < 5; j++) //right
            cell[width - 1][i].edge[j] = 1;
        cell[width - 1][i].edge[1] = 1;
        cell[width - 1][i].edge[5] = 1;
    }
    //top goal
    cell[x][0].edge[1] = 0;
    cell[x][0].edge[3] = 0;
    cell[x][0].edge[7] = 0;

    cell[x + 1][0].edge[0] = 0;
    cell[x + 1][0].edge[7] = 0;

    cell[x - 1][0].edge[2] = 0;
    cell[x - 1][0].edge[3] = 0;

    //bottom goal
    cell[x][height - 1].edge[5] = 0;
    cell[x][height - 1].edge[3] = 0;
    cell[x][height - 1].edge[7] = 0;

    cell[x + 1][height - 1].edge[6] = 0;
    cell[x + 1][height - 1].edge[7] = 0;

    cell[x - 1][height - 1].edge[4] = 0;
    cell[x - 1][height - 1].edge[3] = 0;
}

void print(int x, int y, int win, int width, int height, int p1Turn, int clearHis)
{
/*
    system("CLS");
    int i, row, col;
    //top goal
    for(i = 0; i < width/2 - 1; i++)
        printf(" ");
    if(win == 1)
        printf("+*+");
    else
        printf("+++");
    printf("\n");

    //main map
    for(row = 0; row < height; row++)
    {
        for(col = 0; col < width; col++)
        {
            if(col == x && row == y && win == 0)
                printf("*");
            else
                printf("+");
        }
        printf("\n");
    }

    //bottom goal
    for(i = 0; i < width/2 - 1; i++)
        printf(" ");
    if(win == 2)
        printf("+*+");
    else
        printf("+++");
    printf("\n");
*/

    //new function (with color and line)
    if(clearHis)
        system("CLS");
    int map_width = (width - 1) * 7 + 1;
    int map_height = (height - 1) * 4 + 1;
    int i, j;

    //top goal
    for(i = 0; i < mapWidth(width / 2 - 1); i++)
    {
        printf(" ");
    }
    //setColor(EMPTY); // not needed
    printf("*");
    setColor(P2);
    for(i = 0; i < mapWidth(1) - 1; i++)
    {
        printf("*");
    }
    if(win == 1)
        printf("o");
    else
        printf("2");
    for(i = 0; i < mapWidth(1) - 1; i++)
    {
        printf("*");
    }
    setColor(EMPTY);
    printf("*\n");
    setColor(BORDER);
    for(i = 0; i < mapHeight(1) - 1; i++)
    {
        for(j = 0; j < mapWidth(width / 2 - 1); j++)
        {
            printf(" ");
        }
        printf("*");
        for(j = 0; j < mapWidth(2) - 1; j++)
        {
            printf(" ");
        }
        printf("*\n");
    }

    //main map
    for(j = 0; j < map_height; j++)
    {
        for(i = 0; i < map_width; i++)
        {


            if( (i == 0 || i == map_width - 1 || j == 0 || j == map_height - 1)) //if side
            {
                if( (j == 0 || j == map_height - 1) && i > mapWidth(width / 2 - 1) && i < mapWidth(width / 2 + 1)) //if in front of goal
                {
                    if(i == map_width / 2) // if middle cell in front of goal
                    {
                        if(i == mapWidth(x) && j == mapHeight(y)) //if ball on it
                        {
                            if(p1Turn)
                            {
                                setColor(P1);
                                if(win != 0)
                                    printf("*");
                                else
                                    printf("o");
                            }
                            else
                            {
                                setColor(P2);
                                if(win != 0)
                                    printf("*");
                                else
                                    printf("o");
                            }
                        }
                        else
                        {
                            setColor(EMPTY);
                            printf("*");
                        }
                    }
                    else
                        switch(map[i][j])
                        {
                        case 0:
                            setColor(EMPTY);
                            printf(" ");
                            break;
                        case 1:
                            setColor(P1);
                            printf("*");
                            break;
                        case 2:
                            setColor(P2);
                            printf("*");
                            break;
                        }
                }
                else if(i == mapWidth(x) && j == mapHeight(y)) //if ball on it
                {
                    if(p1Turn)
                    {
                        setColor(P1);
                        if(win != 0)
                            printf("*");
                        else
                            printf("o");
            }
                    else
                    {
                        setColor(P2);
                        if(win != 0)
                            printf("*");
                        else
                            printf("o");
                    }
                }
                else
                {
                    if(checkCell(i,j,width,height))
                    {
                        setColor(EMPTY);
                        printf("*"); //border cell
                    }
                    else
                    {
                        setColor(BORDER);
                        printf("*"); // border line
                    }
                }
            }
            else //if not side :
                switch(map[i][j])
                {
                case 0:
                    if(i == mapWidth(x) && j == mapHeight(y)) //if ball on it
                    {
                        if(p1Turn)
                        {
                            setColor(P1);
                            if(win != 0)
                            printf("*");
                        else
                            printf("o");
                        }
                        else
                        {
                            setColor(P2);
                            if(win != 0)
                            printf("*");
                        else
                            printf("o");
                        }
                    }
                    else
                    {
                        if(checkCell(i,j,width,height))
                        {
                        setColor(EMPTY);
                        printf("*"); //middle cell
                        }
                        else
                        {
                        setColor(EMPTY);
                        printf(" "); //middle empty
                        }
                    }
                    break;
                case 1:
                    setColor(P1);
                    printf("*");
                    break;
                case 2:
                    setColor(P2);
                    printf("*");
                    break;
                }


        }
        printf("\n");
    }

    //bottom goal
    setColor(BORDER);
    for(i = 0; i < mapHeight(1) - 1; i++)
    {
        for(j = 0; j < mapWidth(width / 2 - 1); j++)
        {
            printf(" ");
        }
        printf("*");
        for(j = 0; j < mapWidth(2) - 1; j++)
        {
            printf(" ");
        }
        printf("*\n");
    }
    for(i = 0; i < mapWidth(width / 2 - 1); i++)
    {
        printf(" ");
    }
    setColor(EMPTY);
    printf("*");
    setColor(P1);
    for(i = 0; i < mapWidth(1) - 1; i++)
    {
        printf("*");
    }
    if(win == 2)
        printf("o");
    else
        printf("1");
    for(i = 0; i < mapWidth(1) - 1; i++)
    {
        printf("*");
    }
    setColor(EMPTY);
    printf("*\n");

    //end
    setColor(EMPTY);
}

void setColor(int ForgC)
 {
     WORD wColor;

      HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
      CONSOLE_SCREEN_BUFFER_INFO csbi;

     //We use csbi for the wAttributes word.
     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
          //Mask out all but the background attribute, and add in the forgournd color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
 }

int aiMove(int x, int y, int width, int height, int goalSide)
{
    /*
    Pointing system (weight) :
     1p: going horizontally toward center (comparing x and width/2)
     2p: goalSide == 1 ? go up to goal : go down to goal;
     3p: getting extra move (destination cell.flag) => based on goal side
    -99p: not moving to corner if possible (instant losing) (by checking x,y and width, height)
    -99p: not moving to a place where he will be blocked (by checking its cell.edge considering reaction to the move that will be taken

    how to be smarter?
    moving away from his own goal when ball is near it (not only the closest situation)
    Normal difficulty idea: do this for the next move to (copy cells with a change)
    Hard difficulty idea: do this for the next move and 1 level after that move to (copy 2 times from cells with a change)
    */
    //return 1-8
    //if some moves have the same weight, it'll randomly be chosen
    int i;
    int weight[8] = {0};


    //positive points:
    if (x < width / 2)
    {
        weight[2] += 1;
        weight[3] += 1;
        weight[4] += 1;
    }
    else if (x > width / 2)
    {
        weight[6] += 1;
        weight[7] += 1;
        weight[0] += 1;
    }

    if(goalSide == 1)
    {
        weight[0] += 2;
        weight[1] += 2;
        weight[2] += 2;
    }
    else
    {
        weight[4] += 2;
        weight[5] += 2;
        weight[6] += 2;
    }
    if(cell[x - 1][y - 1].flag)
        weight[0] += 3;
    if(cell[x][y - 1].flag)
        weight[1] += 3;
    if(cell[x + 1][y - 1].flag)
        weight[2] += 3;
    if(cell[x + 1][y].flag)
        weight[3] += 3;
    if(cell[x + 1][y + 1].flag)
        weight[4] += 3;
    if(cell[x][y + 1].flag)
        weight[5] += 3;
    if(cell[x - 1][y + 1].flag)
        weight[6] += 3;
    if(cell[x - 1][y].flag)
        weight[7] += 3;


    //negative points:
    if(x == 1 && y == 1)
        weight[0] -= 99;
    if(x == width - 2 && y == 1)
        weight[2] -= 99;
    if(x == 1 && y == height - 2)
        weight[6] -= 99;
    if(x == width - 2 && y == height - 2)
        weight[4] -= 99;

    if (x == width / 2)
    {
        weight[0] -= 1;
        weight[6] -= 1;
        weight[7] -= 1;

        weight[2] -= 1;
        weight[3] -= 1;
        weight[4] -= 1;
    }
    if(goalSide == 0)
    {
        weight[0] -= 2;
        weight[1] -= 2;
        weight[2] -= 2;
    }
    else
    {
        weight[4] -= 2;
        weight[5] -= 2;
        weight[6] -= 2;
    }
    //check if a way will result in being blocked and losing

    for(i = 0; i < 8; i++)
    {
        int j, block = 0, offsetX = 0, offsetY = 0;
        if(i == 0 || i == 6 || i == 7)
            offsetX = -1;
        else if(i == 2 || i == 3 || i == 4)
            offsetX = 1;

        if(i == 0 || i == 1 || i == 2)
            offsetY = -1;
        else if(i == 4 || i == 5 || i == 6)
            offsetY = 1;

        if( (y == 0 || y == height - 1) && x >= width / 2 - 1 && x <= width / 2 + 1) //if he can goal
        {
            //making sure he goals / don't own goal (considering his goal side)
            //not necessary now because pointing system does his work, but:
            if(y == 0)
            {
                if(goalSide == 1)
                {
                    //up to goal
                    weight[0] += 99;
                    weight[1] += 99;
                    weight[2] += 99;
                }
                else
                {
                    weight[0] -= 99;
                    weight[1] -= 99;
                    weight[2] -= 99;
                }
            }
            else
            {
                if(goalSide == 1)
                {
                    weight[4] -= 99;
                    weight[5] -= 99;
                    weight[6] -= 99;
                }
                else
                {
                    weight[4] += 99;
                    weight[5] += 99;
                    weight[6] += 99;
                }
            }
        }
        else if(x + offsetX < 0 || x + offsetX > width - 1 || y + offsetY < 0 || y + offsetY > height - 1) // if corner
            block = 8;
        else
        {
            for(j = 0; j < 8; j++)
            {
                if(cell[x + offsetX][y + offsetY].edge[j])
                    block++;
            }
        }

        if(block >= 7) //7 = if only one way is remaining (no return path)
            weight[i] -= 99;
    }


    //if moving somewhere is impossible
    for(i = 0; i < 8; i++)
    {
        if(cell[x][y].edge[i])
            weight[i] = -1000;
    }

    //end of pointing
    int max_index[8], counter = 1, max = weight[0];
    max_index[0] = 0;
    for(i = 1; i < 8; i++)
    {
        if(weight[i] > max)
        {
            max = weight[i];
            max_index[0] = i;
            counter = 1;
        }
        else if (weight[i] == max)
        {
            max_index[counter] = i;
            counter++;
        }
    }
    /*
    //uncomment for developing purposes only:
    printf("Random between: ");
    for(i = 0; i < counter; i++){
        printf("%d ", max_index[i]);
    }
    printf("\n");
    //end of developing code.
    */
    int rnd = rand() % counter;
    return max_index[rnd] + 1;
}
