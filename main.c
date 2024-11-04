#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

/*
1. Konvertēju failu uz wav
2. Izlasu visus baitus no faila, sākot ar 44. baitu (izlaižot headeru)
3. Pārveidoju hexadecimālos baitus uz bināriem bitiem

*/
// globaalais jo gribu %%%

// VISU MAX IZMĒRU DEFINĪCIJA
#define par 40000000
// failu definīcijas
int skaitli[par];
int chunkSize[50];
int izmersApjomam;

int min, max;

// No binaaraa uz decimaalo
int binUzDec(int n) 
{ 
    int num = n; 
    int dec_value = 0; 
  
    // Initializing base value to 1, i.e 2^0 
    int base = 1; 
  
    int temp = num; 
    // Extracting the last digit of the binary number 
    while (temp) { 
        int last_digit = temp % 10; 
        // Removing the last digit from the binary number 
        temp = temp / 10; 
  
        // Multiplying the last digit with the base value 
        // and adding it to the decimal value 
        dec_value += last_digit * base; 
  
        // Updating the base value by multiplying it by 2 
        base = base * 2; 
    } 
  
    // Returning the decimal value 
    return dec_value; 
} 


// intu uz bitiem

unsigned int_to_bin(unsigned k)
{
    if (k == 0)
        return 0;
    if (k == 1)
        return 1;
    return (k % 2) + 10 * int_to_bin(k / 2);
}

void printeeBaitus(unsigned char *buff, int len)
{

    const int izmersApjomam = len;
    int i; // < len
    for (i = 44; i < len; i++)
    {

        // Ja dalās ar 8 tad printē jaunā līnijā
        if (i % 8 == 0)
        {
            printf("    |%d\n", i);
        }

        /*
        if (i > 4 && i > 9){
            chunkSize[i-4] = buff[i];
        }*/

        // Izprintē decimālo skaitli
        // printf("Buferis: %02x ",buff[i]);

        // stringu saglabāšanas mahinācijas
        char bufsamplam[8];
        // size_t szk, szl;

        // Izprintē bināro bāzi 2 skaitlim no buffera, kas ir decimāls, tātad dec>bin un tad, ja tam sūdam nav 0 priekšā,
        //  tad to ievieto kreisajā pusē lai sanāktu 8 biti katram kanālam yipēe

        // lai saglabātu tiek 2 reizes izsaukts snprintf, 1. lai noskaidrotu izmeru, 2. lai ierakstiitu bufferii,
        // tad tos paņems konkatenēs, jeb saliks kopā lai izveidotu 16 bitu virkni (nagfig??), un no tās tālāk paņem vismazsvarīgāko bitu, kas ir pēdejais
        if (i % 2 == 0)
        {
            // printf("K: %08d", int_to_bin(buff[i]));
            // drošs buferis un izmērs izveido labu
            // szk = snprintf(NULL, 0, "%08d", int_to_bin(buff[i]));
            // bufk = (char *)malloc(szk + 1);
            // snprintf(bufk, szk+1, "%08d, int_to_bin(buff[i]));

            // snprintf(bufsamplam, 12, "K: %08d", int_to_bin(buff[i]));
            snprintf(bufsamplam, 9, "%08d", int_to_bin(buff[i]));
            printf("K: %s", bufsamplam);
        }
        else
        {
            // printf("L: %08d", int_to_bin(buff[i]));
            // szl = snprintf(NULL, 0, "%08d", int_to_bin(buff[i]));
            // bufl = (char *)malloc(szl + 1);
            // snprintf(bufl, szl+1, "%08d", int_to_bin(buff[i]));

            // snprintf(bufsamplam, 12, "L: %08d", int_to_bin(buff[i]));
            snprintf(bufsamplam, 9, "%08d", int_to_bin(buff[i]));
            printf("L: %s", bufsamplam);
        }
        // printf("\t skaitli[i] = %d \t buff[0]")
        // printf("\tbuf:%d ", bufsamplam[0] - '0');

        skaitli[i - 44] = bufsamplam[7] - '0';

        //<printf("Pēdējāis : %c", bufsamplam[7]);
        // printf("Bufl: %c")
        // printf(" Saktilis: %d", buff[i]);
        // skaitli[i-44] = buff[i];

        // printf(" %d", skaitli[i-44]);

        // snprintf(pakaapiens, 1, "%d",buff[i]);
        // snprintf(pakaapiens,"0x%02x",buff[i]);
        // int elements = i - 8;
        // skaitli[elements] = buff[i];
        // printf("%d",skaitli[i]);
        // printf(" kodējums:%d", buff[i]);
        printf("\t");

    }
    /*
    printf("chunkSize:\n");
    for (int i=0;i< 10;i++){
        printf("0x%02x", chunkSize[i]);
    }
    */
}

int salidzinajums(const void *a, const void *b)
{

    // If a is smaller, positive value will be returned
    return (*(int *)a - *(int *)b);
}

int failotaajs()
{

    FILE *fileptr;

    fileptr = fopen("output.wav", "rb");

    unsigned char buffer[4000000];
    size_t byte_read;

    if (fileptr != NULL)
    {

        // double check this
        do
        { // sizeof(unsigned char) ir baita izmērs: 8 biti un programma veselu skaitli pieņem kā izmēru bitos, tā kā abiem būtu jāstrādā, bet drošs paliek drošs.
            byte_read = fread(buffer, sizeof(unsigned char), 4000000, fileptr);
            printeeBaitus(buffer, byte_read);
        } while (byte_read > 0); // while(byte_read > 0);
        fclose(fileptr);
        printf("\nFails ir ielasīts!");
        int n = sizeof(skaitli) / sizeof(skaitli[5]);
        // qsort(skaitli, n, sizeof(int), salidzinajums);

        char nosaukums[20];
        printf("%d", n);
        printf("\n-----------------------\nAr kādu identifikatoru vēlies atzīmēt dotā audio baitu secību?: ");
        scanf("%s", nosaukums);

        FILE *f = fopen(nosaukums, "w");



        int skaitaamais0;
        int skaitaamais1;
        // Izveido 256 rindiņas ar katra parādīšanās sakitu
        for (int i = 0; i < n; i++)
        {
            if (skaitli[i] == 1)
            {
                skaitaamais1++;
            }
            else if (skaitli[i] == 0)
            {
                skaitaamais0++;
            }
            else
            {
                printf("dirst ej! i = %d", i);
            }
        }
        

        fprintf(f, "nulles: %d | vieninieki: %d", skaitaamais0, skaitaamais1);
        
        printf("\nIevadi ranga minimālo vērtibu: ");
        scanf("%d", &min);
        printf("\nIevadi ranga maksimālo vērtibu: ");
        scanf("%d", &max);
        
        //10
        int rangs = max - min;
        
        //minimalis bitu skaits ko nemt
        int bituSkaitsKoNemt = floor(log2l(rangs)) + 1;
        printf(" bs: %d ", bituSkaitsKoNemt);
        int vertiba;

        do{
            int konverteejamais[32];
            for (int b; b < bituSkaitsKoNemt; b++){
                konverteejamais[b] = skaitli[b];
                printf("%d", konverteejamais[b]);
            }

            int buf = 0;
            for (int i = 0; i < 32; i++) {
            buf = (buf * 10) + konverteejamais[i];
            }
            
            printf(" Bufferis: %d",buf);
                vertiba = binUzDec(buf);
            printf("guh: %d ", vertiba);
        }
        while (vertiba < rangs);

        // fwrite(skaitli, 1, sizeof(skaitli), f);

        fclose(f);



        return 0;
    }
}
/* programma pieņem 2 argumentus: main *norādes uz failu*.m4a */

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Programma prasa norādi uz m4a faila atrašanās vietu, 1. argumentā.");
    }
    else if (argc > 2)
    {
        printf("Programma pieņem tikai 1 argumentu - prasa norādi uz m4a faila atrašanās vietu, 1. argumentā.");
    }
    else
    {

        char komanda[100];
        sprintf(komanda, "ffmpeg -i %s output.wav", argv[1]);
        system(komanda);
        printf("\n\n\nIzdevās!!\n\nBaiti:\n");
        // system("ffplay output.wav");

        failotaajs();
        return 0;
    }
}