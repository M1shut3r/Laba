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
    double  x = 1120.0, y = 704.0, speed = 0.0, xx=0.0, yy=0.0, tempX, tempY = 0 , saveX, saveY, sum_frame=0;
    int WIN=0,sum=0,chek_Y=0,chek_X=0, musicoff=0, chekmusic=0, NumMusic=1;  //WIN - переключение сцен , sum - проверка на то, что эта сторона X во избежания багов (хотя они есть), chek_Y - если мячик летит больше 6 кадров сам, то он может опять удариться по оси Y
    float distance = 0;
    bool lazer_on_off = true, chekswamp = false;
    //-------------------
    int PROVERKA = 0;
    //-------------------

    // Музыка (эфекты)
    Music music;
    music.openFromFile("Around_The_World.ogg");
    music.setVolume(50.f);
    music.play();
    
    Image ball, background, music_icon, next_button;
    // Подгружаем нашу текстуру из файла ball11.png
    ball.loadFromFile("ball11.png");
    background.loadFromFile("background.png");
    music_icon.loadFromFile("music_icon.png");
    next_button.loadFromFile("playbutton.png");

    Texture T_ball, T_background, T_music_icon, T_next_button;
    T_ball.loadFromImage(ball);
    T_background.loadFromImage(background);
    T_music_icon.loadFromImage(music_icon);
    T_next_button.loadFromImage(next_button);

    // Включаем режим сглаживания для нашей текстуры
    T_ball.setSmooth(true);

    // Создаем спрайт и устанавливаем ему нашу текстуру
    Sprite S_ball, S_background, S_music_icon, S_next_button;
    S_ball.setTexture(T_ball);
    S_background.setTexture(T_background);
    S_music_icon.setTexture(T_music_icon);
    S_next_button.setTexture(T_next_button);


    S_music_icon.setPosition(64, 0);
    S_next_button.setPosition(96, 0);

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

        //включает и выключает лазер
        sum_frame++;
        if (sum_frame == 500) lazer_on_off = false;
        else if (sum_frame == 1000) { lazer_on_off = true; sum_frame = 0; }

        //аключение и выключение звука
        if (IntRect(64, 0, 32, 32).contains(Mouse::getPosition(window)) && Mouse::isButtonPressed(Mouse::Left) && chekmusic==100) {
            if (musicoff == 0) { S_music_icon.setColor(Color::Red); musicoff = 1; chekmusic = 0; music.setVolume(0.f); }
            else if (musicoff == 1) { S_music_icon.setColor(Color::White); musicoff = 0; chekmusic = 0; music.setVolume(50.f); }
        }

        //смена музыки           СДЕЛАТЬ , ТУТ БАГ
        if (IntRect(96, 0, 32, 32).contains(Mouse::getPosition(window)) && Mouse::isButtonPressed(Mouse::Left) && chekmusic == 100) {
            chekmusic = 0;
            switch (NumMusic) {
            case 1: music.openFromFile("Hyderabad.ogg"); music.play(); NumMusic++; break;
            case 2: music.openFromFile("music2.ogg"); music.play(); NumMusic++; break;
            case 3: music.openFromFile("marshmello.ogg"); music.play(); NumMusic++; break;


            default: NumMusic = 1; music.openFromFile("Around_The_World.ogg"); music.play(); break;
            }
        }
        
        if (chekmusic != 100) chekmusic++; // делаю задержку в 100 кадров для нажатия на кнопку выключения музыки, чтобы она не баговала 

        //проверка на нажатие лкм
        else  if (Mouse::isButtonPressed(Mouse::Left) && speed <= 0) {
            speed=30.0;
            tempX = Mouse::getPosition(window).x;
            tempY = Mouse::getPosition(window).y;
            //--------------------
            saveX = x;
            saveY = y;
            //----------------------
            distance = sqrt((tempX - x) * (tempX - x) + (tempY - y) * (tempY - y));//считаем дистанцию (длину от точки А до точки Б). формула длины вектора
            xx += 0.1 * speed * (tempX - x) / distance;//идем по иксу с помощью вектора нормали
            yy += 0.1 * speed * (tempY - y) / distance;//идем по игреку так же
        }
        
        //сдвиг позиции мяча по (x,y) с уменьшением скорости
        if(speed > 0){
        if (PROVERKA == 0) {
            x = x + xx;
            y = y + yy;
        }
     //--------------------------------------------------
        if (WIN == 0) {
            for (int i = (int)y / 32; i < ((int)y + 32) / 32; i++) {//проходимся по тайликам, контактирующим с игроком,, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
                
                for (int j = (int)x / 32; j < ((int)x + 32) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
                {

                    if ((TileMap[i + 1][j] == 's' || TileMap[i][j] == 's'  || TileMap[i + 1][j] == '0' || TileMap[i][j] == '0') && sum!=1) { //если символ равен 's' (камень) или '0' (стена)
                        PROVERKA = 1;



                        x += xx;//идем по иксу с помощью вектора нормали
                        y += -yy;//идем по игреку так же

                        chek_X++;
                        sum = 1;
                            
                       

                        cout << endl <<"hit Y poz :";
                        cout << "X:" << i << " Y:" << j << " " << "xBoll:" << x << " yBoll:" << y << " " << " | chek right side " << sum <<"| ";
                    }
                    //-------------------------------- -



                    else if (TileMap[i][j + 1] == 's' || TileMap[i][j] == 's'   || TileMap[i][j + 1] == '0' || TileMap[i][j] == '0') { //если символ равен 's' (камень) или '0' (стена)
                        PROVERKA = 2;

                        if (sum == 1) { //
                            x += -xx*2;
                            y += yy*2;

                        }




                        x += -xx;//идем по иксу с помощью вектора нормали
                        y += yy;//идем по игреку так же

                        cout << "hit X poz :";
                        cout << "X:" << i << " Y:" << j <<" " << "xBoll:" << x << " yBoll:" << y << " " << " | chek left side " << sum << "| ";
                    }


                    //------------------------------------ 

                    if (TileMap[i + 1][j] == 'w' || TileMap[i][j] == 'w' || TileMap[i][j + 1] == 'w') {
                        cout << "You WIN";
                        WIN = 1; //меняет карту на другую
                        speed = 0; //останавливаем мяч
                        x = 160.0;//возвращаем на позицию начала нового ур-я
                        y = 96.0;
                        xx = 0;//убираем направление прошлого шарика 
                        yy = 0;
                    }

                    //-------------------------------------

                    if ((TileMap[i + 1][j] == 'k' || TileMap[i][j] == 'k' || TileMap[i][j + 1] == 'k') && lazer_on_off == true || (TileMap[i + 1][j] == '#' || TileMap[i][j] == '#' || TileMap[i][j + 1] == '#' || TileMap[i][j] == '#')) {
                        speed = 0; //останавливаем мяч
                        x = 1120.0;//возвращаем на начальные позиции
                        y = 704.0;
                        xx = 0;//убираем направление прошлого шарика 
                        yy = 0;
                    }

                    //------------------------------------

                    if ((TileMap[i + 1][j] == '@' || TileMap[i][j] == '@' || TileMap[i][j + 1] == '@') && chekswamp == false) {
                        xx = xx / 2;
                        yy = yy / 2;
                        chekswamp = true;
                    }

                    //-----------------------------------

                    if ((TileMap[i + 1][j] == 'z' || TileMap[i][j] == 'z' || TileMap[i][j + 1] == 'z') && chekswamp == true) {
                        xx = xx * 2;
                        yy = yy * 2;
                        chekswamp = false;
                    }

                }
            }
        }

        //-------------------------------------------------------------------------------------------------------------------------------------ПЕРЕХОД НА ВТОРУЮ КАРТУ
        //else
        //    if(WIN==1)
        //        for (int i = (int)y / 32; i < ((int)y + 32) / 32; i++)//проходимся по тайликам, контактирующим с игроком,, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
        //    for (int j = (int)x / 32; j < ((int)x + 32) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
        //    {
        //        if (TileMap_1[i + 1][j] == 's' || TileMap_1[i][j] == 's' || TileMap_1[i + 1][j] == '0' || TileMap_1[i][j] == '0') { //если символ равен 's' (камень)
        //            PROVERKA = 1;

        //            x += xx;//идем по иксу с помощью вектора нормали
        //            y += -yy;//идем по игреку так же

        //            chek_X++;
        //            sum = 1;



        //            cout << "Y";
        //            cout << i << " " << j << " " << sum << endl;
        //        }
        //        //-------------------------------- -



        //        else if (TileMap_1[i][j + 1] == 's' || TileMap_1[i][j - 1] == 's' || TileMap_1[i][j + 1] == '0' || TileMap_1[i][j-1] == '0') { //если символ равен 's' (камень)
        //            PROVERKA = 2;

        //            if (sum == 1) { //
        //                x += -xx * 2;
        //                y += yy * 2;

        //            }




        //            x += -xx;//идем по иксу с помощью вектора нормали
        //            y += yy;//идем по игреку так же

        //            cout << "X";
        //            cout << i << " " << j << " " << sum << endl;
        //        }


        //        //------------------------------------ 
        //        if (TileMap_1[i + 1][j] == 'w' || TileMap_1[i][j] == 'w' || TileMap_1[i][j + 1] == 'w' || TileMap_1[i][j - 1] == 'w') {
        //            cout << "You WIN";
        //            WIN = 1; //меняет карту на другую
        //        }
        //    }
                //----------------------------------------------------------------------------------------------------------------
                if (PROVERKA == 1) {
                    
                    cout << " hit in side Y | "<<endl;
                    //---------
                    xx = xx;
                    yy = -yy;
                    PROVERKA = 0;
                    //---------
                    chek_X++;
                }

                //--------------------------------


                if (PROVERKA == 2) {
                    cout <<" hit in side X | chek_X = "<<chek_X<<"| ";

                    //---------
                    xx = -xx;
                    yy = yy;//идем по игрику так же
                    //---------

                    if (chek_X >= 2) { yy = -yy; cout << "OOO"; }

                    cout << endl;

                    PROVERKA = 0;
                }
               

                chek_Y++; // Проверка по кадрам, для того, чтобы Y мог отталкнуться от Y1 (сделал для того, чтобы понимать, что мячик отскачил от стены , а не находится в ней)
                if (chek_Y == 6) {
                    sum = 0;
                    chek_Y = 0;
                    chek_X = 0;
                }
     //--------------------------------------------------

        }
        window.clear();


        if(WIN==0)
        for (int i = 0; i < HEIGHT_MAP; i++)
            for (int j = 0; j < WIDTH_MAP; j++)
            {
                if (TileMap[i][j] == ' ')  S_background.setTextureRect(IntRect(0, 0, 32, 32)); //если встретили символ пробел, то рисуем 1й квадратик
                if (TileMap[i][j] == 's')  S_background.setTextureRect(IntRect(32, 0, 32, 32));//если встретили символ s, то рисуем 2й квадратик
                if ((TileMap[i][j] == '0')) S_background.setTextureRect(IntRect(64, 0, 32, 32));//если встретили символ 0, то рисуем 3й квадратик
                if ((TileMap[i][j] == 'w')) S_background.setTextureRect(IntRect(96, 0, 32, 32));//если встретили символ w, то рисуем 4й квадратик
                if ((TileMap[i][j] == 'l')) S_background.setTextureRect(IntRect(128, 0, 32, 32));//если встретили символ l, то рисуем 5й квадратик
                if ((TileMap[i][j] == 'k') && lazer_on_off == true) S_background.setTextureRect(IntRect(160, 0, 32, 32));//если встретили символ k, то рисуем 6й квадратик
                if ((TileMap[i][j] == 'k') && lazer_on_off == false) S_background.setTextureRect(IntRect(0, 0, 32, 32));
                if ((TileMap[i][j] == '#')) S_background.setTextureRect(IntRect(192, 0, 32, 32));//если встретили символ #, то рисуем 7й квадратик
                if ((TileMap[i][j] == '@')) S_background.setTextureRect(IntRect(224, 0, 32, 32));//если встретили символ @, то рисуем 8й квадратик
                if ((TileMap[i][j] == 'L')) S_background.setTextureRect(IntRect(256, 0, 32, 32));//если встретили символ L, то рисуем 9й квадратик
                if ((TileMap[i][j] == 'K') && lazer_on_off == false) S_background.setTextureRect(IntRect(288, 0, 32, 32));//если встретили символ K, то рисуем 10й квадратик
                if ((TileMap[i][j] == 'K') && lazer_on_off == true) S_background.setTextureRect(IntRect(0, 0, 32, 32));
                if ((TileMap[i][j] == 'z')) S_background.setTextureRect(IntRect(320, 0, 32, 32));//если встретили символ z, то рисуем 11й квадратик


                S_background.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат

                window.draw(S_background);//рисуем квадратики на экран               

            }
        if (WIN == 1) {
            for (int i = 0; i < HEIGHT_MAP; i++)
                for (int j = 0; j < WIDTH_MAP; j++)
                {
                    if (TileMap_1[i][j] == ' ')  S_background.setTextureRect(IntRect(0, 0, 32, 32)); //если встретили символ пробел, то рисуем 1й квадратик
                    if (TileMap_1[i][j] == 's')  S_background.setTextureRect(IntRect(32, 0, 32, 32));//если встретили символ s, то рисуем 2й квадратик
                    if (TileMap_1[i][j] == '0') S_background.setTextureRect(IntRect(64, 0, 32, 32));//если встретили символ 0, то рисуем 3й квадратик
                    if (TileMap_1[i][j] == 'w') S_background.setTextureRect(IntRect(96, 0, 32, 32));//если встретили символ w, то рисуем 4й квадратик
                    if (TileMap_1[i][j] == 'l') S_background.setTextureRect(IntRect(128, 0, 32, 32));//если встретили символ l, то рисуем 5й квадратик
                    if ((TileMap_1[i][j] == 'k') && lazer_on_off == true) S_background.setTextureRect(IntRect(160, 0, 32, 32));//если встретили символ L, то рисуем 6й квадратик
                    if ((TileMap_1[i][j] == 'k') && lazer_on_off == false) S_background.setTextureRect(IntRect(0, 0, 32, 32));
                    if (TileMap_1[i][j] == '#') S_background.setTextureRect(IntRect(192, 0, 32, 32));//если встретили символ #, то рисуем 7й квадратик
                    if (TileMap_1[i][j] == '@') S_background.setTextureRect(IntRect(224, 0, 32, 32));//если встретили символ #, то рисуем 8й квадратик
                    if (TileMap_1[i][j] == 'L') S_background.setTextureRect(IntRect(256, 0, 32, 32));//если встретили символ l, то рисуем 5й квадратик
                    if ((TileMap_1[i][j] == 'K') && lazer_on_off == false) S_background.setTextureRect(IntRect(288, 0, 32, 32));//если встретили символ L, то рисуем 6й квадратик
                    if ((TileMap_1[i][j] == 'K') && lazer_on_off == true) S_background.setTextureRect(IntRect(0, 0, 32, 32));



                    S_background.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат

                    window.draw(S_background);//рисуем квадратики на экран

                }
        }

                S_ball.setPosition(x, y);
                window.draw(S_music_icon);
                window.draw(S_next_button);
                window.draw(S_ball);
                window.display();
            }
    return 0;
    }
