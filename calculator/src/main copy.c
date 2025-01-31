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

		GuiButton((Rectangle){10, 20, 70, 50}, "");
		DrawText("UP", 34, 30, 20, BLACK);
		GuiButton((Rectangle){10, 80, 70, 50}, "");
		DrawText("DOWN", 16, 90, 20, BLACK);

		GuiTextBox((Rectangle){90, 20, 410, 100}, "", 1, false);

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
		if (GuiButton((Rectangle){420, 600, 80, 180}, "="))
		{
			num2 = atof(inputText);
			double result = Calculate(num1, num2, currentOperator);
			// sprintf(resultText, "Result: %.2f", result);
			sprintf(resultText, "%.1f", result);
			inputText[0] = '\0';
			currentOperator = 0;
		}

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