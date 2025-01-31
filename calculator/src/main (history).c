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
#define MAX_HISTORY 20
#define MAX_HISTORY_ENTRIES 10

double history[MAX_HISTORY] = {0}; // Array to store the last 10 results
int historyIndex = 0;

char historyEntries[MAX_HISTORY_ENTRIES][2000];
int historyCount = 0;

static const int screenWidth = 520;
static const int screenHeight = 800;

char inputText[MAX_INPUT_LIMIT] = "";
char currentOperator = 0;
double num1 = 0, num2 = 0;
char resultText[2000] = ""; // stores the result array e.g., 12 * 2 = 24

void DrawSubscriptText(float x, float y, const char *mainText, const char *subscriptText)
{
	const int normalFontSize = 40;
	const int subscriptFontSize = 30;
	Font font = GetFontDefault();
	// for text
	DrawTextEx(font, mainText, (Vector2){x, y}, normalFontSize, 0, BLACK);
	// for text to be subscripted
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
		GuiTextBox((Rectangle){20, 140, 470, 120}, input, MAX_INPUT_LIMIT, false);
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
			result = 0.0;
		break;
	case '%':
		result = fmod(firstNum, secondNum);
		break;
	case '^':
		result = pow(firstNum, secondNum);
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
	sprintf(inputText, "%.2f", result);
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
	// sprintf(historyEntry, "%.1f %c %.1f = %.2f", num1, op, num2, result); //! working perfectly

	// Create a string to store the current equation and result
	char historyEntry[200];

	// Format the numbers based on their magnitude
	if (fabs(num1) >= 1e6 || fabs(num1) < 1e-3)
	{
		sprintf(historyEntry, "%.4e", num1); // Scientific notation for num1
	}
	else
	{
		sprintf(historyEntry, "%.2f", num1); // Normal formatting for num1
	}

	char num1Formatted[50];
	strcpy(num1Formatted, historyEntry); // Store formatted num1

	// Format num2 similarly
	if (fabs(num2) >= 1e6 || fabs(num2) < 1e-3)
	{
		sprintf(historyEntry, "%.4e", num2); // Scientific notation for num2
	}
	else
	{
		sprintf(historyEntry, "%.2f", num2); // Normal formatting for num2
	}

	char num2Formatted[50];
	strcpy(num2Formatted, historyEntry); // Store formatted num2

	// Format result similarly
	if (fabs(result) >= 1e6 || fabs(result) < 1e-3)
	{
		sprintf(historyEntry, "%.4e", result); // Scientific notation for result
	}
	else
	{
		sprintf(historyEntry, "%.2f", result); // Normal formatting for result
	}

	char resultFormatted[50];
	strcpy(resultFormatted, historyEntry); // Store formatted result

	// Construct the history entry
	sprintf(historyEntry, "%s %c %s = %s", num1Formatted, op, num2Formatted, resultFormatted);
	// sprintf(historyEntry, "%.1f %c %.1f = %.4e", num1, op, num2, result);

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

void formatResult(double result, char *formattedText)
{
	// If the result is a large value, we'll directly use scientific notation
	if (fabs(result) >= 1e6 || fabs(result) < 1e-3)
	{
		int exponent = (int)log10(fabs(result));  // Get the exponent part
		double base = result / pow(10, exponent); // Normalize the result
		// Format as scientific notation (keeping full precision for the base)
		sprintf(formattedText, "%f × 10^%d", base, exponent);
	}
	else
	{
		// Otherwise, print with normal 3 decimal places
		sprintf(formattedText, "%.2f", result);
	}
}

// Rectange {x,y, width, height}
int main()
{
	InitWindow(screenWidth, screenHeight, "Calculator");
	SetTargetFPS(100);
	bool resultShown = false;
	bool textBoxEditMode = false;

	double current = 0.0;
	double last = 0.0;

	GuiSetStyle(DEFAULT, TEXT_SIZE, 35);
	GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT);
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
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
		if (GuiButton((Rectangle){320, 300, 80, 80}, "mod"))
		{
			currentOperator = '%';
			num1 = atof(inputText);
			inputText[0] = '\0';
		}

		if (GuiButton((Rectangle){420, 300, 80, 80}, "pi"))
		{
			num1 = PI;
			sprintf(inputText, "%.6f", num1);
		}

		if (GuiButton((Rectangle){20, 400, 80, 80}, "7"))
		{
			if (resultShown)
			{
				inputText[0] = '\0';
				resultText[0] = '\0';
				currentOperator = 0;
				num1 = num2 = 0;
				strcat(inputText, "7");
				resultShown = false;
			}

			else
			{
				strcat(inputText, "7");
			}
		}
		if (GuiButton((Rectangle){120, 400, 80, 80}, "8"))
		{
			if (resultShown)
			{
				inputText[0] = '\0';
				resultText[0] = '\0';
				currentOperator = 0;
				num1 = num2 = 0;
				strcat(inputText, "8");
				resultShown = false;
			}

			else
			{
				strcat(inputText, "8");
			}
		}
		if (GuiButton((Rectangle){220, 400, 80, 80}, "9"))
		{
			if (resultShown)
			{
				inputText[0] = '\0';
				resultText[0] = '\0';
				currentOperator = 0;
				num1 = num2 = 0;
				strcat(inputText, "9");
				resultShown = false;
			}

			else
			{
				strcat(inputText, "9");
			}
		}
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

		if (GuiButton((Rectangle){20, 500, 80, 80}, "4"))
		{
			if (resultShown)
			{
				inputText[0] = '\0';
				resultText[0] = '\0';
				currentOperator = 0;
				num1 = num2 = 0;
				strcat(inputText, "4");
				resultShown = false;
			}

			else
			{
				strcat(inputText, "4");
			}
		}
		if (GuiButton((Rectangle){120, 500, 80, 80}, "5"))
		{
			if (resultShown)
			{
				inputText[0] = '\0';
				resultText[0] = '\0';
				currentOperator = 0;
				num1 = num2 = 0;
				strcat(inputText, "5");
				resultShown = false;
			}

			else
			{
				strcat(inputText, "5");
			}
		}
		if (GuiButton((Rectangle){220, 500, 80, 80}, "6"))
		{
			if (resultShown)
			{
				inputText[0] = '\0';
				resultText[0] = '\0';
				currentOperator = 0;
				num1 = num2 = 0;
				strcat(inputText, "6");
				resultShown = false;
			}

			else
			{
				strcat(inputText, "6");
			}
		}

		if (GuiButton((Rectangle){320, 500, 80, 80}, "*"))
		{
			currentOperator = '*';
			num1 = atof(inputText);
			inputText[0] = '\0';
		}
		if (GuiButton((Rectangle){420, 500, 80, 80}, "^"))
		{
			currentOperator = '^';
			num1 = atof(inputText);
			inputText[0] = '\0';
		}

		if (GuiButton((Rectangle){20, 600, 80, 80}, "1"))
		{
			if (resultShown)
			{
				inputText[0] = '\0';
				resultText[0] = '\0';
				currentOperator = 0;
				num1 = num2 = 0;
				strcat(inputText, "1");
				resultShown = false;
			}

			else
			{
				strcat(inputText, "1");
			}
		}
		if (GuiButton((Rectangle){120, 600, 80, 80}, "2"))
		{
			if (resultShown)
			{
				inputText[0] = '\0';
				resultText[0] = '\0';
				currentOperator = 0;
				num1 = num2 = 0;
				strcat(inputText, "2");
				resultShown = false;
			}

			else
			{
				strcat(inputText, "2");
			}
		}
		if (GuiButton((Rectangle){220, 600, 80, 80}, "3"))
		{
			if (resultShown)
			{
				inputText[0] = '\0';
				resultText[0] = '\0';
				currentOperator = 0;
				num1 = num2 = 0;
				strcat(inputText, "3");
				resultShown = false;
			}

			else
			{
				strcat(inputText, "3");
			}
		}
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

		if (GuiButton((Rectangle){420, 600, 80, 180}, "="))
		{
			num2 = atof(inputText);
			double result = Calculate(num1, num2, currentOperator);
			formatResult(result, resultText);
			// sprintf(resultText, "%.2f", result);
			// sprintf(resultText, "%.2e", result); //!stores result in scientific notation
			if (currentOperator)
			{
				AddHistoryEntry(num1, num2, currentOperator, result);
				resultShown = true;
			}

			num1 = 0;
			num2 = 0;
			currentOperator = 0;
			inputText[0] = '\0';
		}

		Rectangle historyBox = (Rectangle){90, 20, 410, 110}; // rectangle of historybox with its parameters
		const int historyVisibleCount = 3;					  // visible records
		const int entryHeight = 40;							  // Height of each history entry

		static int scrollIndex = 0;

		// it will keep scroll stay within limits
		if (scrollIndex < 0)
			scrollIndex = 0;
		if (historyCount < historyVisibleCount)
			scrollIndex = 0;
		else if (scrollIndex > historyCount - historyVisibleCount)
			scrollIndex = historyCount - historyVisibleCount;

		//! drawing history box background
		DrawRectangleRec(historyBox, LIGHTGRAY);
		DrawRectangleLinesEx(historyBox, 4, BLACK); // border

		if (historyCount > 0)
		{
			for (int i = 0; i < historyVisibleCount; i++)
			{
				int index = scrollIndex + i;
				if (index < historyCount && historyEntries[index] != NULL)
				{
					DrawText(historyEntries[index], historyBox.x + 10, historyBox.y + (i * entryHeight) + 5, 22, BLACK);
				}
			}
		}
		else
		{
			DrawText("No history available", historyBox.x + 70, 50, 25, DARKGRAY);
		}

		//! history box ends

		// UP Button (Scroll Up)
		if (GuiButton((Rectangle){10, 20, 70, 50}, ""))
		{
			if (scrollIndex > 0)
				scrollIndex--;
		}
		DrawText("UP", 34, 30, 20, BLACK);

		if (GuiButton((Rectangle){10, 80, 70, 50}, ""))
		{
			if (scrollIndex < historyCount - historyVisibleCount)
				scrollIndex++;
		}
		DrawText("DOWN", 16, 90, 20, BLACK);

		GuiButton((Rectangle){20, 700, 80, 80}, "0") ? strcat(inputText, "0") : "";
		GuiButton((Rectangle){120, 700, 80, 80}, ".") ? strcat(inputText, ".") : "";
		if (GuiButton((Rectangle){220, 700, 80, 80}, ""))
		{
			currentOperator = '*';
			num1 = atof(inputText);
			num2 = num1;
			// HandleSquareButton(); //! this was not letting the calculation stored in history
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