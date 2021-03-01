#define DEFAULE_TIME1 4*60*60
#define DEFAULE_TIME2 1*60*60
#define HANTEST true

// about dev
#define VCC 13
const int strobe = 12;
const int clk = 11;
const int dio = 10;


typedef unsigned long time_t;
typedef long LightNum;

enum Cmd {
	CMD_VOID = 0, CMD_STOP1, CMD_STOP2, CMD_RESET, CMD_YES, CMD_ESC, CMD_READY, CMD_BEGIN, CMD_SLEEP, CMD_GOTOZERO, CMD_STOPBELL, CMD_TIME
};

enum Button {
	BUTTON_VOID = 0, BUTTON_STOP2 = 1, BUTTON_STOP1 = 2, BUTTON_RESET = 4, BUTTON_INTERFACE = 8, BUTTON_TIME = 16, BUTTON_ESC = 32, BUTTON_YES = 64, BUTTON_BEGIN = 128
};

class Timer {
private:
	time_t timeO;
	time_t timeB;
	int state;

protected:
	void reset() {
		timeO = 0;
		timeB = 0;
		state = 0;
	}

	time_t getTime() {
		switch (state)
		{
		case  0:
			return 0;
		case  1:
			return timeB + millis() - timeO;
		case  2:
			return timeB;
		}
	}

public:
	void start() {
		timeO = millis();
		timeB = 0;
		state = 1;
	}
	void begin() {
		switch (state)
		{
		case  0:
			break;
		case  1:
			break;
		case  2:
			timeO = millis();
			state = 1;
			break;
		}
	}
	void stop() {
		switch (state)
		{
		case  0:
			break;
		case  1:
			timeB += millis() - timeO;
			state = 2;
			break;
		case  2:
			break;
		}
	}
};

class TimeConver {
public:
	static int micro2second(time_t time) {
		return time / 1000;
	}
	static time_t second2micro(int time) {
		return time * 1000;
	}
	static long second2display(int time) {
		int h = time / 3600;
		int m = time / 60 - h * 60;
		int s = time - h * 3600 - m * 60;
		return h * 10000 + m * 100 + s;
	}
};

class TimerBack :public Timer {
private:
	int timeAdd = 0;
	int timeAll;
public:
	void reset(int timeAll) {
		this->timeAll = timeAll;
		timeAdd = 0;
		Timer::reset();
	}

	void add(int timeAdd) {}

	time_t getTime() {
		time_t time_go = Timer::getTime();
		time_t time_have = (time_t)timeAll * 1000 + (time_t)timeAdd * 1000;
		return time_have > time_go ? time_have - time_go : 0;
	}

	time_t getTimeMicro() {
		if (HANTEST) {
			return Timer::getTime();
		}
		return 0;
	}
};

class Controler {
private:
	int inter;
public:
	void run(int button) {

	}; //todo
};

class Manager {
protected:
	int state;
	int defaultTime;
	TimerBack *timer;
public:
	Manager() {
		state = 0;
		timer = new TimerBack();
	}

	virtual Cmd button2cmd(int button, int inter) = 0;

	virtual void do_cmd(int cmd) = 0;

	~Manager() {
		delete timer;
	}
};

class Manager1 : public Manager {

public:
	Manager1() : Manager() {
		defaultTime = DEFAULE_TIME1;
		state = 1;
	}

	Cmd button2cmd(int button, int inter) {
		if (inter == 1) {
			switch (button)
			{
			case BUTTON_VOID:
				break;
			case BUTTON_STOP2:
				switch (state)
				{
				case 3:
					return CMD_STOP2;
					break;
				case 6:
					return CMD_STOPBELL;
					break;
				}
				break;
			case BUTTON_STOP1:
				return CMD_STOP1;
				break;
			case BUTTON_RESET:
				switch (state)
				{
				case 4:
					return CMD_RESET;
					break;
				case 5:
					return CMD_ESC;
					break;
				}
				break;
			case BUTTON_INTERFACE:
				break;
			case BUTTON_TIME:
				break;
			case BUTTON_ESC:
				return CMD_ESC;
				break;
			case BUTTON_YES:
				switch (state)
				{
				case 1:
					return CMD_READY;
					break;
				case 5:
					return CMD_YES;
					break;
				}
				break;
			case BUTTON_BEGIN:
				switch (state)
				{
				case 2:
					return CMD_BEGIN;
					break;
				case 4:
					return CMD_BEGIN;
					break;
				case 6:
					return CMD_SLEEP;
					break;
				}
				break;
			}
		}
		return CMD_VOID;
	}
	void do_cmd(Cmd cmd) {

		switch (cmd)
		{
		case CMD_VOID:
			break;
		case CMD_STOP1:
			switch (state)
			{
			case 3:
				state = 4;
				stop1();
				break;
			case 4:
				state = 4;
				stop1();
				break;

			}
			break;
		case CMD_STOP2:
			switch (state)
			{
			case 3:
				state = 4;
				stop2();
				break;
			}
			break;
		case CMD_RESET:
			switch (state)
			{
			case 4:
				state = 5;
				wannaReset();
				break;
			}
			break;
		case CMD_YES:
			switch (state)
			{
			case 5:
				state = 1;
				wannaReset_yes();
				break;
			}
			break;
		case CMD_ESC:
			switch (state)
			{
			case 2:
				state = 1;
				readyd_esc();
				break;
			case 5:
				state = 4;
				wannaReset_esc();
				break;

			}
			break;
		case CMD_READY:
			switch (state)
			{
			case 1:
				state = 2;
				ready();
				break;
			}
			break;
		case CMD_BEGIN:
			switch (state)
			{
			case 2:
				state = 3;
				begin();
				break;
			case 4:
				state = 3;
				begin();
				break;

			}
			break;
		case CMD_SLEEP:
			switch (state)
			{
			case 6:
				state = 3;
				sleep();
				break;
			}
			break;
		case CMD_GOTOZERO:
			switch (state)
			{
			case 3:
				state = 6;
				bell();
				break;
			}
			break;
		case CMD_STOPBELL:
			switch (state)
			{
			case 3:
				state = 1;
				stopBell();
				break;
			}
			break;
		case CMD_TIME:
			break;
		}
	}

protected: // todo
	void stop1() {
	}

	void stop2() {

	}

	void wannaReset() {

	}

	void wannaReset_yes() {

	}

	void readyd_esc() {

	}

	void wannaReset_esc() {

	}

	void ready() {

	}

	void begin() {

	}

	void sleep() {

	}

	void bell() {

	}

	void stopBell() {

	}

};

class Manager2 : public Manager {

public:
	Manager2() : Manager() {
		defaultTime = DEFAULE_TIME2;
		state = 2;
	}

	Cmd button2cmd(Button button, int inter) { // ƒø«∞”¿‘∂∑µªÿø’√¸¡Ó
		if (inter == 2) {
			switch (button)
			{
			case BUTTON_VOID:
				break;
			case BUTTON_STOP2:
				break;
			case BUTTON_STOP1:
				break;
			case BUTTON_RESET:
				break;
			case BUTTON_INTERFACE:
				break;
			case BUTTON_TIME:
				break;
			case BUTTON_ESC:
				break;
			case BUTTON_YES:
				break;
			case BUTTON_BEGIN:
				break;
			}
		}
		return CMD_VOID;
	}
	void do_cmd(Cmd cmd) {
		switch (cmd)
		{
		case CMD_VOID:
			break;
		case CMD_STOP1:
			break;
		case CMD_STOP2:
			switch (state)
			{
			case 3:
				state = 4;
				stop();
				break;
			}
			break;
		case CMD_RESET:
			switch (state)
			{
			case 3:
				state = 2;
				reset();
				break;
			case 4:
				state = 2;
				reset();
				break;
			}
			break;
		case CMD_YES:
			break;
		case CMD_ESC:
			break;
		case CMD_READY:
			break;
		case CMD_BEGIN:
			switch (state)
			{
			case 2:
				state = 3;
				begin();
				break;
			case 4:
				state = 3;
				begin();
				break;
			}
			break;
		case CMD_SLEEP:
			break;
		case CMD_GOTOZERO:
			switch (state)
			{
			case 3:
				state = 2;
				bell();
				break;
			}
			break;
		case CMD_STOPBELL:
			switch (state)
			{
			case 6:
				state = 2;
				stopBell();
				break;
			}
			break;
		case CMD_TIME:
			break;
		}
	}
protected: // todo
	void stop() {

	}

	void reset() {

	}

	void begin() {

	}

	void bell() {

	}

	void stopBell() {

	}
};

class Dev {
private:
	void writeByte(int value)     // unclear
	{
		unsigned char i;
		pinMode(dio, OUTPUT);
		for (i = 0; i < 8; i++)
		{
			digitalWrite(clk, LOW);
			if (value & 0X01) {
				digitalWrite(dio, HIGH);
			}
			else {
				digitalWrite(dio, LOW);
			}
			value >>= 1;
			digitalWrite(clk, HIGH);
		}
		digitalWrite(clk, LOW);
	}

	void sendCommand(uint8_t value)
	{
		digitalWrite(strobe, LOW);
		writeByte(value);
		digitalWrite(strobe, HIGH);
	}

	int readByte(void)          //read a byte.
	{
		unsigned char i;
		int value = 0;;
		pinMode(dio, INPUT); //set input mode.
		for (i = 0; i < 8; i++)
		{
			value >>= 1;
			digitalWrite(clk, LOW);
			delay(1);                           //if CUP is veryfast.
			if (digitalRead(dio) == HIGH)
				value |= 0x80;
			digitalWrite(clk, HIGH);
		}
		digitalWrite(clk, LOW);
		pinMode(dio, OUTPUT); //set output mode.
		return value;
	}

	int readButtons(void)
	{
		int buttons = 0;
		digitalWrite(strobe, LOW);
		pinMode(dio, OUTPUT);
		shiftOut(dio, clk, LSBFIRST, 0x42);
		pinMode(dio, INPUT);
		for (uint8_t i = 0; i < 4; i++)
		{
			int v = readByte() << i;
			buttons |= v;
		}
		pinMode(dio, OUTPUT);
		digitalWrite(strobe, HIGH);
		return buttons;
	}

	void setLed(int value, int position)
	{
		pinMode(dio, OUTPUT);
		sendCommand(0x44);
		digitalWrite(strobe, LOW);
		writeByte(0xC1 + (position << 1));
		writeByte(value);
		digitalWrite(strobe, HIGH);
	}

	void showNumber(LightNum number)
	{
		const LightNum digital[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };
		LightNum pos = 7;
		while (pos >= 0) {
			digitalWrite(strobe, LOW);
			writeByte(0xc0 + pos * 2);
			if (number > 0) {
				LightNum dig = number % 10;
				writeByte(digital[dig]);
				number /= 10;
			}
			else {
				writeByte(0);
			}
			digitalWrite(strobe, HIGH);
			pos--;
		}
	}

	void showTime(LightNum number)
	{
		const LightNum digital_[] =  { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };
		const LightNum digital__[] = { 0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF };
		
		LightNum pos = 7;
		while (pos >= 0) {
			digitalWrite(strobe, LOW);
			writeByte(0xc0 + pos * 2);
			if (number > 0) {
				LightNum dig = number % 10;
				if (pos == 3 || pos == 5)
					writeByte(digital__[dig]);
				else
					writeByte(digital_[dig]);
				number /= 10;
			}
			else {
				writeByte(0);
			}
			digitalWrite(strobe, HIGH);
			pos--;
		}
	}

	void buttonShowHAN(int buttons)
	{
		for (int position = 0; position < 8; position++)
		{
			int mask = 0x1 << position;
			setLed(buttons & mask ? 1 : 0, position);
		}
	}


public:

	void reset()
	{
		sendCommand(0x8b); //set light,0x88-0x8f
		sendCommand(0x40); // set auto increment mode
		digitalWrite(strobe, LOW);
		writeByte(0xc0);   // set starting address to 0
		for (uint8_t i = 0; i < 16; i++)
		{
			writeByte(0x00);
		}
		digitalWrite(strobe, HIGH);
	}

	int getButton() {
		return readButtons();
	}

	void displayTime(LightNum dis) {
		showTime(dis);
	}

	void displayNum(LightNum dis) {
		showNumber(dis);
	}

	

	void led(int ledcode) {
		buttonShowHAN(ledcode);
	}
};

Dev *dev = new Dev();

class Buttoner {
public:
	Button getButton() {
		int button = dev->getButton();
		switch (button)
		{
		case 1:
			return BUTTON_STOP2;
			break;
		case 2:
			return BUTTON_STOP1;
			break;
		case 4:
			return BUTTON_RESET;
			break;
		case 8:
			return BUTTON_INTERFACE;
			break;
		case 16:
			return BUTTON_TIME;
			break;
		case 32:
			return BUTTON_ESC;
			break;
		case 64:
			return BUTTON_YES;
			break;
		case 128:
			return BUTTON_BEGIN;
			break;
		}
		return BUTTON_VOID;
	}
};

Buttoner *buttoner = new Buttoner();


class Displayer {
public:
	void displayTime(time_t time) { //todo about dev
		LightNum t = time / 1000;
		//  t / 3600
		//  t / 60 - t / 3600 * 60
		//  t % 60
		dev->displayTime(t / 3600 * 10000 + (t / 60 - t / 3600 * 60) * 100 + t % 60);
	}

	void displayNum(LightNum num) {
		dev->displayNum(num);
	}
};

Displayer *displayer = new Displayer();
TimerBack *timer1 = new TimerBack();


void setup() {
	pinMode(13, OUTPUT);
	digitalWrite(VCC, LOW);
	Serial.begin(9600);

	// dev
	pinMode(strobe, OUTPUT);
	pinMode(clk, OUTPUT);
	pinMode(dio, OUTPUT);
	dev->reset();
	
	//Test
	timer1->reset(60*60*1+10);
	timer1->start();
}

void loop() {

	// Timer Test
	/*
	digitalWrite(13, HIGH);
	delay(100);
	digitalWrite(13, LOW);
	delay(100);
	TimerBack *b = new TimerBack();
	b->reset(100);
	b->start();
	delay(20);
	Serial.print("hello : ");
	Serial.print(b->getTime());
	Serial.print(" * ");
	Serial.print(b->getTimeMicro());
	Serial.print("\n");
	delete b;
	*/

	// Dev Test
	/*
	for (int i = 1; i < 100000; i++) {
		dev->displayTime(i);
		delay(1);
	}
	*/

	// Display Test

	displayer->displayTime(timer1->getTime());


}
