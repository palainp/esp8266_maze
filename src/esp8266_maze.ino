/* ------------------------------------------------- */
#include <ESPTelnet.h>
#include <WiFiManager.h>
#include "coos.h"
#include "maze.hpp"

// do not define USE_SERIAL with ESP8266/01 as we must use RX for the 3color led
//#define USE_SERIAL

Coos<6, 0> coos; // number 6 in the angle brackets specifies number of user tasks; 0 selects 1 ms tick

WiFiServer server(23);
WiFiClient player;

Maze m;
Player p;

// ESP8266/01s pinout
#define GPIO0 0
#define GPIO2 2
#define TX 1
#define RX 3

const int RED = TX;
const int GREEN = GPIO2;
const int BLUE = GPIO0;

/* ------------------------------------------------- */
void print_to_client(std::string str)
{
  if (str != "")
  {
    str += "\r\n";
    player.write(str.c_str(), str.length());
#ifdef USE_SERIAL
    Serial.print(str.c_str());
#endif
  }
}

// ========================================
// Update the 3color led according to the player led status
// ========================================
void led_color()
{
  while (true)
  {
    COOS_DELAY(100);
    analogWrite(RED, p.red_color());
    analogWrite(GREEN, p.green_color());
    analogWrite(BLUE, p.blue_color());
  }
}

// ========================================
// Wait for incomming connexions
// ========================================
void check_for_connection()
{
  while (true)
  {
    COOS_DELAY(500);
    //check if there are any new clients
    if (server.hasClient())
    {
      if (player && player.connected()) // if a player is currently connected, refuse a new player
      {
        WiFiClient serverClient = server.available();
        std::string str = "Sorry, someone is still searching the way...\r\n";
        serverClient.write(str.c_str(), str.length());
        serverClient.stop();
#ifdef USE_SERIAL
        Serial.println("Connection rejected ");
#endif
      }
      else
      {             // if no player or player disconnected
        if (player) // oh it is in fact disconnected, free it
        {
          player.stop();
        }
        player = server.available();
        print_to_client("\377\375\042\377\373\001"); // force char mode
        print_to_client(BUILD_INFO);
        for (std::string line : logo)
        {
          print_to_client(line);
        }
        for (std::string line : welcome)
        {
          print_to_client(line);
        }
#ifdef USE_SERIAL
        Serial.print("New client.");
#endif
        print_to_client(m.display_cell(p));
      }
    }
  }
}

// ========================================
// Get player's moves to update the screen
// ========================================
void deal_with_client()
{
  //check clients for data
  while (true)
  {
    COOS_DELAY(100);
    if (player && player.connected() && player.available())
    {
      //get data from the telnet client and deal with it
      while (player.available())
      {
        char c = player.read();
        if ((c == 'i' || c == '8') && m.is_set(m.cell(p.x, p.y), N))
        { // to North and no wall
          p.move(N);
        }
        else if ((c == 'k' || c == '2') && m.is_set(m.cell(p.x, p.y), S))
        { // to South and no wall
          p.move(S);
        }
        else if ((c == 'j' || c == '4') && m.is_set(m.cell(p.x, p.y), W))
        { // to West and no wall
          p.move(W);
        }
        else if ((c == 'l' || c == '6') && m.is_set(m.cell(p.x, p.y), E))
        { // to East and no wall
          p.move(E);
        }
        else if (c == 'q')
        { // to quit
#ifdef USE_SERIAL
          Serial.println("player disconnected");
#endif
          player.stop();
          break;
        }
        else
        {
          continue;
        }

        if (!(m.is_in_map(p)))
        {
          print_to_client("loading map...");
          m.load_map(p);
        }

        p.reset_color();
        for (auto &i : m.items)
        {
          i->move(p, m.cell(i->x,i->y));
          i->update_status(p);
        }

        for (auto &i : m.items)
        {
          print_to_client(i->display_text(p));
        }

        print_to_client(m.display_cell(p));
        if (p.status & exit_found)
        {
          m.set_level(p);
        }
      }
    }
  }
}

// ========================================
// Sends input from console to the player screen
// ========================================
void send_from_console()
{
  //check UART for data
  while (true)
  {
    COOS_DELAY(500);
    if (Serial.available())
    {
      size_t len = Serial.available();
      uint8_t sbuf[len+2];
      Serial.readBytes(sbuf, len);
      sbuf[len]='\r';
      sbuf[len+1]='\n';
      //push UART data to all connected telnet clients
      if (player && player.connected())
      {
        player.write(sbuf, len+2);
        delay(1);
      }
    }
  }
}

// ========================================
// Clock prints current time every minute
// ========================================
void clock_func(void)
{
  char buf[10];
  sprintf(buf, " %02d:%02d ", coos.hour(), coos.minute());
  Serial.print(buf);
}

/* ------------------------------------------------- */
void setup()
{
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
#ifdef USE_SERIAL
  Serial.begin(115200);
  delay(10);
#endif

  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result

  bool res;
  res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  // res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

  analogWrite(RED, 0);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 0);

  // led is red = AP, blue = STA
  if (!res)
  {
    analogWrite(RED, 255);
#ifdef USE_SERIAL
    Serial.println("Failed to connect");
#endif
    // ESP.restart();
  }
  else
  {
    //if you get here you have connected to the WiFi
    analogWrite(BLUE, 255);
#ifdef USE_SERIAL
    Serial.println("connected...yeey :)");
#endif
  }

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
#ifdef USE_SERIAL
    Serial.print(".");
#endif
  }

#ifdef USE_SERIAL
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
#endif

  server.begin();
  server.setNoDelay(true);

  srand(0);
  p.teleport(rand() % 5000 + 1000, rand() % 5000 + 1000);
#ifdef USE_SERIAL
  Serial.print("Maze generation...");
#endif
  m.generate(p);
#ifdef USE_SERIAL
  Serial.println("done");
#endif

#if 0
  for (auto i=0; i<100; ++i)
  {
    for (auto j=0; j<100; ++j)
    {
      Serial.printf("%2d ", m.maze[i][j]);
    }
    Serial.printf("\n");
    yield();
  }
#endif

#ifdef USE_SERIAL
  Serial.println("starting coos tasks");
  coos.register_clock(clock_func);
  coos.register_task(send_from_console); // register tasks
#endif
  coos.register_task(check_for_connection); // register tasks
  coos.register_task(deal_with_client);
  coos.register_task(led_color);
  coos.start(); // init registered tasks
}

void loop()
{
  coos.run(); // Coos scheduler
}
