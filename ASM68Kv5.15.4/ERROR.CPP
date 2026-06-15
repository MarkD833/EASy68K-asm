/***********************************************************************
 *
 *		ERRORN.C
 *		Error message printer for 68000 Assembler
 *
 *    Function: printError()
 *		Prints an appropriate message to the specified output
 *		file according to the error code supplied. If the
 *		errorCode is OK, no message is printed; otherwise an
 *		WARNING or ERRORN message is produced. The line number
 *		will be included in the message unless lineNum = -1.
 *
 *	 Usage:	printError(outFile, errorCode, lineNum)
 *		FILE *outFile;
 *		int errorCode, lineNum;
 *
 *      Author: Paul McKee
 *		ECE492    North Carolina State University
 *
 *        Date:	12/12/86
 *
 *    Modified: Charles Kelly
 *              Monroe County Community College
 *              http://www.monroeccc.edu/ckelly
 *
 *              Mar 15, 2002 by Tim Larson
 *              - added code to dump error messages into active forms listview
 *              - added numbers to each error message
 ************************************************************************/


#include <stdio.h>
#include "asm.h"
// include "mainS.h"
// include "textS.h"

extern char buffer[256];  //ck used to form messages for display in windows
extern char numBuf[20];
extern bool WARflag;
extern int lineNumL68;      // listing line number
//TListItem  *ListItem;
extern char includeFile[256];  // name of current include file
extern bool includedFileError; // true if include error message displayed

int printError(FILE *outFile, int errorCode, int lineNum)
{
  if (errorCode == OK)          // if no errors detected
    return NORMAL;
  // if there are only warnings and warnings are disabled
  if (errorCode < MINOR && errorCode > WARNING && WARflag == false)
    return NORMAL;

  if (lineNumL68 >= 0)
    snprintf(numBuf, sizeof(numBuf), "Line %d ", lineNumL68);
  else
    numBuf[0] = '\0';

  // ---------- display warning messages ----------
  switch (errorCode) {
    case NUMBER_TOO_BIG:
      snprintf(buffer, sizeof(buffer), "WARNING: Numeric constant exceeds 32 bits\n");
      break;
    case ASCII_TOO_BIG:
      snprintf(buffer, sizeof(buffer), "WARNING: ASCII constant exceeds 4 characters\n");
      break;
    case INCOMPLETE:
      snprintf(buffer, sizeof(buffer), "WARNING: Evaluation of expression could not be completed\n");
      break;
    case FORCING_SHORT:
      snprintf(buffer, sizeof(buffer), "WARNING: Forcing SHORT addressing disables range checking of extension word\n");
      break;
    case ODD_ADDRESS:
      snprintf(buffer, sizeof(buffer), "WARNING: Origin value is odd (Location counter set to next highest address)\n");
      break;
    case END_MISSING:
      snprintf(buffer, sizeof(buffer), "WARNING: END directive missing, starting address not set\n");
      break;
    case ADDRESS_MISSING:
      snprintf(buffer, sizeof(buffer), "WARNING: Address expected\n");
      break;
    case THEN_EXPECTED:
      snprintf(buffer, sizeof(buffer), "WARNING: THEN expected\n");
      break;
    case DO_EXPECTED:
      snprintf(buffer, sizeof(buffer), "WARNING: DO expected\n");
      break;
    case FORWARD_REF:
      snprintf(buffer, sizeof(buffer), "WARNING: Forward reference may cause 'Symbol value differs...' error\n");
      break;
    case LABEL_TOO_LONG:
      snprintf(buffer, sizeof(buffer), "WARNING: Label too long\n");
      break;
    default :
      if (errorCode < MINOR && errorCode > WARNING)
        snprintf(buffer, sizeof(buffer), "WARNING: No message defined\n");
  }

  // ---------- display error messages ----------
  switch (errorCode) {
    case SYNTAX:
      snprintf(buffer, sizeof(buffer), "ERROR: Invalid syntax\n");
      break;
    case UNDEFINED:
      snprintf(buffer, sizeof(buffer), "ERROR: Undefined symbol\n");
      break;
    case DIV_BY_ZERO:
      snprintf(buffer, sizeof(buffer), "ERROR: Division by zero attempted\n");
      break;
    case INV_OPCODE:
      snprintf(buffer, sizeof(buffer), "ERROR: Invalid opcode\n");
      break;
    case INV_SIZE_CODE:
      snprintf(buffer, sizeof(buffer), "ERROR: Invalid size code\n");
      break;
    case INV_ADDR_MODE:
      snprintf(buffer, sizeof(buffer), "ERROR: Invalid addressing mode\n");
      break;
    case MULTIPLE_DEFS:
      snprintf(buffer, sizeof(buffer), "ERROR: Symbol defined more than once\n");
      break;
    case NO_ENDM:
      snprintf(buffer, sizeof(buffer), "ERROR: ENDM expected\n");
      break;
    case TOO_MANY_ARGS:
      snprintf(buffer, sizeof(buffer), "ERROR: Too many arguments\n");
      break;
    case INVALID_ARG:
      snprintf(buffer, sizeof(buffer), "ERROR: Invalid argument\n");
      break;
    case PHASE_ERROR:
      snprintf(buffer, sizeof(buffer), "ERROR: Symbol value differs between first and second pass\n");
      break;
    case INV_QUICK_CONST:
      snprintf(buffer, sizeof(buffer), "ERROR: Quick immediate data range must be 1 to 8\n");
      break;
    case INV_MOVE_QUICK_CONST:
      snprintf(buffer, sizeof(buffer), "ERROR: Quick immediate data range exceeds 1 byte\n");
      break;
    case INV_VECTOR_NUM:
      snprintf(buffer, sizeof(buffer), "ERROR: Invalid vector number\n");
      break;
    case INV_BRANCH_DISP:
      snprintf(buffer, sizeof(buffer), "ERROR: Branch instruction displacement is out of range or invalid\n");
      break;
    case LABEL_REQUIRED:
      snprintf(buffer, sizeof(buffer), "ERROR: Label required with this directive\n");
      break;
    case INV_DISP:
      snprintf(buffer, sizeof(buffer), "ERROR: Displacement out of range\n");
      break;
    case INV_ABS_ADDRESS:
      snprintf(buffer, sizeof(buffer), "ERROR: Absolute address exceeds 16 bits\n");
      break;
    case INV_8_BIT_DATA:
      snprintf(buffer, sizeof(buffer), "ERROR: Immediate data exceeds 8 bits\n");
      break;
    case INV_16_BIT_DATA:
      snprintf(buffer, sizeof(buffer), "ERROR: Immediate data exceeds 16 bits\n");
      break;
    case NOT_REG_LIST:
      snprintf(buffer, sizeof(buffer), "ERROR: Symbol is not a register list symbol\n");
      break;
    case REG_LIST_SPEC:
      snprintf(buffer, sizeof(buffer), "ERROR: Register list symbol used in an expression\n");
      break;
    case REG_LIST_UNDEF:
      snprintf(buffer, sizeof(buffer), "ERROR: Register list symbol not previously defined\n");
      break;
    case INV_SHIFT_COUNT:
      snprintf(buffer, sizeof(buffer), "ERROR: Invalid constant shift count\n");
      break;
    case INV_OPERATOR:
      snprintf(buffer, sizeof(buffer), "ERROR: Invalid operator\n");
      break;
    case INV_FORWARD_REF:
      snprintf(buffer, sizeof(buffer), "ERROR: Forward references not allowed with this directive\n");
      break;
    case INV_LENGTH:
      snprintf(buffer, sizeof(buffer), "ERROR: Invalid block length\n");
      break;
    case COMMA_EXPECTED:
      snprintf(buffer, sizeof(buffer), "ERROR: Comma expected\n");
      break;
    case FAIL_ERROR:    // user defined error
      //the error message has already been written to buffer
      break;
    case EXCEPTION:     // an exception occurred
      //the error message has already been written to buffer
      break;
    case FILE_ERROR:
      snprintf(buffer, sizeof(buffer), "ERROR: Unable to access specified file\n");
      break;
    case MACRO_NEST:    // possible recursive macro call
      snprintf(buffer, sizeof(buffer), "ERROR: Nested Macro calls are too many levels deep\n");
      break;
    case NO_IF:
      snprintf(buffer, sizeof(buffer), "ERROR: No matching IF statement was found\n");
      break;
    case NO_WHILE:
      snprintf(buffer, sizeof(buffer), "ERROR: No matching WHILE statement was found\n");
      break;
    case NO_REPEAT:
      snprintf(buffer, sizeof(buffer), "ERROR: No matching REPEAT statement was found\n");
      break;
    case NO_DBLOOP:
      snprintf(buffer, sizeof(buffer), "ERROR: No matching DBLOOP statement was found\n");
      break;
    case NO_FOR:
      snprintf(buffer, sizeof(buffer), "ERROR: No matching FOR statement was found\n");
      break;
    case ENDI_EXPECTED:
      snprintf(buffer, sizeof(buffer), "ERROR: IF without ENDI\n");
      break;
    case ENDW_EXPECTED:
      snprintf(buffer, sizeof(buffer), "ERROR: WHILE without ENDW\n");
      break;
    case ENDF_EXPECTED:
      snprintf(buffer, sizeof(buffer), "ERROR: FOR without ENDF\n");
      break;
    case REPEAT_EXPECTED:
      snprintf(buffer, sizeof(buffer), "ERROR: UNTIL without REPEAT\n");
      break;
    case DBLOOP_EXPECTED:
      snprintf(buffer, sizeof(buffer), "ERROR: UNLESS without DBLOOP\n");
      break;
    case LABEL_ERROR:
      snprintf(buffer, sizeof(buffer), "ERROR: Label is not allowed\n");
      break;
    case BAD_BITFIELD:
      snprintf(buffer, sizeof(buffer), "ERROR: Invalid bitfield\n");
      break;
    case ILLEGAL_SYMBOL:
      snprintf(buffer, sizeof(buffer), "ERROR: Illegal symbol\n");
      break;
    default :
      if (errorCode > MINOR)
        snprintf(buffer, sizeof(buffer), "ERROR: No message defined\n");
  } // end switch

  if (outFile)
    listError(numBuf, buffer);  // add error to listing file

  // display error messages in edit window
  // MD direct error message to console
  fprintf(stderr,"%s %s",numBuf, buffer);

  // if include file then display error in proper window
  if (includeFile[0]) {         // if not NULL
    bool openIncFile = true;    // open included file when true
    AnsiString incFile(includeFile);

    /*** RA ***
    // check to see if file is already open
    for(int i = Main->MDIChildCount-1; i >= 0; i--) {
      TextStuff = (TTextStuff*)Main->MDIChildren[i];
      //if(TextStuff->Project.CurrentFile == ExpandFileName(incFile)) { // if file already open
      if(ExpandFileName(TextStuff->Project.CurrentFile).AnsiCompareIC(ExpandFileName(incFile)) == 0) {
        if (TextStuff->Project.Modified) {         // if it is modified
          if (includedFileError == false) {
            AnsiString asBuffer = "An Error occurred in the included file " +
                          ExtractFileName(incFile) +
                          ".\nThe file is currently open and has been modified." +
                          "\nThe line numbers of errors may not be correct.";
            ShowMessage(asBuffer);
            includedFileError = true;   // prevent multiple messages
          }
        }
        openIncFile = false;
        TextStuff->BringToFront();
        break;
      }
    }
    if (openIncFile)
      Main->OpenFile(incFile);
    ******************/
  }
  /*** RA ***
  //Grab the active Child Form
  TTextStuff *Active = (TTextStuff*)Main->ActiveMDIChild; //grab active mdi child

  Active->Messages->Enabled = true; //need to allow user to click errors
  if (Active->Messages->Height < 87)
    Active->Messages->Height = 87;
  ListItem = Active->Messages->Items->Add();
  if (lineNum >= 0)
    snprintf(numBuf, sizeof(numBuf), "Line %d ", lineNum);
  else
    numBuf[0] = '\0';
  ListItem->Caption = lineNum;        // display line number of error
  buffer[strlen(buffer)-1]='\0';      // trim off '\n'
  ListItem->SubItems->Add(buffer);    // add error message to edit window
  ListItem->SubItems->Add(Active->Project.CurrentFile);       // display file
  */
  return NORMAL;
}
