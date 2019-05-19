

class Balls{
	private:
		std::string ballLoad;
		int ballType, startingHour, endingHour;
		bool ballUsed;
	
	public:
		void getLoad(std::string load);
		void setType(int myType);
		int getType();
		void ballStateSetter();
		bool ballStateGetter();
		int whatIsStartHour();
		int whatIsEndHour();
		void getStartHour(int myStartHour);
		void getEndHour(int myEndHour);
		void getUnavailableHours(std::vector<int> unavailableHours); //function returns vector of ints
		std::vector<int> unavailabilityList; //vector contains ints which define hours which this ball is unavailable
		std::string dumpLoad();	
		Balls();	
		~Balls(){};
		

};

//Default constructor defined
Balls::Balls(){
	ballLoad = ""; //No string
	ballType = 0; //default zero type
	ballUsed = false;
	
}
	
//get string of characters for ball load
void Balls::getLoad(std::string load){
	ballLoad = load;
}
//putting contents of balls into hoppers
std::string Balls::dumpLoad(){
	return ballLoad;
}

//ballState methods - used to set ball's state to used or unused
//ballUsed is set to true when the ball has dumped its load, it is false by default
void Balls::ballStateSetter(){
	ballUsed = true;
}
bool Balls::ballStateGetter(){
	return ballUsed;
}

/*Balls may be of type 1, type 2, and type 3. Type 1 balls can only occupy first set of hoppers, 
Type 2 balls can only occupy the second set of hoppers, Type 3 balls can occupy either set.
*/
void Balls::setType(int myType){
	ballType = myType;
}

//Returns int used for ball type to identify which sort of ball it is
int Balls::getType(){
	return ballType;
}

//Ball Start and Ball End methods get the range of times when the ball is valid to be used
void Balls::getStartHour(int myStartHour){
	//-1 to convert from hourly to indexical
	startingHour = myStartHour - 1;
}
void Balls::getEndHour(int myEndHour){
	endingHour = myEndHour - 1;
}

int Balls::whatIsStartHour(){
	return startingHour;
}
int Balls::whatIsEndHour(){
	return endingHour;
}

void Balls::getUnavailableHours (std::vector<int> unavailableHours){
	unavailabilityList = unavailableHours;
}


class Hoppers{
	private:
		bool hopperState;
		int hopperType;
		int random;
		std::string hopperLoad;
		std::string hopperLoad2;
	public:
		void fillHopper(std::string hopperLoader, int ballTypeToCorrectHopper);
		bool hopperStateGetter(int thisBallType);
		std::string dumpHopper();
		std::string dumpHopper2();
		Hoppers();	
		~Hoppers(){};
};

//Default constructor for hoppers
Hoppers::Hoppers(){
	hopperState = false;
	hopperLoad = "";
	hopperLoad2 = "";
}

//load hopper with string of characters, hopperLoad can accept balls of type 1 or 3, hopperLoad2 can accept balls of type 2 or 3 
void Hoppers::fillHopper(std::string hopperLoader, int ballTypeToCorrectHopper){
	if (ballTypeToCorrectHopper == 1){
		hopperLoad = hopperLoader;
	}
	else{
		if (ballTypeToCorrectHopper == 2) {
			hopperLoad2 = hopperLoader;
		}
		else {
			if (hopperLoad.empty()){
				hopperLoad = hopperLoader; // if the top slot is empty, fill it if not try to fill the bottom
			}
			else{
				if (hopperLoad2.empty()){ //if the bottom slot is empty fill it, if not, do nothing
					hopperLoad2 = hopperLoader;
				}
			}
		}
	}
} 

//tells whether the hopper slot is filled or unfilled. If there is a load, returns true, if there is not returns false
bool Hoppers::hopperStateGetter(int thisBallType){	
	switch(thisBallType){
		case (1): //if HopperLoad (the first slot) is filled, slot has the state "1"
			if (!hopperLoad.empty()){
				hopperState = true;
				return hopperState;
			}
			else { //if HopperLoad is empty, slot has state "0"
				hopperState = false;
				return hopperState;
			}
		case (2):	//if HopperLoad2 (the second slot) is filled, slot has the state "1"		
			if (!hopperLoad2.empty()){
				hopperState = true;
				return hopperState;
			}
			else { //if HopperLoad2 (the second slot) is empty, slot has the state "0"	
				hopperState = false;
				return hopperState;
			}		
		case (3): //returns false if either, or both are empty, true ONLY if both are loaded
			if (hopperLoad.empty() || hopperLoad2.empty()){
				hopperState = false;
				return hopperState;
			}
			else{
				hopperState = true;
				return hopperState;
			}		
	}	
}
//Dump contents of hopper 1 
std::string Hoppers::dumpHopper(){
	return hopperLoad;		
}
//Dump contents of hopper 2
std::string Hoppers::dumpHopper2(){
	return hopperLoad2;		
}
