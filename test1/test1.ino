#define DEFAULE_TIME1 4*60*60
#define DEFAULE_TIME2 1*60*60
#define HANTEST 1

typedef unsigned long time_t;

enum Cmd {
	CMD_VOID = 0, CMD_STOP1, CMD_STOP2, CMD_STOPBELL, CMD_RESET, CMD_ESC, CMD_READY, CMD_YES, CMD_BEGIN, CMD_SLEEP, CMD_GOTOZERO
};

enum Button {
	BUTTON_VOID = 0, BUTTON_STOP2, BUTTON_STOP1, BUTTON_RESET, BUTTON_INTERFACE, BUTTON_TIME, BUTTON_ESC, BUTTON_YES, BUTTON_BEGIN
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

	int getTime() {
		return timeAll - Timer::getTime() / 1000 + timeAdd;
	}

	time_t getTimeMicro() {
		if (HANTEST) {
			return Timer::getTime();
		}
		return 0;
	}

};

class Buttoner {
public:
	int getButton() {} //todo  about dev
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
	//state/button-cmd±í
	//state/cmd-do±í
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
	}

	Cmd button2cmd(int button, int inter) {

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
		return CMD_VOID;
	} //todo
	void do_cmd(int cmd) {} //todo
};

class Manager2 : public Manager {

public:
	Manager2() : Manager() {
		defaultTime = DEFAULE_TIME2;
	}

	Cmd button2cmd(Button button, int inter) {
	} //todo
	void do_cmd(int cmd) {} //todo
};

class Displayer {
public:
	void display() { //todo about dev

	}
};


void setup() {
	pinMode(13, OUTPUT);
	Serial.begin(9600);
}

void loop() {
	digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
	delay(100);              // wait for a second
	digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
	delay(100);              // wait for a second
	TimerBack *b = new TimerBack();
	b->reset(100);
	b->start();
	delay(2000);
	Serial.print("hello : ");
	Serial.print(b->getTime());
	Serial.print(" * ");
	Serial.print(b->getTimeMicro());
	Serial.print("\n");
}