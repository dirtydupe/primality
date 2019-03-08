#include <iostream>
#include <string>
#include <bitset>

using namespace std;
void increment();
void decrement();

class BCD			//Binary-coded digit
{
	private:
//	void bitAdder();

	public:
	BCD() = default;

	BCD(char input)
	{
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

	bitset<5> digit;
	void adder(BCD addend);
	void printBCD(void);
//	bitset<1> getBit(int index);
	//void subtractor(BCD minuend, BCD subtrahend);
};

//bitset<1> BCD::getBit(int index)
//{
//	return digit[i];
//}

void BCD::adder(BCD addend)
{
	bitset<1> carry_in = addend.digit.test(4), tmp;

	for(int i = 0; i < 4; i++)
	{
		tmp[0] = digit[i];
		digit[i] = (digit[i] ^ addend.digit.test(i)) ^ carry_in.test(0);
		digit[4] = (tmp[0] & addend.digit.test(i)) |
				 carry_in.test(0) & (tmp[0] ^ addend.digit.test(i));
	}
	//TODO

}

//void BCD::bitAdder()
//{
	//TODO
//}

//For testing*
void BCD::printBCD()
{
	for(int i = 3 ; i >= 0; i--)
	{
		cout << digit[i] << " ";
	}
	cout << "   ";
}

class BCN			//Full binary-coded number
{
	private:
	BCD* n;
	int n_length;

	public:
	BCN(string &n_str) : n(new BCD[n_length = n_str.length()])
	{
		for(int i = 0; i < n_length; i++)
		{
			BCD binDigit(n_str[i]);
			n[i] = binDigit;
		}
	};

	BCD getBCD(int index);	//Returns binary-coded digit at index
	void printBCN(void);
	void adder(BCN addend);
	int getLength(void);
};

BCD BCN::getBCD(int index)
{
	return n[index];
}

int BCN::getLength()
{
	return n_length;
}

void BCN::adder(BCN addend)
{
	for(int i = 0; i < addend.getLength() && n[i].digit[4] != 1 ; i++)
	{
		//TODO ?? TEST if this is ok...
		n[i].adder(addend.getBCD(i));
	}
}

//For testing*
void BCN::printBCN()
{
	for(int i = 0; i < n_length; i++)
	{
		n[i].printBCD();
	}
	cout << endl;
}

int main()
{
	//testing*
	string test1 = "1";
	string test2 = "2";
	BCN obj1(test1);
	obj1.printBCN();
	BCN obj2(test2);
	obj2.printBCN();
	obj1.adder(obj2);
	obj1.printBCN();

	return 0;
}
