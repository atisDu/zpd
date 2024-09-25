#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//failu definīcijas
int skaitli[4000000];


void printeeBaitus(unsigned char* buff, int len)
{   
    int i;
    for (i = 44; i < len; i++)
    {
        if(i%8 == 0){
            printf(" %d\n",i);
        }
        printf("0x%02x",buff[i]);
        

        skaitli[i-44] = buff[i];

        //printf(" %d", skaitli[i-44]);
           
        //snprintf(pakaapiens, 1, "%d",buff[i]);
        //snprintf(pakaapiens,"0x%02x",buff[i]);
        //int elements = i - 8;
        //skaitli[elements] = buff[i];
        //printf("%d",skaitli[i]);
        //printf(" kodējums:%d", buff[i]);
        printf("\t");
    }
}

int salidzinajums(const void* a, const void* b) {
      
      // If a is smaller, positive value will be returned
    return (*(int*)a - *(int*)b);
}



int failotaajs(){
    
    FILE *fileptr;

    fileptr = fopen("output.wav", "rb");
    
    unsigned char buffer[4000000];
    
    if (fileptr != NULL) {
        size_t byte_read;
        // double check this
        do{ //sizeof(unsigned char) ir baita izmērs: 8 biti un programma veselu skaitli pieņem kā izmēru bitos, tā kā abiem būtu jāstrādā, bet drošs paliek drošs.
            byte_read = fread(buffer,sizeof(unsigned char),4000000,fileptr);
            printeeBaitus(buffer, byte_read);
        }while(byte_read > 0);//while(byte_read > 0);
        fclose(fileptr);
        printf("\nFails ir ielasīts!");
        int n = sizeof(skaitli) / sizeof(skaitli[0]);
        qsort(skaitli, n, sizeof(int), salidzinajums);
        
        char nosaukums[20];
        printf("\n-----------------------\nAr kādu identifikatoru vēlies atzīmēt dotā audio baitu secību?: ");
        scanf("%s", nosaukums);

        FILE *f = fopen(nosaukums, "w");
        
        int skaitaamais;
        
        //Izveido 255 rindiņas ar katra parādīšanās sakitu
        for (int i=0; i<n; i++){
            if (i>0){
                if(skaitli[i-1] == skaitli[i]){
                    skaitaamais++;
                    
                } else{
                    fprintf(f, "%d\n", skaitaamais);
                    skaitaamais = 0;
                }
            }
        }
        /*
        for (int i=0; i<n; i++) {
            fprintf(f, "%d\n", skaitli[i]);
        }
        */
        //fwrite(skaitli, 1, sizeof(skaitli), f);
        fclose(f);
        
        
        for (int i = 0; i < n; i++) {
        printf("%d ", skaitli[i]);

    }
    return 0;
}
}
/* programma pieņem 2 argumentus: main *norādes uz failu*.m4a */

int main(int argc, char* argv[]){
    if (argc < 2){
        printf("Programma prasa norādi uz m4a faila atrašanās vietu, 1. argumentā.");
    } else if (argc > 2){
        printf("Programma pieņem tikai 1 argumentu - prasa norādi uz m4a faila atrašanās vietu, 1. argumentā.");
    }else{
    
    char komanda[100];
    sprintf(komanda,"ffmpeg -i %s output.wav", argv[1]);
    system(komanda);
    }
    printf("\n\n\nIzdevās!!\n\nBaiti:\n");
    //system("ffplay output.wav");

    failotaajs();
    return 0;
}