#include <stdio.h>
#include <string.h>

#define MAX_BUF  256

#define IV  0b10110001
#define KEY 0b11001011
#define CTR 0b00110101

void encode(unsigned char*, unsigned char*, int);
void decode(unsigned char*, unsigned char*, int);

unsigned char processCtr(unsigned char, unsigned char);
unsigned char encryptByte(unsigned char, unsigned char, unsigned char);
unsigned char decryptByte(unsigned char, unsigned char, unsigned char);

unsigned char getBit(unsigned char, int);
unsigned char setBit(unsigned char, int);
unsigned char clearBit(unsigned char, int);


int main() {
  char str[8];
  int  choice;
  int count;
  unsigned char ct[MAX_BUF];
  unsigned char pt[MAX_BUF];
  unsigned char text[MAX_BUF];

  printf("\nYou may:\n");
  printf("  (1) Encrypt a message \n");
  printf("  (2) Decrypt a message \n");
  printf("  (0) Exit\n");
  printf("\n  what is your selection: ");
  fgets(str, sizeof(str), stdin);
  sscanf(str, "%d", &choice);

  if (choice == 0)
    return 0;

  switch (choice) {
      case 1:
        
        printf("\nEncrypt: ");
        fgets(text, sizeof(text), stdin);
        
        for(count = 0; text[count] != '\0'; ++count){
            encode(text, ct, sizeof(text));
        }
        
        for(int i = 0; i< count; ++i){
            printf("%d ", ct[i]);
        }
        
        break;


    case 2:

      int numBytes = 0;
      int encryptNum;

      for (int i = 0; i < sizeof(pt) / sizeof(pt[0]); ++i) {
        scanf("%d", &encryptNum);

        if (encryptNum == -1) {
          break;
        }

        ct[i] = encryptNum;
        numBytes++;

      }

      decode(ct, pt, numBytes);
      printf("\n%s", pt);

      break;
  }

  return(0);
}

/*
  Function:  getBit
  Purpose:   retrieve value of bit at specified position
       in:   character from which a bit will be returned
       in:   position of bit to be returned
   return:   value of bit n in character c (0 or 1)
*/
unsigned char getBit(unsigned char c, int n) {

  unsigned char dec = ((c & (1 << n)) >> n);

  return dec;

}

/*
  Function:  setBit
  Purpose:   set specified bit to 1
       in:   character in which a bit will be set to 1
       in:   position of bit to be set to 1
   return:   new value of character c with bit n set to 1
*/
unsigned char setBit(unsigned char c, int n) {
  
  unsigned char dec = c |(1 << n);

  return dec;
}

/*
  Function:  clearBit
  Purpose:   set specified bit to 0
       in:   character in which a bit will be set to 0
       in:   position of bit to be set to 0
   return:   new value of character c with bit n set to 0
*/
unsigned char clearBit(unsigned char c, int n) {

  unsigned char dec = c & (~(1 << n));
  
  return dec;

}


/*Function:  processCtr
  Purpose:   processes the given counter value using the given key, as part of the encryption algorithm
   in/out:   the counter value is copied into a temporary counter which gets updated
       in:   key value is xor with the current bits of the counter
   return:   temporary counter value as the updated counter
*/
unsigned char processCtr(unsigned char ctr, unsigned char key) {

  unsigned char tmp = ctr;
  int val;
    
  if(tmp % 2 == 0) {
    
    for(int x = 0; x <= 7; x+=2){
        
      val = getBit(ctr,x) ^ getBit(key,x);
     
      if(val == 1){
        tmp = setBit(tmp,x);
      }
            
      else{
        tmp = clearBit(tmp,x);
      }
    }

  }
    
  else{

    for(int x = 1; x <= 7; x+=2){
    	
      val = getBit(ctr,x) ^ getBit(key,x);
                
      if(val == 1) {
        tmp = setBit(tmp,x);
      }
            
      else{
        tmp = clearBit(tmp,x);
      }
    }
  }

  return tmp;
}



/*Function:  encryptByte
  Purpose:   encrypts the given plaintext byte pt

       in:   plaintext byte pt
       in:   using the counter value ctr and the 
       in:   previous byte of the ciphertext prev

   return:   corresponding encrypted ciphertext byte
*/
unsigned char encryptByte(unsigned char pt, unsigned char ctr, unsigned char prev) {

    unsigned char tmpBytes = 0b00000000;
    
    for (int x = 0; x <= 7; ++x){
    
        if(getBit(ctr,x) == 1){
        
            if((getBit(pt,x) ^ getBit(prev,x)) == 0){
                tmpBytes = clearBit(tmpBytes,x);
            }
            
            else{
                tmpBytes = setBit(tmpBytes,x);
            }
        }

        else{
            
            if((getBit(pt,x) ^ getBit(prev,7-x)) == 0){
                tmpBytes = clearBit(tmpBytes,x);
            }
            
            else{
                tmpBytes = setBit(tmpBytes,x);
            }
        }
    }
    return tmpBytes;

}



/*Function:  encode
  Purpose:   encrypts each plaintext character into its corresponding ciphertext byte

    in:   takes an array of plaintext characters stored in parameter pt, which contains numBytes bytes
    in:  stores the encrypted byte into the ciphertext array ct

*/
void encode(unsigned char *pt, unsigned char* ct, int numBytes) {

    unsigned char ctr = processCtr(CTR, KEY);
    unsigned char prev = IV;
    
    for (int i = 0; i < numBytes; ++i){
        ct[i] = encryptByte(pt[i], ctr, prev);
        prev = ct[i];
        ctr++;
        ctr = processCtr(ctr, KEY);
    }

}


/*Function:  decryptByte
  Purpose:   decrypts each ciphertext byte into its corresponding plaintext byte

       in:  takes an array of ciphertext bytes stored in parameter ct, which contains numBytes bytes.  
   return:   the corresponding decrypted plaintext byte
*/
unsigned char decryptByte(unsigned char ct, unsigned char ctr, unsigned char prev) {

  int bit;
  unsigned char tmpByte = 0b00000000;

  for (int i = 0; i < 8; ++i) {

    if (getBit(ctr, i) == 0) {

      bit = getBit(ct, i) ^ getBit(prev, 7-i);

      if (bit == 0) {
        tmpByte = clearBit(tmpByte, i);
      }


      else if (bit == 1) {
        tmpByte = setBit(tmpByte, i);
      }
    }

    if (getBit(ctr, i) == 1) {

      bit = getBit(ct, i) ^ getBit(prev, i);

      if (bit == 0) {
        tmpByte = clearBit(tmpByte, i);
      }

      else if (bit == 1) {
        tmpByte = setBit(tmpByte, i);
      }
    }
  }

  return tmpByte;
}




/*Function:  decode
  Purpose:   decrypts each ciphertext byte into its corresponding plaintext byte

    in:   takes an array of ciphertext bytes stored in parameter ct
    in:   numBytes bytes
    in:   stores the decrypted byte into the plaintext array pt
*/
void decode(unsigned char *ct, unsigned char *pt, int numBytes) {

  unsigned char counter = processCtr(CTR, KEY);
  unsigned char previous = IV;
  
  for (int i = 0; i < numBytes; ++i) {

    pt[i] = decryptByte(ct[i], counter, previous);
    previous = ct[i];

    counter++;
    counter = processCtr(counter, KEY);

  }
}
