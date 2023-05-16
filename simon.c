/* Name: Daniel Mok
 * Student Number: 48087951
 * CWL: dmok01
 * Lab: L1E
 * email address: dmok01@student.ubc.ca
*/

#define WINLED 8
#define LOSELED 13
#define MAX_LENGTH 5

int sequence[MAX_LENGTH] = {0};

//define functions prototypes
char const test[] = "Game Starting\n";

//symbolic constants
const int red = 12;
const int yellow = 11;
const int blue = 10;
const int green = 9;
const int redbutton = 7;
const int yellowbutton = 6;
const int bluebutton = 5;
const int greenbutton = 4;


/* Purpose: run through the sequence array and get player press ompares player press at the same time. 
 * Returns 1 if player successfully makes it through the sequence array and 0 if fails.
 * Input:  sequence array and size
 * Output:  1 or 0 (pass/fail)
*/
int sequenceRun( int sequence[], int size );

/* Purpose: will create an array of MAX_LENGTH with random integers from 9 to 12, each symbolizing an LED.
* Input:  array, size of array
* Output:  nothing
*/
void getSequence( int sequence[], int size );

/* Purpose: displays the sequence array, with delay for each light
* Input:  sequence array and its size
* Output:  1 or 0 check if button is pressed during display sequence
*/
int displaySequence( int sequence[], int size );

/* Purpose: will get a button press from the user and store which pin it came from.
 * Input:  nothing
 * returns:  pin pressed
*/
int getButtonPress(void);

/* Purpose: check if any buttons are pressed during the light sequence.
 * Returns 0 if button is not pressed and 1 if buttons is pressed 
*/
int checkButtonPress(void);

// Purpose: Just makes the green LED blink 3 times for win
void displayWin(void);

// Purpose: makes red LED blink 3 times for lose
void displayLose(void);

// Flashes all LED's symbolizing start of the game
void initialize(void);

//initialization
void setup()
{   
//configure pin modes
	pinMode(green, OUTPUT);  // Green LED		
	pinMode(blue, OUTPUT); // Blue LED
	pinMode(yellow, OUTPUT);	// Yellow LED
	pinMode(red, OUTPUT);	// Red LED	 
	pinMode(greenbutton, INPUT);  //Green button	
	pinMode(bluebutton, INPUT);  // Blue button
	pinMode(yellowbutton, INPUT);  // Yellow button
	pinMode(redbutton, INPUT);  // Red button
	pinMode(WINLED, OUTPUT);  // Victory win green LED or something
	pinMode(LOSELED, OUTPUT); // Red loss LED
 
}

//main program loop
void loop()
{
	// initialize the current length (stage 1)
	int currSequence = 1;
	
	// winnable = 1 means the game can be won
	// this goes to 0 for pressing wrong button or when pressing button during sequence
	int winnable = 1;

	int check = 0;
	// gets the sequence for the game of desired length
	getSequence( sequence, MAX_LENGTH );
	
	initialize();

	//continue looping until end of sequence, correct answers, and no button pressed during the display sequence.
	while( currSequence <= 5 && winnable == 1 && check == 0)
	{    
		check = displaySequence( sequence, currSequence);
		// Sets winnable to the 'sequenceRun' of current length remember sequenceRun returns 1 for pass and 0 for fail 
		// so winnable doesn't change if player succeeds if player doesn't winnable is 0 and while loop terminates
		winnable = sequenceRun( sequence, currSequence );
		delay(1000);
		currSequence++;
	}
	// Check for win status
	if(winnable == 1 && check == 0)
		displayWin();
	else
		displayLose();
	delay(1000);
}

void getSequence( int sequence[], int length ) {
  	randomSeed(analogRead(0)); 
  	int temp;
  	for(int i = 0; i < length; i++ ) {
    	temp = random(9,13);
    	sequence[i] = temp;   
  	}
}

int checkButtonPress(void){
	// check if user pushes a button during the light sequence.
		int flag = 0;
      	int pinRead = 4;

      	while( pinRead <= 7 && flag == 0) 
        {
      		if(digitalRead(pinRead) == HIGH)
            {
              flag = 1; 
            }
          	pinRead++;
		}
	return flag;
}

int displaySequence( int sequence[], int size ) {
	int flag = 0;

  	for(int i = 0; i < size; i++ && flag == 0) {
    	digitalWrite(sequence[i], HIGH);
		long currentmillis = millis();

		while (millis() <= 300 + currentmillis)
		{
			if (checkButtonPress() == 1){
				digitalWrite(red, LOW);
				digitalWrite(blue, LOW);
				digitalWrite(green, LOW);
				digitalWrite(yellow, LOW);;
				flag = 1;
				return flag;
			}
		}
    	digitalWrite(sequence[i], LOW);
		currentmillis = millis();

		while (millis() <= 350 + currentmillis)
		{
			if (checkButtonPress() == 1){
				flag = 1;
				return flag;
			}
		}
  	}
	return flag;
}


int getButtonPress(void) 
{
	int count = 0;
  	int temp;
  	int pinNum;
	
  	while(count == 0)
    {  
      	int pinRead = 4;
      	while( pinRead <= 7 && count == 0) 
        {
      		if(digitalRead(pinRead) == HIGH)
            {
				while(digitalRead(pinRead) == HIGH)
				{
					digitalWrite(pinRead + 5,HIGH);
				}
				digitalWrite(pinRead + 5,LOW);
				count++;
				pinNum = pinRead;
            }
          	pinRead++;
     	}    
    }
	return pinNum;
}

void displayPress( int buttonPressed )
{
	digitalWrite(buttonPressed + 5,HIGH);
  	delay(500);
  	digitalWrite(buttonPressed + 5,LOW);  
  	delay(200);
}

int sequenceRun( int sequence[], int size )
{
  	int status = 1;
  	int i = 0;
  	int press;
  	
  	// This will go through the sequence array of desired lengthAnd get the button press and compare it to an index
  	// if they match, all good (1 is winnable). If they don't, status is 0 and while loop terminates
  	while( i < size && status == 1)
    {
       	press = getButtonPress();

       	if(press + 5 != sequence[i])
        status = 0;

       	else
        i++;
    }
  	return status;
}

void displayWin(void)
{
  	for(int i = 0; i < 2; i++)
    {
      	digitalWrite(WINLED,HIGH);
  		delay(200);
  		digitalWrite(WINLED,LOW);
  		delay(200);
    }
}

void displayLose(void)
{
    for(int i = 0; i < 3; i++)
    {
        digitalWrite(LOSELED,HIGH);
  		delay(500);
  		digitalWrite(LOSELED,LOW);
  		delay(300);
    }
}

//flash all led lights initiating beginning of the game.
void initialize(void)
{
	digitalWrite(green,HIGH);
  	digitalWrite(blue,HIGH);
  	digitalWrite(yellow,HIGH);
  	digitalWrite(red,HIGH);
  	delay(1000);
  	digitalWrite(green,LOW);
  	digitalWrite(blue,LOW);
  	digitalWrite(yellow,LOW);
  	digitalWrite(red,LOW);
  	delay(1000);
}

