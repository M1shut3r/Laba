#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<math.h>
#include "map.h"
#include<iostream>
using namespace sf;
using namespace std;


int main()
{
    RenderWindow window(VideoMode(1280  , 800), "SFML works!");
    double  x = 300.0, y = 300.0, speed = 0.0, xx, yy, tempX = 0, tempY = 0 , saveX, saveY ,y1 ,x1;
    int A = 0;
    float distance = 0;
    //-------------------
    int PROVERKA = 0;
    //-------------------

    // Музыка (эфекты)
    Music music;
    music.openFromFile("music2.ogg");
    music.setVolume(10.f);
    music.play();
    

    Image ball, background;
    // Подгружаем нашу текстуру из файла ball11.png
    ball.loadFromFile("ball11.png");
    background.loadFromFile("background.png");

    Texture T_ball, T_background;
    T_ball.loadFromImage(ball);
    T_background.loadFromImage(background);

    // Включаем режим сглаживания для нашей текстуры
    T_ball.setSmooth(true);

    // Создаем спрайт и устанавливаем ему нашу текстуру
    Sprite S_ball, S_background;
    S_ball.setTexture(T_ball);
    S_background.setTexture(T_background);
    S_ball.setPosition(x, y);

    S_ball.setPosition(x, y);
    window.display();


    while (window.isOpen())
    {

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        //проверка на нажатие лкм
        if (Mouse::isButtonPressed(Mouse::Left) && speed <= 0) {
            speed=30.0;
            tempX = Mouse::getPosition(window).x;
            tempY = Mouse::getPosition(window).y;
            //--------------------
            saveX = x;
            saveY = y;
            //----------------------
        }

        //сдвиг позиции мяча по (x,y) с уменьшением скорости
        if (speed >= 0.0) {
            speed = speed - 0.1;
            if (PROVERKA == 0) {
                distance = sqrt((tempX - x) * (tempX - x) + (tempY - y) * (tempY - y));//считаем дистанцию (длину от точки А до точки Б). формула длины вектора

                if (distance > 2) {//этим условием убираем дергание во время конечной позиции спрайта

                    x += 0.1 * speed * (tempX - x) / distance;//идем по иксу с помощью вектора нормали
                    y += 0.1 * speed * (tempY - y) / distance;//идем по игреку так же
                }
            }
     //--------------------------------------------------
                for (int i = (int)y / 32; i < ((int)y + 32) / 32; i++)//проходимся по тайликам, контактирующим с игроком,, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
                    for (int j = (int)x / 32; j < ((int)x + 32) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
                    {
                        if (TileMap[i+1][j] == 's' || TileMap[i][j] == 's') { //если символ равен 's' (камень)
                            PROVERKA = 1;
                         
                            x1 = x;//заменяем начальные координаты x,y
                            y1 = y;

                            distance = sqrt((tempX * 2 - x1) * (tempX * 2 - x1) + (saveY - y1) * (saveY - y1));//считаем дистанцию (длину от точки А до точки Б). формула длины вектора

                            if (distance > 2) {//этим условием убираем дергание во время конечной позиции спрайта

                                x += 0.1 * speed * (tempX * 2 - x1) / distance;//идем по иксу с помощью вектора нормали
                                y += 0.1 * speed * (saveY - y1) / distance;//идем по игреку так же
                            }
                            cout << "Y";
                            cout << i <<" " << j << " " <<endl;
                        }
                        //-------------------------------- -



                        else if(TileMap[i][j+1] == 's' || TileMap[i][j-1] == 's'){ //если символ равен 's' (камень)
                            PROVERKA = 2;

                            x1 = x;//заменяем начальные координаты x,y
                            y1 = y;

                            distance = sqrt((saveX - x1) * (saveX - x1) + (tempY * 2 - y1) * (tempY * 2 - y1));//считаем дистанцию (длину от точки А до точки Б). формула длины вектора

                            if (distance > 2) {//этим условием убираем дергание во время конечной позиции спрайта

                                x += 0.1 * speed * (saveX - x1) / distance;//идем по иксу с помощью вектора нормали
                                y += 0.1 * speed * (tempY * 2 - y1) / distance;//идем по игреку так же
                            }
                            cout << "X";
                            cout << i << " " << j << " " << endl;
                        }

 
                        //------------------------------------ 

                    }
                
                if (PROVERKA == 1 && speed > 0) {
                    distance = sqrt((tempX - x) * (tempX - x) + (saveY - y) * (saveY - y));

                    x += 0.1 * speed * (tempX - x) / distance;//идем по иксу с помощью вектора нормали
                    y += 0.1 * speed * (saveY - y) / distance;//идем по игрику так же

                }

                else if (PROVERKA == 1 && speed <= 0) PROVERKA = 0;
                //--------------------------------


                if (PROVERKA == 2 && speed > 0) {
                    distance = sqrt((saveX - x) * (saveX - x) + (tempY - y) * (tempY - y));

                    x += 0.1 * speed * (saveX - x) / distance;//идем по иксу с помощью вектора нормали
                    y += 0.1 * speed * (tempY - y) / distance;//идем по игрику так же

                }
                else if (PROVERKA == 2 && speed <= 0) PROVERKA = 0;

               
     //--------------------------------------------------

        }
        window.clear();

        for (int i = 0; i < HEIGHT_MAP; i++)
            for (int j = 0; j < WIDTH_MAP; j++)
            {
                if (TileMap[i][j] == ' ')  S_background.setTextureRect(IntRect(0, 0, 32, 32)); //если встретили символ пробел, то рисуем 1й квадратик
                if (TileMap[i][j] == 's')  S_background.setTextureRect(IntRect(32, 0, 32, 32));//если встретили символ s, то рисуем 2й квадратик
                if ((TileMap[i][j] == '0')) S_background.setTextureRect(IntRect(64, 0, 32, 32));//если встретили символ 0, то рисуем 3й квадратик

              

                S_background.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат

                window.draw(S_background);//рисуем квадратики на экран
               
            }

                S_ball.setPosition(x, y);
                window.draw(S_ball);
                window.display();
            }
    return 0;
    }
