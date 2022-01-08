// gierka2.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <vector>


int EXIT = 0;
int clamp(int input, int max, int min);
class Entity; //mówimy dla kompilatora że będzie na pewno klasa Entity niech sie nie martwi

std::vector <Entity*> spis; //spis instancji klasy tutaj bo już w konstruktorze klasy korzystamy z niego
void DrawPoints();

class Entity
{

private:
    COORD c;
    short playable = 0;
    
public:
    static int counter;

    Entity* parent = nullptr;
    std::vector <Entity*> childrens;

    short& X = c.X; //dla przejrzystości referencje
    short& Y = c.Y;
    unsigned char Znak = '#';
    void SetCharacter(char znak)
    {
        if (Znak != '@')
            Znak = znak;
    }
    void SetPlayable(int isPlayable)
    {
        
        playable = isPlayable;
    }
    void SetParent(Entity* prt)
    {
        parent = prt;
        parent->childrens.push_back(this);
        counter++;

    }
    Entity(short x, short y)
    {
        X = x;
        Y = y;
        spis.push_back(this);
    }
    Entity(short x, short y, char znak)
    {
        X = x;
        Y = y;
        Znak = znak;
        spis.push_back(this); //do spisu instancji leci pozdro
    }
    void SetPos(int x, int y)
    {
        c.X = x;
        c.Y = y;
    }
    void Move(int x, int y)
    {
        if (childrens.size() > 0 && y != 0)
            SetCharacter('|');
        else
            SetCharacter('-');

        if (childrens.size() > 0)
        {
            int oldX = X;
            int oldY = Y;
            int newX = oldX;
            int newY = oldY;

            for (size_t i = 0; i < childrens.size(); i++)
            {
                 oldX = childrens[i]->X;
                 oldY = childrens[i]->Y;
                 childrens[i]->X = newX;
                 childrens[i]->Y = newY;

                 
                 if (childrens[i]->Y != oldY)
                     childrens[i]->SetCharacter('|');
                 else
                     childrens[i]->SetCharacter('-');

                 newX = oldX;
                 newY = oldY;


            }

        }
        X = clamp(x+X,119,0);
        Y = clamp(y + Y, 29, 0);
    }
    void RandomMove()
    {
        if (parent != nullptr)
            return;

        int direction = rand() % 2; //0 w x się poruszamy a 1 to y
        int moveAmount = rand() % 2+1;
        int backward = rand() % 2; //jak 0 to do tyłu a jak 1 to przód
        if (!backward)
            moveAmount *= -1;

        if (direction)
            Move(0, moveAmount);
        else
            Move(moveAmount, 0);
    }
    void DrawEntity()
    {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
        std::cout << Znak;
    }

    static void ClearBoard()
    {
        system("cls");
        DrawPoints();
    }
    static void MoveNonPlayable()
    {
        for (size_t i = 0; i < spis.size(); i++)
        {
            if (!spis[i]->playable) 
            {
                spis[i]->RandomMove();
                spis[i]->DrawEntity();
                
            }
        }
    }
    static void DrawNonPlayable()
    {
        for (size_t i = 0; i < spis.size(); i++)
        {
            if (!spis[i]->playable)
                spis[i]->DrawEntity();
        }
    }
    static void CheckAllCollisions()
    {
        //system("cls");
        size_t size = spis.size();
        for (size_t i = 0; i <= (size/2); i++) 
        {
            //std::cout << ":" << i << ":\n\n";
            for (size_t p = i+1; p < size; p++) 
            {
                //std::cout << i << ':' << p<<std::endl;
                if (spis[p]->parent != nullptr) continue;

                if (spis[p]->X == spis[i]->X && spis[p]->Y == spis[i]->Y && spis[i]->playable)
                {
                    spis[p]->SetParent(spis[i]);
                    spis[p]->Znak = '-'; //to oznakowanie, że kolizja z kimś innym już nastąpiła

                    int randomX = rand() % 119 + 1;
                    int randomY = rand() % 29 + 1;
                    new Entity(randomX, randomY, '@'); 
                }
            }

        }
        //Sleep(3000); 
    }
};

void DrawPoints()
{
    COORD pos;
    pos.X = 0;
    pos.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);


    std::cout << "Zlapanych:" << Entity::counter;
}
int clamp(int input, int max, int min)
{
    if (input > max)
        input = max;
    else if (input < min)
        input = min;
    return input;
}
int Entity::counter = 0;
int main()
{
    
    srand(time(NULL));
    Entity* pionek = new Entity(2, 2, '|');
    pionek->SetPlayable(1);

    Entity* drugaMysz = new Entity(10, 10, '@');
    Entity* mysz =  new Entity(50, 50,'@');
    new Entity(60, 50, '@');
    
    int key = '0';
    while (!EXIT)
    {
        if (_kbhit())
            key = _getch();

        switch (key)
        {
            
            case (27):
                EXIT = 1;
                system("CLS");
                std::cout << "Dziekuje za gre uguem";
                Sleep(1500);
                break;
            case (119): 
                pionek->Move(0, -1);
                break;
            case (97):
                pionek->Move(-1, 0);
                break;
            case (115):
                pionek->Move(0, 1);
                break;
            case (100):
                pionek->Move(1, 0);
                break;
              
        }
        if (key != ' ') //aktualizuje plansze po kliku
        {
            Entity::ClearBoard();
            Entity::DrawNonPlayable();
            pionek->DrawEntity(); //jako ostatnie żeby podkreślenie było
            Entity::CheckAllCollisions();
        }
        else
        {
            Sleep(150);
            Entity::ClearBoard();
            Entity::MoveNonPlayable();
            pionek->DrawEntity(); //jako ostatnie żeby podkreślenie było
            Entity::CheckAllCollisions();
        }
        key = ' ';//"zeruje" bufor
        
    }
}


//tablica 