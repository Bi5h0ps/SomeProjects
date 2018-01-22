#include <assert.h>
#include <stdio.h>
#include "watcard.h"


/* useful printf strings:

  "INACTIVE Wat Card!\n" (only used by print_balance)
  "INVALID WatCard ACTIVATION!\n"
  "INVALID WatCard DEACTIVATION!\n"
  "INVALID WatCard RELOAD!\n"
  "INVALID WatCard PURCHASE!\n"
  "[WatCard %d] Activated\n"
  "[WatCard %d] Deactivated. Refund: $%d.%02d\n"
  "[WatCard %d] Balance: $%d.%02d\n"
  "[WatCard %d] Reloaded: $%d.%02d\n"
  "[WatCard %d] Purchase: $%d.%02d\n"
  "[WatCard %d] Promo: $%d.%02d\n"

*/
int id = 00000000;
int pin = 0000;
int balance = 0;;
int status = 0;
int timer = 0;
int min = 0;

//activate(student_id,student_pin) produces none and mutate the status of 
//the Watcard with student_id as id and student_pin as pin to "activated".
//requires:
// student_id must be 8 digits long and student_pin must be 4 digits long.
void activate(const int student_id, const int student_pin) {
  //check if the Watcard is activated
  if (status == 0) {
    //check if the student_id and student_pin are both valid 
    if ( ((student_id / 10000000) > 0) && ((student_id / 10000000) <= 9)) {
      if ( ((student_pin / 1000) > 0) && ((student_pin / 1000) <= 9)) {
        id = student_id;
        pin = student_pin;
        status = 1;
        printf("[WatCard %d] Activated\n",student_id);
        return;
    } else {
      printf("INVALID WatCard ACTIVATION!\n");
      return;
    }
  } else {
    printf("INVALID WatCard ACTIVATION!\n");
    return;
  }
  } else {
    printf("INVALID WatCard ACTIVATION!\n");
    return;
  }
}

//get_id(void) produces the id for the Watcard and it returns -1 if the Watcard
// is valid.
int get_id(void) {
  //check if the Watcard if valid
  if (status) {
    return id;
  } else {
    return -1;
  }
}

//correct_pin(student_pin) produces 1 if the student_pin matches with the
//  pin of the activated Watcard, produces 0 otherwise.
int correct_pin(const int student_pin) {
  //check if the Watcard is valid
  if (status) {
    //check if the pin matches
    if (student_pin == pin) {
      return 1;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

//deactivate(student_pin) produces none and mutate the status of the Watcard to
// "invalid", the amount of refunding will be printed on the screen. The error
// message would occur if the Watcard is invalid or the student_pin does not 
void deactivate(const int student_pin) {
  //check if the Watcard if valid
  if (status) {
    //check if the pin matches
    if (student_pin == pin) {
      //print the message
      printf("[WatCard %d] Deactivated. Refund: $%d.%02d\n",id,balance/100,\
             balance%100);
      //reset all the key data
      status = 0;
      balance = 0;
      timer = 0;
      min = 0;  
    } else {
      //error message
      printf("INVALID WatCard DEACTIVATION!\n");
      return;
    }
  } else {
    //error message
    printf("INVALID WatCard DEACTIVATION!\n");
    return;
  }
}

// print_balance(void) produces none and print the current balance of the 
// Watcard, if the Watcard is invalid, error message is printed instead.
void print_balance(void) {
  //check if the Watcard is valid
  if (status) {
    //print the message
    printf("[WatCard %d] Balance: $%d.%02d\n",id,balance/100,balance%100);
    return;
  } else {
    //print the error message
    printf("INACTIVE WatCard!\n");
    return;
  }
}

//get_balance(void) produces the current balance of the Watcard, -1 is produced
// if the Watcard is invalid.
int get_balance(void) {
  //check if the card is valid
  if (status) {
    return balance;
  } else {
    //error result
    return -1;
  }
}

//reload(amount) produces none and make a deposite of amount into the Watcard
// and print a message. If the amount is not valid or the Watcard is invalid
// error message is displayed instead.
void reload(const int amount) {
  //check if the card is valid
  if (status) {
    //check if the amount is valid
    if (amount > 0) {
      //deposite the amount into the account and print the message
      balance += amount;
      printf("[WatCard %d] Reloaded: $%d.%02d\n",id,amount/100,amount%100);
      return;
    } else {
      //error message
      printf("INVALID WatCard RELOAD!\n");
      return;
    }
  } else {
    //error message
    printf("INVALID WatCard RELOAD!\n");
    return;
  }
}


//promo(amount) deposites the amount into the account
//effects: a message is printed
static void promo (const int amount) {
  balance += amount;
  printf("[WatCard %d] Promo: $%d.%02d\n",id,amount/100,amount%100);
  return;
}


// purchase(student_pin,amount) produces none and has a set of side effects:
// remove balance from the account if the purchase was successful
// keeping on track of the purchasing record, the promotion procedure is
// activated by every 10 purchasing.
// keeping on track of the minimum price within the 10 purchasing cycle
// reset the key data every 10 purchasing cycle
// adding funds to the Watcard
// displaying the promo message and purchasing message
// displaying the error message if the amount is not valid or the balance is
// not enough for the purchasing or the pin is not right or the Watcard is
// not valid.
void purchase(const int student_pin, const int amount) {
  //5 things are checked here:
  //the Watcard must be activated;
  //the purchase price must be valid;
  //the price must be less than the balance;
  //the pin is right;
  //it's the very first time of using this watcard to purchase;
  if (status && (amount > 0) && (amount <= balance)\
      && (student_pin == pin) && (timer == 0 )) {
      min = amount;
      timer += 1;
      balance -= amount;
      printf("[WatCard %d] Purchase: $%d.%02d\n",id,amount/100,amount%100);
      return;
    } else {
//5 things are checked here:
//the Watcard must be activated;
//the purchase price must be valid;
//the price must be less than the balance;
//the pin is right;
//it's neither the very first time nor the 10th time purchasing
    if (status && (amount > 0) && (amount <= balance)\
      && (student_pin == pin) && (timer != 9 )) {
      //if the price is less than the previous cheapest
      //make it to the new cheapest
    if (amount < min) {
      timer += 1;
      min = amount;
      balance -= amount;
      printf("[WatCard %d] Purchase: $%d.%02d\n",id,amount/100,amount%100);
      return;
    } else {
      //if the price is not the cheapest,nothing special happens
      timer += 1;
      balance -= amount;
      printf("[WatCard %d] Purchase: $%d.%02d\n",id,amount/100,amount%100);
      return;
    }
  } else {
//5 things are checked here:
//the Watcard must be activated;
//the purchase price must be valid;
//the price must be less than the balance;
//the pin is right;
//it's the 10th time purchasing
    if (status && (amount > 0) && (amount <= balance)\
      && (student_pin == pin) && (timer == 9 )) {
      //promo procedure is triggered here and the timer shell be set back to 0
      if (amount < min) {
      timer = 0;
      balance -= amount;
      min = amount;
      printf("[WatCard %d] Purchase: $%d.%02d\n",id,amount/100,amount%100);
      promo(min/5);
      return;
      } else {
        timer = 0;
      balance -= amount;
      printf("[WatCard %d] Purchase: $%d.%02d\n",id,amount/100,amount%100);
      promo(min/5);
      } 
    }else {
      printf("INVALID WatCard PURCHASE!\n");
      return;
    }
  }
}
}


     

    
      
    
    



