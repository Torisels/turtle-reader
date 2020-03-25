#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <bitset>


#define SET_POSITION 0x03
#define SET_DIRECTION 0x02
#define MOVE 0x01
#define SET_PEN_STATE 0x00
int right = 0;
int up = 0;

int xp = 0;
int yp = 0;

std::string direction(const unsigned char& dir)
{
	switch (dir)
	{
	case 0x00:
		right = 1;
		up = 0;
		return "Right";
	case 0x01:
		up = 1;
		right = 0;
		return "Up";
	case 0x02:
		right = -1;
		up = 0;
		return "Left";
	case 0x03:
		right = 0;
		up = -1;
		return "Down";
	}
}


std::string color(const unsigned char& col)
{
	switch (col)
	{
	case 0x00:
		return "Black";
	case 0x01:
		return "Red";
	case 0x02:
		return "Green";
	case 0x03:
		return "Blue";
	case 0x04:
		return "Yellow";
	case 0x05:
		return "Cyan";
	case 0x06:
		return "Purple";
	case 0x07:
		return "White";
	}
}


bool handleInstruction(const uint16_t& current, const uint16_t& next)
{
	unsigned char ins = current & 0x03;

	switch (ins)
	{
	case SET_DIRECTION:
		{
			unsigned char dir = (current >> 2) & 0x03;
			std::cout << "Direction: " << direction(dir) << std::endl;
			return false;
		}
	case MOVE:
		{
			uint16_t move = current & 0x0FFF;
			move = move >> 2;

			
			xp += up * move;
			yp += right * move;
			std::cout << "Move by: " << move << " X: " << xp << " Y: " << yp << std::endl;
			return false;
		}
	case SET_PEN_STATE:
		{
			uint16_t state = current >> 3;
			unsigned char col = current >> 13;
			auto colr = color(col);
			state = state & 0x0001;
			std::string out;
			if (state)
			{
				out = "Down";
			}
			else
			{
				out = "Up";
			}
			std::cout << "Set pen state: " << out <<" Color: "<<colr << " X: " << xp << " Y: " << yp << std::endl;
			return false;
		}
	case SET_POSITION:
		{
			uint16_t x = current >> 6;
			uint16_t y = next >> 10;
			y = y & 0x003F;

			xp = x;
			yp = y;
			std::cout << "Set position. X: " << xp << " Y: " << yp << std::endl;
			return true;
		}

	default: break;
	}
	return false;
}


int main()
{
	
	
	std::ifstream input("turtle4.bin", std::ios::binary);

	// copies all data into buffer
	std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

	for (int i = 0; i < buffer.size(); i += 2)
	{
		const uint16_t current = (buffer.at(i) << 8) | buffer.at(i+1);
		uint16_t next = 0;


		if (i == 58)
		{
			next = 0;
		}
		else
		{
			next = (buffer.at(i + 2) << 8) | buffer.at(i + 3);
		}

		std::bitset<8> xd(buffer.at(i));
		std::bitset<8> xf(buffer.at(i + 1));
		std::cout << xd << " | " << xf << "    ";


		if (handleInstruction(current, next))
		{
			i += 2;
		}
	}


	std::cin.get();
}
