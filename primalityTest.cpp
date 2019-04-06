#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <ctype.h>

using namespace std;
//void increment();
//void decrement();
void primalityTest(string);

class BCD                       //Binary-coded digit
{
        public:
        BCD() = default;

        BCD(char input)
        {
        		//Initializing the binary-coded digit
                if(input == '1')
                        digit[0] = true;

                else if(input == '2')
                        digit[1] = true;

                else if(input == '3')
                {
                        digit[0] = true;
                        digit[1] = true;
                }

                else if(input == '4')
                        digit[2] = true;

                else if(input == '5')
                {
                        digit[0] = true;
                        digit[2] = true;
                }

                else if(input == '6')
                {
                        digit[1] = true;
                        digit[2] = true;
                }

                else if(input == '7')
                {
                        digit[0] = true;
                        digit[1] = true;
                        digit[2] = true;
               }

                else if(input == '8')
                        digit[3] = true;

                else if(input == '9')
                {
                        digit[0] = true;
                        digit[3] = true;
                }
        };

        bitset<4> digit;
        bitset<1> adder(BCD addend, bitset<1> carry_in);
        void printBCD(void);
        void zeroOut(void);
        bool isZero();
        bool isOdd(); //not using yet but will come in handy!!!
        int compare(BCD other);
};

BCD estimate_msd_sqrt_n(BCD, int); //Function declaration (stray)
BCD nineBin('9');

int BCD::compare(BCD other)
{
	if((digit[0] == other.digit[0]) & (digit[1] == other.digit[1])
		& (digit[2] == other.digit[2]) & (digit[3] == other.digit[3]))
	{
		return 0;
	}

	for(int i = 3; i >= 0; i--)
	{
		if(digit[i] == other.digit[i])
			continue;

		if(digit[i] == 1)
			return 1;
		else
			return -1;
	}

	return 0;
}

bool BCD::isZero()
{
	return (~digit[0] & ~digit[1] & ~digit[2] & ~digit[3]);
}

bool BCD::isOdd()
{
	return digit[0];
}

void BCD::zeroOut()
{
	digit[0] = 0;
	digit[1] = 0;
	digit[2] = 0;
	digit[3] = 0;
}

bitset<1> BCD::adder(BCD addend, bitset<1> carry_in)
{
        bitset<1> tmp, carry_out;

        for(int i = 0; i < 4; i++)
        {
            tmp[0] = digit[i];
            digit[i] = (tmp[0] ^ addend.digit.test(i)) ^ carry_in.test(0);
            carry_out[0] = (tmp[0] & addend.digit.test(i)) |
                       (carry_in.test(0) & (tmp[0] ^ addend.digit.test(i)));

            carry_in[0] = carry_out[0];
        }

        //If binary-coded digit > 9 then add 6
        if(digit[3] == 1)
        {
        	if(digit[2] == 1 || digit[1] == 1)
            {
        		bitset<4> sixBin('6');
        		carry_in[0] = 0;

            	for(int i = 0; i < 4; i++)
            	{
        	        tmp[0] = digit[i];
        	       	digit[i] = (tmp[0] ^ sixBin.test(i)) ^ carry_in.test(0);

         			carry_out[0] = (tmp[0] & sixBin.test(i)) |
   			                       (carry_in.test(0) & (tmp[0] ^ sixBin.test(i)));

        	        carry_in[0] = carry_out[0];
              	}

            	 carry_out[0] = 1;
            }
        }

        return carry_out;
}

//For testing*
void BCD::printBCD()
{
        for(int i = 3 ; i >= 0; i--)
        {
                cout << digit[i] << " ";
        }
        cout << "   ";
}



class BCN                       //Full binary-coded number
{
        private:
        BCD* n;
        int n_length;

        public:
        BCN(int length)
        {
        	n_length = length;
        	n = new BCD[length];
        }

        BCN(BCD* input, int length)
        {
        	n_length = length;
        	n = input;
        };

        BCN(string n_str) : n(new BCD[n_length = n_str.length()])
        {
        		int str_index = 0;
                for(int i = (n_length - 1); i >= 0; i--)
                {
                        BCD binDigit(n_str[str_index]);
                        n[i] = binDigit;
                        str_index++;
                }
        };

        bitset<1> carry_flag, borrow_flag;

        void setDigitAt(int position, BCD digit);
        BCD getDigitAt(int position);  		//Returns binary-coded digit at index
        void printBCN(void);
        void adder(BCN addend);
        void subtractor(BCN &subtrahend, int start, int end);
        int getLength(void);
        bool isOdd(); //not using yet but will come in handy!!!
        BCD get_msd();
        void swapDigits(int index_a, int index_b);
        void swapDigits(int index_a1, int index_a2, int index_b1, int index_b2); //Swaps a range of digits in n
};


bool BCN::isOdd()
{
	return n[0].isOdd();
}

BCD BCN::get_msd()
{
	return n[n_length -1];
}

void BCN::setDigitAt(int position, BCD digit)
{
		n[position] = digit;
}

BCD BCN::getDigitAt(int position)
{
        return n[position];
}

int BCN::getLength()
{
        return n_length;
}

//Swaps two indices in a BCD
void BCN::swapDigits(int index_a, int index_b)
{
	BCD temp;
	temp = n[index_a];
	n[index_a] = n[index_b];
	n[index_b] = temp;
}

//Sum is stored in n
void BCN::adder(BCN addend)
{
		for(int i = 0; i < addend.getLength() || (carry_flag == 1) ; i++)
        {
                carry_flag = n[i].adder(addend.getDigitAt(i), carry_flag);
        }
        carry_flag[0] = 0;
}

//Difference is stored in n
void BCN::subtractor(BCN &subtrahend, int start, int end)
{
	int comparison;
	bitset<5> temp;
	BCD subDigit;
	bool degreeCheck = true;  //NEEDS WORK!!

	for(int i = start; i <= end || (borrow_flag == 1); i++)
	{
		subDigit = subtrahend.getDigitAt(i);
		comparison = n[i].compare(subDigit);

		if(comparison == 0)
			n[i].zeroOut();

		else if(comparison > 0)
		{
			for(int j = 0; j < 4; j++)
			{
				if(!(n[i].digit[j]) & subDigit.digit[j])
				{
					borrow_flag[0] = 1;
					n[i].digit[j] = 1;

					//Perform borrows inside binary-coded digit
					for(int k = (j + 1); k < 4; k++)
					{
						if(n[i].digit[k] == 0)
							n[i].digit[k] = 1;

						else
						{
							n[i].digit[k] = 0;
							break;
						}
					}
				}

				temp[0] = n[i].digit[j];
				n[i].digit[j] = ((temp[0] & ~subDigit.digit[j]) | borrow_flag[0]);

				borrow_flag = 0;
			}
		}

		//Subtrahend digit is greater than minuend digit
		else
		{
			/*Subtract 1 from next, higher-degree, nonzero digit - If BCD = 0 then BCD = 9
			 * i.e. - Perform borrow(s)*/
			if(degreeCheck)
			{
				int borrow_index = i;
				do
				{
					borrow_index++;

					n[borrow_index].digit[0] = ~n[borrow_index].digit[0];
					temp[1] = (n[borrow_index].digit[1] & ~n[borrow_index].digit[0]) |
							(n[borrow_index].digit[2] & ~n[borrow_index].digit[1] & n[borrow_index].digit[0])
							| (n[borrow_index].digit[3] & n[borrow_index].digit[0]);
					temp[2] = (n[borrow_index].digit[2] & ~n[borrow_index].digit[0]) |
							(n[borrow_index].digit[2] & n[borrow_index].digit[1]) |
							(n[borrow_index].digit[3] & n[borrow_index].digit[0]);
					temp[3] = (~n[borrow_index].digit[3] & ~n[borrow_index].digit[2] &
							~n[borrow_index].digit[1] & n[borrow_index].digit[0]) |
							(n[borrow_index].digit[3] & ~n[borrow_index].digit[0]);

					//n[borrow_index].digit[0] = temp[0];
					n[borrow_index].digit[1] = temp[1];
					n[borrow_index].digit[2] = temp[2];
					n[borrow_index].digit[3] = temp[3];

				}
				while(degreeCheck && (n[borrow_index].digit[0] &
									  ~n[borrow_index].digit[1] &
									  ~n[borrow_index].digit[2] &
									  n[borrow_index].digit[3]));
			}

			//Add 10 and hold digit in temp
			temp[4] = (n[i].digit[2] & n[i].digit[1]) | n[i].digit[3];
			temp[3] = ~n[i].digit[3] & (~n[i].digit[2] | ~n[i].digit[1]);
			temp[2] = (~n[i].digit[2] & n[i].digit[1]) | (n[i].digit[2] & ~n[i].digit[1]);
			temp[1] = ~n[i].digit[1];
			temp[0] = n[i].digit[0];

			//Subtract minuend digit from temp and store difference in n
			for(int j = 0; j < 4; j++)
			{
				if(~(temp[j]) & subDigit.digit[j])
				{
					borrow_flag[0] = 1;
					temp[j] = 1;

					//Perform borrows inside binary-coded digit
					for(int k = (j + 1); k < 4; k++)
					{
						if(temp[k] == 0)
							temp[k] = 1;

						else
						{
							temp[k] = 0;
							break;
						}
					}
				}

				n[i].digit[j] = ((temp[j] & ~subDigit.digit[j]) | borrow_flag[0]);

				borrow_flag = 0;
			}
		}
	}

	borrow_flag[0] = 0;
}

//For testing*
void BCN::printBCN()
{
        for(int i = (n_length - 1); i >= 0; i--)
        {
                n[i].printBCD();
        }
        cout << endl;
}



int main()
{
        primalityTest("testprime.txt");

        return 0;
}

void primalityTest(string inputPath)								//running estimate: O(log(n) + sqrt(n)log(n))
{
	int length_n = 0, length_m = 1, length_m_max;
	char nChar;

	ifstream inputFile(inputPath, std::ifstream::in);

	//Iterate through file, counting length of test prime			O(length(n)) = O(log(n))
	while(inputFile.get(nChar))
	{
		if(!isdigit(nChar))
			continue;

		length_n++;
	}

	inputFile.clear();					//Clear EOF flag
	inputFile.seekg(0, std::ios::beg); 	//Return to beginning

	//Number which is being tested for primality as an array of binary coded digits
	BCN n(length_n);

	//Building n													O(length(n)) = O(log(n))
	int i = length_n - 1;
	while(inputFile.get(nChar))
	{
		if(!isdigit(nChar))
					continue;

		n.setDigitAt(i, BCD(nChar));
		i--;
	}

	inputFile.close();

	//testing****************
	cout << "n = ";
	n.printBCN();
	//***********************

	//Calculating maximum length of m
	if(length_n % 2 > 0)
		length_m_max = (length_n + 1) / 2;
	else
		length_m_max = length_n / 2;

	//Initializing m = 3
	BCN m(length_n);
	m.setDigitAt(0, BCD('3'));

	//testing****************
	m.setDigitAt(1, BCD('0'));
	m.setDigitAt(2, BCD('2'));
	length_m = 3;
	cout << "\nm = ";
	m.printBCN();
	//***********************

	//Estimating upper bound of m
	BCD msd_sqrt_n = estimate_msd_sqrt_n(n.get_msd(), length_n);	//Estimating msd(sqrt(n))
	BCN m_max(length_m_max);
	m_max.setDigitAt(length_m_max - 1, msd_sqrt_n);
	for(int i = 0; i < length_m_max - 1; i++)
	{
		m_max.setDigitAt(i, nineBin);
	}

	/*Testing if m divides n*/
	//Swapping digits in m to build a number equivalent to m * 10^(length(n) - 1) - *INITIAL SWAP*
	for(int i = length_m - 1, j = 1; i >= 0; i--, j++)
	{
		m.swapDigits(i, length_n - j);
	}

	//testing******
	cout << "\nm after swaps: ";
	m.printBCN();

	int headIndex = length_n - 1;
	int subIndex = length_n - length_m;
	int bcdCompare = n.getDigitAt(headIndex).compare(m.getDigitAt(headIndex));

	if(bcdCompare == 0 || bcdCompare == 1)
	{
		do
		{
			n.subtractor(m, subIndex, headIndex);
			bcdCompare = n.getDigitAt(headIndex).compare(m.getDigitAt(headIndex));
		}
		while(bcdCompare == 0 || bcdCompare == 1);
	}
	else
	{
		if(!(subIndex == 0))
		{
			for(int i = subIndex; i < length_n; i++)
			{
				m.swapDigits(i, i - 1);
			}
			subIndex--;
		}
		else
		{
			/*if there is no digit in a higher degree and n = 0
				then m divides n
			 *if there is no digit in a higher degree and digit at headIndex in m
			 	 is greater than digit at headIndex in n, then m does not divide n*/
		}
	}

/*
	//testing
	cout << "length_n: " << length_n << endl;
	cout << "length_m: " << length_m << endl;
	cout << "length_m_max: " << length_m_max << endl;

	BCN test(1);
	test.setDigitAt(0, BCD('3'));
	for(int i = 0; i < 187618446; i++)
		n.subtractor(test, 0, 0);

	n.printBCN();
*/

	//testing
	cout << "\nn = ";
	n.printBCN();
	cout << "length_n: " << length_n << endl;
	cout << "\nm = ";
	m.printBCN();
	cout << "\nlength_m_max: " << length_m_max << endl;
	cout << "m_max = ";
	m_max.printBCN();

	//Subtractor test
//	cout << "\nSubtractor test: (n - (m * 10^(length(n) - 1))\n";
//	n.subtractor(m, length_n - 1, length_n - 1);
//	n.printBCN();


/*	//Adder test
	cout << "\n\nadder test: (n + m)\n";
	n.adder(m);
	n.printBCN();

	//Swap test
	n.swapDigits(0, 2);
	cout << endl << "\nswapDigits() test: " << endl;
	n.printBCN();

	//BCD compare test
	cout << endl << "\ncompare() test: " << endl;
	BCD a('5');
	BCD b('4');
	int compTest = a.compare(b);
	cout << "a: ";
	a.printBCD();
	cout << endl << "b: ";
	b.printBCD();
	cout << endl << "compTest a.compare(b): " << compTest << endl;


	//BCN subtractor test
	cout << "\n\nsubtractor() test:\n  ";
	BCN x("6905");
	x.printBCN();
	BCN y("999");
	cout << "- \t     ";
	y.printBCN();
	x.subtractor(y, 0 , y.getLength() - 1);
	cout << "= ";
	x.printBCN();
*/
}

BCD estimate_msd_sqrt_n(BCD msd_n, int length_n)
{
	bool length_n_odd = (length_n % 2) > 0;

	if(msd_n.digit[0] & ~msd_n.digit[1] & ~msd_n.digit[2] & ~msd_n.digit[3])
	{
		if(length_n_odd)
			return BCD('1');
		else
			return BCD('4');
	}

	else if(~msd_n.digit[0] & msd_n.digit[1] & ~msd_n.digit[2] & ~msd_n.digit[3])
	{
		if(length_n_odd)
			return BCD('1');
		else
			return BCD('5');
	}

	else if(msd_n.digit[0] & msd_n.digit[1] & ~msd_n.digit[2] & ~msd_n.digit[3])
	{
		if(length_n_odd)
			return BCD('1');
		else
			return BCD('6');
	}

	else if(~msd_n.digit[0] & ~msd_n.digit[1] & msd_n.digit[2] & ~msd_n.digit[3])
	{
		if(length_n_odd)
			return BCD('2');
		else
			return BCD('7');
	}

	else if(msd_n.digit[0] & ~msd_n.digit[1] & msd_n.digit[2] & ~msd_n.digit[3])
	{
		if(length_n_odd)
			return BCD('2');
		else
			return BCD('7');
	}

	else if(~msd_n.digit[0] & msd_n.digit[1] & msd_n.digit[2] & ~msd_n.digit[3])
	{
		if(length_n_odd)
			return BCD('2');
		else
			return BCD('8');
	}

	else if(msd_n.digit[0] & msd_n.digit[1] & msd_n.digit[2] & ~msd_n.digit[3])
	{
		if(length_n_odd)
			return BCD('2');
		else
			return BCD('8');
	}

	else if(~msd_n.digit[0] & ~msd_n.digit[1] & ~msd_n.digit[2] & msd_n.digit[3])
	{
		if(length_n_odd)
			return BCD('2');
		else
			return BCD('9');
	}
	else
	{
		if(length_n_odd)
			return BCD('3');
		else
			return BCD('9');
	}
}
