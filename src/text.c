#include "myLib.h"
#include "text.h"

void drawChar(int row, int col, char ch, uint16_t color)
{
	for(int my_col=0; my_col < 8; my_col++)
	{
		for(int my_row = 0; my_row < 6; my_row++)
		{
			if(fontdata_6x8[OFFSET(my_col, my_row, 6) + ch * 48])
			{
				setPixel(row + my_row, col + my_col, color);
			}
		}
	}
}

void drawString(int col, int row, char *str, uint16_t color)
{
	while(*str)
	{
		drawChar(row, col, *str++, color);
		row += 6;
	}


}
