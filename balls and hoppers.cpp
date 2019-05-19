
#include <iostream>
#include <algorithm>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <ctime>
#include <fstream>
#include "ballsandhoppers.h"

//Time printout. This is so that it's easy for the user to see what "hour of" translates to

void timeConverterPrinter(int& libOpen, int& libClose, int& ampm){
	int diff, timeConv, setMyTime;	
	if (ampm == 1){
		setMyTime = libOpen;
	}
	else if (ampm == 2){
		setMyTime = libOpen + 12;
	}
	
	diff = (libClose + 12) - setMyTime;
	for(int f=0;f<diff;f++){	
		if (setMyTime <= 12) {
			std::cout << "|  " << libOpen+f << ":00  ";	
		}	
	}
	std::cout << '\n';
	for(int g=0;g<diff;g++){
		std::cout << "|    " << g+1 << "    ";
	}
	std::cout << '\n';		
}

//Empty Hopper Filler. Finds hoppers with empty slots, cycles through unused balls, places balls in hopper

void emptyHopperFiller(std::vector<Hoppers> &availableHopper, std::vector<Balls> &selectedBall, const int hopperIndex, const int ballIndex ){
	if (availableHopper[hopperIndex].dumpHopper()==""){
		for(int ballCount=0;ballCount<ballIndex;ballCount++){
			//if the selected ball is of type 1 or type 3 and it has not been used yet
			if((selectedBall[ballCount].getType()==1 || selectedBall[ballCount].getType()==3)  && selectedBall[ballCount].ballStateGetter()!=true){
				availableHopper[hopperIndex].fillHopper(selectedBall[ballCount].dumpLoad(),1);
				selectedBall[ballCount].ballStateSetter();
			}
		}
	}
	else if(availableHopper[hopperIndex].dumpHopper2()==""){
		for(int ballCount=0;ballCount<ballIndex;ballCount++){
			//if the selected ball is of type 1 or type 3 and it has not been used yet
			if((selectedBall[ballCount].getType()==2 || selectedBall[ballCount].getType()==3)  && selectedBall[ballCount].ballStateGetter()!=true){
				availableHopper[hopperIndex].fillHopper(selectedBall[ballCount].dumpLoad(),2);
				selectedBall[ballCount].ballStateSetter();
			}
		}
	}
}


//"Engine" of the program. Assign a ball to a random hopper. If hopper is empty, dump ball load into hopper. If filled, 
//bounce ball and run function again
//What is needed: if vector array has been totally filled, halt and assign no more balls
//This is so that if there are more balls than hoppers, it won't keep bouncing forever
//Add a counter that adds each time loop is run, use while loop

void randomBallBounce(std::vector<Hoppers> &availableHopper, std::vector<Balls> &selectedBall, const int hopperIndex, int ballIndex, int loopCounter){
	
	int bounce = rand() % hopperIndex; //generate some random number between 0 and hopperIndex
	//cycle through the recursive loop only as long as the number of loops is less than the number of hoppers
	if (loopCounter < hopperIndex){		
		//do this only if it's not one of the unavailable hours
		if (std::find(selectedBall[ballIndex].unavailabilityList.begin(), selectedBall[ballIndex].unavailabilityList.end(), bounce) == selectedBall[ballIndex].unavailabilityList.end()){
			//now add a clause to account for when someone starts or ends - loop only executes for balls if they start at hour x and end at hour y
			//this is to control for people who start shifts at a certain time and end before closing, or start during the middle of the day and end later	
			if (bounce >= selectedBall[ballIndex].whatIsStartHour() && bounce <= selectedBall[ballIndex].whatIsEndHour() ) {   		
				if(availableHopper[bounce].hopperStateGetter(selectedBall[ballIndex].getType())==false){ //hopperStateGetter sees if the  hopper has a value or not.
					// If hopperStateGetter returns false, put ball's payload into hopper, if true, run function again 
					//needs something so it doesn't fill both top and bottom hopper slots with the same type3 ball
					/* "if the top is free and the loaded item in the bottom is not equal to the value of the ball, OR, if 
					 the bottom is free and the loaded item in the top is not equal to the value of the ball"
					 THEN
					 "fill the hopper slot with the correspondent ball of that type
					*/
					//btw this code is hideous, and should be made less complex
					if ((availableHopper[bounce].hopperStateGetter(1) == false && availableHopper[bounce].dumpHopper2() != selectedBall[ballIndex].dumpLoad()) 
						^ (availableHopper[bounce].hopperStateGetter(2) == false && availableHopper[bounce].dumpHopper() != selectedBall[ballIndex].dumpLoad())
						 ^ (availableHopper[bounce].hopperStateGetter(1) == false && availableHopper[bounce].hopperStateGetter(2) == false ) ){
						availableHopper[bounce].fillHopper(selectedBall[ballIndex].dumpLoad(),selectedBall[ballIndex].getType());
						selectedBall[ballIndex].ballStateSetter();			
					}
					else{//if ball bounces, then the loop can run again until the ball finds a place
						loopCounter++;		
						randomBallBounce(availableHopper, selectedBall, hopperIndex, ballIndex, loopCounter);//run again with same params
					}			
				}
				else{//if ball bounces, then the loop can run again until the ball finds a place
					loopCounter++;		
					randomBallBounce(availableHopper, selectedBall, hopperIndex, ballIndex, loopCounter);//run again with same params
				}
			}
			else {
				loopCounter++;		
				randomBallBounce(availableHopper, selectedBall, hopperIndex, ballIndex, loopCounter);//run again with same params
			}
		}
		else {
				loopCounter++;		
				randomBallBounce(availableHopper, selectedBall, hopperIndex, ballIndex, loopCounter);//run again with same params
		}					
	}		
}



int main(void){
	
	int m, b, numNames, totalBalls, thisBallType, entryHour, exitHour, libOpen, libClose, unavailableHour, ampm;
	char choose, save;
	int loopCounter = 0;
	std::vector<int> unavailableHours;
	std::ofstream myFile;
	srand(time(nullptr));
	std::cout << "AUTOMATIC DESK SCHEDULER v.0.1-alpha release \n";
	std::cout << "******************************************** \n";
	std::cout << "Written by Anh-Vu Doan, 2015 \n";
	std::cout << '\n';
	std::cout << "Input library's opening: ";
	std::cin >> libOpen;
	std::cout << "Morning or Afternoon? Choose (1) AM ; (2) PM:  ";
	std::cin >> ampm;
	std::cout << "Input library's closing: ";
	std::cin >> libClose;
	std::cout << "Input number of desk hours: ";
	std::cin>>m;
	std::cout << "Input number of desk staff: " ;
	std::cin>>numNames;
	std::cout << "Input max number of hours per person: ";
	std::cin>>b;

	totalBalls = numNames * b;
	std::vector <Hoppers> hopperSet(m);
	std::vector <Balls> ballSet;
	std::string nameFiller;	
	std::string myFileName;	
		
	//Create as many balls as there are names - for each name, generate x ball objects in the vector, fetch string value and put in each created object

	for(int x=0;x<numNames;x++){
		std::cout << "Input name: ";
		std::cin >> nameFiller;
		
		std::cout << "Input position (1) REF-ONLY; (2) CIRC-ONLY; (3) REF/CIRC: ";
		std::cin >> thisBallType;
		
		timeConverterPrinter(libOpen, libClose, ampm);		
		
		//start/end times
		std::cout << "Enter hour which this person starts (e.g. 1 = 1st hour, 2 = 2nd hour, etc.): ";
		std::cin >> entryHour;		
		
		std::cout << "Enter hour which this person ends (e.g. 6 = 6th hour, 8 = 8th hour, etc.): ";
		std::cin >> exitHour;
		
		//enter times which this person is unavailable (if any), if none enter "0".
		do{
			std::cout << "Enter hours which this person is unavailable. If none enter 0. When finished, enter 0.";
			std::cin >> unavailableHour;			
			unavailableHours.push_back(unavailableHour);
		}while(unavailableHour != 0);			
		
		for(int i=0;i<b;i++){	
			Balls *ballptr = new Balls;
			ballptr->getLoad(nameFiller);
			ballptr->setType(thisBallType);
			ballptr->getStartHour(entryHour);
			ballptr->getEndHour(exitHour);
			ballptr->getUnavailableHours(unavailableHours);
			ballSet.emplace_back(*ballptr); //push back object vector with ball element
			delete ballptr;
		}	
	}
	//Main do/while loop. User can reroll names if she so desires
	do{	
		//clear all hoppers before starting loop
		for (int clearAllHoppers=0;clearAllHoppers<m;clearAllHoppers++){
			hopperSet[clearAllHoppers].fillHopper("",1);
			hopperSet[clearAllHoppers].fillHopper("",2);
		}				
		//run randomBallBounce for each ball in ballSet
		for (int j=0;j<totalBalls;j++){
			randomBallBounce(hopperSet, ballSet, m , j, 0);
		}			
		//for each hopper, check and see if it has empty slots
		//if it is empty, find an unused ball, check if it is valid for that slot, if it is, put it in the slot
		
		for (int cleanUp=0;cleanUp<m;cleanUp++){
			emptyHopperFiller(hopperSet, ballSet, cleanUp, b);
		}
		
		//Show type 1 hoppers
		for(int k=0;k<m;k++){
			std::cout << hopperSet[k].dumpHopper() << " , ";	
		}	
		std::cout << std::endl;	
		//Show type 2 hoppers
		for(int v=0;v<m;v++){
			std::cout << hopperSet[v].dumpHopper2() << " , ";	
		}	
		std::cout << std::endl;	
		std::cout << "Process terminated. Reroll (Y/N)?: ";
		std::cin >> choose;
	}while(choose == 'Y' || choose == 'y');	

	//ability to save to file
	std::cout << "Save output to file? (Y/N): ";
	std::cin >> save;
	if(save == 'Y' || save =='y'){
		std::cout << "Name of file?: ";
		std::cin >> myFileName;
		myFile.open(myFileName + ".csv");
		myFile << "INFO;";
		for(int output1=0;output1<m;output1++){
			myFile << hopperSet[output1].dumpHopper() << ";";	
		}	
		myFile << '\n';	
		//Dump result of ball bounce and empty the type-2 hoppers
		myFile << "CIRC;";
		for(int output2=0;output2<m;output2++){
			myFile << hopperSet[output2].dumpHopper2() << ";";	
		}	
		std::cout << "Output saved as" << myFileName << ".csv";
		myFile.close();
	}
	else {
		return 0;
	}
}


