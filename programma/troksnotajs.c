#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

//'Ja extra arguments: pieņemt to kā audio failu jau ielasītu, noskippot serkciju'
//Ielasīt 2 virnkes failos, bināro bez random un bināro modificēto

/*
1. Konvertēju failu uz wav
2. Izlasu visus baitus no faila, sākot ar 44. baitu (izlaižot headeru)
3. Pārveidoju hexadecimālos baitus uz bināriem bitiem
*/

// max izmēra definīcija
size_t par = 900000000;


// bināro mainīgo skaitļu definīcijas
//int binFikseetie[par];

int *binFikseetie = NULL;
int *binInti = NULL;
int *nejausieSkaitliRobezaas = NULL;



int chunkSize[50];

//int nejausieSkaitliRobezaas[par];

int min, max;



// no veseliem skaitļiem uz bitiem

unsigned int_to_bin(unsigned k)
{
    if (k == 0)
        return 0;
    if (k == 1)
        return 1;
    return (k % 2) + 10 * int_to_bin(k / 2);
}

// Funkcija, kas ielasa baitus no audio faila masīvā

void printeeBaitus(unsigned char *buff, int len, int jau_ielasitais)
{

    
    
    
    int i; // < len
    
     for (i = 44; i < len; i++)
    {
        //pagaidu strings lai ielasītu 8 bitus
        char bufsamplam[9];
        

            // snprintf(bufsamplam, 12, "K: %08d", int_to_bin(buff[i]));
            snprintf(bufsamplam, 9, "%08d", int_to_bin(buff[i]));
        

       int indekss_ar_pieskatiito = i + jau_ielasitais - 44;
        binFikseetie[indekss_ar_pieskatiito] = bufsamplam[0] - '0';

        binInti[indekss_ar_pieskatiito] = buff[indekss_ar_pieskatiito];

    }
}

int salidzinajums(const void *a, const void *b)
{

    // Ja a ir mazāks, atgriezīs pozitīvu vērtību
    return (*(int *)a - *(int *)b);
}

// Ielasa nejuašo skaitļu datus failā
int failotaajs(char nosaukumsBez[20])
{

    FILE *fileptr;

    char atvert[20];
    sprintf(atvert, "trokšņi/%s.wav", nosaukumsBez);
    fileptr = fopen(atvert, "rb");

//    unsigned char buffer[40000000];
//    Lai risinātu jūsu problēmu ar segfault (core dumped), ja pievienojat nullīti jebkurai no mainīgajām, ir vairāki iespējamie iemesli, kas var izraisīt šo kļūdu. Galvenais, kas jāņem vērā, ir atmiņas piešķiršana un pārsniegšana.

unsigned char *buffer = malloc(par * sizeof(unsigned char));
if (buffer == NULL) {
    printf("Atmiņas piešķiršana neizdevās!\n");
    exit(1);
}

binFikseetie = malloc(par * sizeof(int));
if (binFikseetie == NULL) {
    printf("Neizdevās piešķirt atmiņu binFikseetie!\n");
    exit(1);
}

binInti = malloc(par * sizeof(int));
if (binInti == NULL) {
    printf("Neizdevās piešķirt atmiņu binInti!\n");
    exit(1);
}

nejausieSkaitliRobezaas = malloc(par * sizeof(int));
if (nejausieSkaitliRobezaas == NULL) {
    printf("Neizdevās piešķirt atmiņu nejausieSkaitliRobezaas!\n");
    free(binFikseetie);
    exit(1);
}


    int kopejais_ielasitais = 0;
    size_t bitu_lasitaajs;

    if (fileptr != NULL)
    {

        
        do
        { // sizeof(unsigned char) ir baita izmērs: 8 biti un programma veselu skaitli pieņem kā izmēru bitos, tā kā abiem būtu jāstrādā, bet drošs paliek drošs.
            bitu_lasitaajs = fread(buffer, sizeof(unsigned char), 9000000, fileptr);
            printf("kopejais_ielasitais: %d\n", kopejais_ielasitais);
            printeeBaitus(buffer, bitu_lasitaajs, bitu_lasitaajs);
            kopejais_ielasitais = kopejais_ielasitais + bitu_lasitaajs;

            //Paplašina binFikseetie un NejausiSkaitliRobezaas izmērus..
        if (par <= kopejais_ielasitais){
        printf("\nPielāgo masīvu izmērus...\n");
        
         
        binFikseetie = realloc(binFikseetie, kopejais_ielasitais * sizeof(int));
        if (binFikseetie == NULL) {
            printf("Neizdevās paplašināt binFikseetie!\n");
        exit(1);
        }
        
        binInti = realloc(binInti, kopejais_ielasitais * sizeof(int));
        if (binInti == NULL) {
            printf("Neizdevās paplašināt binInti!\n");
        exit(1);
        }


        nejausieSkaitliRobezaas = realloc(nejausieSkaitliRobezaas, kopejais_ielasitais * sizeof(int));
        if (nejausieSkaitliRobezaas == NULL) {
            printf("Neizdevās piešķirt atmiņu nejausieSkaitliRobezaas!\n");
        exit(1);
        }
        
        printf("\nBin fikseetie izmers: %ld \n", par);
        }
        

        } while (bitu_lasitaajs > 0); // while(bitu_lasitaajs > 0);
        
        //free(buffer);
        fclose(fileptr);
        printf("\nFails ir ielasīts!");

        char nosaukums[20];
        //char nosaukumsBez[20];
        printf(" Izmērs kopejais_ielasitais %d baiti.", kopejais_ielasitais);
        printf("\n-----------------------\nAr kādu identifikatoru vēlies atzīmēt dotā audio baitu secību?: ");
        //scanf("%s", nosaukumsBez);
        snprintf(nosaukums, 20, "dati/%s", nosaukumsBez);
        FILE *f = fopen(nosaukums, "w");

        
        
        //Ievada bitus 0 un 1 failā
        char asciiLimiteetieNsk[40];
        snprintf(asciiLimiteetieNsk, 40, "dati/%s_ascii.txt", nosaukumsBez);
        FILE *asciiLimiteetie = fopen(asciiLimiteetieNsk,"w");
        int skaitiitaajs;
        for (int o = 0; o < kopejais_ielasitais; o++){
            if (o % 25 == 0 && skaitiitaajs < 40){
                fprintf(asciiLimiteetie, "\n");
            }
            
            if (o == 0){
                skaitiitaajs = 1;
            }
            else if (binFikseetie[o] == binFikseetie[o-1]){
            skaitiitaajs++;
            } else {
            skaitiitaajs = 0;
            }
            if (skaitiitaajs < 40){ // 40 120 300
            fprintf(asciiLimiteetie, "%d", binFikseetie[o]);
        }
        }

        fclose(asciiLimiteetie);

    
        printf("\nIevadi ranga minimālo vērtibu (no): ");
        scanf("%d", &min);
        printf("\nIevadi ranga maksimālo vērtibu (līdz): ");
        scanf("%d", &max);
    


        // 10
        int rangs = max - min;

        // minimalis bitu skaits ko nemt
        int bituSkaitsKoNemt = floor(log2l(rangs)) + 1;
        printf("\nBitu skaits ko ņemt: %d\n", bituSkaitsKoNemt);
        
            // ielasa bitu virkni ar x(bituSkaitsKoNemt elementiem un pārveido to uz decimālo)
       
        //iteracija 
        int iteracijaKameer = 0;
        // Izmanto, lai uzskaitītu iterāciju, jeb izpilžu skaitu pilnai cikla izpildei un vareetu sarezinaat to ar /bituSkaitsKoNemt/ un tādejādi ņemt nākamos /bituSkaitsKoNemt/ skaitļus katrā izpildē
        int iteracija = 0;
        int paBitusk;
        int decis;
        
        do{ 
            int decis = 0;
            char binaraaVirkne[32] = {0};
            for (int b = 0; b < bituSkaitsKoNemt; b++){
                //Bit shifts, tas veido kopīgu bināro skaitli un parveido to decimālā skaitlī
                decis = (decis << 1) | binFikseetie[b + paBitusk];
                //Papildu pārbaude, ir 1 vai 0? Ja ir tad ieraksta.
                binaraaVirkne[b] = binFikseetie[b + paBitusk] ? '1' : '0';
                }
            iteracija++;
            paBitusk = bituSkaitsKoNemt * iteracija;
            binaraaVirkne[bituSkaitsKoNemt] = '\0';
            if (decis <= max && decis >= 0){ 
                nejausieSkaitliRobezaas[iteracijaKameer] = decis; 
                iteracijaKameer++;
                }
        } while (paBitusk < kopejais_ielasitais);

        //int n = sizeof(nejausieSkaitliRangaa) / sizeof(nejausieSkaitliRangaa[0]);
        int n = iteracijaKameer;
        printf("Nejaušie skaitļi izmērs: %d\n", n);
        printf("kopejais_ielasitais savukārt: %d\n", kopejais_ielasitais);
        free(buffer);
         
        
        char asciiNemodifNsk[40];
        snprintf(asciiNemodifNsk, 40, "dati/%s_robezas.txt", nosaukumsBez);
        FILE *asciiNemodif = fopen(asciiNemodifNsk,"w");

        FILE *binaaraisIntFails;
        char binaaraisIntFailsNsk[20];
        sprintf(binaaraisIntFailsNsk, "dati/%s_binarais.bin", nosaukumsBez);
        binaaraisIntFails = fopen(binaaraisIntFailsNsk, "wb");
    

        for (int o = 0; o < kopejais_ielasitais; o++){
            if (o % 25 == 0 && skaitiitaajs < 20){
                    fprintf(asciiNemodif, "\n");
            }
            if (o == 0){
                skaitiitaajs = 1;
            }//nejausieSkaitliRobezaas[o] == nejausieSkaitliRobezaas[o-1]
            else if (binInti[o] == binInti[o-1]){
            skaitiitaajs++;
            } else {
            skaitiitaajs = 0;
            }
            if (skaitiitaajs < 20){ // 40 120 300
            //fprintf(asciiNemodif, "%d", nejausieSkaitliRobezaas[o]);
            fwrite(&binInti[o], sizeof(int), 1, binaaraisIntFails);

        }    
            //printf("%d\n",binInti[o]);
        }

    fclose(asciiNemodif);
	fclose(binaaraisIntFails);
    
        fclose(f);         
        free(binFikseetie);
        free(nejausieSkaitliRobezaas);

        return 0;
    }
    
    return 1;
}
/* programma pieņem 2 argumentus: main *norādes uz failu*.m4a */

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Programma prasa 1. (jauns/eksist) norādi uz m4a faila atrašanās vietu, 2. argumentā.");
    }
    else if (argc > 4)
    {
        printf("Programma pieņem tikai 2 argumentu - prasa norādi uz m4a faila atrašanās vietu, 1. argumentā");
    }
    else if (0 == strcmp(argv[1], "nav")) {

        char IerKomanda[100];
        //char KonvKomanda[100];
        //maiņā no 600
        sprintf(IerKomanda, "ffmpeg -t 2400 -f pulse -i default trokšņi/%s.wav", argv[2]);
        //sprintf(KonvKomanda, "ffmpeg -i %s output.wav", argv[1]); 2400
        system(IerKomanda);
        printf("\n\n\nIerakstītas 40 minūtes!!\n");
        // system("ffplay output.wav");

        failotaajs(argv[2]);
        return 0;
    } else if (0 == strcmp(argv[1], "ir")) {
        failotaajs(argv[2]);
    }
}


