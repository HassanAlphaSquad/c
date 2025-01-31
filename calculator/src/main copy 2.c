#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define MAX_INPUT_LIMIT 2000

#define MAX_HISTORY 10

// Global variables
double history[MAX_HISTORY] = {0}; // Array to store the last 10 results
int historyIndex = 0;
#define HISTORY_BOX_HEIGHT 300
#define HISTORY_BOX_WIDTH 470
#define MAX_HISTORY_ENTRIES 10

// Global variables for history
char historyEntries[MAX_HISTORY_ENTRIES][200]; // Array to store history as strings
int historyCount = 0;						   // Count of history entries

static const int screenWidth = 520;
static const int screenHeight = 800;

char inputText[MAX_INPUT_LIMIT] = "";
char currentOperator = 0;
double num1 = 0, num2 = 0;
char resultText[100] = "";

void DrawSubscriptText(float x, float y, const char *mainText, const char *subscriptText)
{

	const int normalFontSize = 40;
	const int subscriptFontSize = 30;

	Font font = GetFontDefault();

	// Draw the main text (x)
	DrawTextEx(font, mainText, (Vector2){x, y}, normalFontSize, 0, BLACK);

	// Draw the subscript text (2)
	DrawTextEx(font, subscriptText, (Vector2){x + 30, y}, subscriptFontSize, 0, BLACK);
}

void ProcessInput(char *input, double *firstNum, double *secondNum, char op)
{
	if (strlen(input) > 0)
	{
		*firstNum = atof(input);
	}

	if (op != 0)
	{
		GuiTextBox((Rectangle){20, 140, 470, 120}, input, MAX_INPUT_LIMIT, true);
	}
}

double Calculate(double firstNum, double secondNum, char op)
{
	double result = 0;
	switch (op)
	{
	case '+':
		result = firstNum + secondNum;
		break;
	case '-':
		result = firstNum - secondNum;
		break;
	case '*':
		result = firstNum * secondNum;
		break;
	case '/':
		if (secondNum != 0)
			result = firstNum / secondNum;
		else
			sprintf(resultText, "Error: Division by zero");
		break;
	case '%':
		if (secondNum != 0.0)
		{
			result = fmod(firstNum, secondNum);
		}
		else
		{
			printf("Error: Division by zero in fmod()\n");
		}
		break;

	default:
		break;
	}

	return result;
}

void HandleSqrtButton()
{
	if (strlen(inputText) == 0)
		return;

	num1 = atof(inputText);

	if (num1 < 0)
	{
		sprintf(inputText, "Error");
		return;
	}

	double result = sqrt(num1);
	sprintf(inputText, "%.1f", result);
}

void HandleSquareButton()
{
	if (strlen(inputText) == 0)
		return;

	num1 = atof(inputText);

	double result = pow(num1, 2);
	sprintf(inputText, "%.1f", result);
}

void AddToHistory(double result)
{
	// Shift the history array and add the new result at the beginning
	for (int i = MAX_HISTORY - 1; i > 0; i--)
	{
		history[i] = history[i - 1];
	}
	history[0] = result; // Add the new result to history
	if (historyIndex < MAX_HISTORY - 1)
		historyIndex++; // Increase the index to point to the latest
}

void AddHistoryEntry(double val1, double val2, char op, double result)
{
	// Create a string to store the current equation and result
	char historyEntry[200];

	// Format history entry (operand1 operator operand2 = result)
	sprintf(historyEntry, "%.1f %c %.1f = %.1f", num1, op, num2, result);

	// Shift older entries to make space for the new one
	if (historyCount < MAX_HISTORY_ENTRIES)
	{
		historyCount++;
	}
	for (int i = historyCount - 1; i > 0; i--)
	{
		strcpy(historyEntries[i], historyEntries[i - 1]);
	}

	// Store the new entry
	strcpy(historyEntries[0], historyEntry);
}

// Rectange {x,y, width, height}
int main()
{
	InitWindow(screenWidth, screenHeight, "Calculator");
	SetTargetFPS(60);

	bool textBoxEditMode = true;

	double current = 0.0;
	double last = 0.0;

	GuiSetStyle(DEFAULT, TEXT_SIZE, 35);
	GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT);
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
		// GuiLabel((Rectangle){(screenWidth / 2) - 80, 20, 250, 100}, "Calculator");

		// GuiButton((Rectangle){10, 20, 70,50},"");
		// DrawText("UP",34,30,20,BLACK);
		// GuiButton((Rectangle){10, 80, 70,50},"");
		// DrawText("DOWN",16,90,20,BLACK);

		// Buttons (including the UP and DOWN buttons for history navigation)
		// if (GuiButton((Rectangle){10, 20, 70, 50}, ""))
		// { // UP Button
		// 	if (historyIndex > 0)
		// 		historyIndex--;
		// 	sprintf(resultText, "%.1f", history[historyIndex]);
		// }
		// DrawText("UP", 34, 30, 20, BLACK);

		// if (GuiButton((Rectangle){10, 80, 70, 50}, ""))
		// { // DOWN Button
		// 	if (historyIndex < MAX_HISTORY - 1 && history[historyIndex + 1] != 0)
		// 		historyIndex++;
		// 	sprintf(resultText, "%.1f", history[historyIndex]);
		// }
		// DrawText("DOWN", 16, 90, 20, BLACK);

		// GuiTextBox((Rectangle){90, 20, 410, 100}, "", 1, false); //! for history box

		GuiTextBox((Rectangle){20, 140, 470, 120}, inputText, sizeof(inputText) - 1, textBoxEditMode);

		if (strlen(inputText) == 0 || currentOperator == 0)
		{
			ProcessInput(inputText, &num1, &num2, currentOperator);
		}

		if (GuiButton((Rectangle){20, 300, 80, 80}, "C"))
		{
			inputText[0] = '\0';
			resultText[0] = '\0';
			currentOperator = 0;
			num1 = num2 = 0;
		}
		GuiButton((Rectangle){120, 300, 80, 80}, "(") ? strcat(inputText, "(") : "";
		GuiButton((Rectangle){220, 300, 80, 80}, ")") ? strcat(inputText, ")") : "";
		// GuiButton((Rectangle){320, 300, 80, 80}, "mod") ? strcat(inputText, "%") : "";
		if (GuiButton((Rectangle){320, 300, 80, 80}, "mod"))
		{
			currentOperator = '%';
			num1 = atof(inputText);
			inputText[0] = '\0';
		}

		if (GuiButton((Rectangle){420, 300, 80, 80}, "PIE"))
		{
			num1 = PI;
			sprintf(inputText, "%.6f", num1);
		}

		GuiButton((Rectangle){20, 400, 80, 80}, "7") ? strcat(inputText, "7") : "";
		GuiButton((Rectangle){120, 400, 80, 80}, "8") ? strcat(inputText, "8") : "";
		GuiButton((Rectangle){220, 400, 80, 80}, "9") ? strcat(inputText, "9") : "";
		if (GuiButton((Rectangle){320, 400, 80, 80}, "/"))
		{
			currentOperator = '/';
			num1 = atof(inputText);
			inputText[0] = '\0';
		}
		if (GuiButton((Rectangle){420, 400, 80, 80}, "sqrt"))
		{
			HandleSqrtButton();
		}

		GuiButton((Rectangle){20, 500, 80, 80}, "4") ? strcat(inputText, "4") : "";
		GuiButton((Rectangle){120, 500, 80, 80}, "5") ? strcat(inputText, "5") : "";
		GuiButton((Rectangle){220, 500, 80, 80}, "6") ? strcat(inputText, "6") : "";
		if (GuiButton((Rectangle){320, 500, 80, 80}, "*"))
		{
			currentOperator = '*';
			num1 = atof(inputText);
			inputText[0] = '\0';
		}
		GuiButton((Rectangle){420, 500, 80, 80}, "^") ? strcat(inputText, "^") : "";
		if (GuiButton((Rectangle){420, 500, 80, 80}, "^"))
		{
			currentOperator = '^';
			num1 = atof(inputText);
			inputText[0] = '\0';
		}

		GuiButton((Rectangle){20, 600, 80, 80}, "1") ? strcat(inputText, "1") : "";
		GuiButton((Rectangle){120, 600, 80, 80}, "2") ? strcat(inputText, "2") : "";
		GuiButton((Rectangle){220, 600, 80, 80}, "3") ? strcat(inputText, "3") : "";
		if (GuiButton((Rectangle){320, 600, 80, 80}, "-"))
		{
			//! HINTS
			// double c = last - current;
			// last = current;
			// current = 0.0;

			currentOperator = '-';
			num1 = atof(inputText);
			inputText[0] = '\0';
		}
		// if (GuiButton((Rectangle){420, 600, 80, 180}, "="))
		// {
		// 	num2 = atof(inputText);
		// 	double result = Calculate(num1, num2, currentOperator);
		// 	// sprintf(resultText, "Result: %.2f", result);
		// 	sprintf(resultText, "%.1f", result);
		// 	inputText[0] = '\0';
		// 	currentOperator = 0;
		// }
		if (GuiButton((Rectangle){420, 600, 80, 180}, "="))
		{
			num2 = atof(inputText);
			double result = Calculate(num1, num2, currentOperator);
			sprintf(resultText, "%.1f", result);

			// Add history entry
			AddHistoryEntry(num1, num2, currentOperator, result);

			// Reset for next calculation
			num1 = 0;
			num2 = 0;
			currentOperator = 0;
			inputText[0] = '\0';
		}

		// Rectangle historyBox = (Rectangle){90, 20, 410, 100};
		// int scrollPosition = GuiScrollBar((Rectangle){HISTORY_BOX_WIDTH + 30, 100, 20, HISTORY_BOX_HEIGHT}, 0, historyCount * 30, 30);

		// // Draw each history entry inside the scrollable box
		// for (int i = 0; i < historyCount; i++)
		// {
		// 	DrawText(historyEntries[i], historyBox.x + 10, historyBox.y + (i * 30) - scrollPosition, 20, BLACK);
		// }

		// // Define history box area
		// Rectangle historyBox = (Rectangle){90, 20, 410, 100}; // Box to show history
		// const int historyVisibleCount = 3;					  // Number of records visible at a time
		// const int entryHeight = 30;							  // Height of each history entry

		// // Scroll position (determines which history items to show)
		// static float scrollOffset = 0;

		// // Calculate the max scroll range
		// float scrollMax = (historyCount > historyVisibleCount) ? (historyCount - historyVisibleCount) * entryHeight : 0;

		// // Ensure scrolling is within valid limits
		// if (scrollOffset < 0)
		// 	scrollOffset = 0;
		// if (scrollOffset > scrollMax)
		// 	scrollOffset = scrollMax;

		// // Draw history box background
		// DrawRectangleRec(historyBox, LIGHTGRAY);
		// DrawRectangleLinesEx(historyBox, 2, BLACK);

		// // Only draw history entries within the visible scroll area
		// for (int i = 0; i < historyVisibleCount; i++)
		// {
		// 	int index = (int)(scrollOffset / entryHeight) + i;
		// 	if (index < historyCount)
		// 	{ // Prevent out-of-bounds access
		// 		DrawText(historyEntries[index], historyBox.x + 10, historyBox.y + (i * entryHeight), 20, BLACK);
		// 	}
		// }

		// // Check if scrolling is needed (history exceeds visible space)
		// if (historyCount > historyVisibleCount)
		// {
		// 	// Draw scrollbar inside history box
		// 	Rectangle scrollBar = {historyBox.x + historyBox.width + 5, historyBox.y, 15, historyBox.height};

		// 	// User interacts with the scrollbar
		// 	GuiSliderBar(scrollBar, "", "", &scrollOffset, 0, scrollMax);
		// }

		// // Draw the history box background
		// DrawRectangleRec(historyBox, LIGHTGRAY);
		// DrawRectangleLinesEx(historyBox, 2, BLACK);

		// // Draw only the visible history entries based on scroll position
		// for (int i = 0; i < historyVisibleCount; i++)
		// {
		// 	int index = (scrollOffset / 30) + i;
		// 	if (index < historyCount)
		// 	{
		// 		DrawText(historyEntries[index], historyBox.x + 10, historyBox.y + (i * 30), 20, BLACK);
		// 	}
		// }


// Define history box area
Rectangle historyBox = (Rectangle){90, 20, 410, 100}; // Box to show history
const int historyVisibleCount = 3;  // Number of records visible at a time
const int entryHeight = 30;         // Height of each history entry

// Scroll position (determines which history items to show)
static int scrollIndex = 0;  // Now based on index, not pixel offset

// Ensure scroll stays within limits
if (scrollIndex < 0) scrollIndex = 0;
if (scrollIndex > historyCount - historyVisibleCount)
    scrollIndex = historyCount - historyVisibleCount;

// Draw history box background
DrawRectangleRec(historyBox, LIGHTGRAY);
DrawRectangleLinesEx(historyBox, 2, BLACK);

// Only draw history entries within the visible scroll area
for (int i = 0; i < historyVisibleCount; i++)
{
    int index = scrollIndex + i;
    if (index < historyCount && historyEntries[index] != NULL)  // Ensure index is valid
    { 
        DrawText(historyEntries[index], historyBox.x + 10, historyBox.y + (i * entryHeight), 20, BLACK);
    }
}

// UP Button (Scroll Up)
if (GuiButton((Rectangle){10, 20, 70, 50}, ""))
{
    if (scrollIndex > 0) scrollIndex--;  // Move view up
}
DrawText("UP", 34, 30, 20, BLACK);

// DOWN Button (Scroll Down)
if (GuiButton((Rectangle){10, 80, 70, 50}, ""))
{
    if (scrollIndex < historyCount - historyVisibleCount) scrollIndex++; // Move view down
}
DrawText("DOWN", 16, 90, 20, BLACK);



//!working
		// Define history box area
// Rectangle historyBox = (Rectangle){90, 20, 410, 100}; // Box to show history
// const int historyVisibleCount = 3;  // Number of records visible at a time
// const int entryHeight = 30;         // Height of each history entry

// // Scroll position (determines which history items to show)
// static float scrollOffset = 0;

// // Calculate the max scroll range
// float scrollMax = (historyCount > historyVisibleCount) ? (historyCount - historyVisibleCount) * entryHeight : 0;

// // Ensure scrolling is within valid limits
// if (scrollOffset < 0) scrollOffset = 0;
// if (scrollOffset > scrollMax) scrollOffset = scrollMax;

// // Draw history box background
// DrawRectangleRec(historyBox, LIGHTGRAY);
// DrawRectangleLinesEx(historyBox, 2, BLACK);

// // Only draw history entries within the visible scroll area
// for (int i = 0; i < historyVisibleCount; i++)
// {
//     int index = (int)(scrollOffset / entryHeight) + i;
//     if (index < historyCount)
//     { 
//         DrawText(historyEntries[index], historyBox.x + 10, historyBox.y + (i * entryHeight), 20, BLACK);
//     }
// }

// // Check if scrolling is needed (history exceeds visible space)
// if (historyCount > historyVisibleCount)
// {
//     // Draw **VERTICAL** scrollbar inside history box
//     Rectangle scrollBar = {historyBox.x + historyBox.width - 10, historyBox.y, 10, historyBox.height};

//     // Use a vertical `GuiSliderBar`
//     GuiSliderBar(scrollBar, "", "", &scrollOffset, 0, scrollMax);
// }
//!working


		GuiButton((Rectangle){20, 700, 80, 80}, "0") ? strcat(inputText, "0") : "";
		GuiButton((Rectangle){120, 700, 80, 80}, ".") ? strcat(inputText, ".") : "";
		if (GuiButton((Rectangle){220, 700, 80, 80}, ""))
		{
			HandleSquareButton();
		}
		DrawSubscriptText(240, 720, "x", "2");
		if (GuiButton((Rectangle){320, 700, 80, 80}, "+"))
		{
			currentOperator = '+';
			num1 = atof(inputText);
			inputText[0] = '\0';
		}

		if (strlen(resultText) > 0)
		{
			GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT);
			GuiLabel((Rectangle){0, 140, 470, 120}, resultText);
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}

// *HINTS:*
// first convert buffer into float as soon as i get input
// put a check to add previous to current
// do this with all of the operators

// take input from user
// evaluate the input type
// run cases on that and call function as per input type