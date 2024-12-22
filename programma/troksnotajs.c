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
// globaalais jo gribu %%%

// max izmēra definīcija
size_t par = 900000000;


// bināro mainīgo skaitļu definīcijas
//int binFikseetie[par];

int *binFikseetie = NULL;
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
        /*
        if (i % 1000000 == 0){
        // Ja dalās ar 8 tad printē jaunā līnijā
        printf("    |%d\n", i);
        }
        */

        // stringu saglabāšanas mahinācijas
        char bufsamplam[9];
        
        // Izprintē bināro bāzi 2 skaitlim no buffera, kas ir decimāls, tātad dec>bin un tad, ja tam sūdam nav 0 priekšā,
        //  tad to ievieto kreisajā pusē lai sanāktu 8 biti katram kanālam yipēe

        // lai saglabātu tiek 2 reizes izsaukts snprintf, 1. lai noskaidrotu izmeru, 2. lai ierakstiitu bufferii,
        // tad tos paņems konkatenēs, jeb saliks kopā lai izveidotu 16 bitu virkni (nagfig??), un no tās tālāk paņem vismazsvarīgāko bitu, kas ir pēdejais
        //if (i % 2 == 0)
        //{

            // snprintf(bufsamplam, 12, "K: %08d", int_to_bin(buff[i]));
            snprintf(bufsamplam, 9, "%08d", int_to_bin(buff[i]));
            //printf("K: %s", bufsamplam);
            //printf("A: %d\n", bufsamplam[0] - '0');
        //}
        /*
        else
        {
    
            // snprintf(bufsamplam, 12, "L: %08d", int_to_bin(buff[i]));
            snprintf(bufsamplam, 9, "%08d", int_to_bin(buff[i]));
            //printf("L: %s", bufsamplam);
        }
        */
       int indekss_ar_pieskatiito = i + jau_ielasitais - 44;
        binFikseetie[indekss_ar_pieskatiito] = bufsamplam[0] - '0';
        //printf("%d", indekss_ar_pieskatiito);
        
        //printf("\t");
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

nejausieSkaitliRobezaas = malloc(par * sizeof(int));
if (nejausieSkaitliRobezaas == NULL) {
    printf("Neizdevās piešķirt atmiņu nejausieSkaitliRobezaas!\n");
    free(binFikseetie);
    exit(1);
}


    int kop_read = 0;
    size_t byte_read;

    if (fileptr != NULL)
    {

        // double check this
        do
        { // sizeof(unsigned char) ir baita izmērs: 8 biti un programma veselu skaitli pieņem kā izmēru bitos, tā kā abiem būtu jāstrādā, bet drošs paliek drošs.
            byte_read = fread(buffer, sizeof(unsigned char), 9000000, fileptr);
            printf("kop_read: %d\n", kop_read);
            printeeBaitus(buffer, byte_read, byte_read);
            kop_read = kop_read + byte_read;

            //Paplašina binFikseetie un NejausiSkaitliRobezaas izmērus..
        if (par <= kop_read){
        printf("\nPielāgo masīvu izmērus...\n");
        
         
        binFikseetie = realloc(binFikseetie, kop_read * sizeof(int));
        if (binFikseetie == NULL) {
            printf("Neizdevās paplašināt binFikseetie!\n");
        exit(1);
        }
        
        nejausieSkaitliRobezaas = realloc(nejausieSkaitliRobezaas, kop_read * sizeof(int));
        if (nejausieSkaitliRobezaas == NULL) {
            printf("Neizdevās piešķirt atmiņu nejausieSkaitliRobezaas!\n");
        exit(1);
        }
        
        printf("\nBin fikseetie izmers: %ld \n", par);
        }
        

        } while (byte_read > 0); // while(byte_read > 0);
        
        //free(buffer);
        fclose(fileptr);
        printf("\nFails ir ielasīts!");

        


        char nosaukums[20];
        //char nosaukumsBez[20];
        printf(" IzmērsKop_read %d baiti.", kop_read);
        printf("\n-----------------------\nAr kādu identifikatoru vēlies atzīmēt dotā audio baitu secību?: ");
        //scanf("%s", nosaukumsBez);
        snprintf(nosaukums, 20, "dati/%s", nosaukumsBez);
        FILE *f = fopen(nosaukums, "w");

        
        
        //Ievada bitus 0 un 1 failā
        char binNemainitoNsk[40];
        snprintf(binNemainitoNsk, 40, "dati/%s_biti.txt", nosaukumsBez);
        FILE *binNemainitie = fopen(binNemainitoNsk,"w");
        int skaitiitaajs;
        for (int o = 0; o < kop_read; o++){
            if (o == 0){
                skaitiitaajs = 1;
            }
            else if (binFikseetie[o] == binFikseetie[o-1]){
            skaitiitaajs++;
            } else {
            skaitiitaajs = 0;
            }
            if (skaitiitaajs < 400){ // 40 120 300
            fprintf(binNemainitie, "%d", binFikseetie[o]);
        }
        }

        fclose(binNemainitie);

  
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
        // Izmanto, lai uzskaitītu iterāciju, jeb izpilžu skaitu pilnai cikla izpildei un vareetu sarezinaat to ar /bituSkaitsKoNemt/ un tādejādi ņemt nākamos /bituSkaitsKoNemt/ skaitļus katrā izpildē
        int iteracija = 0;
        int paBitusk;
        int decis;
        
        do{ 
            int decis = 0;
            char binaraaVirkne[32] = {0};
            for (int b = 0; b < bituSkaitsKoNemt; b++){
                //Bit shifts, taa veido kopiigu bin skaitli un parveido decimaalajaa
                decis = (decis << 1) | binFikseetie[b + paBitusk];
                //Ir 1 vai 0? Ja ir tad AIZIET!
                binaraaVirkne[b] = binFikseetie[b + paBitusk] ? '1' : '0';
                }
            iteracija++;
            paBitusk = bituSkaitsKoNemt * iteracija;
            binaraaVirkne[bituSkaitsKoNemt] = '\0';
            if (decis <= max && decis >= 0){ 
                //printf("pa bitu sk: %d ", paBitusk);
                //printf("decimāli = %d, bin = %s\n, iteracija = %d vajag = 3569772", decis, binaraaVirkne, iteracija);
                nejausieSkaitliRobezaas[iteracijaKameer] = decis; 
                iteracijaKameer++;

                }
        } while (paBitusk < kop_read);
        //39861152 (ja dala ar 4)
        //39443035

        //probleema: tiek turpinaata cilpa arii pee skaitli apjoma paarsniegsanas, un tas rezulteejas lielā kļūdā ielādētajām 0


        //int n = sizeof(nejausieSkaitliRangaa) / sizeof(nejausieSkaitliRangaa[0]);
        int n = iteracijaKameer;
        printf("Nejaušie skaitļi izmērs: %d\n", n);
        printf("Kop_read savukārt: %d\n", kop_read);
        free(buffer);
         
        
        char binModificeetieNsk[40];
        snprintf(binModificeetieNsk, 40, "dati/%s_nemodif_biti.txt", nosaukumsBez);
        FILE *binModificeetie = fopen(binModificeetieNsk,"w");
        
        for (int o = 0; o < kop_read; o++){
            fprintf(binModificeetie, "%d", nejausieSkaitliRobezaas[o]);
        /*
        skaitiitaajs = 0;
        for (int o = 0; o < kop_read; o++){
            if (o == 0){
                skaitiitaajs = 1;
            }
            else if (nejausieSkaitliRobezaas[o] == nejausieSkaitliRobezaas[o-1]){
            skaitiitaajs++;
            } else {
            skaitiitaajs = 0;
            }
            if (skaitiitaajs < 300){ // 40 120 300 1000
            fprintf(binModificeetie, "%d", nejausieSkaitliRobezaas[o]);
        }
        */
        }

    fclose(binModificeetie);
	
    /*
	printf("Laiks tvaicēt");
	//Ja 0 atkārtojas virknē vairāk kā 7, tad viss
	char labotieModificeetieNsk[40];
	snprintf(labotieModificeetieNsk, 40, "dati/%s_labotieRobezaas.txt", nosaukumsBez);
	FILE *labotieModificeetie = fopen(labotieModificeetieNsk, "w");
	int virknesSkaitaamais;
	for (int b = 0; b < n; n++){
	if (b == 0){
		virknesSkaitaamais++;
	} 
	if (virknesSkaitaamais > 16){
	//ss
	printf("");
	}
	else if (nejausieSkaitliRobezaas[b-1] == nejausieSkaitliRobezaas[b]){
		fprintf(labotieModificeetie, "%d", nejausieSkaitliRobezaas[b]);	
		virknesSkaitaamais++; 	
	} 
	else {	
		virknesSkaitaamais = 0;
		fprintf(labotieModificeetie, "%d", nejausieSkaitliRobezaas[b]);	
	}
	}
    
	fclose(labotieModificeetie);
    */
        qsort(nejausieSkaitliRobezaas, n, sizeof(int), salidzinajums);
        int skaitaamais;
        
        
        for (int x = 0; x < n; x++){
            if (x<1){
                skaitaamais++;
            }
            if(nejausieSkaitliRobezaas[x-1] == nejausieSkaitliRobezaas[x]){
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
/*        
        char SkewFailaNsk[40];
        
        printf("\nVai labot datus, lai skaitļi neatkārtojas?\n");
        snprintf(SkewFailaNsk, 40, "dati/%s_bezNovirzes.txt", nosaukumsBez);
        
        FILE *binfails = fopen(SkewFailaNsk, "w");
       
        int izmers = sizeof(binFikseetie) / sizeof(binFikseetie[0]);

        for (int v; v < izmers; v++){
            if (v == 0){
                fprintf(binfails, "%d", binFikseetie[v]);
            } else if (binFikseetie[v] == binFikseetie[v-1]){
            printf(" ");
            } else {
            fprintf(binfails, "%d", binFikseetie[v]);
        }}

        fclose(binfails);
*/         
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
        sprintf(IerKomanda, "ffmpeg -t 1800 -f pulse -i default trokšņi/%s.wav", argv[2]);
        //sprintf(KonvKomanda, "ffmpeg -i %s output.wav", argv[1]);
        system(IerKomanda);
        printf("\n\n\nIerakstītas 30 minūtes!!\n");
        // system("ffplay output.wav");

        failotaajs(argv[2]);
        return 0;
    } else if (0 == strcmp(argv[1], "ir")) {
        failotaajs(argv[2]);
    }
}


