#include <stdio.h>
#include <stdlib.h>

const unsigned int MAX_CHARACTER_COUNT = 850;
const unsigned int MAX_PAGE_COLUMN = 60;
const unsigned int MAX_PAGE_ROW = 25;

void createPage(const char *token);
void printPage(const char *token);
int tokenToSerial(const char *token);
char *serialToToken(unsigned int serial);
void clearPage();

int main()
{
   // get latest token (at the top of file)
   char latestToken[5];
   FILE* in = fopen("info.txt", "r");
   fgets(latestToken, 5, in);
   fclose(in);

   char *token = latestToken;
   unsigned int pageNumber = tokenToSerial(token);

   int input = 0;

   do
   {
      //clearPage(); // cannot get it to work
      printPage(token);
      printf("next 1) back 2) search 3) create 4) exit 0)\n");
      printf("enter: ");

      scanf("%d", &input);

      if(input < 0 || input > 5)
      {
         printf("invalid input\n");
      }

      if(input == 1)
      {
         unsigned int serial = tokenToSerial(token);
         if(serial < tokenToSerial(latestToken) + 1)
         {
            serial++;
            token = serialToToken(serial);
         }
         else
         {
            printf("nothing more\n");
         }
      }
      else if(input == 2)
      {
         unsigned int serial = tokenToSerial(token);
         if(serial > 1)
         {
            serial--;
            token = serialToToken(serial);
         }
         else
         {
            printf("cannot go back\n");
         }
      }
      else if(input == 3)
      {
         unsigned int number = 0;
         printf("page #: ");
         scanf("%d", &number);

         if(number > 0 && number < tokenToSerial(latestToken) + 1)
         {
            token = serialToToken(number);
         }
         else
         {
            printf("page does not exist\n");
         }
      }
      else if(input == 4)
      {
         unsigned int serial = tokenToSerial(latestToken);
         serial++;
         token = serialToToken(serial);
         createPage(token);
      }
      else
      {
         printf("that is not a valid input\n");
      }
   } 
   while(input != 0);

   return 0;
}

void createPage(const char *token)
{
   FILE *out;
   // opens file in append mode so I can add on to it
   out = fopen("info.txt", "r+");
   if(out == NULL)
   {
      printf("could not open file\n");
      exit(1);
   }

   // initial page setup
   // adds latest token to top of page
   fprintf(out, "%s", token);
   // rewrite the file because it replace it if I dont
   char *data = NULL;
   char buffer = getc(out);
   unsigned int size = 0;

   while((buffer = getc(out)) != EOF)
   {
      data = realloc(data, size + 1);
      if(data == NULL)
      {
         printf("error reallocating memory\n");
      }
      data[size++] = buffer;
   }
   data[size - 1] = '\0';
   fseek(out, 5, SEEK_SET);
   fprintf(out, "%s\n", data);
   // print token
   fprintf(out, "%s\n", token);

   // date
   char date[11];
   unsigned int pass = 0;
   do
   {
      printf("date (MM/DD/YYYY): ");
      buffer = getc(stdin);
      fgets(date, 11, stdin);

      unsigned int length = 0;
      while(date[length] != '\0')
      {
         length++;
      }
      if(length > 10)
      {
         printf("incorrect length\n");
         pass = 1;
      }
      else
      {
         for(int i = 0; i < 8; i++)
         {
            if(i == 2 || i == 5)
            {
               i++;
            }
            else if(date[i] == '/' || (date[2] != '/' || date[5] != '/'))
            {
               printf("incorrect format\n");
               pass = 1;
               break;
            }
            else
            {
               pass = 0;
            }
         }
      }
   }
   while(pass == 1);
   fprintf(out, "%s\n", date);
   
   // entry
   char *entry = NULL;;
   size = 0;
   buffer = getc(stdin);

   printf("enter: ");

   // loop until the user presses enter
   while((buffer = getc(stdin)) != '\n')
   {
      entry = realloc(entry, size + 1);
      entry[size] = buffer;
      size++;
   }
   entry = realloc(entry, size + 1);
   entry[size] = '\0';

   fprintf(out, "%s", entry);
     
   // end of page identifie
   fprintf(out, "\n");
   fprintf(out, "~\n");
   fclose(out);
}

void printPage(const char *token)
{
   FILE *in;
   in = fopen("info.txt", "r+");
   if(in == NULL)
   {
      printf("could not open file\n");
   }
   fseek(in, 5, SEEK_SET);

   unsigned int read = 0;
   unsigned int length = 0;
   unsigned int column = 0;
   unsigned int row = 0;
   char buffer[4] = "#XXX";
   char *word = NULL;
   char letter;

   for(int i = 0; i < MAX_PAGE_COLUMN; i++)
   {
      printf("#");
   }
   printf("\n\n");
   for(int i = 0; i < MAX_PAGE_COLUMN - 8; i++)
   {
      printf(" ");
   } 
   printf("page ");
   while((letter = getc(in)) != EOF)
   {
      if(read != 1)
      {
         // shift letters in buffer left one
         for(int i = 0; i < 3; i++)
         {
            buffer[i] = buffer[i + 1];
         }
         buffer[3] = letter;
         // see if buffer and token are equal
         for(int i = 0; i < 4; i++)
         {
            if(buffer[i] == token[i])
            { 
               read = 1;
            }
            else
            {
               read = 0;
               break;
            }
         }
      }
      else if(letter == '~')
      {
         read = 0;
      }

      if(read == 1 && letter != '~')    
      {
         word = realloc(word, length + 1);
         if(word == NULL)
         {
            printf("error reallocting\n");
         }
         word[length++] = letter;
         column++;
         if(column > MAX_PAGE_COLUMN - 1)
         {
            printf(" \n");
            column = 0;
            row++;
         }
         if(letter == ' ')
         {
            word = realloc(word, length + 1);
            if(word == NULL)
            {
               printf("error reallocating memory\n");
            }
            word[length] = '\0';
            printf("%s", word);
            length = 0;
            word = NULL;
         }
      }
   }
   word = realloc(word, length + 1);
   if(word == NULL)
   {
      printf("error reallocating memory\n");
   }
   word[length] = '\0';
   printf("%s" , word);
   length = 0;
   word = NULL;

   printf("\n");
   for(int i = 0; i < MAX_PAGE_COLUMN; i++)
   {
      printf("#");
   }
   printf("\n");
   fclose(in);
}

int tokenToSerial(const char *token)
{
   unsigned int serial = 0;
   for(int i = 1; i < 4; i++)
   {
      serial = serial * 10 + ((int)token[i] - 48);
   }
   return serial;
}

char *serialToToken(unsigned int serial)
{
   static char token[4];
   token[0] = '#';
   for(int i = 1; i < 4; i++)
   {
      int digit = serial % 10;
      token[i] = digit + '0';
      serial /= 10;
   }
   // reverse
   for(int i = 1; i < 3; i++)
   {
      char temp = token[i];
      token[i] = token[4 - i];
      token[4 - i] = temp;
   }
   return token; 
}

void clearPage()
{
   for(int i = 0; i < 40; i++)
   {
      printf("\033[A");
   }
}
