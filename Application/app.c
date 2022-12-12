#include<stdio.h>
#include "app.h"

void Application_Start (void)
{
    ST_cardData_t New_Client;

    EN_cardError_t  Card_Error =  Check_CardData(&New_Client);

    if(Card_Error != CARD_OK)
    {
        return;
    }

    ST_terminalData_t New_Terminal;

    EN_terminalError_t Terminal_Error = Check_Terminal(&New_Terminal , &New_Client);

    if(Terminal_Error != CARD_OK)
    {
        return;
    }

    ST_transaction_t New_Transaction ;
    New_Transaction.cardHolderData = New_Client;
    New_Transaction.terminalData = New_Terminal;

    Check_Server(&New_Transaction);
}

EN_cardError_t Check_CardData(ST_cardData_t *Card_Data)
{
    printf("*------------------ Card Data ------------------*\n");

    EN_cardError_t error ;

    error = getCardHolderName(Card_Data);

    if (error != CARD_OK)
    {
        printf(" \n Wrong Name");
        return error;
    }

    error = getCardExpiryDate(Card_Data);

    if (error != CARD_OK)
    {
        printf(" \n Wrong Date");
        return error;
    }

    error =getCardPAN(Card_Data);

    if (error != CARD_OK)
    {
        printf(" \n Wrong Number");
        return error;
    }

    return CARD_OK;
}
EN_terminalError_t Check_Terminal(ST_terminalData_t *Term_Data , ST_cardData_t *Card_Data)
{
    printf(" \n ");
    printf("*------------------------ TERMINAL Data ------------------------* \n");
    EN_terminalError_t error ;
    getTransactionDate(Term_Data);

    setMaxAmount(Term_Data);

    error = isCardExpired(*Card_Data , *Term_Data);

    if (error != TERMINAL_OK)
    {
        printf(" \n Expired Card");
        return error;
    }

    error = isValidCardPAN(Card_Data);

    if (error != TERMINAL_OK)
    {
        printf(" \n Invalid Card");
        return error;
    }

    error = getTransactionAmount(Term_Data);

    if (error != TERMINAL_OK)
    {
        printf(" \n Invalid Amount");
        return error;
    }

    error = isBelowMaxAmount(Term_Data);

    if (error != TERMINAL_OK)
    {
        printf(" \n Exceeded the maximum value");
        return error;
    }

    return TERMINAL_OK;
}
EN_transState_t Check_Server(ST_transaction_t *Trans_Data)
{
    printf("\n");
    printf("*--------------------- Server Data --------------------*\n");
    EN_transState_t serverError = recieveTransactionData(Trans_Data);

    switch (serverError)
    {
        case DECLINED_INSUFFECIENT_FUND:
            printf("Low Balance");
            break;

        case DECLINED_STOLEN_CARD:
            printf("Stolen Card");
            break;

        case INTERNAL_SERVER_ERROR :
            printf("Transaction unsuccessfully");
            break;

        case APPROVED:
            printf("Transaction successfully");
            break;

        default:
            printf("Error");
    }

}
