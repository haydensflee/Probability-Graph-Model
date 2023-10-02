#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
using namespace std;


int main(int argc, char **argv)
{
	string input1;
	string input2;
	input1=argv[1];
	input2=argv[2];
	string line;

	string tempInput;
	vector<string> stringData;
	vector<string> stringQuery;
	vector<string> varNames;
	vector<char> tempCharVec;
	string inputVar;
	vector<int> spaceIndex;
	vector<int> tempIntVec;
	vector<int> matVector;
	vector<int> conditionIndexVec;
	vector<string> conditionNameVec;
	int tempInt;
	vector<int> coord;
	vector<vector<int>> trueFalseVecVec;
	string probVarString;
	int probVarIndex;
	float outputFloatArray[2]={0,0};
	int counter=0;
	int indexChoiceOne=-1;
	int indexChoiceTwo=-1;
	int remainder;
	int binaryNum=0;
	int decimalNum=0;
	vector<vector<int>> trueFalseVecVecTemp;
	vector<vector<int>> trueFalseVecVecSingle;
	vector<int> counterVec;
	bool entryExists=false;
	char c;
	bool input=true;

	vector<float> randomNumber;
    // Reading in first file
	ifstream inFile;
	inFile.open(input1);
	if (!inFile) {
		exit(1);
	}
	while(inFile)
	{
		inFile>>line;
		inFile.get(c);
		stringData.push_back(line);
	}
	
	inFile.close();
	inFile.open(input2);
	if (!inFile) {
		//cout << "Unable to open file";
		exit(1);
	}
	while(inFile)
	{

		inFile>>line;
		inFile.get(c);
		stringQuery.push_back(line);
	}
	inFile.close();
	int N=stoi(stringData[0]);
	vector<int> parentVec[N];
	for (int i=1; i<1+N; i++)
	{
		varNames.push_back(stringData[i]);
	}

	for(int i=1+N; i<1+N+N*N; i+=N)
	{
		for(int j=i; j<i+N; j++)
		{
			matVector.push_back(stoi(stringData[j]));
		}
	}

	//		GET COORDINATES OF THE PARENT CHILD CONNECTIONS FROM MATRIX
	for(int i=0; i<N; i++)
	{
		int k=N*i;
		for(int j=0; j<N; j++)
		{
			tempInt=matVector[k+j];
			if(tempInt==1)
			{
				coord.push_back(i);
				coord.push_back(j);
			}
		}	
	}


	//		SET UP ARRAY OF VECTORS THAT CONTAIN INDEX TO PARENTS
	for(int i=0; i<N; i++)
	{
		parentVec[i].push_back(-1);
	}

	for(int i=1; i<coord.size(); i+=2)
	{
		parentVec[coord[i]].push_back(coord[i-1]);
	}
	int zeroPCount=0;
	int onePCount=0;
	int twoPCount=0;
	int threePCount=0;
	int fourPCount=0;
	int fivePCount=0;

	vector<float> zeroCPT;
	vector<float> oneCPT;
	vector<float> twoCPT;
	vector<float> threeCPT;
	vector<float> fourCPT;
	vector<float> fiveCPT;

	vector<float> tempCPT;
	vector<vector<float>> allCPT;
	vector<int> numberOfParents;
	for(int i=1; i<coord.size(); i+=2)
	{
		if(coord[i]==0)
		{
			zeroPCount++;
		}
		else if(coord[i]==1)
		{
			onePCount++;
		}
		else if(coord[i]==2)
		{
			twoPCount++;
		}
		else if(coord[i]==3)
		{
			threePCount++;
		}
		else if(coord[i]==4)
		{
			fourPCount++;
		}
		else if(coord[i]==5)
		{
			fivePCount++;
		}
	}
	numberOfParents.push_back(zeroPCount);
	numberOfParents.push_back(onePCount);
	numberOfParents.push_back(twoPCount);
	numberOfParents.push_back(threePCount);
	numberOfParents.push_back(fourPCount);
	numberOfParents.push_back(fivePCount);

	int a=1+N+N*N;


	//		SET UP CONDITIONAL PROBABILITY TABLES
	for(int i=0; i<N; i++)
	{

		tempCPT.clear();
		int m=numberOfParents[i];
		for(int j=a; j<a+pow(2,m)*2; j++)
		{
			tempCPT.push_back(stof(stringData[j]));
		}
		a=a+pow(2,m)*2;
		allCPT.push_back(tempCPT);
	}
	float r;
	//		GETTING RID OF "P=" FROM QUERY
	string tempString="";
	string appendString="";
	for(int i=2; i<stringQuery[0].size(); i++)
	{
		appendString=stringQuery[0].at(i);
		tempString.append(appendString);
	}
	probVarString=tempString;
	vector<int> trueFalseVec;
	//		RANDOM NUMBER GENERATION
	int samples = 1000000;
	for(int i=0; i<N*samples; i++)
	{
		trueFalseVec.push_back(-1);
		r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		randomNumber.push_back(r);

		//		IF CPT IS ONLY SIZE 2 (NO PARENTS) IMMEDIATELY PERFORM THE TRUE/FALSE LOGIC
		if(allCPT[i % N].size()==2)
		{
			if(r<allCPT[i % N][0])
			{
				trueFalseVec[i]=1;
			}
			else
			{
				trueFalseVec[i]=0;
			}
		}
	}
	//		ISOLATING CONDITION STRINGS
	inputVar=tempString;
	char tf='-';
	for (int i=2; i<stringQuery.size()-1; i++)
	{
		tempString="";
		for(int j=0; j<stringQuery[i].size(); j++)
		{
			appendString=stringQuery[i].at(j);
			if(appendString!="=")
			{
				if(true)
				{
					tempString.append(appendString);
				}
			}
			else			// if there's an "=" sign
			{
				tf=stringQuery[i][j+1];
				for(int k=0; k<N; k++)
				{
					if(tempString==varNames[k])
					{
						if(tf=='t')
						{
							for(int l=k; l<N*samples; l+=N)
							{
								trueFalseVec[l]=1;
							}
							
						}
						else if(tf=='f')
						{
							for(int l=k; l<N*samples; l+=N)
							{
								trueFalseVec[l]=0;
							}
						}	
					}
				}
				break;
			}
		}
		conditionNameVec.push_back(tempString);
	}

	tempInt;


	//		SEPARATE THE EXTREMELY LONG VECTOR OF TRUE/FALSE INTO THEIR RESPECTIVE SAMPLES
	for(int i=0; i<samples; i++)
	{
		tempIntVec.erase(tempIntVec.begin(),tempIntVec.end());
		for(int j=0; j<N; j++)
		{
			tempIntVec.push_back(trueFalseVec[i*N+j]);
		}
		trueFalseVecVecTemp.push_back(tempIntVec);
	}
	

	//		LOGIC FOR READING IN PARENT TRUE/FALSE VALUES TO USE TO READ CPT AND COMPLETELY FILL OUT TRUE/FALSE COMBINATION
	for(int i=0; i<samples; i++)
	{
		tempIntVec=trueFalseVecVecTemp[i];
		for(int a=0; a<N; a++)
		{
			binaryNum=0;
			decimalNum=0;
			if(tempIntVec[a]==-1)
			{
				for(int j=1; j<parentVec[a].size(); j++)
				{
					tempInt = parentVec[a][j];
					binaryNum = binaryNum+tempIntVec[tempInt] * pow(10,parentVec[a].size()-j-1);
				}
				int temp = binaryNum;
				int base =1;
				if(true)
				{
					while (temp) {
						int last_digit = temp % 10;
						temp = temp / 10;

						decimalNum += last_digit * base;

						base = base * 2;
					}
				}
				indexChoiceOne=2*decimalNum;
				indexChoiceTwo=2*decimalNum+1;	

				if(randomNumber[i*N + a]<allCPT[a % N][indexChoiceOne])
				{
					trueFalseVec[i*N + a]=1;
				}
				else
				{
					trueFalseVec[i*N + a]=0;
				}
			}
		}
		
		if(i%N==N-1)
		{
			counter++;
		}
	}
	for(int i=0; i<samples; i++)
	{
		tempIntVec.erase(tempIntVec.begin(),tempIntVec.end());
		for(int j=0; j<N; j++)
		{
			tempIntVec.push_back(trueFalseVec[i*N+j]);
		}
		trueFalseVecVec.push_back(tempIntVec);
	}

	tempIntVec.erase(tempIntVec.begin(),tempIntVec.end());
	tempIntVec.push_back(-1);

	counter=0;

	//		LOGIC FOR CREAING A VECTOR WITH ONLY ONE COPY OF EACH COMBINATION OF TRUES/FALSES
	for(int i=0; i<samples; i++)
	{
		entryExists=false;
		tempIntVec=trueFalseVecVec[i];

		if(i==0)
		{
			trueFalseVecVecSingle.push_back(tempIntVec);
			counterVec.push_back(1);
		}
		for(int j=0; j<trueFalseVecVecSingle.size(); j++)
		{
			if(tempIntVec==trueFalseVecVecSingle[j])
			{
				entryExists=true;
				tempInt=j;
			}
		}
		if(entryExists)
		{
			counterVec[tempInt]++;
			if(i==0)
			{
				counterVec[tempInt]--;
			}
		}
		else
		{
			trueFalseVecVecSingle.push_back(tempIntVec);
			counterVec.push_back(1);
		}		
	}
	for(int i=0; i<N; i++)
	{
		for(int j=0; j<conditionNameVec.size(); j++)
		{
			if(conditionNameVec[j]==varNames[i])
			{
				conditionIndexVec.push_back(i);
			}
		}
	}

	//			WEIGHTS CALCULATIONS
	binaryNum=0;
	decimalNum=0;
	float weight=1;
	float tempWeight=0;
	vector<float> weightVec;
	float weightArray [trueFalseVecVecSingle.size()];
	entryExists=false;
	for(int i=0; i<samples; i++)
	{
		entryExists=false;
		tempIntVec=trueFalseVecVec[i];
		weight=1;
		for(int j=0; j<conditionIndexVec.size(); j++)
		{
			binaryNum=0;
			decimalNum=0;
			for(int k=1; k<parentVec[conditionIndexVec[j]].size(); k++)
			{
				tempInt = parentVec[conditionIndexVec[j]][k];
				binaryNum = binaryNum+tempIntVec[tempInt] * pow(10,parentVec[conditionIndexVec[j]].size()-k-1);

			}
			int temp = binaryNum;
			int base =1;
			if(true)
			{
				while (temp) {
					int last_digit = temp % 10;
					temp = temp / 10;

					decimalNum += last_digit * base;

					base = base * 2;
				}
			}
			

			indexChoiceOne=2*decimalNum;
			indexChoiceTwo=2*decimalNum+1;
			indexChoiceOne=indexChoiceTwo-tempIntVec[conditionIndexVec[j]];

			tempWeight=allCPT[conditionIndexVec[j]][indexChoiceOne];

			weight=weight*tempWeight;
		}
		
		weightVec.push_back(weight);
		for(int j=0; j<trueFalseVecVecSingle.size(); j++)		
		{

			if(tempIntVec==trueFalseVecVecSingle[j])
			{
				weightArray[j]=weight;
			}
		}
	}


	
	for(int i=0; i<N; i++)
	{
		if(probVarString==varNames[i])
		{
			probVarIndex=i;
		}
	}
	for(int i=0; i<trueFalseVecVecSingle.size(); i++)
	{
		
		tempInt=trueFalseVecVecSingle[i][probVarIndex];
		outputFloatArray[tempInt] = outputFloatArray[tempInt] + counterVec[i] * weightArray[i];
		

	}
	tempInt=outputFloatArray[0]+outputFloatArray[1];

	//		EASY PRINTING FOR DEBUGGING
	// for(int i=0; i<trueFalseVecVecSingle.size(); i++)
	// {
	// 	cout<<counterVec[i]<<" x ";
	// 	for(int j=0; j<trueFalseVecVecSingle[i].size(); j++)
	// 	{
	// 		cout<<trueFalseVecVecSingle[i][j];
	// 	}
	// 	cout<<" with weight "<<weightArray[i];
	// 	cout<<endl;
	// }

	cout<<outputFloatArray[1]/tempInt<<" "<<1-outputFloatArray[1]/tempInt;

	return 0;
}