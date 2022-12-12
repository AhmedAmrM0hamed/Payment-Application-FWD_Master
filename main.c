#include <stdio.h>
#include "Application/app.h"


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





/**
int main (void)
{
    Application_Start();
    return 0;
}
 */