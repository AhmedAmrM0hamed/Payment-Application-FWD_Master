#include <stdio.h>
#include <time.h>
#include "terminal.h"
#define charToInt(x) ((x) % 48)  /* to convert characters to a numbers */

EN_terminalError_t getTransactionAmount(ST_terminalData_t* Term_Data)
{
    printf("Please Enter Transaction Amount : ");

    scanf_s("%f", &Term_Data->transAmount);

    if (Term_Data->transAmount <= 0)
    {
        return INVALID_AMOUNT;
    }
    return TERMINAL_OK;
}

EN_terminalError_t getTransactionDate(ST_terminalData_t* Term_Data)
{
    /* get the real time from the system */
    time_t now = time(NULL);
    struct  tm* current_time = localtime (&now);

    strftime(Term_Data->transactionDate, 11 , "%d/%m/%Y" , current_time);

    return CARD_OK;

}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
    /* transforming the date into a numeric values to check it easily */ 
    uint8_t card_month = charToInt(cardData.cardExpirationDate[0]) * 10 + charToInt(cardData.cardExpirationDate[1]);
    uint8_t card_year = charToInt(cardData.cardExpirationDate[3]) * 10 + charToInt(cardData.cardExpirationDate[4]);

    uint8_t current_month = charToInt(termData.transactionDate[3]) * 10 + charToInt(termData.transactionDate[4]);
    uint8_t current_year = charToInt(termData.transactionDate[8]) * 10 + charToInt(termData.transactionDate[9]);

    if (current_year > card_year)
    {
        return EXPIRED_CARD;
    }
    else if (current_year == card_year && current_month > card_month)
    {
        return EXPIRED_CARD;
    }

    return TERMINAL_OK;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
    /* Luhn check method */

    uint8_t i = 0, multi = 2, sum = 0, temp;

    while (cardData->primaryAccountNumber[i] != '\0')
    {
        /* doubling the even placed digits from the leftmost */
        temp = charToInt(cardData->primaryAccountNumber[i]) * multi;

        /* if the doubled value is consisted of two digits sum them up */
        if (temp > 9)
		{
            temp = (temp / 10) + (temp % 10);
        }

        // toggling the multi between one and two
        if (multi == 2)
        {
            multi--;
        }
        else
        {
            multi++;
        }
        /* summing up all the values to perform the last check */ 
        sum += temp;

        i++;

    }

    /* the final check is to see the summation is a multiplyer of ten */
    if (sum % 10 != 0)
    {
        return INVALID_CARD;
    }

    return TERMINAL_OK;

}

EN_terminalError_t setMaxAmount(ST_terminalData_t* Term_Data)
{
    Term_Data->maxTransAmount = 10000 ;
    return TERMINAL_OK;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* Term_Data)
{
    if (Term_Data->transAmount > Term_Data->maxTransAmount)
    {
        return EXCEED_MAX_AMOUNT;
    }
    return TERMINAL_OK;
}


 /**
  * getTransactionAmount function testing


int main()
{
    ST_terminalData_t  Terminal_Data ;
    EN_terminalError_t  error = getTransactionAmount(&Terminal_Data);

    if(error != TERMINAL_OK){
        printf("Invalid Amount");
        return 0;
    }

    printf(" \n %f" , Terminal_Data.transAmount);
}
*/


 /**
 getTransactionDate function testing


int main()
 {
    ST_terminalData_t  terminal_Data ;

    getTransactionDate(&terminal_Data);

    printf("%s", terminal_Data.transactionDate);
}
*/


 /**
  isCardExpired function testing

int main()
  {
    ST_cardData_t  card_Data = {"Ahmed Amr Sayed", "5370233590092076" , "11/22"};
    ST_terminalData_t  terminal_Data ;

    getTransactionDate(&terminal_Data);

    EN_terminalError_t  error = isCardExpired(card_Data , terminal_Data);

    if(error == TERMINAL_OK) {
        printf("A valid card");
        return 0;
    } else
        printf("Expired Card");
    return 0;

}
*/


  /**
   isValidCardPAN function testing


int main()
   {
    ST_cardData_t  card_Data = {"Ahmed Amr Sayed", "5370233590092076" , "11/22"};

    EN_terminalError_t error = isValidCardPAN(&card_Data);

    if(error == TERMINAL_OK)
    {
        printf("A valid card");
        return 0;
    } else
        printf("Card is not valid");

    return 0;
}
*/

  /**
   setMaxAmount function testing

   int main ()
{
    ST_terminalData_t terminal_Data ;
    EN_terminalError_t error = setMaxAmount(&terminal_Data) ;
    if (error != TERMINAL_OK)
        printf("Invalid amount");
    else
        printf("A valid amount \n");
        printf("Amount = %f",terminal_Data.maxTransAmount);

    return 0 ;

}
  */

 /**
  isBelowMaxAmount function testing

int main()
{
    ST_terminalData_t  terminal_Data ;

    terminal_Data.transAmount = 4000;

    setMaxAmount(&terminal_Data);

    EN_terminalError_t error = isBelowMaxAmount(&terminal_Data);

    if(error == TERMINAL_OK){

        printf("A valid amount");
        return 0;
    }

    printf("The amount is not valid");
}

*/
