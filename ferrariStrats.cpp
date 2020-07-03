#include <iostream>
#include <string>
#include <sstream>
#include <vector>

/*
	F1 STRATEGY PLANNER
	Enter avg. lap times, time to make a pitstop, and the num. of laps and let the program tell you what tyre strategy to take.
*/

struct GrandPrix
{
	int laps = 0, bestComb;
	float bestTime, thisTime = 0.0, pitTime = 0.0;
	std::string stringTimes[3];
	std::vector<std::vector<int>> strats;
	float times[3] = {0.0, 0.0, 0.0};
	unsigned short durability[3] = {15, 25, 40}; // laps
};

void enterData(GrandPrix &gp)
{
	std::cout << "Enter your avg. lap times for soft, medium and hard tyres; eg. [1:43:872].\n";
	std::getline(std::cin, gp.stringTimes[0]);
	std::getline(std::cin, gp.stringTimes[1]);
	std::getline(std::cin, gp.stringTimes[2]);
	//std::cin >> gp.stringTimes[0] >> gp.stringTimes[1] >> gp.stringTimes[2];
	std::cout << "Enter avg. time to drive through the pits: ";
	std::cin >> gp.pitTime;
	std::cout << "Enter number of laps: ";
	std::cin >> gp.laps;

	for(int i = 0; i < 3; i++)
		std::cout << gp.stringTimes[i] << ' ';
	std::cout << '\n';
}

void stringToTime(GrandPrix &gp)
{
	std::string stringFraction[3][3] = {"","",""};
	float fraction[3][3];
	for(int i = 0, j = 0, k = 0; ;)
	{
		if(gp.stringTimes[j][k] == ':')
		{
			i++;
			k++;
		}
		if(k == gp.stringTimes[j].length())
		{
			k = 0;
			j++;
			i = 0;
			if(j == 3) break;
		}
		stringFraction[j][i] += gp.stringTimes[j][k];
		k++;
	}

	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
		{
			std::stringstream fracStream(stringFraction[i][j]);
			fracStream >> fraction[i][j];
			if(j == 0) gp.times[i] += (fraction[i][j] * 60);
			else if(j == 1) gp.times[i] += fraction[i][j];
			else gp.times[i] += (fraction[i][j] * 0.001);
		}

	std::cout << "\nTimes for tyres in fract.: ";
	for(int i = 0; i < 3; i++)	std::cout << gp.times[i] << "  ";
	std::cout << '\n';
}

void reverseVector(std::vector<int> &vect, int start, int end)
{
	while(start < end)
	{
		int temp = vect[start];
		vect[start] = vect[end];
		vect[end] = temp;
		start++;
		end--;
	}
}

void makeCombinations(GrandPrix &gp) // dela (perhaps)
{
	for(int i = 0; i < 3; i++)
	{
		int lapsLeft = gp.laps, chosenTyre = i;
		std::vector<int> tempStrat;
		std::vector<int> reverseTempStrat;
		if(lapsLeft - gp.durability[chosenTyre] <= 0)	tempStrat.push_back(chosenTyre);
		else
		{
			while(lapsLeft > 0)
			{
				tempStrat.push_back(chosenTyre);
				reverseTempStrat.push_back(chosenTyre);
				lapsLeft -= gp.durability[chosenTyre];
				if(lapsLeft <= 10) break; // better have more wear on the tyre than go for a pitstop
				else if(lapsLeft > 10  && lapsLeft <= gp.durability[0]) chosenTyre = 0;
				else if(lapsLeft > gp.durability[0] && lapsLeft < gp.durability[1]) chosenTyre = 1;
				else chosenTyre = 2;
			}
		}
		reverseVector(reverseTempStrat, 0, reverseTempStrat.size()-1);
		gp.strats.push_back(tempStrat);
		gp.strats.push_back(reverseTempStrat);
	}

	for(int i = 0; i < gp.strats.size(); i++)
	{
		for(int j = 0; j < gp.strats[i].size(); j++)
			std::cout << gp.strats[i][j] << ' ';
		std::cout << '\n';
	}
}

void getBestTime(GrandPrix &gp)
{
	for(int i = 0; i < 3; i++) // gets best time for soft, med, or hard only
	{
		int lap = 1;
		float durabilityWear = 0.1, wear = 0.0;
		gp.thisTime = 0.0;
		for(int k = 0; lap < gp.laps; k++)
		{
			if(k > gp.durability[i])
			{
				if((gp.laps - lap) <= 10)
				{
					gp.thisTime += gp.times[i] + durabilityWear;
					durabilityWear += 0.1;
				}
				else
				{
					gp.thisTime += gp.times[i] + gp.pitTime;
					k = 0;
				}
			}
			else	gp.thisTime += gp.times[i] + wear;
			lap++;
			wear += 0.025;
		}
		if(i == 0 || gp.bestTime > gp.thisTime) gp.bestTime = gp.thisTime;
		if(i == 0) std::cout << "Time with only SOFT: " << gp.thisTime;
		else if(i == 1) std::cout << "Time with only MEDIUM: " << gp.thisTime <<'\n';
		else std::cout << "Time with only HARD: " << gp.thisTime << '\n';
	}

	for(int i = 0; i < gp.strats.size(); i++) // switcha vrstice
	{
		gp.thisTime = 0.0;
		int lap = 1;
		for(int j = 0; j < gp.strats[i].size(); j++) // gre cez vrstico
		{
			float durabilityWear = 0.1, wear = 0.0, stintTime = 0.0;
			for(int k = 1; lap <= gp.laps; k++)
			{
				if(k >= gp.durability[gp.strats[i][j]])
				{
					if((gp.laps - lap) <= 10)
					{
						stintTime += gp.times[gp.strats[i][j]] + durabilityWear;
						durabilityWear += 0.1;
					}
					else break;
				}
				else	stintTime += gp.times[gp.strats[i][j]] + wear;
				wear += 0.025;
				lap++;
			}
			gp.thisTime += stintTime + gp.pitTime;
		}
		std::cout << i+1 << ". kombinacija: " << gp.thisTime << '\n';
		if(i == 0 || gp.bestTime > gp.thisTime)
		{
			gp.bestTime = gp.thisTime;
			gp.bestComb = i;
		}
	}
}

void printBestCombination(GrandPrix &gp)
{
	std::cout << "\nThe best combination of tyres is:";
	for(int j = 0; j < gp.strats[gp.bestComb].size(); j++)
	{
		if(gp.strats[gp.bestComb][j] == 0) std::cout << " SOFT ";
		else if(gp.strats[gp.bestComb][j] == 1) std::cout << " MEDIUM ";
		else std::cout << " HARD ";
	}
	std::cout << "\nWith the time of: " << gp.bestTime << '\n';
}

int main()
{
	// AUS GP
	/* time for pitstop = 23s
	   58 laps */
	GrandPrix gp;
	enterData(gp);
	stringToTime(gp);
	makeCombinations(gp);
	getBestTime(gp);
	printBestCombination(gp);
	return 0;
}