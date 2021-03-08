#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int package_installed, package_removed, package_upgraded, cont;
int alpm_scriptlet, alpm, pacman;
char* date = "-";
char* name = "-";
char* status = "-";
char* type = " ";

struct Info{
    char *packageName;
    char *packageInstall;
    char *packageUpdate;
    char *packageRemove;
    int packageUpdateN;
};

struct Info list[4500];
void analizeLog(char *logFile, char *report);
int string(char *str);
void writeReport(char *report);
void readLine(char *line);
char *substring(char *line,int start,int end);
char* getLine(int id);

int main(int argc, char **argv) {
    if (argc < 5 || strcmp(argv[1],"-input") != 0 || strcmp(argv[3],"-report") != 0) {
        return 0;
    }
    analizeLog(argv[2], argv[4]);
    return 0;
}

int string(char *str){
    int contador=0;
    while (*str != '\0') {
        contador++, str++;
    }
    return contador;
}

char *substring(char *line,int start,int end){
    char *palabra=malloc(sizeof(char)*(end-start+1));
    for (int x = 0; x<end-start; x++){
        palabra[x] = line[x+start];
    }
    palabra[end-start] = '\0';
    return palabra;
}

void readLine(char *line){
    int startn = 0;
    int endn = 0;
    int flag = 0;

    if(line[0] == '['){ 
        startn = 1;
        endn = 1;

    while (line[endn] != ']'){
        endn++;
    }
        date = substring(line,startn,endn);
        endn++;
        endn++;
    }
    
    while(line[endn]!= ' '){
        endn++;
    }

    endn++;
    startn = endn;

    char* pacmann = strstr(line, "[PACMAN]");
    char* alpmn = strstr(line, "[ALPM]");
    char* alpmscriptletn = strstr(line, "[ALPM-SCRIPTLET]");

    if(pacmann != NULL){
        pacman++;
    }
    else if(alpmn != NULL){
        alpm++;
    }
    else if(alpmscriptletn != NULL){
        alpm_scriptlet++;
    }
    
    if((startn < string(line))){
        if (line[startn] == 'i' && line[startn + 1] == 'n' && line[startn + 2] == 's' && line[startn + 3] == 't'){
            endn = 10 + startn;
            status = "installed";
            flag = 1;
        }
        else if (line[startn] == 'u' && line[startn + 1] == 'p' && line[startn + 2] == 'g' && line[startn + 3] == 
        'r'&& line[startn + 4] == 'a'&& line[startn + 5] == 'd'&& line[startn + 6] == 'e'&& line[startn + 7] == 'd'){
            endn = 9 + startn;
            status = "upgraded";
            flag = 1;
        }
        else if (line[startn] == 'r' && line[startn+ 1] == 'e' && line[startn + 2] == 'm' && line[startn + 3] == 
        'o'&& line[startn + 4] == 'v'&& line[startn + 5] == 'e'&& line[startn + 6] == 'd'){
            endn = 8 + startn;
            status = "removed";
            flag = 1;
        }else if (line[startn] == 'r' && line[startn+ 1] == 'e' && line[startn + 2] == 'i' && line[startn + 3] == 
        'n'&& line[startn + 4] == 's'&& line[startn + 5] == 't'&& line[startn + 6] == 'a'){
            endn = 12 + startn;
            status = "reinstalled";
            flag = 1;
        }
    }

    startn = endn;
    while(line[endn] != ' '){
        endn++;
    }
    
    if(flag == 1){
        name = substring(line,startn,endn);
    }else{
        name = "-";
    }
}

void writeReport(char *report){
    FILE * fp = fopen(report,"w");
    char* sentence = (char*)malloc(sizeof(char)*35);

    fprintf(fp, "Pacman Packages Report\n");
    fprintf(fp, "----------------------\n");
    sprintf(sentence, "- Installed packages : \t%d\n",package_installed);
    fprintf(fp, sentence, string(sentence));
    sprintf(sentence, "- Removed packages : \t%d\n", package_removed);
    fprintf(fp, sentence, string(sentence));
    sprintf(sentence, "- Upgraded packages : \t%d\n", package_upgraded);
    fprintf(fp,sentence,string(sentence));
    sprintf(sentence, "- Current installed : \t%d\n", package_installed-package_removed);
    fprintf(fp, sentence, string(sentence));

    fprintf(fp, "-------------\n");
    fprintf(fp, "General Stats\n");
    fprintf(fp, "-------------\n");
    sprintf(sentence, "- Oldest Package : \t%s\n", list[0].packageName);
    fprintf(fp, sentence, string(sentence));
    sprintf(sentence, "- Newest Package : \t%s\n", list[package_installed - 1].packageName);
    fprintf(fp, sentence, string(sentence));
    fprintf(fp, "- Package with no upgrades : ");
    for(int i = 0 ; i < package_installed ; i++){
        if(list[i].packageUpdateN == 0){
            fprintf(fp, "%s, ", list[i].packageName);
        }
    }
    fprintf(fp, sentence, string(sentence));
    sprintf(sentence, "- [ALPM-SCRIPTTLET] type count : \t%d\n", alpm_scriptlet);
    fprintf(fp, sentence, string(sentence));
    sprintf(sentence, "- [ALPM] count : \t%d\n", alpm);
    fprintf(fp, sentence, string(sentence));
    sprintf(sentence, "- [PACMAN] count : \t%d\n", pacman);
    fprintf(fp, sentence, string(sentence));

    fprintf(fp, "----------------");
    fprintf(fp, "\nList of packages\n");
    fprintf(fp, "----------------\n");
    for(int x = 0 ; x < package_installed ; x++){
        if(list[x].packageName != NULL){
            sprintf(sentence, "- Package Name : %s\n", list[x].packageName);
            fprintf(fp, sentence, string(sentence)+1);
            sprintf(sentence, "\t- Install date : %s\n", list[x].packageInstall);
            fprintf(fp, sentence, string(sentence)+1);
            sprintf(sentence, "\t- Last update date :%s\n", list[x].packageUpdate);
            fprintf(fp, sentence, string(sentence)+1);
            sprintf(sentence, "\t- How many updates :%d\n", list[x].packageUpdateN);
            fprintf(fp, sentence, string(sentence)+1);
            sprintf(sentence, "\t- Removal date : %s\n", list[x].packageRemove);
            fprintf(fp, sentence, string(sentence)+1);
        }
    }
}

char* getLine(int id){
    int acumulator = 0;
    int numL;
    int size = 100;
    char *line;
    line = calloc(size, sizeof(char));
    while((numL = read(id, line+acumulator, size)) > 0){
        for(int x=acumulator;x<acumulator+numL;x++){
            if(line[x] == '\n'){
                lseek(id, x-(acumulator+numL)+1, SEEK_CUR);
                line[x] = '\0';
                return line;
            }
        }
        acumulator += numL;
        line = realloc(line, acumulator+size);
    }
    if(acumulator > 0){
        line[acumulator] = '\0';
        return line;
    }

    return NULL;
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);
    
    int id = open(logFile, O_RDONLY);
    if (id < 0) {
        perror("No File");
        return;
    }
    
    char *line;
    line=getLine(id);
    while ((line) != NULL){
        readLine(line);
        if(strcmp(status, "installed") == 0 && strcmp(name,"-") > 0){
            list[cont].packageName = name;
            list[cont].packageInstall = date;
            list[cont].packageUpdate="-";
            list[cont].packageRemove="-";
            cont++;
            package_installed++;
        }else if(strcmp(status, "upgraded") == 0 && (strcmp(name,"-") > 0 || strcmp(name,"-") < 0)){
            for(int x=0;x<cont;x++){
                if(strcmp(list[x].packageName,name)==0){
                    list[x].packageUpdate=date;
                    list[x].packageUpdateN++;
                    package_upgraded++;
                    break;
                }
            }
        }else if(strcmp(status,"removed") == 0 && (strcmp(name,"-") > 0 || strcmp(name,"-") < 0)){
            for(int x=0;x<cont;x++){
                if(strcmp(list[x].packageName,name)==0){
                    list[x].packageRemove=date;
                    package_removed++;
                    break;
                }
            }
        }
        else if(strcmp(status, "reinstalled") == 0 && (strcmp(name,"-") > 0 || strcmp(name,"-") < 0)){
            list[cont].packageName=name;
            list[cont].packageInstall="-";
            list[cont].packageUpdate="-";
            list[cont].packageRemove="-";
            cont++;
        }
        line=getLine(id);
    }
    close(id);
    writeReport(report);
    printf("Report is generated at: [%s]\n", report);
}