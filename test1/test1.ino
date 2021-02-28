#define DEFAULE_TIME1 4*60*60
#define DEFAULE_TIME2 1*60*60


typedef unsigned long time_t;

void setup() {
	pinMode(13, OUTPUT);
}

void loop() {
	digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
	delay(1000);              // wait for a second
	digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
	delay(100);              // wait for a second
}

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
			return timeB + micros() - timeO;
		case  2:
			return timeB;
		}
	}

public:
	void start() {
		timeO = micros();
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
			timeO = micros();
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
			timeB += micros() - timeO;
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

	virtual void button2cmd(int button, int inter) = 0;

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

	void button2cmd(int button, int inter) {} //todo
	void do_cmd(int cmd) {} //todo
};

class Manager2 : public Manager {

public:
	Manager2() : Manager() {
		defaultTime = DEFAULE_TIME2;
	}

	void button2cmd(int button, int inter) {} //todo
	void do_cmd(int cmd) {} //todo
};

class Displayer {
public:
	void display() { //todo about dev
		
	}
};