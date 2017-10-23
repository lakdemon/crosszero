#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>

int main()
{
    sf::IpAddress ip = sf::IpAddress::getLocalAddress();  //Локальный ip Адресс
    sf::TcpSocket socket;//программный интерфейс для обеспечения обмена данными между процессами
    sf::Packet packet;  //Для осуществления пакетной передачи дынных
    std::cout << "Your ip: " << ip << std::endl;
    char mode;
    std::cout << "Enter game mode (s/c): ";
    std::cin >> mode;
    bool myturn;
    bool block = true;
    int full=false;
    float timer=0.0;
    float maxtime;


    if(mode == 's'){
      sf::TcpListener listener;
      std::cout << "Enter max time for game: ";
      std::cin >> maxtime;
      std::cout << "Waiting for player..."<<std::endl;
      listener.listen(2000);
      listener.accept(socket);
      myturn=true;
      packet << maxtime;
      socket.send(packet);
      packet.clear();
      }    //который будет содержать новое соединение 
    
    else if(mode == 'c'){
      std::cout << "ip to connect: ";
      //std::cin >> ip;
      ip = "192.168.0.102";
      socket.connect(ip, 2000);
      myturn=false;
      socket.receive(packet);
      packet.clear();
      packet >> maxtime;
      std::cout << "Max time for game: " << maxtime << std::endl;

      } //ip и Порт



    sf::RenderWindow window(sf::VideoMode(390, 450), "xoxoxo");
    window.setFramerateLimit(60);
    sf::Color   backstageC(0x222831ff);
    sf::Color       basicC(0x393E46ff);
    sf::Color      orangeC(0xf96D00ff);
    sf::Color       whiteC(0xf2f2f2ff);
    sf::Color transparentC(0x00000000);

    std::cout << ip << std::endl;

    int field[9];
    for (int i = 0; i < 9; ++i){
        field[i]=0;}

    sf::CircleShape circle[9];
    sf::RectangleShape rect[9];
    sf::RectangleShape cross[9][2];
    
    for (int i = 0; i < 9; ++i){
        cross[i][0].setSize(sf::Vector2f(50, 5));
        cross[i][0].rotate(45);
        cross[i][1].setSize(sf::Vector2f(50, 5));
        cross[i][1].rotate(-45);

        rect[i].setSize(sf::Vector2f(100, 100));
        rect[i].setFillColor(basicC);
        rect[i].setOutlineThickness(1);
        rect[i].setOutlineColor(basicC);

        circle[i].setRadius(40);
        circle[i].setFillColor(transparentC);
        circle[i].setOutlineThickness(4);
        circle[i].setOutlineColor(whiteC);
    }

     circle[0].setPosition(sf::Vector2f(35,85));
      circle[1].setPosition(sf::Vector2f(155,85));
       circle[2].setPosition(sf::Vector2f(275,85));
        circle[3].setPosition(sf::Vector2f(35,205));
         circle[4].setPosition(sf::Vector2f(155,205));
          circle[5].setPosition(sf::Vector2f(275,205));
           circle[6].setPosition(sf::Vector2f(35,325));
            circle[7].setPosition(sf::Vector2f(155,325));
              circle[8].setPosition(sf::Vector2f(275,325));

     cross[0][0].setPosition(sf::Vector2f(35+25,85+20));
      cross[1][0].setPosition(sf::Vector2f(155+25,85+20));
       cross[2][0].setPosition(sf::Vector2f(275+25,85+20));
        cross[3][0].setPosition(sf::Vector2f(35+25,205+20));
         cross[4][0].setPosition(sf::Vector2f(180,225));
          cross[5][0].setPosition(sf::Vector2f(275+25,205+20));
           cross[6][0].setPosition(sf::Vector2f(35+25,325+20));
            cross[7][0].setPosition(sf::Vector2f(155+25,325+20));
              cross[8][0].setPosition(sf::Vector2f(275+25,325+20));

     cross[0][1].setPosition(sf::Vector2f(35+20,85+55));
      cross[1][1].setPosition(sf::Vector2f(155+20,85+55));
       cross[2][1].setPosition(sf::Vector2f(275+20,85+55));
        cross[3][1].setPosition(sf::Vector2f(35+20,205+55));
         cross[4][1].setPosition(sf::Vector2f(155+20,205+55));
          cross[5][1].setPosition(sf::Vector2f(275+20,205+55));
           cross[6][1].setPosition(sf::Vector2f(35+20,325+55));
            cross[7][1].setPosition(sf::Vector2f(155+20,325+55));
              cross[8][1].setPosition(sf::Vector2f(275+20,325+55));

     rect[0].setPosition(sf::Vector2f(25,75));
      rect[1].setPosition(sf::Vector2f(145,75));
       rect[2].setPosition(sf::Vector2f(265,75));
        rect[3].setPosition(sf::Vector2f(25,195));
         rect[4].setPosition(sf::Vector2f(145,195));
          rect[5].setPosition(sf::Vector2f(265,195));
           rect[6].setPosition(sf::Vector2f(25,315));
            rect[7].setPosition(sf::Vector2f(145,315));
              rect[8].setPosition(sf::Vector2f(265,315));
    
    while (window.isOpen() && timer<maxtime)
    {
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();}

        int mousex = sf::Mouse::getPosition(window).x;
        int mousey = sf::Mouse::getPosition(window).y;

        full=true;
        for(int i=0;i<9;i++){
          if(field[i]==0)
            full=false;}

        if(full){
          for (int i = 0; i < 9; ++i){
            field[i]=0;}}

          if(myturn && !block)
          {
            for(int i=0;i<9;i++)
            {
              if(field[i]==0 && mousex>rect[i].getPosition().x 
              && mousex <= rect[i].getPosition().x+100 
              && mousey>rect[i].getPosition().y 
              && mousey <= rect[i].getPosition().y+100)
              { 
                rect[i].setOutlineColor(orangeC);
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                  if(mode=='s')field[i]=1;
                  else field[i]=2;
                  myturn=false;
                  block=true;
                  //std::cout << "current block:"<<i<<std::endl;
                  packet << i;
                  //std::cout << "sending packet:"<<packet<<::std::endl<<std::endl;
                  socket.send(packet);
                  packet.clear();
                }
              }
              else
                rect[i].setOutlineColor(basicC);
            }
          }
          if(!myturn && !block)
          {
            socket.receive(packet);
            int target;
            //std::cout << "Getting packet:"<<packet<<std::endl;
            packet >> target;
            //std::cout << "Getting block:"<<target<<std::endl<<std::endl;
            packet.clear();
            if(mode=='s')
             field[target]=2;
            else 
              field[target]=1;
            myturn=true;
            block=true;
          }
        
        window.clear(backstageC);
        for(int i=0;i<9;i++){
            window.draw(rect[i]);
            if(field[i]==1)
                window.draw(circle[i]);
            else if(field[i]==2){
                window.draw(cross[i][0]);
                window.draw(cross[i][1]);}
        }
        window.display();
        if(block)
          block=false;
        timer+=0.1;
        //std::cout << timer << std::endl;
        //sleep(sf::milliseconds(10));//Задержка
        //system("clear");
       // for (int i = 0; i < 9; ++i)
        //{
         // std::cout << field[i] << " ";
         // if(i==2 || i==5 || i==8)
         //   std::cout << std::endl;
        //}
        //std::cout<<std::endl;

    }

    return 0;
}


