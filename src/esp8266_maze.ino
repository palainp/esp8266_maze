/* ------------------------------------------------- */
#include <ESPTelnet.h>
#include <WiFiManager.h>
#include "coos.h"
#include "maze.hpp"

Coos<6, 0> coos; // number 6 in the angle brackets specifies number of user tasks; 0 selects 1 ms tick

WiFiServer server(23);
WiFiClient player;

uint32_t x, y;
Maze m;

/* ------------------------------------------------- */
void print_to_client(std::string str)
{
  if (str!="")
  {
    str += "\r\n";
    player.write(str.c_str(), str.length());
    Serial.print(str.c_str());
  }
}

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
        Serial1.println("Connection rejected ");
      }
      else
      {             // if no player or player disconnected
        if (player) // oh it is in fact disconnected, free it
        {
          player.stop();
        }
        player = server.available();
        // TODO : force character mode
        print_to_client(BUILD_INFO);
        for (std::string line : logo)
        {
          print_to_client(line);
        }
        for (std::string line : welcome)
        {
          print_to_client(line);
        }
        Serial1.print("New client.");
        print_to_client(m.display_cell(x, y, m.status));
      }
    }
  }
}

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
          if ((c == 'i' || c == '8') && m.is_set(x, y, N))
          { // to North and no wall
            x--;
          }
          else if ((c == 'k' || c == '2') && m.is_set(x, y, S))
          { // to South and no wall
            x++;
          }
          else if ((c == 'j' || c == '4') && m.is_set(x, y, W))
          { // to West and no wall
            y--;
          }
          else if ((c == 'l' || c == '6') && m.is_set(x, y, E))
          { // to East and no wall
            y++;
          }
          else if (c == 'q')
          { // to quit
            break;
          }
          else
          {
            continue;
          }

          if (!(m.is_in_map(x, y)))
          {
            m.load_map(x, y);
          }

          for (auto &i : m.items)
          {
            i->update_status(x, y, m.status);
          }
          for (auto &i : m.items)
          {
            print_to_client(i->display_text(x, y, m.status));
          }

          print_to_client(m.display_cell(x, y, m.status));
        }
    }
  }
}

void send_from_console()
{
  //check UART for data
  while (true)
  {
    COOS_DELAY(500);
    if (Serial.available())
    {
      size_t len = Serial.available();
      uint8_t sbuf[len];
      Serial.readBytes(sbuf, len);
      //push UART data to all connected telnet clients
      if (player && player.connected())
      {
        player.write(sbuf, len);
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
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  Serial.begin(115200);
  delay(10);

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

  if (!res)
  {
    Serial.println("Failed to connect");
    // ESP.restart();
  }
  else
  {
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
  }

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  server.setNoDelay(true);

  srand(0);
  x = rand() % 5000 + 1000;
  y = rand() % 5000 + 1000;
  Serial.print("Maze generation...");
  m.generate(x, y);
  Serial.println("done");

  Serial.println("starting coos tasks");
  coos.register_clock(clock_func);
  coos.register_task(check_for_connection); // register tasks
  coos.register_task(deal_with_client);
  coos.register_task(send_from_console);
  coos.start(); // init registered tasks
}

void loop()
{
  coos.run(); // Coos scheduler
}
