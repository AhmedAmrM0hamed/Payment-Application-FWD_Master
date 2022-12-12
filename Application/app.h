#ifndef APP_H_
#define APP_H_

#include "../Server/server.h"
#include "../Terminal/terminal.h"
#include "../Card/card.h"

void Application_Start(void);


EN_cardError_t      Check_CardData(ST_cardData_t *cardData);
EN_terminalError_t  Check_Terminal(ST_terminalData_t *termData , ST_cardData_t *cardData);
EN_transState_t     Check_Server(ST_transaction_t *transData);

#endif