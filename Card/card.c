#include <stdio.h>
#include "card.h"
#include <string.h>

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
    printf("Please enter the card name : ");
    gets(cardData->cardHolderName);
    uint32_t i = strlen(cardData->cardHolderName);

    if (i > 24 || i < 20) /* to Check the name length */
	{
        return WRONG_NAME;
    }
    return CARD_OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) 
{
    printf("Please enter the Expiry date as 'MM/YY' : ");
    gets(cardData->cardExpirationDate);
    uint32_t i = strlen(cardData->cardExpirationDate);

    if (i != 5 || cardData->cardExpirationDate[2] != '/')  /*to check length and formula*/
    {
        return WRONG_EXP_DATE;
    }

    return CARD_OK;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
    printf("Please enter the card PAN : ");
    gets(cardData->primaryAccountNumber);
    uint32_t i = strlen(cardData->primaryAccountNumber);

    if (i > 19 || i < 16) /* to check lengh */
    {
        return WRONG_PAN;
    }

    return CARD_OK;
}

  /** getCardHolderName function testing
 int main()
   {
     ST_cardData_t Client ;
     EN_cardError_t error = getCardHolderName(&Client);

     if(error == CARD_OK)
     {
         printf("Name is correct");
         return 0;
     }

     printf("Name is not correct");
 }
*/
  /** getCardExpiryDate function testing
 int main()
   {
     ST_cardData_t Client ;
     EN_cardError_t error = getCardExpiryDate(&Client);

     if(error == CARD_OK)
     {
         printf("Valid Date");
         return 0;
     }
     else
         printf("Invalid Date");
 }
*/

 /** getCardPAN function testing
 int main()
 {
     ST_cardData_t Client ;
     EN_cardError_t error = getCardPAN(&Client);

     if(error == CARD_OK)
     {
         printf("PAN is correct");
         return 0;
     }
     printf("PAN is not correct");
 }
*/


