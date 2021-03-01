#define DEFAULE_TIME1 4*60*60
#define DEFAULE_TIME2 1*60*60
#define HANTEST true

typedef unsigned long time_t;

enum Cmd {
	CMD_VOID = 0, CMD_STOP1, CMD_STOP2, CMD_RESET, CMD_YES, CMD_ESC, CMD_READY, CMD_BEGIN, CMD_SLEEP, CMD_GOTOZERO, CMD_STOPBELL, CMD_TIME
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

	Cmd button2cmd(Button button, int inter) { // Ä¿Ç°ÓÀÔ¶·µ»Ø¿ÕÃüÁî
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
			stop();
			break;
		case CMD_RESET:
			reset();
			break;
		case CMD_YES:
			break;
		case CMD_ESC:
			break;
		case CMD_READY:
			break;
		case CMD_BEGIN:
			begin();
			break;
		case CMD_SLEEP:
			break;
		case CMD_GOTOZERO:
			bell();
			break;
		case CMD_STOPBELL:
			stopBell();
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
	digitalWrite(13, HIGH);
	delay(100);
	digitalWrite(13, LOW);
	delay(100);
	TimerBack *b = new TimerBack();
	b->reset(100);
	b->start();
	delay(2000);
	Serial.print("hello : ");
	Serial.print(b->getTime());
	Serial.print(" * ");
	Serial.print(b->getTimeMicro());
	Serial.print("\n");
	delete b;
}