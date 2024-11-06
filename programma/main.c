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
int skaitliFikseetie[par];
int chunkSize[50];
int izmersApjomam;

int nejausieSkaitliRangaa[par];

int min, max;



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

    int i; // < len
    for (i = 44; i < len; i++)
    {

        // Ja dalās ar 8 tad printē jaunā līnijā
        if (i % 8 == 0)
        {
            //printf("    |%d\n", i);
        }


        // stringu saglabāšanas mahinācijas
        char bufsamplam[8];

        // Izprintē bināro bāzi 2 skaitlim no buffera, kas ir decimāls, tātad dec>bin un tad, ja tam sūdam nav 0 priekšā,
        //  tad to ievieto kreisajā pusē lai sanāktu 8 biti katram kanālam yipēe

        // lai saglabātu tiek 2 reizes izsaukts snprintf, 1. lai noskaidrotu izmeru, 2. lai ierakstiitu bufferii,
        // tad tos paņems konkatenēs, jeb saliks kopā lai izveidotu 16 bitu virkni (nagfig??), un no tās tālāk paņem vismazsvarīgāko bitu, kas ir pēdejais
        if (i % 2 == 0)
        {

            // snprintf(bufsamplam, 12, "K: %08d", int_to_bin(buff[i]));
            snprintf(bufsamplam, 9, "%08d", int_to_bin(buff[i]));
            //printf("K: %s", bufsamplam);
        }
        else
        {
    
            // snprintf(bufsamplam, 12, "L: %08d", int_to_bin(buff[i]));
            snprintf(bufsamplam, 9, "%08d", int_to_bin(buff[i]));
            //printf("L: %s", bufsamplam);
        }
        

        skaitliFikseetie[i - 44] = bufsamplam[7] - '0';

        //printf("\t");
        if (i == (len-1)){
            izmersApjomam = i+1;
        }
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

        char nosaukums[20];
        printf(" Izmērs: %d baiti.", izmersApjomam);
        printf("\n-----------------------\nAr kādu identifikatoru vēlies atzīmēt dotā audio baitu secību?: ");
        scanf("%s", nosaukums);

        FILE *f = fopen(nosaukums, "w");


        
        printf("\nIevadi ranga minimālo vērtibu (no): ");
        scanf("%d", &min);
        printf("\nIevadi ranga maksimālo vērtibu (līdz): ");
        scanf("%d", &max);
        
        // 10
        int rangs = max - min;

        // minimalis bitu skaits ko nemt
        int bituSkaitsKoNemt = floor(log2l(rangs)) + 1;
        printf("\nBitu skaits ko ņemt: %d\n", bituSkaitsKoNemt);
        
            // ielasa bitu virkni ar x(bituSkaitsKoNemt elementiem un pārveido to uz deci)
       
        //iteracija 
        int iteracijaKameer = 0;
        //izmanto lai uzskaitiitu iteraacijas pilnai cikla izpildei un vareetu sarezinaat to ar 4 un tadejadi nemt naakamos 4 skaitlus katraa izpildee
        int iteracija = 0;
        int paBitusk;
        int decis;
        //const int o = izmersApjomam;
        //int skaitli[o];
        /*
        for (int l = 0; l < izmersApjomam; l++){
            skaitli[l] = skaitliFikseetie[l];
            printf("%d", skaitli[l]);
        }
        */ 
        do{ 
            int decis = 0;
            char binaraaVirkne[33] = {0};
            for (int b = 0; b < bituSkaitsKoNemt; b++){
                //Bit shifts, taa veido kopiigu bin skaitli un parveido decimaalajaa
                decis = (decis << 1) | skaitliFikseetie[b + paBitusk];
                //Ir 1 vai 0? Ja ir tad AIZIET!
                binaraaVirkne[b] = skaitliFikseetie[b + paBitusk] ? '1' : '0';
                }
            iteracija++;
            paBitusk = bituSkaitsKoNemt * iteracija;
            binaraaVirkne[bituSkaitsKoNemt] = '\0';
            if (decis <= max && decis >= 0){ 
                //printf("pa bitu sk: %d ", paBitusk);
                //printf("decimāli = %d, bin = %s\n, iteracija = %d vajag = 3569772", decis, binaraaVirkne, iteracija);
                nejausieSkaitliRangaa[iteracijaKameer] = decis; 
                iteracijaKameer++;

                }
        } while (paBitusk < izmersApjomam);
        //39861152 (ja dala ar 4)
        //39443035

        //probleema: tiek turpinaata cilpa arii pee skaitli apjoma paarsniegsanas, un tas rezulteejas lielā kļūdā ielādētajām 0


        //int n = sizeof(nejausieSkaitliRangaa) / sizeof(nejausieSkaitliRangaa[0]);
        int n = iteracijaKameer;
        printf("Nejaušie skaitļi izmērs: %d\n", n);
        qsort(nejausieSkaitliRangaa, n, sizeof(int), salidzinajums);
        int skaitaamais;

        for (int x = 0; x < n; x++){
            if (x<1){
                skaitaamais++;
            }
            if(nejausieSkaitliRangaa[x-1] == nejausieSkaitliRangaa[x]){
                    skaitaamais++;
            }else{
             fprintf(f, "%d \t %d\n", min, skaitaamais);
             printf("%d \t %d\n", min, skaitaamais);
             min++;
             skaitaamais = 0;
            }
            }
        
        


        //Nejausieskaitlirangaa >> sortoju >> ja pedejais ir vienaads ++, ja ne tad sak jaunu

        /*z
        do{9881888
        //Ielasa konverteejamaa binaros skaitlus apjomaa bitu skaits ko nemt
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
        */

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