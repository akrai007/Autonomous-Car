#include <SoftwareSerial.h>
String sendData(String command, const int timeout, boolean debug) ;
void forward();                     //prototypes for different functions used
void backward();                    // defined at the end of code
void right();
void left();
void hault();

#define DEBUG true
#define FORWARD 'U'   //551486205
#define BACKWARD 'D'  //551518845
#define RIGHT  'R'    //551510175
#define LEFT  'L'     //551542815
#define HAULT 'S'     //551494365

char code   ;
int rx = 2 ;
int tx = 3 ;

int en_1 = 5, en_2 = 6 ;                                                  //enable pins for L293D  declared
int in1 = 7, in2 = 8, in3 = 10, in4 = 9  ;                                //input pins declared according to connections
int fwd_speed = 0, rev_speed = 0, rturn_speed = 0, lturn_speed = 0  ;     //turn speeds defaulted to 0

SoftwareSerial usb(rx, tx); // make TX Arduino line is pin 2, make RX Arduino line is pin 3.
//const char* ssid = "FCB" ;
//const char* password = "viscabarca" ;



void setup()
{

  Serial.begin(115200);  //esp works at 115200
  usb.begin(9600);       //serial data to usb for printing on screen work at 9600
  pinMode(in1, OUTPUT);                                                 //declare input pins of L293D as output for arduino
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);                                                 //receiver pin is declared as input for arduino in the library file included.
  pinMode(in4, OUTPUT);



  sendData("AT+RST\r\n", 2000, DEBUG); // reset module
  sendData("AT+CWMODE=3\r\n", 1000, DEBUG); // configure as access point
  //  sendData("AT+CWJAP=\"FCB\",\"viscabarca\"\r\n",2000,DEBUG); // reset module
  //  delay (2000) ;                                              //give time for esp to connect
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG); // turn on server on port 80
  sendData("AT+CIFSR\r\n", 1000, DEBUG); // get ip address

}

void loop()
{
  String temp = "" ;    //make a temporary string
  boolean flag = false ;
  while (Serial.available() ) {       // if esp sending data then
    temp = Serial.readString() ;      // read incoming string
    usb.println(temp) ;               // print string on serial window
    flag = true ;                     // set flag true so get into next decision only if a string received in this iteration

  }
  if (flag) {
    if ( temp.length() < 15) {      // get code only if received string is the transmitted character
      usb.print( "output:  " );     // print the string received
      usb.print( temp);
      int len = temp.length() ;
      code = temp.charAt(len - 1) ; // code for movement is last character of received string with length < 25
      usb.print("Code is :  ");
      usb.print(code) ;     // print the code character for making decision
      switch ( code ) {                                              //case structure to identify which task is to  be performed
        case FORWARD :                                                 //call forward function if forward button is pressed
          forward();
          usb.println("forward function called") ;
          break;

        case BACKWARD :
          backward ();                                            //call backward function if backward button is pressed
          usb.println("backward function called") ;
          break ;

        case RIGHT :                                                  //call right turn function if right button is pressed
          right();
          usb.println("right turn function called") ;
          break ;

        case LEFT :
          left();                                                    //call left turn function if left button is pressed
          usb.println("left turn function called") ;
          break ;

        case HAULT :
          hault();                                                  //call hault function if central button is pressed
          usb.println("hault function called") ;
          break ;

        default :
          ;                                                        //do nothing if any invalid key is pressed
      }

    }
  }



}




/*
  Name: sendData
  Description: Function used to send data to ESP8266.
  Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
  Returns: The response from the esp8266 (if there is a reponse)
*/
String sendData(String command, const int timeout, boolean debug)
{
  String response = "";

  Serial.print(command); // send the read character to the esp8266

  long int time = millis();

  while ( (time + timeout) > millis())
  {
    while (Serial.available())
    {

      // The esp has data so display its output to the serial window
      char c = Serial.read(); // read the next character.
      response += c;
    }
  }

  if (debug)
  {
    usb.print(response);
  }

  return response;
}

void forward() {                                                //function to move car forward
  /*  if ( fwd_speed < 255 ) {
      fwd_speed += 51 ;
      }*/
  fwd_speed = 255 ;                                             //value to be written to the enable pin of L293D
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite (en_1, fwd_speed);
  analogWrite (en_2, fwd_speed);
}


void backward() {                                              //function to move car backward
  /* if ( rev_speed < 255 ) {
     rev_speed += 51 ;
    }*/
  rev_speed = 255 ;                                           //value to be written to the enable pin of L293D
  digitalWrite(in1, LOW );
  digitalWrite(in2, HIGH );
  digitalWrite(in3, LOW );
  digitalWrite(in4, HIGH);
  analogWrite (en_1, rev_speed);
  analogWrite (en_2, rev_speed);
}


void right() {                                                //function to turn car right
  /*  if ( rturn_speed < 255 ) {
      rturn_speed += 51 ;
    }*/
  rturn_speed = 255 ;                                         //value to be written to the enable pin of L293D
  digitalWrite(in1, HIGH );
  digitalWrite(in2, LOW );
  digitalWrite(in3, LOW );
  digitalWrite(in4, HIGH);
  analogWrite (en_1, rturn_speed);
  analogWrite (en_2, rturn_speed);
}


void left() {                                                //function to turn car left
  /* if ( lturn_speed < 255 ) {
     lturn_speed += 51 ;
    } */
  lturn_speed = 255 ;                                       //value to be written to the enable pin of L293D
  digitalWrite(in1, LOW );
  digitalWrite(in2, HIGH );
  digitalWrite(in3, HIGH );
  digitalWrite(in4, LOW );
  analogWrite (en_1, lturn_speed);
  analogWrite (en_2, lturn_speed);
}


void hault() {                                            //function to stop the car
  digitalWrite(in1, LOW );
  digitalWrite(in2, LOW );
  digitalWrite(in3, LOW );
  digitalWrite(in4, LOW );
  analogWrite (en_1, 0 );
  analogWrite (en_2, 0 );
  fwd_speed = 0 ;
  rev_speed = 0 ;
  lturn_speed = 0 ;
  rturn_speed = 0 ;

}

