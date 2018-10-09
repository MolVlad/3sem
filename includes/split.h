/*
  ******************************************************************************
  * @file		split.h
  * @author		Molodcov Vladislav
  * @institute		MIPT
  * @departament	DREC
  * @group		717
  * @version		V1.1
  * @date		1-October-2018
  * @brief		split-functions
  ******************************************************************************
*/

#ifndef __SPLIT_H__
#define __SPLIT_H__

/* Includes ----------------------------------------------------------------------*/

/* Defgroup for sizes ------------------------------------------------------------*/

#define MAX_SIZE_TEXT		10000
#define MAX_SIZE_DELIMITERS	10
#define MAX_NUM_WORDS		100
#define MAX_SIZE_WORD		100

/* Structure for SPLIT -----------------------------------------------------------*/

typedef struct
{
	int maxSizeText;
	int maxSizeDelimiters;
	int maxNumWords;
	int maxSizeWord;
	int count;
	int delay;

	char * text;
	char * delimiters;
	char ** words;
} splitStruct;

/* Declarations of SPLIT-functions -----------------------------------------------*/

int splitInit(splitStruct * split);
int splitScan(splitStruct * split);
int splitSplit(splitStruct * split);
int splitPrint(splitStruct * split);
int splitFree(splitStruct * split);

#endif /* __SPLIT_H__ */
