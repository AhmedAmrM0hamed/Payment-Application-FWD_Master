#include <stdio.h>
#include "server.h"
#include <string.h>
#include "../Data Types/dataTypes.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"

ST_accountsDB_t current_Card;
uint8_t  card_index = 0; /* used to go directly to the record in updating the balance */
uint64_t transactionNumber = 0;

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
    if (isValidAccount(&transData->cardHolderData) != SERVER_OK) 
	{
        transData->transState = DECLINED_STOLEN_CARD;
        saveTransaction(transData);
        return DECLINED_STOLEN_CARD;
    }

    if (isBlocked(transData->cardHolderData)) 
	{
        transData->transState = DECLINED_STOLEN_CARD;
        saveTransaction(transData);
        return DECLINED_STOLEN_CARD;
    }

    if (isAmountAvailable(&transData->terminalData) != SERVER_OK) 
	{
        transData->transState = DECLINED_INSUFFECIENT_FUND;
        saveTransaction(transData);
        return DECLINED_INSUFFECIENT_FUND;
    }

    updateBalance(transData->terminalData.transAmount);

    transData->transState = APPROVED;

    transData->transactionSequenceNumber = transactionNumber++;

    if (saveTransaction(transData) != SERVER_OK)
	{
        return INTERNAL_SERVER_ERROR;
    }

    return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{

    ST_accountsDB_t  temp ;
    FILE* databaseFilePtr ;
    uint8_t  i;
    if ((databaseFilePtr = fopen("../Server/Database/database file.dat", "rb")) == NULL) {
        return ACCOUNT_NOT_FOUND;
    }

    while (!feof(databaseFilePtr))
	{
        i = fread(&temp, sizeof(ST_accountsDB_t), 1, databaseFilePtr);

        if (i == 0 || temp.primaryAccountNumber[0] == '\0')
            return ACCOUNT_NOT_FOUND;

        if (strcmp(temp.primaryAccountNumber, cardData->primaryAccountNumber) == 0) 
		{
            current_Card = temp;
            fclose(databaseFilePtr);
            return SERVER_OK;
        }
        card_index++;
    }

    fclose(databaseFilePtr);
    return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData) 
{

    if (termData->transAmount <= current_Card.balance) 
	{
        return SERVER_OK;
    }

    return LOW_BALANCE;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData) 
{

    FILE* transFilePtr;

    if ((transFilePtr = fopen("../Server/Database/transaction file.dat", "ab")) == NULL)
        return SAVING_FAILED;

    fwrite(transData, sizeof(ST_transaction_t), 1, transFilePtr);

    fclose(transFilePtr);

    return SERVER_OK;
}

EN_serverError_t listSavedTransactions(uint32_t transactionSequenceNumber, ST_transaction_t* transData)
{

    FILE* transFilePtr;

    if ((transFilePtr = fopen("../Server/Database/transaction file.dat", "rb")) == NULL)
        return TRANSACTION_NOT_FOUND;

    fseek(transFilePtr, (transactionSequenceNumber - 1) * sizeof(ST_transaction_t), SEEK_SET);

    if (!fread(transData, sizeof(ST_transaction_t), 1, transFilePtr))
        return TRANSACTION_NOT_FOUND;

    fclose(transFilePtr);

    return SERVER_OK;
}

EN_transState_t updateBalance(float transAmount) 
{

    current_Card.balance -= transAmount;

    FILE* transFilePtr;

    if ((transFilePtr = fopen("../Server/Database/database file.dat", "rb+")) == NULL)
        return INTERNAL_SERVER_ERROR;

    fseek(transFilePtr, card_index * sizeof(ST_accountsDB_t), SEEK_SET);

    if (!fwrite(&current_Card, sizeof(ST_transaction_t), 1, transFilePtr))
        return INTERNAL_SERVER_ERROR;

    fclose(transFilePtr);

    return APPROVED;
}

uint8_t isBlocked(ST_cardData_t cardData)
{
    uint8_t i;
    ST_cardData_t temp;
    FILE* databaseFilePtr;
    /* open the file in the reading in the binary mode */
    if ((databaseFilePtr = fopen("../Server/Database/blockList file.dat", "rb")) == NULL)
	{
        return 0;
    }
    while (!feof(databaseFilePtr))
	{
        i = fread(&temp, sizeof(ST_cardData_t), 1, databaseFilePtr);

        if (i != 0 && strcmp(temp.primaryAccountNumber, cardData.primaryAccountNumber) == 0)
		{
            fclose(databaseFilePtr);
            return 1;
        }
    }
    fclose(databaseFilePtr);
    return 0;
}

void Send_Data ()
{
    ST_accountsDB_t database1[255] ={{1000   ,"4117394584032808"},
									 {12000 ,"5370233590092076"},
									 {500000,"5078034830297201"}};

    FILE* databaseFilePtr;

    if ((databaseFilePtr = fopen("../Server/Database/database file.dat", "w")) == NULL)
	{
        puts("out of control");
        return;
    }

    fwrite(database1, sizeof(ST_accountsDB_t), 3, databaseFilePtr);

    fclose(databaseFilePtr);

    if ((databaseFilePtr = fopen("../Server/Database/blockList file.dat", "w")) == NULL)
	{
        puts("out of control in block list");
        return;
    }

    fwrite(&(database1[2]), sizeof(ST_accountsDB_t), 1, databaseFilePtr);

    fclose(databaseFilePtr);

}


 /**
  isValidAccount function testing


int main ()
 {
    ST_cardData_t card_Data = {"Ahmed Amr Mohamed Sayed" , "4117394584032808" , "11/23"};

    EN_serverError_t  error = isValidAccount(&card_Data);

    if(error == SERVER_OK)
        puts("Valid card");
    else
        puts("Account isn't in the database ");
}
*/



 /**

 isAmountAvailable function testing

 int main()
 {
     current_Card.balance = 2000 ;

     ST_terminalData_t  terminal_Data = { 1500 , 5000,"24/10/2022"};

    EN_serverError_t error = isAmountAvailable(&terminal_Data);

    if(error == SERVER_OK)
        puts("An available amount");
    else
        puts("The amount is not available");
 }

 */

    /**
     saveTransaction function testing

int main()
{
    ST_cardData_t card_Data = {"Ahmed Amr Mohamed Sayed" , "4117394584032123", "11/23"};
    ST_terminalData_t  terminal_Data = { 500 , 5000,"24/07/2022"};
    ST_transaction_t transaction_Data = {card_Data, terminal_Data,APPROVED,2};

    saveTransaction(&transaction_Data);

    ST_transaction_t Transac;

    listSavedTransactions(2,&Transac);

    printf("%s\n" , Transac.cardHolderData.cardHolderName);
    printf("%s\n" , Transac.cardHolderData.primaryAccountNumber);
    printf("%s\n" , Transac.cardHolderData.cardExpirationDate);
    printf("%d\n" , Transac.transactionSequenceNumber);
}
*/


 /**
  receiveTransactionData function testing

int main()
{
    ST_cardData_t card_Data = {"Ahmed Amr Mohamed Sayed"   , "4117394584032123" , "11/21"};
    ST_cardData_t some_one = {"Ahmed Omar Mohamed" , "4117394584032000" , "11/21"};
    ST_terminalData_t  terminal_Data = { 500 , 5000,"24/07/2022"};
    ST_transaction_t transaction ={card_Data, terminal_Data,APPROVED,1};

    EN_transState_t error = recieveTransactionData(&transaction);

    switch (error) {

        case DECLINED_INSUFFECIENT_FUND:
            printf("Low Balance");
            return  0;

        case DECLINED_STOLEN_CARD:
            printf("Stolen Card");
            return  0;

        case INTERNAL_SERVER_ERROR :
            printf("Transaction unsuccessfully");
            return  0;

        case APPROVED:
            printf("Transaction successfully");
            return  0;
    }

}
*/

 /**

  isBlocked function testing

 int main()
{

    ST_cardData_t Client ;
    EN_cardError_t Error = getCardPAN(&Client);
    if(Error== CARD_OK) {
        EN_transState_t State = isBlocked(Client);

        if (State == 1)
        {
            printf("Valid card");
        } else
        {
            printf("Stolen Card");
        }
    }
    else
    {
        printf("Wrong PAN!!");
    }
    return 0;
}


  */