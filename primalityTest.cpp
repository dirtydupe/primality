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
        bitset<4> sixBin;
        bitset<1> adder(BCD addend, bitset<1> carry_in);
        void printBCD(void);
        void zeroOut(void);
  //      void setBitToOne(int index);	//implement*** NEEDED???????
        //void subtractor(BCD minuend, BCD subtrahend);
        int compare(BCD other);
};

int BCD::compare(BCD other) // TODO
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
        	//**testing**
        	cout << "BEFORE:\tdigit[" << i << "]: " << digit[i] << endl;

            tmp[0] = digit[i];
            digit[i] = (tmp[0] ^ addend.digit.test(i)) ^ carry_in.test(0);
            carry_out[0] = (tmp[0] & addend.digit.test(i)) |
                       (carry_in.test(0) & (tmp[0] ^ addend.digit.test(i)));

            //**testing**
            cout << "AFTER:\tdigit[" << i << "]: " << digit[i] << endl;

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

        cout << endl; //for testing****
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
        void swapDigits(int index_a, int index_b);
        void swapDigits(int index_a1, int index_a2, int index_b1, int index_b2);
};

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

//Swaps a range of indices in the BCD
void BCN::swapDigits(int index_a1, int index_a2, int index_b1, int index_b2)
{
	//Not known if essential yet...
}

//Sum is stored in n
void BCN::adder(BCN addend)
{
		for(int i = 0; i < addend.getLength() || (carry_flag == 1) ; i++)
        {
                carry_flag = n[i].adder(addend.getDigitAt(i), carry_flag);

                //TODO: add ability to resize & reinitialize BCD if needed
        }
        carry_flag[0] = 0;
}

//Difference is stored in n
void BCN::subtractor(BCN &subtrahend, int start, int end)
{
	int comparison;
	bitset<1> temp;
	BCD subDigit;

	for(int i = start; i <= end || (borrow_flag == 1); i++)
	{
		subDigit = subtrahend.getDigitAt(i);
		comparison = n[i].compare(subDigit);

		if(comparison == 0)
			n[i].zeroOut();

		else if(comparison > 0)
		{
			for(int x = 0; x < 4; x++)
			{
				if((n[i].digit[x] == 0) & subDigit.digit[x])
				{
					borrow_flag[0] = 1;

					//Perform borrows inside binary-coded digit
					for(int y = (x + 1); y < 4; y++)
					{
						if(n[i].digit[y] == 0)
							n[i].digit[x] = 1;

						else
						{
							n[i].digit[x] = 0;
							break;
						}
					}
				}

				temp[0] = n[i].digit[x];
				n[i].digit[x] = ((temp[0] & ~subDigit.digit[x]) | borrow_flag[0]);

				borrow_flag = 0;
			}
		}

		else
		{
			//TODO
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

	//Iterate through file, counting length of test prime								O(length(n)) = O(log(n))
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

	//Building n																		O(length(n)) = O(log(n))
	int i = length_n - 1;
	while(inputFile.get(nChar))
	{
		if(!isdigit(nChar))
					continue;

		n.setDigitAt(i, BCD(nChar));
		i--;
	}

	//Calculating maximum length of m
	if(length_n % 2 > 0)
		length_m_max = (length_n + 1) / 2;
	else
		length_m_max = length_n / 2;

	//Initializing m = 3
	BCN m(length_n);
	m.setDigitAt(0, BCD('3'));

	//testing
	cout << "n = ";
	n.printBCN();
	cout << "length_m_max: " << length_m_max << endl;
	cout << "length_n: " << length_n << endl;
	cout << "m = ";
	m.printBCN();

	//Adder test
	cout << "adder test:\n";
	n.adder(m);
	n.printBCN();

	//Swap test
	n.swapDigits(0, 2);
	cout << endl << "swapDigits() test: " << endl;
	n.printBCN();

	//BCD compare test
	BCD a('5');
	BCD b('4');
	int compTest = a.compare(b);
	cout << "a: ";
	a.printBCD();
	cout << endl << "b: ";
	b.printBCD();
	cout << endl << "compTest a.compare(b): " << compTest << endl;

	//BCN subtractor test
	BCN x("8043");
	x.printBCN();
	BCN y("199");
	cout << "\n\nsubtractor() test:\n";
	x.subtractor(y, 0 , y.getLength());
	x.printBCN();

	inputFile.close();
}
